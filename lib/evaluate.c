#define EXIT_IMPL
#include "exit.h"
#include "evaluate.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


static bool found_numeric(ASTBinaryNode *n) {
  return (n->tkn.kind == NUMERIC_F || n->tkn.kind == NUMERIC_I);
}

void token_append(TokenStream *stream, Token tk) {
  if (stream->n_tks >= stream->cap) PANIC(EXIT_STREAM_OVERFLOW);
  stream->tks[stream->n_tks++] = tk;
}

Token next_token(TokenStream *ts) {
  if (ts->n_tks > 0) {
    Token tk = ts->tks[0];  // Get the first token in the array
    ts->tks++;              // Move the array pointer to the next token
    ts->n_tks--;            // Decrement the count of tokens
    return tk;
  }
  // Return a token that acts as the end-of-stream indicator
  Token end_token = { .kind = TERMINATOR };
  return end_token;
}

TokenStream lex_expr(char *expr) {
  TokenStream stream = { .n_tks = 0, .cap = MAX_TOKEN_STREAM, .tks = NULL };
  stream.tks = (Token *) malloc(stream.cap * sizeof(Token));
  if (stream.tks == NULL) PANIC(EXIT_STREAM_ALLOC_FAIL);
  while (*expr != '\0') {
    if (isspace(*expr)) expr++;
    switch (*expr) {
    case '+': token_append(&stream, (Token) { .kind = OP_ADD }); expr++; break;
    case '-': token_append(&stream, (Token) { .kind = OP_SUB }); expr++; break;
    case '*': token_append(&stream, (Token) { .kind = OP_MUL }); expr++; break;
    case '/': token_append(&stream, (Token) { .kind = OP_DIV }); expr++; break;
    case '(': token_append(&stream, (Token) { .kind = OPAREN }); expr++; break;
    case ')': token_append(&stream, (Token) { .kind = CPAREN }); expr++; break;
    default : {
      if (!isdigit(*expr)) {
        fprintf(stderr, "bad char: %c\n", *expr);
        PANIC(EXIT_STREAM_INVALID_CHAR); 
      }

      char *expr_end; 
      long x = strtol(expr, &expr_end, 10); 

      // REFACTOR with ternaries I think
      if (*expr_end == '.') token_append(&stream, (Token) {
            .kind = NUMERIC_F,
            .value.fval = strtod(expr, &expr_end),
          });
      else token_append(&stream, (Token) {
            /* .kind = NUMERIC_I, */
            /* .value.ival = x */
            .kind = NUMERIC_F,
            .value.fval = x,
          });
      expr = expr_end; 
    }
    }
  }
  return stream;
}

void token_stream_trace(TokenStream *stream) {
  if (stream->n_tks == 0) printf("-- EMPTY STREAM --\n");
  size_t count = 0;
  while (count < stream->n_tks) {
    switch (stream->tks[count].kind) {
    case OP_SUB: printf("[op: %s]", STR_FROM_OP(OP_SUB)); break;
    case OP_MUL: printf("[op: %s]", STR_FROM_OP(OP_MUL)); break;
    case OP_ADD: printf("[op: %s]", STR_FROM_OP(OP_ADD)); break;
    case OP_DIV: printf("[op: %s]", STR_FROM_OP(OP_DIV)); break;
    case OPAREN: printf("[op: %s]", STR_FROM_OP(OPAREN)); break;
    case CPAREN: printf("[op: %s]", STR_FROM_OP(CPAREN)); break;
    case NUMERIC_F:
      printf("[%s: %lf]", STR_FROM_OP(NUMERIC_F),
             stream->tks[count].value.fval);
      break;
    case NUMERIC_I:
      printf("[%s: %ld]", STR_FROM_OP(NUMERIC_I),
             stream->tks[count].value.ival);
      break;
    case TERMINATOR: PANIC(EXIT_STREAM_UNEXPECTED_TERMINATOR);
    }
    count++;
    printf(count < stream->n_tks ? " -> " : "\n");
  }
}

ASTBinaryNode *ast_node_init(Token tk, ASTBinaryNode *lhs, ASTBinaryNode *rhs)
{
  ASTBinaryNode *node = (ASTBinaryNode *) malloc(sizeof(ASTBinaryNode));
  if (node == NULL) PANIC(EXIT_PARSE_NODE_ALLOC_FAIL);
  node->tkn.kind = tk.kind;
  node->tkn.value = tk.value;
  node->lhs = lhs; node->rhs = rhs;
  return node;
}

OpStack op_stack_init(size_t cap) {
  Token *ops = (Token *) malloc(cap * sizeof(Token));
  if (ops == NULL) PANIC(EXIT_PARSE_STACK_ALLOC_FAIL);
  return (OpStack) { .ops = ops, .sp  = 0, .cap = cap };
}

void op_push(OpStack *stack, Token op) {
  if (stack->sp >= stack->cap) PANIC(EXIT_PARSE_OVERFLOW);
  stack->ops[stack->sp++] = op;
}

Token op_pop(OpStack *stack) {
  if (stack->sp <= 0) PANIC(EXIT_PARSE_UNDERFLOW);
  return stack->ops[--stack->sp];
}

