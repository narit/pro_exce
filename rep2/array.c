#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "sword.h"

/**
 *  配列に単語を追加する
 *  @param char *str 追加する文字列
 *  @param int doc_index 現在のドキュメントのID
 *  @return int 挿入位置
 */
int add_array(char *str, int doc_index)
{
    int i;
    
    ARRAY *tmp;
    
    // 配列の中を探す
    for (i = 0; i < n_words; i++)
    {
        if (strcmp(words[i].word, str) == 0)
        {
            if (insert_list(&(words[i].indices), doc_index)) // 単語の出現行番号を保存
                words[i].freq++;    // 重複がなければ出現頻度を増やす
            return i;   // ヒットしたIDを返す
        }
    }
    
    // 単語の格納配列の大きさが足りないとき
    if (n_words >= n_words_max)
    {
        n_words_max += WORD_SIZE_UNIT; // 少し大きくする
        // メモリを再度割り当てる
        if ((tmp = (ARRAY *)realloc(words, sizeof(ARRAY) * n_words_max)) == NULL)
        {
            printf("Memory Reallocation Error\n");
            exit(1);
        }
        else
        {
            words = tmp;  // 新しい領域を指す
        }
    }
    
    // 新しく作成
    words[n_words].word = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    if (words[n_words].word == NULL)
    {
        printf("Memory Allocation Error\n");
        exit(1);
    }
    // 文字列のコピー
    memcpy(words[n_words].word, str, sizeof(char) * (strlen(str) + 1));
    
    // 各種値の初期化
    words[n_words].freq = 1;
    words[n_words].indices.next = NULL;
    insert_list(&(words[n_words].indices), doc_index);  // 現在の行の番号を追加
    words[n_words].second_words.next = NULL;    // 初期化
    
    return n_words++; // 追加した位置を返す
}



/**
 *  2つの単語を関連付ける関数
 *  @param int pre_word_id 一つ前の単語のID
 *  @param int cur_word_id 現在の単語のID
 *  @param char *second_word 追加する文字列
 *  @param int doc_index 現在のドキュメントのID
 *  @return int 格納できたかどうか(0:重複有り, 1:成功)
 */
int associate_words(int pre_word_id, int cur_word_id, char *second_word, int doc_index)
{
    return insert_sword(&(words[pre_word_id].second_words), cur_word_id, second_word, doc_index);
}

/**
 *  配列の中身を表示する関数
 *      for Debug
 */
void show_array()
{
    int i;
    SWORD *p;
    
    for (i = 0; i < n_words; i++)
    {
        printf("%s ->", words[i].word);
        for (p = words[i].second_words.next; p != NULL; p = p->next)
            printf(" %s[%d],", p->word, p->freq);
        putchar('\n');
    }
}

/**
 *  配列の解放を行う関数
 */
void free_array()
{
    int i;
    
    for (i = 0; i < n_words; i++)
    {
        free_sword(words[i].second_words.next); // 次の単語の線形リストを解放
        free_list(words[i].indices.next);   // 転置索引の解放
        free(words[i].word);                // 単語の領域の解放
    }
    free(words);    // 配列の解放
}