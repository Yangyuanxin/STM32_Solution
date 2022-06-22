/*----------------------------------------------------------------------------/
/  BIN2HEX - Binary to Hex converter  R0.01
/  http://elm-chan.org/
/-----------------------------------------------------------------------------/
/ BIN2HEX is a hex file utility intended mainly for embedded developping.
/ It is a Free Software opened under license policy of GNU GPL.

   Copyright (C) 2018, ChaN, all right reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



const char *Usage =
	"BIN2HEX - Binary to HEX file converter R0.01  (C)ChaN, 2018\n\n"
	"Usage: bin2hex <switch..> <binfile>\n"
	"Switch: -O<offset>  offset address [-o0]\n"
    "        -F{I|M}     hex file format, intel or motorola [-fi]\n"
	;


unsigned long Base = 0x00000000;
int Format = 0;
int BlockSize = 32;
FILE *fp;


int load_commands (int argc, char **argv)
{
	char *cp, c, *cmdlst[20], cmdbuff[256];
	int cmd;
	unsigned char b;


	cmd = 0; cp = cmdbuff;

	/* Get command line parameters */
	while(--argc && (cmd < (sizeof(cmdlst) / sizeof(cmdlst[0]) - 1)))
		cmdlst[cmd++] = *++argv;
	cmdlst[cmd] = NULL;

	/* Analyze command line parameters... */
	for(cmd = 0; cmdlst[cmd] != NULL; cmd++) {
		cp = cmdlst[cmd];

		if(*cp == '-') {	/* Command switches... */
			cp++;
			switch (tolower(*cp++)) {
				case 'o' :	/* -o<offset> */
					Base = strtoul(cp, &cp, 0);
					break;

				case 'f' :	/* -f<format> */
					c = tolower(*cp++);
					switch (c) {
					case 'i' :
						Format = 0;
						break;
					case 'm' :
						Format = 1;
						break;
					default :
						return 5;
					}
					break;

				case 'b' :	/* -b<blksize> */
					BlockSize = strtoul(cp, &cp, 0);
					for (b = 1; b; b *= 2) {
						if (BlockSize == b) break;
					}
					if (!b) return 5;
					break;

				default :	/* invalid command */
					return 5;
			} /* switch */
			if(*cp >= ' ') return 5;	/* option trails garbage */
		} /* if */
		else {	/* source file */
			if((fp = fopen(cp, "rb")) == NULL) {
				fprintf(stderr, "%s : Unable to open.\n", cp);
				return 2;
			}
		} /* else */

	} /* for */

	return fp ? 0 : 5;
}




/* Put an Intel Hex data block */

void put_hexline (
	FILE *fp,						/* output stream */
	const unsigned char *buffer,	/* pointer to data buffer */
	unsigned short ofs,				/* block offset address */
	unsigned char count,			/* data byte count */
	unsigned char type				/* block type */
) {
	unsigned char sum;

	/* Byte count, Offset address and Record type */
	fprintf(fp, ":%02X%04X%02X", count, ofs, type);
	sum = count + (ofs >> 8) + ofs + type;

	/* Data bytes */
	while (count--) {
		fprintf(fp, "%02X", *buffer);
		sum += *buffer++;
	}

	/* Check sum */
	fprintf(fp, "%02X\n", (unsigned char)-sum);
}


/* Put a Mot Hex data block */

void put_motline (
	FILE *fp,						/* output stream */
	const unsigned char *buffer,	/* pointer to data buffer */
	unsigned long ofs,				/* block offset address */
	unsigned int count,				/* data byte count */
	unsigned int format				/* block type */
) {
	unsigned char sum;

	/* Byte count, Offset address and Record type */
	if (format == 1 || format == 9) {
		fprintf(fp, "S%u%02X%04lX", format, count + 3, ofs);
		sum = count + 3 + (ofs >> 8) + ofs;
	} else if (format == 2 || format == 8) {
		fprintf(fp, "S%u%02X%06lX", format, count + 4, ofs);
		sum = count + 4 + (ofs >> 16) + (ofs >> 8) + ofs;
	} else if (format == 3 || format == 7) {
		fprintf(fp, "S%u%02X%08lX", format, count + 5, ofs);
		sum = count + 5 + (ofs >> 24) + (ofs >> 16) + (ofs >> 8) + ofs;
	} else {
		return;
	}

	/* Data bytes */
	while (count--) {
		fprintf(fp, "%02X", *buffer);
		sum += *buffer++;
	}

	/* Check sum */
	fprintf(fp, "%02X\n", (unsigned char)~sum);
}




int bin2hex (
	FILE* ifp,				/* input stream */
	FILE* ofp,				/* output stream */
	int blksize,			/* Number of bytes per hex block */
	unsigned long ofs		/* Offset address */
) {
	unsigned char buff[128], bs[2];
	unsigned long tail;
	int rd, ext, wrap;


	fseek(ifp, 0, SEEK_END);
	tail = ftell(ifp) + ofs;
	rewind(ifp);
	ext = 0;
	if (tail > 0x10000) ext = 1;
	if (tail > 0x100000) ext = 2;

	wrap = 1;
	while (!feof(ifp)) {
		rd = fread(buff, 1, blksize, ifp);
		if (rd) {
			if (wrap) {
				if (ext == 1) {
					bs[0] = (unsigned char)((ofs & 0xFFFF0000) >> 12);
					bs[1] = 0;
					put_hexline(ofp, bs, 0, 2, 2);
				}
				if (ext == 2) {
					bs[0] = (unsigned char)(ofs >> 24);
					bs[1] = (unsigned char)(ofs >> 16);
					put_hexline(ofp, bs, 0, 2, 4);
				}
				wrap = 0;
			}
			put_hexline(ofp, buff, (unsigned short)ofs, (unsigned char)rd, 0);
		} else {
			if (ferror(ifp)) return 3;
		}
		ofs += rd;
		if ((ofs & 0xFFFF) < rd) wrap = 1;
	}

	put_hexline(ofp, NULL, 0, 0, 1);

	return 0;
}



int bin2mot (
	FILE* ifp,				/* input stream */
	FILE* ofp,				/* output stream */
	int blksize,			/* Number of bytes per hex block */
	unsigned long ofs		/* Offset address */
) {
	unsigned char buff[128];
	unsigned long tail;
	int rd, fmt;


	fseek(ifp, 0, SEEK_END);
	tail = ftell(ifp) + ofs;
	rewind(ifp);
	fmt = 1;
	if (tail > 0x10000) fmt = 2;
	if (tail > 0x100000) fmt = 3;

	while (!feof(ifp)) {
		rd = fread(buff, 1, blksize, ifp);
		if (rd) {
			put_motline(ofp, buff, ofs, (unsigned char)rd, fmt);
		} else {
			if (ferror(ifp)) return 3;
		}
		ofs += rd;
	}

	put_motline(ofp, NULL, 0, 0, 10 - fmt);

	return 0;
}



int main (int argc, char *argv[])
{
	int rc;


	rc = load_commands(argc, argv);
	if(rc) {
		if(rc == 5) fputs(Usage, stderr);
	} else {
		if (Format) {	/* Motorola S format */
			rc = bin2mot(fp, stdout, BlockSize, Base);
		} else {		/* Intel Hex format */
			rc = bin2hex(fp, stdout, BlockSize, Base);
		}
	}
	return rc;
}


