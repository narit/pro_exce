#ifndef _CALC_H_
#define _CALS_H_

#include "array.h"

/**
 *  共起の強度計算方式
 */
typedef enum {
    /** Overlap係数 */
    CM_OVERLAP,
    /** ダイス係数 */
    CM_DICE,
    /** tスコア */
    CM_TSCORE,
    /** 相互情報量(Mutual Information Score) */
    CM_MISCORE,
    /** 要素数 */
    CM_NUM_MODE,
} CALC_MODE;

#define RESULT_SIZE_UNIT 1000

/* 計算結果を格納する構造体 */
typedef struct _results
{
    char *first;    // 1つ目の単語
    char *second;   // 2つ目の単語
    double indicator;   // 強度
} RESULTS;

extern RESULTS **results;   // 結果の一覧
extern int n_results;       // 結果数
extern int n_results_max;   // 最大数

extern ARRAY **words;       // 単語の列
extern int n_words;         // 振ったID = 単語数
extern int n_words_max;     // 最大数

extern int threshold;       // 計算に用いる出現頻度の最小値

/**
 *  結果を保存する関数
 *  @param char *word1
 *  @param char *word2
 *  @param double indicator
 */
void insert_result(char *word1, char *word2, double indicator);

/**
 *  強度を計算して保存する関数
 *  @param CALC_MODE mode 計算する方式
 */
void calc_indicator(CALC_MODE mode);

/**
 *  結果のソートを行う関数
 */
void sort_results();

/**
 *  結果の表示を行う関数
 *  @param const int offset オフセット
 *  @param const int n_display 表示数
 */
void show_results(const int offset, const int n_display);

/**
 *  結果を解放する関数
 */
void free_results();

#endif