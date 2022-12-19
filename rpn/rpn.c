#include <ctype.h>
#include <stdio.h>
#include <string.h>

struct stack {
  int arr[10];
  int len;
};

void push(struct stack *s, int e) {
  if (s->len < 10) {
    s->arr[s->len++] = e;
  }
}

int pop(struct stack *s) {
  if (s->len > 0) {
    int res = s->arr[s->len - 1];
    s->arr[s->len--] = 0;
    return res;
  }
  return 0;
}

int peek(struct stack *s) {
  if (s->len > 0) {
    return s->arr[s->len - 1];
  }
  return 0;
}

int read_int() {
  int res, c;

  res = 0;
  while (isdigit(c = getchar())) {
    res = res * 10 + c - '0';
  }
  ungetc(c, stdin);
  return res;
}

int isoperator(int arg) {
  return arg == '+' || arg == '-' || arg == '*' || arg == '/';
}

int iswhitespace(int arg) { return arg == ' '; }

void read_until_newline() {
  while (getchar() != '\n')
    ;
}

void print_error(int nbr, char c) {
  printf("line %i: error at %c\n", nbr, c);
  read_until_newline();
}

int evaluate_line(struct stack s, int nbr) {
  memset(s.arr, 0, 10 * sizeof(int));
  s.len = 0;

  int c;
  while (1) {
    c = getchar();

    if (c == EOF) {
      return 1;
    } else if (c == '\n') {
      if (s.len != 1) {
        printf("line %i: error at \\n\n", nbr);
      } else {
        printf("line %i: %i\n", nbr, peek(&s));
      }
      return 0;
    } else if (isdigit(c)) {
      if (s.len == 10) {
        print_error(nbr, c);
        return 0;
      }
      ungetc(c, stdin);
      push(&s, read_int());
    } else if (isoperator(c)) {
      if (s.len < 2) {
        print_error(nbr, c);
        return 0;
      }
      int rhs = pop(&s);
      int lhs = pop(&s);
      if (c == '+') {
        push(&s, lhs + rhs);
      } else if (c == '-') {
        push(&s, lhs - rhs);
      } else if (c == '*') {
        push(&s, lhs * rhs);
      } else if (c == '/') {
        if (rhs == 0) {
          print_error(nbr, c);
          return 0;
        }
        push(&s, lhs / rhs);
      }
    } else if (!iswhitespace(c)) {
      print_error(nbr, c);
      return 0;
    }
  }
}

int main(void) {
  int nbr;
  struct stack s;

  nbr = 0;
  while (!evaluate_line(s, ++nbr))
    ;
}
