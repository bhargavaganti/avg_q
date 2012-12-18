/*
 * Copyright (C) 2012 Bernd Feige
 * 
 * This file is part of avg_q.
 * 
 * avg_q is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * avg_q is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with avg_q.  If not, see <http://www.gnu.org/licenses/>.
 */
/*{{{}}}*/
/*{{{}}}*/

/*{{{  Includes*/
#include <stdio.h>
#include <stdlib.h>
#ifdef __GNUC__
#include <unistd.h>
#endif
#include <string.h>
#include <read_struct.h>
#ifndef LITTLE_ENDIAN
#include <Intel_compat.h>
#endif
#include "transform.h"
#include "bf.h"
#include "tucker.h"
/*}}}  */

/*{{{  External declarations*/
extern char *optarg;
extern int optind, opterr;
/*}}}  */

char **mainargv;
enum { FILEARG=0, END_OF_ARGS
} args;
#define MAINARG(x) mainargv[optind+x]

int 
main(int argc, char **argv) {
 int filearg;
 int errflag=0, c;

 /*{{{  Process command line*/
 mainargv=argv;
 while ((c=getopt(argc, argv, ""))!=EOF) {
 }

 if (argc-optind<END_OF_ARGS || errflag>0) {
  fprintf(stderr, "Usage: %s [options] filename1 filename2 ...\n"
   " Options are:\n"
  , argv[0]);
  exit(1);
 }

 for (filearg=FILEARG; argc-optind-filearg>=END_OF_ARGS; filearg++) {
  char *filename=MAINARG(filearg);
  FILE *INFILE=fopen(filename,"rb");
  int eventno;

  if(INFILE==NULL) {
   fprintf(stderr, "%s: Can't open file %s\n", argv[0], filename);
   continue;
  }

  struct tucker_header header;
  if (read_struct((char *)&header, sm_tucker, INFILE)==0) {
   fprintf(stderr, "%s: Short file %s\n", argv[0], filename);
   continue;
  }
#ifdef LITTLE_ENDIAN
  change_byteorder((char *)&header, sm_tucker);
#endif
  print_structcontents((char *)&header, sm_tucker, smd_tucker, stdout);

  printf("Event codes:\n");
  for (eventno=0; eventno<header.NEvents; eventno++) {
   char EventCode[5];
   if ((int)fread((void *)&EventCode, 4, 1, INFILE)!=1) {
    fprintf(stderr, "%s: Error reading event code table in file %s\n", argv[0], filename);
    continue;
   }
   EventCode[4]=(char)0;
   printf(" %2d %s\n", eventno+1, EventCode);
  }

  fclose(INFILE);
 }

 return 0;
}
