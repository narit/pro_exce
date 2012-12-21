/*
 * nの階乗を計算する
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉(Iwanari Tatsuya)
 */

#include <stdio.h>

/* 階乗の計算をする再帰的関数 */
double kaijo(int n) {
  if (n == 0) {
    return 1.0;
  }
  else {
    return (double)n * kaijo(n - 1);
  }
}

int main(void) {
  int n;
  
  printf("nの階乗を計算します\n");
  
  for (n = 0; n <= 30; n++) {
    printf("%2d ! = %20.0lf\n", n, kaijo(n));
  }

  return 0;
}
