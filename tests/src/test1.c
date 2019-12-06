#include <stdint.h>

struct BigStruct {
  uint64_t a;
  uint64_t b;
  uint64_t c;
};

struct LargerStruct {
	struct BigStruct a;
	struct BigStruct b;
};

int main() {
  char* val = malloc(sizeof(int)*50);
  struct LargerStruct d;
  d.a.a = 20;
  int sum = 0;
  for (int i = 0; i < 10; ++i) {
	  printf("%d\n", d.a.a);
	  sum += i;
  }
printf("%d\n", sum);
  return 0;
}