// Helper functions
static int precedence(token_kind op) {
  switch(op) {
  case OP_MUL: case OP_DIV: return 2;
  case OP_ADD: case OP_SUB: return 1;
  // no ops
  case CPAREN: case OPAREN:
  case NUMERIC_I: case NUMERIC_F:
  case TERMINATOR:
  default: return 0;
  }
}

static int is_right_associative(token_kind op) {
  (void) op;
  return 0; // all ops are left associative for now
}

ASTBinaryNode* parse_stream(TokenStream *ts) {
  OpStack op_stack = op_stack_init(MAX_PARSE);
  ASTBinaryNode *node_stack[MAX_PARSE];
  size_t node_sp = 0;

  Token tkn;
  while ((tkn = next_token(ts)).kind != TERMINATOR) {
    switch (tkn.kind) {
    case NUMERIC_F:
    case NUMERIC_I:
        node_stack[node_sp++] = ast_node_init(tkn, NULL, NULL); break;
    case OPAREN: op_push(&op_stack, tkn); break;
    case CPAREN:
      while (op_stack.sp > 0 &&
             op_stack.ops[op_stack.sp - 1].kind != OPAREN)
      {
        ASTBinaryNode *rhs = node_stack[--node_sp];
        ASTBinaryNode *lhs = node_stack[--node_sp];
        Token op = op_pop(&op_stack);
        node_stack[node_sp++] = ast_node_init(op, lhs, rhs);
      }
      if (op_stack.sp == 0 || op_stack.ops[op_stack.sp - 1].kind != OPAREN)
        PANIC(EXIT_PARSE_UNBALANCED_PARENS);
      op_pop(&op_stack);
      break;
    case OP_SUB: case OP_MUL: case OP_ADD: case OP_DIV: case TERMINATOR:
    default:  // op_stack
      while (op_stack.sp > 0 &&
             precedence(op_stack.ops[op_stack.sp - 1].kind) >=
             precedence(tkn.kind) &&
             !is_right_associative(tkn.kind)) {
        ASTBinaryNode *rhs = node_stack[--node_sp];
        ASTBinaryNode *lhs = node_stack[--node_sp];
        Token op = op_pop(&op_stack);
        node_stack[node_sp++] = ast_node_init(op, lhs, rhs);
      }
      op_push(&op_stack, tkn);
    }
  }

  // final reduction to a single AST
  while (op_stack.sp > 0) {
    ASTBinaryNode *rhs = node_stack[--node_sp];
    ASTBinaryNode *lhs = node_stack[--node_sp];
    Token op = op_pop(&op_stack);
    node_stack[node_sp++] = ast_node_init(op, lhs, rhs);
  }

  if (node_sp != 1) PANIC(EXIT_PARSE_UNBALANCED_PARENS);
  return node_stack[0];
}

void ast_parse_trace(ASTBinaryNode *node, int depth) {
  if (node == NULL) return;
  // print spaces for current depth level
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }

  switch (node->tkn.kind) {
  case NUMERIC_F: printf("%.2lf\n", node->tkn.value.fval); break;
  case NUMERIC_I: printf("%ld\n", node->tkn.value.ival); break;
  case OP_ADD: printf("+\n"); break;
  case OP_SUB: printf("-\n"); break;
  case OP_MUL: printf("*\n"); break;
  case OP_DIV: printf("/\n"); break;
  case OPAREN: case CPAREN: case TERMINATOR:
  default: printf("Unknown\n");  // UNREACHABLE
  }
  ast_parse_trace(node->lhs, depth + 1);
  ast_parse_trace(node->rhs, depth + 1);
}

Token eval_ast(ASTBinaryNode *node) {
  if (found_numeric(node)) return node->tkn;

  Token a = eval_ast(node->lhs);
  Token b = eval_ast(node->rhs);

  Token result = (Token) { .kind = set_kind(&a, &b) };

  switch (node->tkn.kind) {
  case OP_ADD:
    if (result.kind == NUMERIC_I)
      result.value.ival = a.value.ival + b.value.ival;
    else result.value.fval = as_float(&a) + as_float(&b);
    break;
  case OP_SUB:
    if (result.kind == NUMERIC_I)
      result.value.ival = a.value.ival - b.value.ival;
    else result.value.fval = as_float(&a) - as_float(&b);
    break;
  case OP_MUL:
    if (result.kind == NUMERIC_I)
      result.value.ival = a.value.ival * b.value.ival;
    else result.value.fval = as_float(&a) * as_float(&b);
    break;
  case OP_DIV:
    if (result.kind == NUMERIC_I) {
      if (b.value.ival == 0) PANIC(EXIT_DIVIDE_BY_ZERO);
      result.value.ival = a.value.ival / b.value.ival;
    }
    else {
      if (b.value.fval == 0) PANIC(EXIT_DIVIDE_BY_ZERO);
      result.value.fval = as_float(&a) / as_float(&b);
    }
    break;
  default: break;
  }
  return result;
}

int read_from_eval(char *buffer, Token tkn) {
  switch (tkn.kind) {
  case NUMERIC_F: return sprintf(buffer, "%g",  tkn.value.fval);
  case NUMERIC_I: return sprintf(buffer, "%ld", tkn.value.ival);
  default: return -1;
  }
}
