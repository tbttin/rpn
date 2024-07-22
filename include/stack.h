#ifndef _STACK_H_INCLUDED_
#define _STACK_H_INCLUDED_

struct stack {
  Node nodes[32];
  int idx;
};

typedef struct stack Stack;

void
s_init(Stack *sta) { sta->idx = -1; }

int
s_is_empty(Stack *sta) { return sta->idx < 0; }

int
s_push(Stack *sta, Node *node)
{
  sta->nodes[++sta->idx] = *node;
  return 0;
}

Node *
s_top(Stack *sta)
{
  if (s_is_empty(sta)) return NULL;
  return &sta->nodes[sta->idx];
}

Node *
s_pop(Stack *sta)
{
  if (s_is_empty(sta)) return NULL;
  return &sta->nodes[sta->idx--];
}

#endif /* _STACK_H_INCLUDED_ */

