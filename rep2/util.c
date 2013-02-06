#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "util.h"

/**
 * 文字列を必要に応じて拡張しながら文字を追加する関数
 * @param char **str 結果の文字列
 * @param char c 入力文字列
 * @param int *len 現在の文字列の長さ
 * @param int *max 現在の文字列の最大長
 */
void set_character(char **str, char c, int *len, int *max)
{
    char *tmp;
    
    // 格納できる領域より大きいなら
    if (*len >= *max)
    {
        (*max) *= 2;  // 領域を2倍に拡張する
        // メモリを再度割り当てる
        if ((tmp = (char *)realloc(*str, sizeof(char) * (*max))) == NULL)
        {
            printf("Memory Reallocation Error\n");
            free(*str);
            exit(1);
        }
        else
        {
            *str = tmp;  // 新しい領域に値を指す
        }
    }
    (*str)[(*len)++] = c; // 文字を追加する
}

/**
 * 入力を読み込む関数
 * @return char * 読み込んだ文字列
 */
char *read_input(void)
{
    int c;
    char *input;    // 入力された文字列
    int len = 0;    // バッファの現在の文字数
    int len_max = BUFFER_SIZE_UNIT; // バッファの文字数の初期単位
    
    // ある程度の大きさで確保
    input = (char *)calloc(len_max, sizeof(char));
    
    // 終わりまで1文字ずつ取得
    while (1)
    {
        c = getc(stdin);    // 文字の取得
        // 改行文字か判別
        if (c == '\n' || c == '\r' || c == EOF)
        {
            // NULL文字を追加しておく
            set_character(&input, '\0', &len, &len_max);
            break;  // 繰り返しを抜ける
        }
        // 空白文字でない場合
        else
        {
            // 文字の保存
            set_character(&input, tolower(c), &len, &len_max);
        }
    }
    
    return input;   // 取得文字列を返す
}

/**
 * 入力をint型として読み込む関数
 * @return int * 読み込んだ整数
 */
int read_input_as_int(void)
{
    char *input;
    int in;
    
    input = read_input();
    in = atoi(input);
    free(input);
    return in;
}