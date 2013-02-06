/**
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉
 *
 * 文書検索
 *  工夫した点
 *  参考文献
 *      [1]関数のリファレンス
 *      「ライブラリ関数一覧」,
 *      http://www9.plala.or.jp/sgwr-t/lib/libtop.html
 *      [2]クイックソート
 *      「qsort」,
 *      http://www.cc.kyoto-su.ac.jp/~yamada/ap/qsort.html
 *  あとは授業の資料と辞書以外の文献は見てはいません
 */

#include <stdio.h>
#include <ctype.h>  // isspace()のため
#include <stdlib.h> // malloc()のため
#include <string.h> // strcmp()のため
#include "document.h"
#include "array.h"
#include "list.h"
#include "util.h"
#include "calc.h"


// 読み取り時の状態変数
typedef enum {S_SPACE, S_WORD} STATE;

/* 関数のプロトタイプ宣言 START */
void read_file(char *);
/* 関数のプロトタイプ宣言 END */


#define WORD_LENGTH_UNIT 10     // 単語の文字数の初期単位
#define LINE_LENGTH_UNIT 100    // 行の文字数の初期単位


char **document = NULL; // ドキュメントを表す動的配列
int n_docs = 0;         // 振ったID = ドキュメント数
int n_docs_max = DOCUMENT_SIZE_UNIT;    // 最初の大きさ

ARRAY *words = NULL;    // 単語の列
int n_words = 0;        // 振ったID = 単語数
int n_words_max = WORD_SIZE_UNIT;       // 最初の大きさ

RESULTS *results = NULL;    // 結果の一覧
int n_results = 0;          // 結果数
int n_results_max = RESULT_SIZE_UNIT;   // 最初の大きさ

/**
 *  要素の確保を行う関数
 */
void init()
{
    // ある程度の大きさで確保
    document = (char **)malloc(sizeof(char *) * n_docs_max);
    words = (ARRAY *)malloc(sizeof(ARRAY) * n_words_max);
    results = (RESULTS *)malloc(sizeof(RESULTS) * n_results_max);
    if (document == NULL || words == NULL || results == NULL)
    {
        printf("Memory Allocation Error\n");
        exit(1);
    }
}

/**
 *  要素の解放を行う関数
 */
void free_all()
{
    free_document();    // ドキュメントの解放
    free_array();       // 配列の解放
    free_results();     // 結果の解放
}

/**
 * ファイルを読み込んでドキュメントと転置索引を作る関数
 * @param char *file_name 開くファイル名
 */
void read_file(char *file_name)
{
    FILE *fp;
    int c;
    int pre_word_id = -1, cur_word_id;   // 単語のID
    char *word; // 現在の単語
    char *line; // 現在の行
    int word_len = 0;   // 単語の現在の文字数
    int line_len = 0;   // 行の現在の文字数
    int word_len_max = WORD_LENGTH_UNIT;    // 単語の文字数の初期単位
    int line_len_max = LINE_LENGTH_UNIT;    // 行の文字数の初期単位
    STATE state = S_SPACE;    // 状態
    
    if ((fp = fopen(file_name, "r")) == NULL)
    {
        printf("File Open Error: %s\n", file_name);
        exit(1);
    }
    
    // ある程度の大きさで確保
    word = (char *)malloc(sizeof(char) * word_len_max);
    line = (char *)malloc(sizeof(char) * line_len_max);
    
    if (word == NULL || line == NULL)
    {
        printf("Memory Allocation Error\n");
        exit(1);
    }
    
    // 終わりまで1文字ずつ取得
    while (1)
    {
        c = getc(fp);   // 文字の取得
        
        // 空白文字か判別
        if (isspace(c) || c == EOF)
        {
            if (state == S_WORD)    // 文字が入力されていたなら
            {
                // 単語にNULL文字を追加
                set_character(&word, '\0', &word_len, &word_len_max);
                cur_word_id = add_array(word, n_docs);    // 配列に追加
                if (pre_word_id != -1)  // 前回の単語がある場合
                    associate_words(pre_word_id, cur_word_id, word, n_docs); // 関連付け
                
                word_len = 0;               // 単語の文字数の初期化
                pre_word_id = cur_word_id;  // 前回の単語のIDを保存
            }
            
            if (c == '\n' || c == '\r' || c == EOF)  // 改行かファイルの終わりなら
            {
                // 行にNULL文字を追加
                set_character(&line, '\0', &line_len, &line_len_max);
                
                // 行をドキュメントとして保存
                if (strlen(line) > 0)
                    insert_document(line);  // 配列に追加
                
                line_len = 0;       // 行の文字数の初期化
                pre_word_id = -1;   // 前回の単語はない
            }
            else
            {
                // 行の場合は空白も入れる
                set_character(&line, c, &line_len, &line_len_max);
            }
            
            if (c == EOF)   // 繰り返しを抜ける
                break;
            
            state = S_SPACE;    // 状態の変更
        }
        // 空白文字でない場合(単語の場合)
        else
        {
            // 文字の保存
            set_character(&word, tolower(c), &word_len, &word_len_max); // 小文字で保存
            set_character(&line, c, &line_len, &line_len_max);
            state = S_WORD; // 状態の変更
        }
    }
    
    // 解放
    fclose(fp);
    free(word);
    free(line);
}

int main(int argc, char *argv[])
{
    char *file_name = " Usage: -f filename";
    char *input;
    int n_display;
    int i;
    
    init(); // 要素の確保
    
    // コマンドライン引数の読み込み
    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) // 次の文字列があれば
            file_name = argv[i + 1];    // 開くファイルを読み込む
    }

    read_file(file_name);   // ファイル読み込み
    printf("Total number of documents read = %d\n", n_docs);
    printf("Total number of words in the index = %d\n\n", n_words);
    fflush(stdout);
    
    calc_overlaps();    // overlap係数の計算
    printf("calc finished\n");
    fflush(stdout);
    
    sort_results();     // 結果のソート
    
    printf("How many results do you want to display?: ");
    input = read_input();   // 表示数を入力
    n_display = atoi(input);
    
    show_results(n_display);    // 結果の表示
    
    free(input);
    free_all(); // 要素の解放
    
    return 0;
}