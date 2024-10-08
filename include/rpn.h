#ifndef _RPN_H_INCLUDED_
#define _RPN_H_INCLUDED_

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include "op.h"
#include "stack.h"

enum erro {
  ERR_NONE,
  ERR_UNMATCH,
  ERR_OVER_FLOW,
  ERR_INVAL_OPR,
  ERR_DIV_ZERO,
  ERR_INVAL_ERRON,
  ERR_COUNT,
};

static char *errors[ERR_COUNT] = {
  [ERR_NONE]        = "",
  [ERR_UNMATCH]     = "The expression contains unmatched parentheses.\n",
  [ERR_OVER_FLOW]   = "The expression contains really big number.\n",
  [ERR_INVAL_OPR]   = "The expression contains invalid operators.\n",
  [ERR_DIV_ZERO]    = "Devided by zero!\n",
  [ERR_INVAL_ERRON] = "Invalid error number.\n",
};

enum erro erron = 0;

static const Opr_Prop oprs[OPR_COUNT] = { /* Operator properties. */
  /*
   * Associativiry will be use only when the precedences are equal.
   * Different programming languages may have different associativity
   * and precedence for the same type of operator.
   */
  [OPR_PLUS]     = { .str = "+", .prec = 1, },
  [OPR_MINUS]    = { .str = "-", .prec = 1, },
  [OPR_MULT]     = { .str = "*", .prec = 2, },
  [OPR_DIV]      = { .str = "/", .prec = 2, },
  [OPR_POW]      = { .str = "^", .prec = 3, .assc = ASSC_RIGHT, },
  [OPR_UNA_MNUS] = { .str = "-", .prec = 4, .assc = ASSC_RIGHT, },
  [OPR_O_PAR]    = { .str = "<open parenthesis>", },
  [OPR_C_PAR]    = { .str = "<close parenthesis>", },
};

static int
rpn_get_prec(Opr opr) { return oprs[opr].prec; }

static int
rpn_get_assc(Opr opr) { return oprs[opr].assc; }

/*
 * This function return an int, not an enumeration,
 * IF assign this function return value to an enum,
 * UNEXPECTED result may happen.
 */
static int
rpn_is_opr(char *str)
{
  for (int i = 0; i < OPR_COUNT; i++)
  {
    if (! strcmp(str, oprs[i].str)) { return i; }
  }
  return -1;
}

static void
rpn_display(Stack *sta)
{
  const Opd_Opr *opd_opr;
  for (int idx = 0; idx <= sta->idx; idx++)
  {
    opd_opr = &sta->nodes[idx];
    switch (opd_opr->op_type)
    {
      case TYPE_OPR:
        printf("%s ", oprs[(Opr)opd_opr->opd_opr].str);
        break;
      case TYPE_OPD:
        printf("%g ", opd_opr->opd_opr);
        break;
      default:
        break;
    }
  }
  printf("\n");
}

static int
rpn_trans_or_not(Opr a, Opr b)
{
  /*
   * o1 is the current token, o2 is the operator on top of the
   * operators stack:
   *
   * + if o2 is not '(', AND
   * + if o2 prec > o1 prec, OR
   *   the prec is the same but o1 is LEFT ASSOCIATIVE,
   *
   * >>transfer o2 to postfix stack<<.
   */
  int is_higher_prec = rpn_get_prec(a) > rpn_get_prec(b);
  int is_same_prec   = rpn_get_prec(a) == rpn_get_prec(b);
  int is_left_assc   = rpn_get_assc(a) == ASSC_LEFT;
  return is_higher_prec || (is_same_prec && is_left_assc);
}

/*
 * If oppr_cmp is NULL, transfer all ops from scr to dest.
 * Otherwise use opr_cmp to compare opr with opr on top of src.
 */
static int
rpn_transfer(Stack *src, Stack *dest,
         int (*opr_cmp)(Opr, Opr),
         Opr opr)
{
  Opd_Opr *top;
  if (opr_cmp)
  {
    while((top = s_top(src))
          && top->op_type == TYPE_OPR
          && (Opr)top->opd_opr != OPR_O_PAR
          && opr_cmp((Opr)top->opd_opr, opr)) { s_push(dest, s_pop(src)); }
  }
  else
  {
    while((top = s_top(src)))
    {
      if (top->op_type == TYPE_OPR
          && (Opr)top->opd_opr == OPR_O_PAR)
      {
        return ERR_UNMATCH;
      }
      s_push(dest, s_pop(src));
    }
  }
  return ERR_NONE;
}

static char *
rpn_c_to_str(char *pc, char c)
{
  *pc = c;
  *(pc + 1) = '\0';
  return pc;
}

double
rpn_str_to_opd(const char *str, char **endptr)
{
  double opd;
  opd = strtod(str, endptr);
  if (errno == ERANGE)
  {
    erron = ERR_OVER_FLOW;
    return 0.0;
  }
  return opd;
}

