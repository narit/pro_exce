#ifndef _CALC_H_
#define _CALS_H_

#include "array.h"

#define RESULT_SIZE_UNIT 1000

typedef struct _results
{
    char *first;
    char *second;
    double overlap;
} RESULTS;

extern RESULTS *results;    // 結果の一覧
extern int n_results;       // 結果数
extern int n_results_max;   // 最大数

extern ARRAY *words;    // 単語の列
extern int n_words;     // 振ったID = 単語数
extern int n_words_max; // 最大数

/**
 *  結果を保存する関数
 *  @param char *word1
 *  @param char *word2
 *  @param double overlap
 */
void insert_result(char *word1, char *word2, double overlap);

/**
 *  Overlap係数を計算して保存する関数
 */
void calc_overlaps();

/**
 *  結果のソートを行う関数
 */
void sort_results();

/**
 *  結果の表示を行う関数
 *  @param int n_display 表示数
 */
void show_results(int n_display);

/**
 *  結果を解放する関数
 */
void free_results();

#endif