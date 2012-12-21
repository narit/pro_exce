/*
 * 入力の中で偶数番目の文字列のみ表示する
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉(Iwanari Tatsuya)
 */

#include <stdio.h>

int main(void) {
  char c;
  int f_even = 1; // 偶数かどうか
  
  while ((c = getchar()) != EOF) {
    if (f_even = !f_even) { // 偶数のとき
      putchar(c);  // 出力
    }
  }

  return 0;
}
