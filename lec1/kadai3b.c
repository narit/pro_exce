/*
 * nの階乗を計算する
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉(Iwanari Tatsuya)
 */

#include <stdio.h>

/* 階乗の計算をする再帰的関数 */
long long kaijo(int n) {
  if (n == 0) {
    return 1.0;
  }
  else {
    return n * kaijo(n - 1);
  }
}

int main(void) {
  long long n;
  
  printf("nの階乗を計算します\n");
  
  for (n = 0; n <= 30; n++) {
    printf("%2lld ! = %20lld\n", n, kaijo(n));
  }

  return 0;
}
