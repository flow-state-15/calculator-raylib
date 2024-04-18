#ifndef EVALUATE_H_
#define EVALUATE_H_

#include <stdlib.h>
#include <stdbool.h>

#define MAX_TOKEN_STREAM 256
#define MAX_PARSE 256

#define STR_FROM_OP(OP) ""#OP""

// BEGIN: TOKENS
typedef enum {
  OP_SUB, OP_ADD, OP_MUL, OP_DIV,
  OPAREN, CPAREN,
  NUMERIC_I, NUMERIC_F,
  TERMINATOR,
} token_kind;
typedef union { long ival; double fval; } ValueNumeric ;
typedef struct { token_kind kind; ValueNumeric value; } Token;
typedef struct {
  Token *tks;
  size_t n_tks;
  size_t cap;
} TokenStream;
void token_append(TokenStream *, Token);
TokenStream lex_expr(char *);
void token_stream_trace(TokenStream *);
// END: TOKENS
// BEGIN: AST
typedef struct ASTBinaryNode {
  struct ASTBinaryNode *lhs, *rhs;
  Token tkn;
} ASTBinaryNode;
ASTBinaryNode *ast_node_init(Token, ASTBinaryNode *, ASTBinaryNode *);
typedef struct {
  Token *ops;
  size_t sp;
  size_t cap;
} OpStack;
OpStack op_stack_init(size_t cap);
void    op_push(OpStack *, Token);
Token   op_pop(OpStack *);
ASTBinaryNode *parse_stream(TokenStream *);
void ast_parse_trace(ASTBinaryNode *, int);
// END: AST

// BEGIN: EVAL
Token eval_ast(ASTBinaryNode *);
int read_from_eval(char *, Token);
static inline token_kind set_kind(Token *t1, Token *t2) {
  return (t1->kind == NUMERIC_F || t2->kind == NUMERIC_F)
    ? NUMERIC_F
    : NUMERIC_I;
}
static inline double as_float(Token *tkn) {
  return tkn->kind == NUMERIC_F ? tkn->value.fval : (double) tkn->value.ival;
}
static bool found_numeric(ASTBinaryNode *n) {
  return (n->tkn.kind == NUMERIC_F || n->tkn.kind == NUMERIC_I);
}

// END: EVAL
#endif //EVALUATE_H_
