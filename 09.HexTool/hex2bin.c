/*----------------------------------------------------------------------------/
/  HEX2BIN - Hex to Binary converter  R0.01
/  http://elm-chan.org/
/-----------------------------------------------------------------------------/
/ HEX2BN is a hex file utility intended mainly for embedded developping.
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
	"HEX2BIN - Hex file to Binary file converter R0.01  (C)ChaN, 2018\n"
	"\n"
	"Usage: hex2bin <sw..> <hex_file> [<hex_file>] ...\n"
	"Switch: -B<value> Base address of input hex file [-b0]\n"
	"        -T<value> Top address in output file [-t0x100000]\n"
	"        -O<file>  Output File [-ooutput.bin]\n"
	;

unsigned long Base = 0x00000000;	/* -b<value> Base addres to convert */
unsigned long Top =  0x00100000;	/* -t<value> Max length of output file */
char OutFile[256] = "output.bin";	/* -o<file> Output File */

int UnderRange, OverRange;		/* Error flags */



/*-----------------------------------------------------------------------
  Hex format manupilations
-----------------------------------------------------------------------*/


/* Pick a hexdecimal value from hex record */

unsigned long get_valh (
	char **lp,			/* pointer to line read pointer */
	int count,		 	/* number of digits to get (2,4,6,8) */
	unsigned char *sum	/* byte check sum */
) {
	unsigned long val = 0;
	unsigned char n;


	do {
		n = *(*lp)++;
		if ((n -= '0') >= 10) {
			if ((n -= 7) < 10) return 0xFFFFFFFF;
			if (n > 0xF) return 0xFFFFFFFF;
		}
		val = (val << 4) + n;
		if (count & 1) *sum += (unsigned char)val;
	} while (--count);
	return val;
}




/* Load Intel Hex and Motorola S format file into data buffer */ 

long convert_hexfile (
	FILE* ifp,			/* Input stream in text */
	FILE* ofp,			/* Output file in binary */
	unsigned long base,	/* Base address to convert */
	unsigned long top	/* Max size of output file */
) {
	char line[600];						/* line input buffer */
	unsigned char buff[256];			/* Hex block data buffer */
	char *lp;							/* line read pointer */
	long lnum = 0;						/* input line number */
	unsigned short seg = 0, hadr = 0;	/* address expantion values for intel hex */
	unsigned long addr, count, n, i;
	unsigned char sum;


	while (fgets(line, sizeof line, ifp) != NULL) {
		lnum++;
		lp = &line[1]; sum = 0;

		if (line[0] == ':') {	/* Intel Hex format */
			if ((count = get_valh(&lp, 2, &sum)) > 0xFF) return lnum;	/* byte count */
			if ((addr = get_valh(&lp, 4, &sum)) > 0xFFFF) return lnum;	/* offset */

			switch (get_valh(&lp, 2, &sum)) {	/* record type? */
				case 0x00 :	/* Data record */
					for (i = 0; i < count; i++) {	/* Load data block */
						n = get_valh(&lp, 2, &sum);
						if (n > 0xFF) return lnum;
						buff[i] = (unsigned char)n;
					}
					addr += (seg << 4) + (hadr << 16);
					if (addr < base) {
						UnderRange = 1;
						break;
					}
					addr -= base;
					if (addr >= top) {
						OverRange = 1;
						break;
					}
					if (fseek(ofp, addr, SEEK_SET)) return -1;
					if (fwrite(buff, 1, (size_t)count, ofp) != (size_t)count) return -1;
					break;

				case 0x01 :	/* End record */
					if (count != 0) return lnum;
					break;

				case 0x02 :	/* Segment base record [19:4] */
					if (count != 2) return lnum;
					seg = (unsigned short)get_valh(&lp, 4, &sum);
					if (seg == 0xFFFF) return lnum;
					break;

				case 0x03 :	/* Program start address record (segment:offset) */
					if (count != 4) return lnum;
					get_valh(&lp, 8, &sum);	/* Don't case */
					break;

				case 0x04 :	/* High address base record [31:16] */
					if (count != 2) return lnum;
					hadr = (unsigned short)get_valh(&lp, 4, &sum);
					if (hadr == 0xFFFF) return lnum;
					break;

				case 0x05 :	/* Program start address record (linear) */
					if (count != 4) return lnum;
					get_valh(&lp, 8, &sum);	/* Don't case */
					break;

				default:	/* invalid block */
					return lnum;
			} /* switch */
			if (get_valh(&lp, 2, &sum) > 0xFF) return lnum;	/* Get check sum */
			if (sum) return lnum;							/* Test check sum */
			continue;
		}

		if (line[0] == 'S') {	/* Motorola S format */
			if ((*lp >= '1')&&(*lp <= '3')) {

				switch (*lp++) {	/* record type? (S1/S2/S3) */
					case '1' :
						count = get_valh(&lp, 2, &sum) - 3;
						if (count > 0xFF) return lnum;
						addr = get_valh(&lp, 4, &sum);
						if (addr > 0xFFFF) return lnum;
						break;
					case '2' :
						count = get_valh(&lp, 2, &sum) - 4;
						if (count > 0xFF) return lnum;
						addr = get_valh(&lp, 6, &sum);
						if (addr > 0xFFFFFF) return lnum;
						break;
					default :
						count = get_valh(&lp, 2, &sum) - 5;
						if (count > 0xFF) return lnum;
						addr = get_valh(&lp, 8, &sum);
						if (addr == 0xFFFFFFFF) return lnum;
				}
				for (i = 0; i < count; i++) {	/* Load data block */
					n = get_valh(&lp, 2, &sum);
					if (n > 0xFF) return lnum;
					buff[i] = (unsigned char)n;
				}
				if (addr < base) {
					UnderRange = 1;
				} else {
					addr -= base;
					if (addr >= top) {
						OverRange = 1;
					} else {
						if (fseek(ofp, addr, SEEK_SET)) return -1;
						if (fwrite(buff, 1, (size_t)count, ofp) != (size_t)count) return -1;
					}
				}
				if (get_valh(&lp, 2, &sum) > 0xFF) return lnum;	/* get check sum */
				if (sum != 0xFF) return lnum;					/* test check sum */
			} /* switch */
			continue;
		} /* if */

		if (line[0] >= ' ') return lnum;
	} /* while */

	return feof(ifp) ? 0 : -1;
}



