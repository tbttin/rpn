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
const char *file_name = "tests.in";

FILE *
open_file(const char *fname, char *mode)
{
  FILE *fp = fopen(fname, mode);
  if (! fp)
  {
    fprintf(stderr, "Error: can not open file \"%s\". %s.\n", file_name, strerror(errno));
    exit(EXIT_FAILURE);
  }
  return fp;
}

char *
get_expr(FILE *fp)
{
  char *expr = NULL;
  size_t len = 0; /* allocated */
  ssize_t n_read; /* read */
  while ((n_read = getline(&expr, &len, fp)) != -1)
  {
    expr[n_read - 1] = '\0';
    return expr;
  }
  if (errno)
  {
    fprintf(stderr, "Error while reading \"%s\" file. %s.\n", file_name, strerror(errno));
    exit(errno);
  }
  return NULL;
}

int
main (void)
{
  FILE *fp = open_file(file_name, "r");
  char *expr = NULL;
  double r;
  while ((expr = get_expr(fp)))
  {
    r = rpn_solve(expr);
    if (erron)
    {
      fprintf(stderr, "Error: \"%s\": %s", expr, rpn_err_str(erron));
      rpn_err_clear();
      continue;
    }
    printf("%s = %lf\n", expr, r);
  }
  free(expr);
  fclose(fp);
  return 0;
}

