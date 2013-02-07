#ifndef _LIST_H_
#define _LIST_H_

/* 索引の線形リスト用構造体 */
typedef struct _index
{
    int id;
    struct _index *next;
} INDEX;

/**
 * 線形リストに索引を追加する関数
 * @param INDEX *head 追加するリストの先頭
 * @param int id 追加する索引
 * @return int 格納できたかどうか(0:重複有り, 1:成功)
 */
int insert_list(INDEX *head, int id);

/**
 * 線形リストの内容を表示する関数
 * @param INDEX *head 表示していくリストの先頭
 *      for Debug
 */
void show_list(INDEX *head);

/**
 * 線形リストの解放をする関数
 * @param INDEX *head 解放していくリストの先頭
 */
void free_list(INDEX *head);

#endif