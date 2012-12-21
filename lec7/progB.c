/*
 * csvファイルの内容を表示するプログラム
 * 学籍番号: 岩成達哉
 * 氏名: 03-123006
 */
#include <stdio.h>
#include <string.h>

#define FNAME "address.csv"  // ファイル名
#define MAX_LEN (100)        // 文字列の最大長

int main(void) {
  FILE *fp; // ファイルポインタ
  char fromCSV[MAX_LEN]; // csvファイルからの読み込み
  char *token;           // トークン
  int row, col;          // 行，列のカウント
  
  // ファイルを開く(読み取りモード)ときのエラー処理
  if ( (fp = fopen(FNAME, "r")) == NULL) {
    printf("File Open Error\n");
    return 1;
  }
  
  // 読み取り開始
  row = 1;
  while ( fgets(fromCSV, MAX_LEN, fp) ){
    // 文字列を行単位で渡す
    token = strtok(fromCSV, ", \n");

    // 初めの行なら行数を表示
    if (token != NULL) {
      printf("---- row[%2d] ----\n", row++);
    }
    
    // カラムを表示していく
    col = 1;
    while ( token != NULL) {
      printf("col[%2d]: %s\n", col++, token);
      token = strtok(NULL, ", \n");
    }
  }
  
  // ファイルを閉じる
  fclose(fp);

  return 0;
}
