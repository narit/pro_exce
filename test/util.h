#ifndef _UTIL_H_
#define _UTIL_H_

#define BUFFER_SIZE_UNIT 100    // バッファの大きさの初期単位

/**
 * 文字列を必要に応じて拡張しながら文字を追加する関数
 * @param char **str 結果の文字列
 * @param char c 入力文字列
 * @param int *len 現在の文字列の長さ
 * @param int *max 現在の文字列の最大長
 */
void set_character(char **str, char c, int *len, int *max);

/**
 * 入力を読み込む関数
 * @return char * 読み込んだ文字列
 */
char *read_input(void);

#endif