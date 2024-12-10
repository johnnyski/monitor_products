/*
    NASA/TRMM, Code 910.1.
    This is the TRMM Office Radar Software Library.
    Copyright (C) 1996  John H. Merritt of Applied Research Corporation,
                        Landover, Maryland, a NASA/GSFC on-site contractor.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
	*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define _USE_BSD
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <signal.h>


/* Prototype definitions within this file. */
FILE *input_pipe (FILE *fp, char *cmd);
FILE *output_pipe (FILE *fp, char *cmd);

FILE *input_pipe (FILE *fp, char *cmd)
{
  /* Pass the data begin read from 'fp' first through this input pipe.
   *
   * Illustrating w/ shell syntax:
   *    OLD:  prog < fp
   *    NEW:  cmd < fp | prog
   */
  FILE *fpipe;
  int save_fd;

  save_fd = dup(0);
  close(0); /* Redirect stdin for gzip. */
  dup(fileno(fp));

  /* For example: cmd =  "gzip -d -f --stdout" */
  fpipe = popen(cmd, "r");
  if (fpipe == NULL) {
	perror("input_pipe");
	return fp;
  }
  close(0);
  dup(save_fd);
  return fpipe;
}

FILE *output_pipe (FILE *fp, char *cmd)
{
  /* Pass the data that is was to go to 'fp' first through this
   * filter.  Output goes to the original file.
   *
   * Illustrating w/ shell syntax:
   *    OLD:  prog > fp
   *    NEW:  prog | cmd > fp
   */

  FILE *fpipe;
  int save_fd;

  fflush(NULL); /* Flush all buffered output before opening this pipe. */
  save_fd = dup(1);
  close(1); /* Redirect stdout for gzip. */
  dup(fileno(fp));

  /* For example: cmd = "gzip -1 -c" */
  fpipe = popen(cmd, "w");
  if (fpipe == NULL) {
	perror("output_pipe");
	return fp;
  }
  close(1);
  dup(save_fd);
  return fpipe;
}


