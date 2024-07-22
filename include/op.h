#ifndef _OP_H_INCLUDED_
#define _OP_H_INCLUDED_

enum opr { /* IDs for supported operators. */
  OPR_PLUS,
  OPR_MINUS,
  OPR_MULT,
  OPR_DIV,
  OPR_POW,
  OPR_UNA_MNUS,
  OPR_O_PAR,
  OPR_C_PAR,
  OPR_COUNT,
};

enum assc { /* Operator's associativity. */
  ASSC_LEFT,
  ASSC_RIGHT,
  ASSC_NONE,
};

enum op_type { /* To determine the current node is a operand or operator. */
  TYPE_OPD,
  TYPE_OPR,
};

struct opr_prop { /* Operator properties. */
  int prec; /* precedence */
  enum assc assc; /* associativity */
  char *str; /* operator literal string ("+", "sin", .etc) */
};

struct node { /* A data structure's node. */
  double opd_opr; /* operator id (enum opr) or operand (double) */
  enum op_type op_type; /* TYPE_OPR, TYPE_OPD */
};

typedef struct node Node; /* for stack usage */
typedef struct node Opd_Opr; /* for more straightforward */
typedef enum opr Opr;
typedef struct opr_prop Opr_Prop;

#endif /* _OP_H_INCLUDED_ */

