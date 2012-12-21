/*
 * csvファイルに新規書き込みを行うプログラム
 * 学籍番号: 岩成達哉
 * 氏名: 03-123006
 */
#include <stdio.h>

#define FNAME "address.csv"  // ファイル名
#define MAX_LEN (100)        // 文字列の最大長

int main(void) {
  FILE *fp; // ファイルポインタ
  char name[MAX_LEN], address[MAX_LEN]; // 格納するデータ

  // ファイルを開く(上書きモード)ときのエラー処理
  if ( (fp = fopen(FNAME, "w")) == NULL) {
    printf("File Open Error\n");
    return 1;
  }

  // 名前の入力
  printf("name: ");
  scanf("%s", name);

  // 住所の入力
  printf("address: ");
  scanf("%s", address);
  
  // ファイルへ出力
  fprintf(fp, "%s,%s\n", name, address);
  
  // ファイルを閉じる
  fclose(fp);

  return 0;
}
