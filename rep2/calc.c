#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"

/**
 *  結果を保存する関数
 *  @param char *first
 *  @param char *second
 *  @param double overlap
 */
void insert_result(char *first, char *second, double overlap)
{
    RESULTS *tmp;
    
    // 結果の格納配列の大きさが足りないとき
    if (n_results >= n_results_max)
    {
        n_results_max += RESULT_SIZE_UNIT; // 少し大きくする
        // メモリを再度割り当てる
        if ((tmp = (RESULTS *)realloc(results, sizeof(RESULTS) * n_results_max)) == NULL)
        {
            printf("Memory Reallocation Error\n");
            free_results();
            exit(1);
        }
        else
        {
            results = tmp;  // 新しい領域を指す
        }
    }
    
    // 結果の文字列のペアを確保
    results[n_results].first  = (char *)malloc(sizeof(char) * (strlen(first)  + 1));
    results[n_results].second = (char *)malloc(sizeof(char) * (strlen(second) + 1));
    if (results[n_results].first == NULL || results[n_results].second == NULL)
    {
        printf("Memory Allocation Error\n");
        exit(1);
    }
    
    // 文字列のコピー
    memcpy(results[n_results].first,  first,  sizeof(char) * (strlen(first)  + 1));
    memcpy(results[n_results].second, second, sizeof(char) * (strlen(second) + 1));
    
    results[n_results].overlap = overlap;   // overlap係数
    
    n_results++;   // ドキュメント数の更新
}

/**
 *  Overlap係数を計算して保存する関数
 */
void calc_overlaps()
{
    int i;
    double overlap;
    SWORD *p;
    
    for (i = 0; i < n_words; i++)
    {
        // 1度しか出現していないときは計算しない
        if (words[i].freq == 1)
            continue;
        
        for (p = words[i].second_words.next; p != NULL; p = p->next)
        {
            // 1度しか出現していないときは計算しない
            if (p->freq == 1)
                continue;
            
            // overlap係数の計算 |S(a, b)| / min(|S(a)|, |S(b)|)
            overlap
            = (double)(p->freq)
            / (double)(words[i].freq < p->freq ? words[i].freq : p->freq);
            insert_result(words[i].word, p->word, overlap);   // 登録
        }
    }
}

/**
 *  クイックソートの比較方法 private
 */
int compare_results(const void *a, const void *b)
{
    double tmp = ((RESULTS *)b)->overlap - ((RESULTS *)a)->overlap;
    int res;
    
    // 大きさの比較をする
    if (tmp > 0.0)
        res =  1;
    else if (tmp < 0.0)
        res = -1;
    else
        res =  0;
    
    return res;
}


/**
 *  結果のソートを行う関数
 */
void sort_results()
{
    qsort(results, n_results, sizeof(RESULTS), compare_results);
}

/**
 *  結果の表示を行う関数
 *  @param int n_display 表示数
 */
void show_results(int n_display)
{
    int i;
    for (i = 0; i < n_display && i < n_results; i++)
        printf("[%5d]: %20s - %20s > %10f\n",
               i+1, results[i].first, results[i].second, results[i].overlap);
}

/**
 *  結果を解放する関数
 */
void free_results()
{
    int i;
    
    for (i = 0; i < n_results; i++)
    {
        free(results[i].first);
        free(results[i].second);
    }
    free(results);
}
