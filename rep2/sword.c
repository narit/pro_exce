#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sword.h"

/**
 * 線形リストに要素を追加する関数
 * @param SWORD *head 追加するリストの先頭
 *  @param int cur_word_id 現在の単語のID
 *  @param char *second_word 追加する文字列
 *  @param int doc_index 現在のドキュメントのID
 * @return int 格納できたかどうか(0:重複有り, 1:成功)
 */
int insert_sword(SWORD *head, int cur_word_id, char *second_word, int doc_index)
{
    SWORD *p, *q, *new_ele;
    
    p = head->next;
    q = head;
    
    while (p != NULL)
    {
        // 同じ単語があるかどうか
        if (strcmp(p->word, second_word) == 0)
        {
            // 二つ目の単語の出現行番号を保存
            if (insert_list(&(p->indices), doc_index))
                (p->freq)++;   // 重複がなければ出現回数を増やす
            return 0;
        }
        q = p;
        p = p->next;
    }
    
    // 同じ物がなければ追加
    new_ele = (SWORD *)malloc(sizeof(SWORD));
    if (new_ele == NULL)
    {
        printf("Memory Allocation Error\n");
        exit(1);
    }
    new_ele->id = cur_word_id;  // 単語のID
    
    // 単語の領域を確保
    new_ele->word = (char *)malloc(sizeof(char) * (strlen(second_word) + 1));
    if (new_ele->word == NULL)
    {
        printf("Memory Allocation Error\n");
        exit(1);
    }
    strcpy(new_ele->word, second_word); // 単語をコピー
    new_ele->freq = 1;              // 出現回数の初期化
    new_ele->indices.next = NULL;   // 初期化
    insert_list(&(new_ele->indices), doc_index);    // リストのIDを追加
    q->next = new_ele;
    new_ele->next = NULL;
    return 1;
}

/**
 * 線形リストの内容を表示する関数
 * @param SWORD *head 表示していくリストの先頭
 *      for Debug
 */
void show_swords(SWORD *head)
{
    SWORD *p, *q;
    
    p = head->next;
    
    // 終わりまで見ていく
    while (p != NULL)
    {
        q = p;
        p = p->next;
        printf("[%d]%s \n", q->id, q->word);
    }
    printf("\n");
}

/**
 * 線形リストの解放をする関数
 * @param INDEX *head 解放していくリストの先頭
 */
void free_sword(SWORD *head)
{
    SWORD *p, *q;
    
    p = head;
    
    // 終わりまで解放していく
    while (p != NULL)
    {
        q = p;
        p = p->next;
        free(q->word);
        free_list((q->indices).next);
        free(q);
    }
}