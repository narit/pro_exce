#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "list.h"
#include "sword.h"

#define WORD_SIZE_UNIT 100           // 単語数の初期単位

/* 単語用構造体 */
typedef struct _word_array
{
    char *word;     // 単語
    int freq;       // 出現回数
    INDEX indices;  // 転置索引
    SWORD second_words;  // 次の単語の一覧
} ARRAY;

extern ARRAY *words;       // 単語の列
extern int n_words;        // 振ったID = 単語数
extern int n_words_max;    // 最初の大きさ


/**
 *  配列に単語を追加する関数
 *  @param char *str 追加する文字列
 *  @param int doc_index 現在のドキュメントのID
 *  @return int 挿入位置
 */
int add_array(char *strm, int doc_index);

/**
 *  2つの単語を関連付ける関数
 *  @param int pre_word_id 一つ前の単語のID
 *  @param int cur_word_id 現在の単語のID
 *  @param char *second_word 追加する文字列
 *  @param int doc_index 現在のドキュメントのID
 *  @return int 格納できたかどうか(0:重複有り, 1:成功)
 */
int associate_words(int pre_word_id, int cur_word_id, char *second_word, int doc_index);

/**
 *  配列の中身を表示する関数
 *      for Debug
 */
void show_array();

/**
 *  配列の解放を行う関数
 */
void free_array();

#endif