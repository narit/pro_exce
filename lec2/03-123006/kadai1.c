/*
 * 逆数を計算する
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉(Iwanari Tatsuya)
 */

#include <stdio.h>

int main(void) {
  int n;

  printf("1/nを計算します\n");

  for (n = 1; n <= 20; n++) {
    printf("1 / %2d = %lf\n", n, (double)1/n);
  }
  return 0;
}
