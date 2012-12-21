/*
 * 入力された文章の単語ごとにカウントする
 * 学籍番号: 03-123006
 * 氏名: 岩成 達哉(Iwanari Tatsuya)
 */

#include <stdio.h>
#include <ctype.h>  // isspace()のため
#include <stdlib.h> // malloc()のため
#include <string.h> // strcmp()のため

#define S_SPACE 0
#define S_WORD 1

typedef char * ELEMENT;  // クイックソートに渡す配列の要素の型

// ----- 関数のプロトタイプ宣言 START -----
/* 文字列の格納 */
char** store_string(int *n_words);

/* クイックソート */
void quick(ELEMENT x[ ], int left, int right);

/* 要素の比較 */
int compare(ELEMENT x1, ELEMENT x2);

/* x[i]とx[j]の入れ替え */
void swap(ELEMENT x[ ], int i, int j);

/* 結果の表示 */
void show_result(char **words, int n_words);

/* メモリの解放 */
void free_words(char **words, int n_words);
// ----- 関数のプロトタイプ宣言 END -----



/* 文字列の格納 */
char** store_words(int *n_words) {
  int c;               // 入力文字列
  char **words;        // 文字列のリスト
  int num = 0;         // 単語数
  int len = 0;         // 文字列の長さ
  int state = S_SPACE; // 状態を管理
  
  // 初期化
  if ( (words = (char **)malloc(0)) == NULL) {
    printf("Memory Allocatiton Error\n");
    exit(1);
  }

  // 終わりまで1文字ずつ取得
  while ((c = getchar()) != EOF) {
    // 空白文字か判別
    if (isspace(c)) {
      if (state == S_WORD) {
	// 文字長を拡張
	if ( (words[num] = (char *)realloc(words[num], sizeof(char) * (len + 1))) == NULL) {
	  printf("Memory Allocatiton Error\n");
	  exit(1);
	}
       	words[num][len] = '\0'; // ヌル文字の挿入
	num++; // 次の単語へ
	len = 0;   // 文字数の初期化
      }
      state = S_SPACE;  // 状態の変更
    } 
    // 空白文字でない場合(単語の場合)    
    else {
      if (state == S_SPACE) {
	// 単語数の拡張
	if ( (words = (char **)realloc(words, sizeof(char *) * (num + 1))) == NULL) {
	  printf("Memory Allocatiton Error\n");
	  exit(1);
	}
      }
      if (len == 0) {
	// 単語の領域を確保
	if ( (words[num] = (char *)malloc(0)) == NULL) {
	  printf("Memory Allocatiton Error\n");
	  exit(1);
	}
      }
      // 文字長を拡張
      if ( (words[num] = (char *)realloc(words[num], sizeof(char) * (len + 1))) == NULL) {
	printf("Memory Allocatiton Error\n");
	exit(1);
      }
      words[num][len++] = c;  // 文字の保存
      state = S_WORD; // 状態の変更
    }
  }
  *n_words = num; // 単語数
  return words;   // 単語のリスト
}

/* クイックソート */
void quick(ELEMENT x[ ], int left, int right)
{
  int i, j;
  ELEMENT pivot;

  i = left;   // ソートする配列の一番小さい要素の添字
  j = right;  // ソートする配列の一番大きい要素の添字

  // 基準値を配列の中央
  pivot = x[(left + right) / 2];
  while (1) {
    // pivotより大きい値になるまで
    while (compare(x[i], pivot) < 0) {
      i++;
    }

    // pivotより小さい値になるまで
    while (compare(pivot, x[j]) < 0) {
      j--;
    }
    // i >= j なら終わり
    if (i >= j) {
      break;
    }
    
    swap(x, i, j);  // x[i]とx[j]を交換
    i++; // 次のデータへ
    j--; // 
  }
  
  // 左の配列をクイックソート
  if (left < i - 1) {
    quick(x, left, i - 1);
  }
  // 右の配列をクイックソート
  if (j + 1 <  right) {
    quick(x, j + 1, right);
  }
}

/* 要素の比較 */
int compare(ELEMENT x, ELEMENT y) {
  return strcmp(x, y);
}

/* 配列の要素を交換する */
void swap(ELEMENT x[ ], int i, int j)
{
  ELEMENT temp;

  temp = x[i];
  x[i] = x[j];
  x[j] = temp;
}

/* 結果の表示 */
void show_result(char **words, int n_words) {
  int count;
  int i;

  // 結果の表示
  for (i = 0, count = 1; i < n_words - 1; i++) {
    if (strcmp(words[i], words[i+1]) == 0) {
      count++;
    }
    else {
      printf("出現頻度: %5d, 単語: %s\n", count, words[i]);
      count = 1;
    }
  }
  printf("出現頻度: %5d, 単語: %s\n", count, words[i]);
}

/* メモリの解放 */
void free_words(char **words, int n_words) {
  int i;
  for (i = 0; i < n_words; i++) {
    free(words[i]);
  }
  free(words);
}

int main(void) {
  int n_words;  // 単語数
  char **words; // 文字のリスト
  int i;

  words = store_words(&n_words); // 文字列の格納
  printf("読み取り完了\n");
  
  quick(words, 0, n_words - 1);  // クイックソート
  printf("ソート完了\n");
  
  printf("結果の表示\n");
  show_result(words, n_words);   // 結果の表示

  free_words(words, n_words);    // メモリの解放
  printf("メモリの解放完了\n");

  return 0;
}
