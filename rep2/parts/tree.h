#ifndef _TREE_H_
#define _TREE_H_

#include "list.h"

/* 2つ目の単語の2分木 */
typedef struct _second_word
{
    int id;         // 単語のID
    int freq;       // 出現回数
    INDEX indices;  // 転置索引
    struct _second_word *left;
    struct _second_word *right;
} TREE;

/**
 *  木に要素を追加する関数
 *  @param TREE *root 木の根
 *  @param int cur_word_id 追加する単語のID
 *  @param int doc_index 現在のドキュメントのID
 *  @return int 格納できたかどうか(0:重複有り, 1:成功)
 */
int insert_tree(TREE *root, int cur_word_id, int doc_index);

/**
 *  木の内容を表示する関数(帰りがけ順)
 *  @param TREE *p 表示したい部分木
 *      for Debug
 */
void show_tree(TREE *p);

/**
 *  木を解放する関数(通りがけ順で削除)
 *  @param TREE *root 解放する部分木の根
 *  @param TREE *p 解放する部分木の節
 */
void free_tree(TREE *root, TREE *p);

#endif