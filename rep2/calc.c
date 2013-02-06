#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "calc.h"

/**
 *  結果を保存する関数
 *  @param char *first
 *  @param char *second
 *  @param double overlap
 */
void insert_result(char *first, char *second, double indicator)
{
    RESULTS **tmp;
    
    // 結果の格納配列の大きさが足りないとき
    if (n_results >= n_results_max)
    {
        n_results_max *= 2; // 倍の大きさにする
        // メモリを再度割り当てる
        if ((tmp = (RESULTS **)realloc(results, sizeof(RESULTS *) * n_results_max)) == NULL)
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
    results[n_results] = (RESULTS *)malloc(sizeof(RESULTS));
    results[n_results]->first  = (char *)malloc(sizeof(char) * (strlen(first)  + 1));
    results[n_results]->second = (char *)malloc(sizeof(char) * (strlen(second) + 1));
    if (results[n_results]->first == NULL || results[n_results]->second == NULL)
    {
        printf("Memory Allocation Error\n");
        exit(1);
    }
    
    // 文字列のコピー
    memcpy(results[n_results]->first,  first,  sizeof(char) * (strlen(first)  + 1));
    memcpy(results[n_results]->second, second, sizeof(char) * (strlen(second) + 1));
    
    results[n_results]->indicator = indicator;   // 強度
    
    n_results++;   // 結果の数の更新
}

/**
 *  Overlap係数の計算をする関数
 *  @param ARRAY *ele1 1つ目の単語の要素
 *  @param ARRAY *ele2 2つ目の単語の要素
 *  @param TREE *conb 2つの単語の要素
 *  @return double overlap係数
 */
double calc_overlap(ARRAY *ele1, ARRAY *ele2, TREE *conb)
{
    // overlap係数の計算 |S(a, b)| / min(|S(a)|, |S(b)|)
    return (double)(conb->freq) / (double)(ele1->freq < ele2->freq ? ele1->freq : ele2->freq);
}

/**
 *  ダイス係数の計算をする関数
 *  @param ARRAY *ele1 1つ目の単語の要素
 *  @param ARRAY *ele2 2つ目の単語の要素
 *  @param TREE *conb 2つの単語の要素
 *  @return double ダイス係数
 */
double calc_dice_score(ARRAY *ele1, ARRAY *ele2, TREE *conb)
{
    // ダイス係数の計算 2 * |S(a, b)| / (|S(a)| + |S(b)|)
    return (double)(2 * conb->freq) / (double)(ele1->freq + ele2->freq);
}

/**
 *  tスコアの計算をする関数
 *  @param ARRAY *ele1 1つ目の単語の要素
 *  @param ARRAY *ele2 2つ目の単語の要素
 *  @param TREE *conb 2つの単語の要素
 *  @return double tスコア
 */
double calc_t_score(ARRAY *ele1, ARRAY *ele2, TREE *conb)
{
    // tスコアの計算 [|S(a, b)| - (|S(a)| * |S(b)| / |S(all)|)] / sqrt(|S(a, b)|)
    return ((double)conb->freq
            - (double)(ele1->freq * ele2->freq / n_words))
    / sqrt(conb->freq);
}

/**
 *  相互情報量の計算をする関数
 *  @param ARRAY *ele1 1つ目の単語の要素
 *  @param ARRAY *ele2 2つ目の単語の要素
 *  @param TREE *conb 2つの単語の要素
 *  @return double 相互情報量
 */
double calc_mi_score(ARRAY *ele1, ARRAY *ele2, TREE *conb)
{
    // 相互情報量の計算 log2(|S(a, b)| * |S(all)|) / (|S(a)| * |S(b)|)
    return log((double)(conb->freq * n_words) / (double)(ele1->freq * ele2->freq)) / log(2);
}

/**
 *  強度を計算する再帰関数
 *  @param ARRARY *first 最初の単語が格納された要素
 *  @param TREE *conb 2つの単語の要素
 *  @param double *get_indicator(ARRAY *, ARRAY *, TREE *) 強度を返す関数
 */
void calc_indicator_in_tree(ARRAY *first, TREE *conb,
                            double (*get_indicator)(ARRAY *, ARRAY *, TREE *))
{
    double indicator;
    
    // 要素が登録されていない
    if (conb->left == conb || conb->right == conb)
        return ;
    
    if (conb->left != NULL)   // 右へ
        calc_indicator_in_tree(first, conb->left, get_indicator);
    
    if (conb->right != NULL)  // 右へ
        calc_indicator_in_tree(first, conb->right, get_indicator);
    
    // 閾値よりも低い時は計算しない
    if (words[conb->id]->freq >= threshold)
    {
        // 強度の計算
        indicator = get_indicator(first, words[conb->id], conb);
        insert_result(first->word, words[conb->id]->word, indicator);   // 登録
    }
}

/**
 *  強度を計算して保存する関数
 *  @param CALC_MODE mode 計算する方式
 */
void calc_indicator(CALC_MODE mode)
{
    // 計算方法
    static double (*calc_method[])(ARRAY *, ARRAY *, TREE *)
    = {
        calc_overlap,
        calc_dice_score,
        calc_t_score,
        calc_mi_score,
    };
    int i;
    
    for (i = 0; i < n_words; i++)
    {
        // 閾値よりも低い時は計算しない
        if (words[i]->freq < threshold)
            continue;
        
        calc_indicator_in_tree(words[i],
                               &(words[i]->second_words),
                               calc_method[mode]);
    }
}

/**
 *  クイックソートの比較方法 private
 */
int compare_results(const void *a, const void *b)
{
    double tmp = (*(RESULTS **)b)->indicator - (*(RESULTS **)a)->indicator;
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
    qsort(results, n_results, sizeof(RESULTS *), compare_results);
}

/**
 *  結果の表示を行う関数
 *  @param const int offset オフセット
 *  @param const int n_display 表示数
 */
void show_results(const int offset, const int n_display)
{
    int last_index = offset + n_display;
    int i;
    
    printf("---------- results ---------\n");
    for (i = offset; i < n_results && i < last_index; i++)
        printf("[%5d]: %20s - %20s > %10f\n",
               i+1, results[i]->first, results[i]->second, results[i]->indicator);
    printf("----------------------------\n");
}

/**
 *  結果を解放する関数
 */
void free_results()
{
    int i;
    
    for (i = 0; i < n_results; i++)
    {
        free(results[i]->first);
        free(results[i]->second);
        free(results[i]);
    }
    free(results);
    
    // 情報の初期化
    results = NULL;
    n_results = 0;
    n_results_max = RESULT_SIZE_UNIT;
}
