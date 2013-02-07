#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/**
 * 線形リストに索引を追加する関数
 * @param INDEX *head 追加するリストの先頭
 * @param int id 追加する索引
 * @return int 格納できたかどうか(0:重複有り, 1:成功)
 */
int insert_list(INDEX *head, int id)
{
    INDEX *p, *q, *new_ele;
    
    p = head->next;
    q = head;
    
    while (p != NULL)
    {
        if (p->id == id)    // 同じものがあるなら追加しない
            return 0;
        q = p;
        p = p->next;
    }
    
    // 同じ物がなければ追加
    new_ele = (INDEX *)malloc(sizeof(INDEX));
    if (new_ele == NULL)
    {
        fprintf(stderr, "Memory Allocation Error\n");
        exit(1);
    }
    new_ele->id = id;
    q->next = new_ele;
    new_ele->next = NULL;
    return 1;
}

/**
 * 線形リストの内容を表示する関数
 * @param INDEX *head 表示していくリストの先頭
 *      for Debug
 */
void show_list(INDEX *head)
{
    INDEX *p, *q;
    
    p = head->next;
    
    // 終わりまで見ていく
    while (p != NULL)
    {
        q = p;
        p = p->next;
        printf("-> %d ", q->id);
    }
    printf("\n");
}

/**
 * 線形リストの解放をする関数
 * @param INDEX *head 解放していくリストの先頭
 */
void free_list(INDEX *head)
{
    INDEX *p, *q;
    
    p = head;
    
    // 終わりまで解放していく
    while (p != NULL)
    {
        q = p;
        p = p->next;
        free(q);
    }
}