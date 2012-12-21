/*
 * getoptを使ってコマンド引数を処理
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int c;           // オプション指定
  char *string;    // 数値変換元文字列
  double *numbers; // 数値
  int size;        // 数値の最大格納数
  char *token;     // トークン文字列
  int n = 0;       // トークン数
  int i;           // for文のカウンタ

  while((c = getopt(argc, argv, "n:s:")) != -1){
    switch(c){
    case 'n':
      // 数値配列の確保
      size = atoi(optarg);
      numbers = (double *)malloc(sizeof(double) * size);
      if (numbers == NULL) {
	printf("malloc error\n");
	return 1;
      }
      break;
    case 's':
      // 変換文字列の代入
      string = optarg;
      break;
    }
  }

  // 1つ目のトークンを確保
  token = strtok(string, ",");

  // 数値に変換して代入
  while(token != NULL){
    // 確保した数だけ格納
    if (n < size) {
      numbers[n++] = atof(token);
    }
    else {
      break;
    }
    token = strtok(NULL, ",");
  }

  for (i = 0; i < n; i++) {
    printf("numbers[%2d]:%f\n", i, numbers[i]);
  }

  free(numbers);

  return 0;
}
