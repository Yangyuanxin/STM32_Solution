#### 1、使用mmap内存映射获取的方法

mmap 文件并搜索到特定行号的偏移量。由于映射的文件实际上是内存，因此一旦找到偏移量，就没有额外的存储过程。 

```c
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
 
/* following code assumes all file operations succeed. In practice,
 * return codes from open, close, fstat, mmap, munmap all need to be
 * checked for error.
*/
int read_file_line(const char *path, int line_no)
{
	struct stat s;
	char *buf;
	off_t start = -1, end = -1;
	size_t i;
	int ln, fd, ret = 1;
 
	if (line_no == 1) start = 0;
	else if (line_no < 1){
		warn("line_no too small");
		return 0; /* line_no starts at 1; less is error */
	}
 
	line_no--; /* back to zero based, easier */
 
	fd = open(path, O_RDONLY);
	fstat(fd, &s);
 
	/* Map the whole file.  If the file is huge (up to GBs), OS will swap
	 * pages in and out, and because search for lines goes sequentially
	 * and never accesses more than one page at a time, penalty is low.
	 * If the file is HUGE, such that OS can't find an address space to map
	 * it, we got a real problem.  In practice one would repeatedly map small
	 * chunks, say 1MB at a time, and find the offsets of the line along the
	 * way.  Although, if file is really so huge, the line itself can't be
	 * guaranteed small enough to be "stored in memory", so there.
	 */
	buf = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
 
	/* optional; if the file is large, tell OS to read ahead */
	madvise(buf, s.st_size, MADV_SEQUENTIAL);
 
	for (i = ln = 0; i < s.st_size && ln <= line_no; i++) {
		if (buf[i] != '\n') continue;
 
		if (++ln == line_no) start = i + 1;
		else if (ln == line_no + 1) end = i + 1;
	}
 
	if (start >= s.st_size || start < 0) {
		warn("file does not have line %d", line_no + 1);
		ret = 0;
	} else {
		/* 	do something with the line here, like
		write(STDOUT_FILENO, buf + start, end - start);
			or copy it out, or something
		*/
	}
 
	munmap(buf, s.st_size);
	close(fd);
 
	return ret;
}
```

#### 2、使用ANSI C的方法获取

这个版本不依赖于POSIX API，如`mmap，`而是坚持ANSI C功能。此版本也适用于不可搜索的文件，因此可以通过管道馈送。它执行有限但充分的错误检查。也就是说`，get_nth_line`在所有失败时都返回 NULL，并且调用方可以通过在输入文件上调用`feof（）`和`ferror（）`来区分 EOF、文件读取错误和内存不足。 

```c
#include <stdio.h>
#include <stdlib.h>
 
#define BUF_SIZE ( 256 )
 
char *get_nth_line( FILE *f, int line_no )
{
    char   buf[ BUF_SIZE ];
    size_t curr_alloc = BUF_SIZE, curr_ofs = 0;
    char   *line      = malloc( BUF_SIZE );
    int    in_line    = line_no == 1;
    size_t bytes_read;
 
    /* Illegal to ask for a line before the first one. */
    if ( line_no < 1 )
        return NULL;
 
    /* Handle out-of-memory by returning NULL */
    if ( !line )
        return NULL;
 
    /* Scan the file looking for newlines */
    while ( line_no && 
            ( bytes_read = fread( buf, 1, BUF_SIZE, f ) ) > 0 )
    {
        int i;
 
        for ( i = 0 ; i < bytes_read ; i++ )
        {
            if ( in_line )
            {
                if ( curr_ofs >= curr_alloc )
                {
                    curr_alloc <<= 1;
                    line = realloc( line, curr_alloc );
 
                    if ( !line )    /* out of memory? */
                        return NULL;
                }
                line[ curr_ofs++ ] = buf[i];
            }
 
            if ( buf[i] == '\n' )
            {
                line_no--;
 
                if ( line_no == 1 )
                    in_line = 1;
 
                if ( line_no == 0 )
                    break;
            }
        }
    }
 
    /* Didn't find the line? */
    if ( line_no != 0 ) 
    {
        free( line );
        return NULL;
    }
 
    /* Resize allocated buffer to what's exactly needed by the string 
       and the terminating NUL character.  Note that this code *keeps*
       the terminating newline as part of the string. 
     */
    line = realloc( line, curr_ofs + 1 );
 
    if ( !line ) /* out of memory? */
        return NULL;
 
    /* Add the terminating NUL. */
    line[ curr_ofs ] = '\0';
 
    /* Return the line.  Caller is responsible for freeing it. */
    return line;
}
 
 
/* Test program.  Prints out the 7th line of input from stdin, if any */
int main( int argc, char *argv[] )
{
    char *line7 = get_nth_line( stdin, 7 );
 
    if ( line7 )
    {
        printf("The 7th line of input was:\n%s\n", line7 );
        free( line7 );
    } else
    {
        printf("Did not find the 7th line of input.  Reason:  ");
        if ( feof( stdin ) )
            puts("End of file reached.");
        else if ( ferror( stdin ) )
            puts("Error reading input.");
        else
            puts("Out of memory.");
    }
 
    return 0;
}
```

#### 3、C++实现方法

```c++
#include <string>
#include <fstream>
#include <iostream>
 
int main( ) {
   std::cout << "Which file do you want to look at ?\n" ;
   std::string input ;
   std::getline( std::cin , input ) ;
   std::ifstream infile( input.c_str( ) , std::ios::in ) ;
   std::string file( input ) ;
   std::cout << "Which file line do you want to see ? ( Give a number > 0 ) ?\n" ;
   std::getline( std::cin , input ) ;
   int linenumber = std::stoi( input ) ;
   int lines_read = 0 ;
   std::string line ;
   if ( infile.is_open( ) ) {
      while ( infile ) {
	 getline( infile , line ) ;
	 lines_read++ ;
	 if ( lines_read == linenumber ) {
	    std::cout << line << std::endl ;
	    break ; 
	 }
      }
      infile.close( ) ;
      if ( lines_read < linenumber ) 
	 std::cout << "No " << linenumber << " lines in " << file << " !\n" ;
      return 0 ;
   }
   else {
      std::cerr << "Could not find file " << file << " !\n" ;
      return 1 ;
   }
}
```

