/*
 * 入力された文章の単語を長さごとにカウントしてヒストグラムを表示するプログラム
 * 学籍番号: 03-123006
 * 氏名: 岩成 達哉(Iwanari Tatsuya)
 */

#include <stdio.h>
#include <ctype.h>  // isspace()のため

#define MAX_LENGTH 20

int main(void) {
  int count[MAX_LENGTH] = {0};  // 文字数の結果格納用
  int c;                // 入力文字
  int length;           // 単語の長さ
  int n_line = 0, n_word = 0, n_char = 0;// 行数，単語数，文字数(改行文字を含む)
  int max;
  int i, j;

  // 終わりまで1文字ずつ取得
  while ((c = getchar()) != EOF) {
    
    // 空白文字か判別
    if (isspace(c)) {
      // 改行文字の場合は行数をカウント
      if (c == '\n') {
	++n_line;
      }
      // 単語の長さを記録
      if (length > 0 && length <= MAX_LENGTH) {
	++count[length-1];
      }
      ++n_word;    // 単語の数をカウント
      length = 0;  // 単語の長さを初期化
    
    // 空白文字でない場合(単語の場合)
    } else {
      ++length;    // 単語の長さをカウント
    }
    ++n_char;      // 文字数をカウント
  }

  // 以下，結果の表示
  printf("#line = %d, #word = %d, #character = %d\n", n_line, n_word, n_char);
  printf("-----\n");

  for (i = 0; i < MAX_LENGTH; ++i) {
    printf("%2d: %d\n", i+1, count[i]);
  }

  printf("-----\n");

  // 最大値を求める
  max = (count[0] /= 50);   // 最大値の初期化
  for (i = 1; i < MAX_LENGTH; ++i) {
    if (max < (count[i] /= 50)) {
      max = count[i];
    }
  }
  
  // ヒストグラムの表示
  for (i = max; i > 0; --i) {
    for (j = 0; j < MAX_LENGTH; ++j) {
      if (i <= count[j]) {
	printf("  #");
      }
      else {
	printf("   ");
      }
    }
    printf("\n");
  }
  
  printf("-----\n");

  // 数字の表示
  for (i = 0; i < MAX_LENGTH; ++i) {
    printf("%3d", i+1);
  }
  printf("\n");
  
  return 0;
}
