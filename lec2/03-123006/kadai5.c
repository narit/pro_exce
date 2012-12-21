/*
 * 行頭の単語のみ表示
 * 学籍番号: 03-123006
 * 氏名: 岩成 達哉(Iwanari Tatsuya)
 */

#include <stdio.h>
#include <ctype.h>  // isspace()のため

#define SPACE 1
#define FRONT 2
#define SKIP 0

int main(void) {
  int c;             // 入力文字
  int state = SPACE; // 状態を持つ

  // 終わりまで1文字ずつ取得
  while ((c = getchar()) != EOF) {
    // 空白文字か判別
    if (isspace(c)) {
      // 改行文字の場合の場合は次の単語が行頭になる
      if (c == '\n') {
	putchar(c);
	state = SPACE;
      }
      else {
        // 行の初めに空白があるなら読み飛ばし
        // 行頭を表示後なら行末まで表示しない 
	if (state == FRONT)
        {
	  state = SKIP;
	}
      }
    }
    else { // 空白文字でない場合
      // 行頭なら
      if (state) {
	putchar(c);
	state = FRONT;
      }
    }
  }

  return 0;
}
