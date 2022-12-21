#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "poly.h"

struct poly_t {
  int coefficient;
  int exponent;
  poly_t *next;
};

// static void consume_whitespace(const char *s, int *i) {
//   while (s[*i] == ' ') {
//     *i += 1;
//   }
// }

static int read_coefficient(const char *s, int *i) {
  int res;
  int isnegative;

  // consume_whitespace(s, i);

  res = 0;
  isnegative = 0;
  while (s[*i] != '\0' && s[*i] != 'x') {
    if (isdigit(s[*i])) {
      res = res * 10 + s[*i] - '0';
    } else if (s[*i] == '-') {
      isnegative = 1;
    }
    ++(*i);
  }

  if (res == 0) {
    return 1;
  }

  if (isnegative) {
    res *= -1;
  }
  return res;
}

static int read_exponent(const char *s, int *i) {
  int res;

  if (s[*i] != 'x') {
    return 0;
  } else if (s[++(*i)] != '^') {
    return 1;
  }

  res = 0;
  while (s[*i] != '\0' && s[*i] != ' ') {
    if (isdigit(s[*i])) {
      res = res * 10 + s[*i] - '0';
    }
    ++(*i);
  }

  return res;
}

poly_t *new_poly_from_string(const char *s) {
  poly_t *res;
  int i;

  if (*s == '\0') {
    return NULL;
  }

  res = malloc(sizeof(poly_t));

  i = 0;
  res->coefficient = read_coefficient(s, &i);
  res->exponent = read_exponent(s, &i);
  res->next = new_poly_from_string(s + i);

  return res;
}

void free_poly(poly_t *p) {
  if (p == NULL) {
    return;
  }

  free_poly(p->next);
  free(p);
}

static void simplify(poly_t *p) {
  poly_t *t, *temp;

  if (p == NULL) {
    return;
  }

  while (p != NULL) {
    t = p;
    while (t->next != NULL) {
      if (t->next->exponent == p->exponent) {
        p->coefficient += t->next->coefficient;

        temp = t->next;
        t->next = t->next->next;
        free(temp);
      }
      t = t->next;
    }
    p = p->next;
  }
}

poly_t *mul(poly_t *p0, poly_t *p1) {
  poly_t *p, *pstart, *p1start;
  p = malloc(sizeof(poly_t));
  pstart = p;
  p1start = p1;

  while (p0 != NULL) {
    while (p1 != NULL) {
      p->coefficient = p0->coefficient * p1->coefficient;
      p->exponent = p0->exponent + p1->exponent;
      if (p0->next == NULL && p1->next == NULL) {
        p->next = NULL;
      } else {
        p->next = malloc(sizeof(poly_t));
        p = p->next;
      }
      p1 = p1->next;
    }
    p0 = p0->next;
    p1 = p1start;
  }

  simplify(pstart);
  return pstart;
}

void print_poly(poly_t *p) {
  if (p == NULL) {
    return;
  }

  if (p->coefficient != 1 || p->exponent == 0) {
    if (p->coefficient > 0) {
      printf("%i", p->coefficient);
    } else if (p->coefficient < 0) {
      printf("%i", -p->coefficient);
    }
  }

  if (p->exponent > 0) {
    putchar('x');

    if (p->exponent > 1) {
      putchar('^');
      printf("%i", p->exponent);
    }
  }

  if (p->next == NULL) {
    putchar('\n');
  } else {
    if (p->next->coefficient >= 0) {
      printf(" + ");
    } else {
      printf(" - ");
    }
    print_poly(p->next);
  }
}