/*-----------------------------------------------------------------------
  Command line analysis
-----------------------------------------------------------------------*/


static
int load_commands (int argc, char** argv)
{
	char *cp, *cmdlst[10], cmdbuff[256];
	int cmd;
	long n;
	FILE *ifp = 0, *ofp = 0;


	cmd = 0; cp = cmdbuff;

	/* Get command line parameters */
	while (--argc && (cmd < (sizeof cmdlst / sizeof cmdlst[0] - 1))) {
		cmdlst[cmd++] = *++argv;
	}
	cmdlst[cmd] = NULL;

	/* Analyze command line parameters... */
	for (cmd = 0; cmdlst[cmd] != NULL; cmd++) {
		cp = cmdlst[cmd];

		if (*cp == '-') {	/* Command switches... */
			cp++;
			switch (tolower(*cp++)) {
				case 'b' :	/* -b<val> Base address */
					Base = strtoul(cp, &cp, 0);
					break;

				case 't' :	/* -t<val> Top address */
					Top = strtoul(cp, &cp, 0);
					break;

				case 'o' :	/* -o<file> Output file */
					strcpy(OutFile, cp);
					break;

				default :	/* invalid command */
					return 1;
			}
			if(*cp >= ' ') return 1;	/* option trails garbage */
		}
		 else {		/* HEX Files... */
			if (!ofp) {
				fprintf(stderr, "Creating \"%s\"...", OutFile);
				ofp = fopen(OutFile, "wb");
				if (!ofp) {
					fprintf(stderr, "Unable to create output file.\n");
					return 2;
				}
				fprintf(stderr, "passed.\n");
			}
			fprintf(stderr, "Converting \"%s\"...", cp);
			ifp = fopen(cp, "rt");
			if (!ifp) {
				fprintf(stderr, "Unable to open.\n");
				return 2;
			}
			n = convert_hexfile(ifp, ofp, Base, Top);
			fclose(ifp);
			if (n) {
				if (n < 0) {
					fprintf(stderr, "file access failure.\n");
				} else {
					fprintf(stderr, "hex format error at line %ld.\n", n);
				}
				return 2;
			}
			fprintf(stderr, "passed.\n");
		}
	}

	if (!ofp) return 1;
	return 0;
}



int main (int argc, char* argv[])
{
	int rc;


	rc = load_commands(argc, argv);
	if (rc) {
		if (rc == 1) fputs(Usage, stderr);
		return rc;
	} else {
		if (UnderRange) fputs("Some data under base address was truncated.\n", stderr);
		if (OverRange) fputs("Some data over max output size was truncated.\n", stderr);
	}
	return rc;
}

