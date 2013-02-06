#ifndef _SWORD_H_
#define _SWORD_H_

#include "list.h"

/* 2つ目の単語の線形リスト */
typedef struct _second_word
{
    int id;
    char *word;     // 単語
    int freq;       // 出現回数
    INDEX indices;  // 転置索引
    struct _second_word *next;
} SWORD;

/**
 * 線形リストに要素を追加する関数
 * @param SWORD *head 追加するリストの先頭
 *  @param int cur_word_id 現在の単語のID
 *  @param char *second_word 追加する文字列
 *  @param int doc_index 現在のドキュメントのID
 * @return int 格納できたかどうか(0:重複有り, 1:成功)
 */
int insert_sword(SWORD *head, int cur_word_id, char *second_word, int doc_index);

/**
 * 線形リストの内容を表示する関数
 * @param SWORD *head 表示していくリストの先頭
 *      for Debug
 */
void show_swords(SWORD *head);

/**
 * 線形リストの解放をする関数
 * @param SWORD *head 解放していくリストの先頭
 */
void free_swords(SWORD *head);

#endif