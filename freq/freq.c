#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bucket {
  char *key;
  int value;
  struct bucket *next;
} bucket_t;

void free_bucket(bucket_t *b) {
  if (b == NULL) {
    return;
  }

  free_bucket(b->next);
  free(b->key);
  free(b);
}

int strhash(const char *str) {
  const char *p;
  int g;
  int h = 0;

  for (p = str; *p != '\0'; p++) {
    h = (h << 4) + (int)(*p);
    g = h & 0xF0000000L;
    if (g != 0) {
      h = h ^ (g >> 24);
    }
    h = h & ~g;
  }
  return h % 500;
}

void add(bucket_t **ht, const char *str) {
  int idx;
  bucket_t *b;
  char *p;

  idx = strhash(str);

  b = malloc(sizeof(bucket_t));
  p = malloc(strlen(str) + 1);
  strcpy(p, str);
  b->key = p;
  b->value = 1;
  b->next = ht[idx];

  ht[idx] = b;
}

bucket_t *search(bucket_t **ht, char *str) {
  int idx;
  bucket_t *b;

  idx = strhash(str);

  if (ht[idx] == NULL) {
    return NULL;
  }

  b = ht[idx];
  while (b != NULL) {
    if (strcmp(str, b->key) == 0) {
      return b;
    }
    b = b->next;
  }
  return NULL;
}

int isprime(int n) {
  int i;

  for (i = 2; i <= n / 2; ++i) {
    if (n % i == 0) {
      return 0;
    }
  }
  return n != 1;
}

int main(void) {
  bucket_t *ht[500];
  bucket_t *found, *b;
  char buffer[64];
  char *str, *maxstr;
  int nbr, i, max;

  memset(ht, 0, sizeof(ht));
  nbr = 1;
  while ((str = fgets(buffer, 64, stdin)) != NULL) {
    buffer[strlen(buffer) - 1] = '\0';
    found = search(ht, buffer);
    if (isprime(nbr)) {
      if (found == NULL || found->value == 0) {
        printf("trying to delete %s: not found\n", buffer);
      } else {
        found->value = 0;
        printf("trying to delete %s: deleted\n", buffer);
      }
    } else {
      if (found == NULL || found->value == 0) {
        add(ht, buffer);
        printf("added %s\n", buffer);
      } else {
        ++found->value;
        printf("counted %s\n", buffer);
      }
    }
    ++nbr;
  }

  maxstr = "";
  max = 0;
  for (i = 0; i < 500; ++i) {
    if (ht[i] != NULL) {
      b = ht[i];
      while (b != NULL) {
        if (b->value > max) {
          maxstr = b->key;
          max = b->value;
        } else if (b->value == max) {
          if (strcmp(b->key, maxstr) < 0) {
            maxstr = b->key;
            max = b->value;
          }
        }
        b = b->next;
      }
    }
  }
  printf("result: %s %i\n", maxstr, max);

  for (i = 0; i < 500; ++i) {
    free_bucket(ht[i]);
  }
  return 0;
}
