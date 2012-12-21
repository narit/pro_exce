/*
 * 2のn乗を計算する
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉(Iwanari Tatsuya)
 */

#include <stdio.h>
#include <math.h>

int main(void) {
  int n;
  
  printf("2のn乗を計算します\n");
  
  for (n = 0; n <= 20; n++) {
    printf("2 ^ %2d = %12.0lf\n", n, pow(2, n));
  }

  return 0;
}