static int
rpn_in_to_post(const char *expr, Stack *postfix_stack)
{
  const char *pc;
  Stack oprs_stack;
  Opd_Opr opd_opr, prev_opd_opr;
  int opr;
  char opr_str[8] = "\0";
  char *endptr;
  s_init(&oprs_stack);
  for (pc = expr; *pc; pc++)
  {
    if (NDEBUG == 1)
    {
      printf("Token: %c\n", *pc);
    }
    if (*pc >= '0' && *pc <= '9')
    {
      opd_opr.op_type = TYPE_OPD;
      opd_opr.opd_opr = rpn_str_to_opd(pc, &endptr);
      if (erron)
      {
        return erron;
      }
      pc = endptr - 1; /* pc++ later in for loop */
      s_push(postfix_stack, &opd_opr);
    }
    /* if function, push to operator stack */
    else if ((opr = rpn_is_opr(rpn_c_to_str(opr_str, *pc))) >= 0)
    {
      /*
       * Unary operator detection:
       * - "-1", the operator appear at the very start of the input expr
       * - "- -1", after another operator
       * - "(-1)", after a '('
       */
      if (prev_opd_opr.op_type == TYPE_OPR /* '(', '-', '+' all are operators */
          || (s_is_empty(postfix_stack) && s_is_empty(&oprs_stack))) /* or this the first operator */
      {
        if (opr == OPR_MINUS)
        {
          opr = OPR_UNA_MNUS;
        }
        else if (opr == OPR_PLUS)
        {
          /* We can just ignore the unary '+' operator. */
          continue;
        }
        else
        {
          return ERR_INVAL_OPR;
          /* return error code */
        }
      }
      int ret = rpn_transfer(&oprs_stack, postfix_stack, &rpn_trans_or_not, opr);
      if (ret)
      {
        return ret;
      }
      opd_opr.op_type = TYPE_OPR;
      opd_opr.opd_opr = opr;
      s_push(&oprs_stack, &opd_opr);
    }
    else if (*pc == '(')
    {
      /* Push the '(' to the stack as an operator but we do not treat
       * '(' as an operator, rpn_is_opr("(") will return -1; */
      opd_opr.op_type = TYPE_OPR;
      opd_opr.opd_opr = OPR_O_PAR;
      s_push(&oprs_stack, &opd_opr);
    }
    else if (*pc == ')')
    {
      Opd_Opr *opr_top;
      while((opr_top = s_top(&oprs_stack))
            && (Opr)opr_top->opd_opr != OPR_O_PAR)
      {
        s_push(postfix_stack, s_pop(&oprs_stack));
      }
      if (opr_top)
      {
        s_pop(&oprs_stack); /* opp the '(' */
      }
      else
      {
        return ERR_UNMATCH;
      }
      /* if there is a function at the top of operator stack, transfer */
    }
    prev_opd_opr = opd_opr;
    if (NDEBUG == 1)
    {
      printf("Operator stack: ");
      rpn_display(&oprs_stack);
      printf(" Postfix stack: ");
      rpn_display(postfix_stack);
    }
  }
  return rpn_transfer(&oprs_stack, postfix_stack, NULL, '\0');
}

static double
rpn_cal(Opr opr, double a, double b)
{
  double r;
  switch (opr)
  {
    case OPR_PLUS:
      r = a + b;
      break;
    case OPR_MINUS:
      r = a - b;
      break;
    case OPR_MULT:
      r = a * b;
      break;
    case OPR_DIV:
      if (b == 0.0)
      {
        erron = ERR_DIV_ZERO;
        return 0;
      }
      r = a / b;
      break;
    case OPR_POW:
      r = pow(a, b);
      break;
    case OPR_UNA_MNUS:
      r = -a;
      break;
    default:
      break;
  }
  return r;
}

static double
rpn_solve_posfix(Stack *postfix_stack)
{
  Stack opds_stack;
  Node *top_postfix = NULL;
  double a, b, r;
  s_init(&opds_stack);
  while ((top_postfix = s_pop(postfix_stack)))
  {
    if (top_postfix->op_type == TYPE_OPD)
    {
      s_push(&opds_stack, top_postfix);
    }
    else if (top_postfix->op_type == TYPE_OPR)
    {
      if ((Opr)top_postfix->opd_opr == OPR_UNA_MNUS)
      {
        a = s_pop(&opds_stack)->opd_opr;
        r = rpn_cal((Opr)top_postfix->opd_opr, a, 0.0);
        if (erron)
        {
          return 0.0;
        }
      }
      else
      {
        b = s_pop(&opds_stack)->opd_opr;
        a = s_pop(&opds_stack)->opd_opr;
        r = rpn_cal((Opr)top_postfix->opd_opr, a, b);
        if (erron)
        {
          return 0.0;
        }
      }
      Opd_Opr opd = {
        .op_type = TYPE_OPD,
        .opd_opr = r,
      };
      s_push(&opds_stack, &opd);
    }
    if (NDEBUG == 2)
    {
      printf("Operands stack: ");
      rpn_display(&opds_stack);
      printf("Postfix stack: ");
      rpn_display(postfix_stack);
    }
  }
  /* check invalid oprs */
  return s_pop(&opds_stack)->opd_opr;
}

double
rpn_solve(const char *expr)
{
  Stack postfix_stack, inverted_p_s;
  int ret;
  s_init(&postfix_stack);
  ret = rpn_in_to_post(expr, &postfix_stack);
  if (ret)
  {
    erron = ret;
    return 0.0;
  }
  s_init(&inverted_p_s);
  ret = rpn_transfer(&postfix_stack, &inverted_p_s, NULL, '\0');
  if (ret)
  {
    erron = ret;
    return 0.0;
  }
  return rpn_solve_posfix(&inverted_p_s);
}

char *
rpn_err_str(int err_num)
{
  if (err_num >= 0 && err_num <= ERR_COUNT)
  {
    return errors[err_num];
  }
  else
  {
    erron = ERR_INVAL_ERRON;
    return NULL;
  }
}

void rpn_err_clear()
{
  erron = ERR_NONE;
}

#endif /* _RPN_H_INCLUDED_ */

