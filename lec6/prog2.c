/*
 * getoptを使わずコマンド引数を処理
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *string;    // 数値変換元文字列
  double *numbers; // 数値
  int size = 0;    // 数値の最大格納数
  char *token;     // トークン文字列
  int n = 0;       // トークン数
  int i;           // for文のカウンタ

  for (i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-n") == 0) {
      // 数値配列の確保
      if (i + 1 < argc) {
	size = atoi(argv[i + 1]);
	numbers = (double *)malloc(sizeof(double) * size);
	if (numbers == NULL) {
	  printf("malloc error\n");
	  return 1;
	}
      }
    }
    else if(strcmp(argv[i], "-s") == 0) {
      // 変換文字列の代入
      if (i + 1 < argc) {
	string = argv[i + 1];
      }
    }
  }

  // 1つ目のトークンを確保
  token = strtok(string, ",");

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
