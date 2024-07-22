/*
 *       Author: tbttin
 *         File: rpn.c
 *  Description: Rerverse Polish Notation
 *   Created on: Jul 11 2024 10:54 +0700
 */

#include <fcntl.h>
#include <stdlib.h>

#ifndef NDEBUG
#define NDEBUG 0
#endif /* NDEBUG */

#include "rpn.h"

/* Depends on where the progam are invoked. */
const char *file_name = "expressions.in";

int
main (void)
{
  FILE *fp = fopen(file_name, "r");
  if (! fp)
  {
    fprintf(stderr, "Cant not open file '%s'\n", file_name);
    return 1;
  }
  char *line = NULL;
  size_t len = 0; /* allocated */
  ssize_t n_read; /* read */
  double r;
  while ((n_read = getline(&line, &len, fp)) != -1)
  {
    line[n_read - 1] = '\0';
    printf("%s", line);
    r = rpn_solve(line);
    printf(" = %g\n", r);
  }
  if (errno)
  {
    fprintf(stderr, "Error while reading \"%s\" file.\n", file_name);
    exit(errno);
  }
  free(line);
  fclose(fp);
  return 0;
}

