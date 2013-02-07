/**
 * 学籍番号: 03-123006
 * 学年: 3年(工学部編入生)
 * 氏名: 岩成達哉
 *
 * 共起表現のリストアップ
 *  工夫した点
 *      ・強度を計算する方法をいくつか設けた
 *          ・Overlap係数
 *          ・ダイス係数
 *          ・tスコア
 *          ・相互情報量(MIスコア)
 *          ・MI3スコア
 *          ・Zスコア
 *      ・計算に用いるかどうかの閾値を入力できるようにした
 *      ・出力の範囲を指定できるようにした
 *      ・CSV形式でファイルに主力できるようにした
 *  参考文献
 *      [1]関数のリファレンス
 *          「ライブラリ関数一覧」，
 *          http://www9.plala.or.jp/sgwr-t/lib/libtop.html
 *      [2]クイックソート
 *          「qsort」，
 *          http://www.cc.kyoto-su.ac.jp/~yamada/ap/qsort.html
 *      [3]共起の強度の計算方法1
 *              ・ダイス係数
 *              ・tスコア
 *              ・相互情報量(MIスコア)
 *          「コロケーションの強度をどう測るか ―ダイス係数,t スコア,相互情報量を中心として―」，
 *          http://language.sakura.ne.jp/s/ilaa/ishikawa_20080317.pdf
 *      [4]共起の強度の計算方法2
 *              ・MI3スコア
 *              ・Zスコア
 *          「取得したKWICデータに対する統計を取る（Collocation）」，
 *          http://en.sourceforge.jp/projects/chaki/wiki/%E5%8F%96%E5%BE%97%E3%81%97%E3%81%9FKWIC%E3%83%87%E3%83%BC%E3%82%BF%E3%81%AB%E5%AF%BE%E3%81%99%E3%82%8B%E7%B5%B1%E8%A8%88%E3%82%92%E5%8F%96%E3%82%8B%EF%BC%88Collocation%EF%BC%89
 *      [5]強度の計算方法の説明
 *          「共起性の計算方法」
 *          http://c-faculty.chuo-u.ac.jp/~rhotta/course/2009a/hellog/cat_collocation.html
 *      [6]2分木への要素の
 *              ・挿入(insert_tree)
 *              ・表示(show_tree)
 *              ・削除(free_tree)[show_treeをもとに]
 *          「C言語アルゴリズム-２分木」，
 *          http://masudahp.web.fc2.com/cl/algo/btree01.html
 *  あとは授業の資料と辞書以外の文献は見てはいません
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// -----------------document-------------------
#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#define DOCUMENT_SIZE_UNIT 100  // ドキュメント数の初期単位
char **document = NULL; // ドキュメントを表す動的配列
int n_docs = 0;         // 振ったID = ドキュメント数
int n_docs_max = DOCUMENT_SIZE_UNIT;    // 最初の大きさ

/* 関数のプロトタイプ宣言 START */
void insert_document(char *str);
void show_document(void);
void free_document(void);
/* 関数のプロトタイプ宣言 END */

/**
 *  ドキュメントを保存する関数
 *  @param char *str 追加するドキュメントの文字列
 */
void insert_document(char *str)
{
    char **tmp;
    
    // ドキュメント格納配列の大きさが足りないとき
    if (n_docs >= n_docs_max)
    {
        n_docs_max *= 2; // 倍の大きさにする
        // メモリを再度割り当てる
        if ((tmp = (char **)realloc(document, sizeof(char *) * n_docs_max)) == NULL)
        {
            fprintf(stderr, "Memory Reallocation Error\n");
            free_document();
            exit(1);
        }
        else
        {
            document = tmp;  // 新しい領域を指す
        }
    }
    // ドキュメントの要素を確保
    document[n_docs] = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    if (document[n_docs] == NULL)
    {
        fprintf(stderr, "Memory Allocation Error\n");
        exit(1);
    }
    // 文字列のコピー
    memcpy(document[n_docs], str, sizeof(char) * (strlen(str) + 1));
    n_docs++;   // ドキュメント数の更新
}

/**
 *  ドキュメントの一覧を表示する関数
 *      for Debug
 */
void show_document(void)
{
    int i;
    
    for (i = 0; i < n_docs; i++)
        printf("doc[%3d]: %s\n", i, document[i]);
}

/**
 *  ドキュメントのメモリ解放をする関数
 */
void free_document(void)
{
    int i;
    
    for (i = 0; i < n_docs; i++)    // ドキュメント数だけ解放
        free(document[i]);
    
    free(document);
}
#endif
// ----------------/document-------------------

// -------------------list---------------------
#ifndef _LIST_H_
#define _LIST_H_

/* 索引の線形リスト用構造体 */
typedef struct _index
{
    int id;
    struct _index *next;
} INDEX;

/* 関数のプロトタイプ宣言 START */
int insert_list(INDEX *head, int id);
void show_list(INDEX *head);
void free_list(INDEX *head);
/* 関数のプロトタイプ宣言 END */

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
#endif
// ------------------/list---------------------

// -------------------tree---------------------
#ifndef _TREE_H_
#define _TREE_H_

/* 2つ目の単語の2分木 */
typedef struct _second_word
{
    int id;         // 単語のID
    int freq;       // 出現回数
    INDEX indices;  // 転置索引
    struct _second_word *left;
    struct _second_word *right;
} TREE;

/* 関数のプロトタイプ宣言 START */
int insert_tree(TREE *root, int cur_word_id, int doc_index);
void show_tree(TREE *p);
void free_tree(TREE *root, TREE *p);
/* 関数のプロトタイプ宣言 END */

/**
 *  木に要素を追加する関数
 *  @param TREE *root 木の根
 *  @param int cur_word_id 追加する単語のID
 *  @param int doc_index 現在のドキュメントのID
 *  @return int 格納できたかどうか(0:重複有り, 1:成功)
 */
int insert_tree(TREE *root, int cur_word_id, int doc_index)
{
    TREE *p, *new_ele;
    
    p = root; // ルートのアドレスを格納
    
    // 最初ならデータをそのまま格納
    if (p->left == root || p->right == root)
    {
        p->id = cur_word_id;    // 単語のIDがわかれば配列にアクセスできる
        p->freq = 1;            // 頻度の初期化
        p->indices.next = NULL; // 初期化
        insert_list(&(p->indices), doc_index); // 索引の追加
        p->left = NULL;
        p->right = NULL;
        return 1;
    }
    
    // 領域確保(新しい節を作成)
    if((new_ele = (TREE *)malloc(sizeof(TREE))) == NULL)
    {
        fprintf(stderr, "Memory Allocation Error\n");
        exit(1);
    }
    
    // 枝をたどって節を探す
    while (p->id != cur_word_id)
    {
        if (p->id < cur_word_id)
        {
            if(p->left == NULL)
            {
                p->left = new_ele;
                break;
            }
            // 左の節へ
            p = p->left;
        }
        else
        {
            if(p->right == NULL)
            {
                p->right = new_ele;
                break;
            }
            // 右の節へ
            p = p->right;
        }
    }
    
    // 登録済みならインデックスを追加
    if(p->id == cur_word_id)
    {
        // 二つ目の単語の出現行番号を保存
        if (insert_list(&(p->indices), doc_index))
            (p->freq)++;   // 重複がなければ出現回数を増やす
        
        free(new_ele);
        return 0;
    }
    
    // 登録済みでないなら新しい節にデータを登録
    new_ele->id = cur_word_id;      // 単語のIDがわかれば配列にアクセスできる
    new_ele->freq = 1;              // 頻度の初期化
    new_ele->indices.next = NULL;   // 初期化
    insert_list(&(new_ele->indices), doc_index); // 索引の追加
    new_ele->left = NULL;           // 初期化
    new_ele->right = NULL;          // 初期化
    return 1;
}

/**
 *  木の内容を表示する関数(帰りがけ順)
 *  @param TREE *p 表示したい部分木
 *      for Debug
 */
void show_tree(TREE *p)
{
    // 要素が登録されていない
    if (p->left == p || p->right == p)
        return ;
    
    // 左へ
    if(p->left != NULL)
        show_tree(p->left);
    
    printf("id: %d\n", p->id);
    
    // 右へ
    if(p->right != NULL)
        show_tree(p->right);
}

/**
 *  木を解放する関数(通りがけ順で削除)
 *  @param TREE *root 解放する部分木の根
 *  @param TREE *p 解放する部分木の節
 */
void free_tree(TREE *root, TREE *p)
{
    // 要素が登録されていない
    if (p->left == p || p->right == p)
        return ;
    
    // 左へ
    if (p->left != NULL)
        free_tree(root, p->left);
    
    // 右へ
    if (p->right != NULL)
        free_tree(root, p->right);
    
    free_list(p->indices.next);
    
    if (p != root)
        free(p);
}
#endif
// ------------------/tree---------------------

// ------------------array---------------------
#ifndef _ARRAY_H_
#define _ARRAY_H_

#define WORD_SIZE_UNIT 100           // 単語数の初期単位

/* 単語用構造体 */
typedef struct _word_array
{
    char *word;     // 単語
    int freq;       // 出現回数
    INDEX indices;  // 転置索引
    TREE second_words;  // 次の単語の一覧
} ARRAY;

ARRAY **words = NULL;   // 単語の列
int n_words = 0;        // 振ったID = 単語数
int n_words_max = WORD_SIZE_UNIT;       // 最初の大きさ

/* 関数のプロトタイプ宣言 START */
int add_array(char *strm, int doc_index);
int associate_words(int pre_word_id, int cur_word_id, int doc_index);
void show_array(void);
void free_array(void);
/* 関数のプロトタイプ宣言 END */

/**
 *  配列に単語を追加する
 *  @param char *str 追加する文字列
 *  @param int doc_index 現在のドキュメントのID
 *  @return int 挿入位置
 */
int add_array(char *str, int doc_index)
{
    int i;
    
    ARRAY **tmp;
    
    // 配列の中を探す
    for (i = 0; i < n_words; i++)
    {
        if (strcmp(words[i]->word, str) == 0)
        {
            if (insert_list(&(words[i]->indices), doc_index)) // 単語の出現行番号を保存
                words[i]->freq++;    // 重複がなければ出現頻度を増やす
            return i;   // ヒットしたIDを返す
        }
    }
    
    // 単語の格納配列の大きさが足りないとき
    if (n_words >= n_words_max)
    {
        n_words_max *= 2; // 2倍の大きさにする
        // メモリを再度割り当てる
        if ((tmp = (ARRAY **)realloc(words, sizeof(ARRAY *) * n_words_max)) == NULL)
        {
            fprintf(stderr, "Memory Reallocation Error\n");
            exit(1);
        }
        else
        {
            words = tmp;  // 新しい領域を指す
        }
    }
    
    // 新しく作成
    words[n_words] = (ARRAY *)malloc(sizeof(ARRAY));
    if (words[n_words] == NULL)
    {
        fprintf(stderr, "Memory Allocation Error\n");
        exit(1);
    }
    words[n_words]->word = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    if (words[n_words]->word == NULL)
    {
        fprintf(stderr, "Memory Allocation Error\n");
        exit(1);
    }
    // 文字列のコピー
    memcpy(words[n_words]->word, str, sizeof(char) * (strlen(str) + 1));
    
    // 各種値の初期化
    words[n_words]->freq = 1;
    words[n_words]->indices.next = NULL;
    insert_list(&(words[n_words]->indices), doc_index);  // 現在の行の番号を追加
    words[n_words]->second_words.left = &(words[n_words]->second_words);  // 初期化
    words[n_words]->second_words.right = &(words[n_words]->second_words); // 初期化
    
    return n_words++; // 追加した位置を返す
}

/**
 *  2つの単語を関連付ける関数
 *  @param int pre_word_id 一つ前の単語のID
 *  @param int cur_word_id 現在の単語のID
 *  @param int doc_index 現在のドキュメントのID
 *  @return int 格納できたかどうか(0:重複有り, 1:成功)
 */
int associate_words(int pre_word_id, int cur_word_id, int doc_index)
{
    return insert_tree(&(words[pre_word_id]->second_words), cur_word_id, doc_index);
}

/**
 *  配列の中身を表示する関数
 *      for Debug
 */
void show_array()
{
    int i;
    
    for (i = 0; i < n_words; i++)
    {
        printf("%s ->", words[i]->word);
        show_tree(&(words[i]->second_words));
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
        free_tree(&(words[i]->second_words), &(words[i]->second_words)); // 次の単語の線形リストを解放
        free_list(words[i]->indices.next);   // 転置索引の解放
        free(words[i]->word);                // 単語の領域の解放
        free(words[i]);
    }
    free(words);    // 配列の解放
}
#endif
// -----------------/array---------------------

// ------------------result--------------------
#ifndef _RESULT_H_
#define _RESULT_H_

/**
 *  共起の強度計算方式
 */
typedef enum
{
    /** Overlap係数 */
    CM_OVERLAP,
    /** ダイス係数 */
    CM_DICE,
    /** tスコア */
    CM_TSCORE,
    /** 相互情報量(Mutual Information Score) */
    CM_MISCORE,
    /** MI3スコア */
    CM_MI3SCORE,
    /** Zスコア */
    CM_ZCORE,
    /** 要素数 */
    CM_NUM_MODE,
} CALC_MODE;

/* 計算結果を格納する構造体 */
typedef struct _results
{
    char *first;    // 1つ目の単語
    char *second;   // 2つ目の単語
    double indicator;   // 強度
} RESULTS;

#define RESULT_SIZE_UNIT 1000
RESULTS **results = NULL;    // 結果の一覧
int n_results = 0;          // 結果数
int n_results_max = RESULT_SIZE_UNIT;   // 最初の大きさ

char *mode_string[CM_NUM_MODE]
= {
    "Overlap",
    "Dice Score",
    "t Score",
    "Mutual Information Score",
    "MI3 Score",
    "Z Score",
};

/* 関数のプロトタイプ宣言 START */
void insert_result(char *word1, char *word2, double indicator);
void calc_indicator(CALC_MODE mode, int threshold);
void sort_results(void);
void show_results(const int offset, const int n_display);
void free_results(void);
/* 関数のプロトタイプ宣言 END */

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
            fprintf(stderr, "Memory Reallocation Error\n");
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
        fprintf(stderr, "Memory Allocation Error\n");
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
    // tスコアの計算 [|S(a, b)| - (|S(a)| * |S(b)| / |S(all)|)] / √(|S(a, b)|)
    return ((double)conb->freq
            - (double)(ele1->freq * ele2->freq / n_words))
    / sqrt(conb->freq);
}

/**
 *  相互情報量(MIスコア)の計算をする関数
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
 *  MI3スコアの計算をする関数
 *  @param ARRAY *ele1 1つ目の単語の要素
 *  @param ARRAY *ele2 2つ目の単語の要素
 *  @param TREE *conb 2つの単語の要素
 *  @return double 相互情報量
 */
double calc_mi3_score(ARRAY *ele1, ARRAY *ele2, TREE *conb)
{
    double conb_freq = (double)conb->freq;
    // MI3スコアの計算 log2(|S(a, b)|^3 * |S(all)|) / (|S(a)| * |S(b)|)
    return log((double)(conb_freq * conb_freq * conb_freq * n_words)
               / (double)(ele1->freq * ele2->freq)) / log(2);
}

/**
 *  Zスコアの計算をする関数
 *  @param ARRAY *ele1 1つ目の単語の要素
 *  @param ARRAY *ele2 2つ目の単語の要素
 *  @param TREE *conb 2つの単語の要素
 *  @return double 相互情報量
 */
double calc_z_score(ARRAY *ele1, ARRAY *ele2, TREE *conb)
{
    // 中心語が生起する以外の場所に，共起語が現れる確率p
    double p = (double)ele2->freq / (double)(n_words - ele1->freq);
    // 期待される共起の回数E
    double E = p * ele1->freq;
    
    // Zスコアの計算 (|S(a, b)| - E) / √(E(1 - p))
    return (double)(conb->freq - E) / sqrt(E * (1 - p));
}

/**
 *  強度を計算する再帰関数
 *  @param ARRARY *first 最初の単語が格納された要素
 *  @param TREE *conb 2つの単語の要素
 *  @param int threshold 計算に用いる出現頻度の最小値
 *  @param double *get_indicator(ARRAY *, ARRAY *, TREE *) 強度を返す関数
 */
void calc_indicator_in_tree(ARRAY *first, TREE *conb, int threshold,
                            double (*get_indicator)(ARRAY *, ARRAY *, TREE *))
{
    double indicator;
    
    // 要素が登録されていない
    if (conb->left == conb || conb->right == conb)
        return ;
    
    if (conb->left != NULL)   // 右へ
        calc_indicator_in_tree(first, conb->left,  threshold, get_indicator);
    
    if (conb->right != NULL)  // 右へ
        calc_indicator_in_tree(first, conb->right, threshold, get_indicator);
    
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
 *  @param int threshold 計算に用いる出現頻度の最小値
 */
void calc_indicator(CALC_MODE mode, int threshold)
{
    // 計算方法
    static double (*calc_method[])(ARRAY *, ARRAY *, TREE *)
    = {
        calc_overlap,
        calc_dice_score,
        calc_t_score,
        calc_mi_score,
        calc_mi3_score,
        calc_z_score,
    };
    int i;
    
    for (i = 0; i < n_words; i++)
    {
        // 閾値よりも低い時は計算しない
        if (words[i]->freq < threshold)
            continue;
        
        calc_indicator_in_tree(words[i],
                               &(words[i]->second_words),
                               threshold,
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
#endif
// -----------------/result--------------------


// -------------------csv----------------------
#ifndef _CSV_H_
#define _CSV_H_

/**
 *  CSV形式のファイルを作成する関数
 *  @param char *file_name ファイル名
 *  @param int offset オフセット
 *  @param int n_display 出力する数
 */
void write_to_csv(char *file_name, int offset, int n_display)
{
    FILE *fp;
    int last_index = offset + n_display;
    int i;
    
    if ((fp = fopen(file_name, "w")) == NULL)
    {
        fprintf(stderr, "File Open Error: %s\n", file_name);
        exit(1);
    }
    
    for (i = offset; i < n_results && i < last_index; i++)
        fprintf(fp, "%d, %s, %s, %f\n",
                i+1, results[i]->first, results[i]->second, results[i]->indicator);
    
    fclose(fp);
}
#endif
// ------------------/csv----------------------

// -------------------util---------------------
#ifndef _UTIL_H_
#define _UTIL_H_

#define BUFFER_SIZE_UNIT 100    // バッファの大きさの初期単位

/* 関数のプロトタイプ宣言 START */
void set_character(char **str, char c, int *len, int *max);
char *read_input(void);
int read_input_as_int(void);
/* 関数のプロトタイプ宣言 END */

/**
 * 文字列を必要に応じて拡張しながら文字を追加する関数
 * @param char **str 結果の文字列
 * @param char c 入力文字列
 * @param int *len 現在の文字列の長さ
 * @param int *max 現在の文字列の最大長
 */
void set_character(char **str, char c, int *len, int *max)
{
    char *tmp;
    
    // 格納できる領域より大きいなら
    if (*len >= *max)
    {
        (*max) *= 2;  // 領域を2倍に拡張する
        // メモリを再度割り当てる
        if ((tmp = (char *)realloc(*str, sizeof(char) * (*max))) == NULL)
        {
            fprintf(stderr, "Memory Reallocation Error\n");
            free(*str);
            exit(1);
        }
        else
        {
            *str = tmp;  // 新しい領域に値を指す
        }
    }
    (*str)[(*len)++] = c; // 文字を追加する
}

/**
 * 入力を読み込む関数
 * @return char * 読み込んだ文字列
 */
char *read_input(void)
{
    int c;
    char *input;    // 入力された文字列
    int len = 0;    // バッファの現在の文字数
    int len_max = BUFFER_SIZE_UNIT; // バッファの文字数の初期単位
    
    // ある程度の大きさで確保
    input = (char *)calloc(len_max, sizeof(char));
    
    // 終わりまで1文字ずつ取得
    while (1)
    {
        c = getc(stdin);    // 文字の取得
        // 改行文字か判別
        if (c == '\n' || c == '\r' || c == EOF)
        {
            // NULL文字を追加しておく
            set_character(&input, '\0', &len, &len_max);
            break;  // 繰り返しを抜ける
        }
        // 空白文字でない場合
        else
        {
            // 文字の保存
            set_character(&input, tolower(c), &len, &len_max);
        }
    }
    
    return input;   // 取得文字列を返す
}

/**
 * 入力をint型として読み込む関数
 * @return int * 読み込んだ整数
 */
int read_input_as_int(void)
{
    char *input;
    int in;
    
    input = read_input();
    in = atoi(input);
    free(input);
    return in;
}
#endif
// ------------------/util---------------------

// -------------------main---------------------
/**
 *  読み取り時の状態変数
 */
typedef enum
{
    /** 空白 */
    S_SPACE,
    /** 文字 */
    S_WORD,
} STATE;

/**
 *  動作の項目
 */
typedef enum
{
    /** 終了 */
    A_QUIT = -1,
    /** 結果の表示 */
    A_SHOW_RESULTS,
    /** 強度の計算方法の変更 */
    A_CHANGE_INDICATOR,
    /** 精度の変更 */
    A_CHANGE_PRECISION,
    /** CSV形式でファイル書き出し */
    A_WRITE_CSV,
} ACTION;

#define WORD_LENGTH_UNIT 10     // 単語の文字数の初期単位
#define LINE_LENGTH_UNIT 100    // 行の文字数の初期単位

/* 関数のプロトタイプ宣言 START */
void init(void);
void free_all(void);
void read_file(char *file_name);
void show_results_in_range(void);
int change_mode(CALC_MODE *mode);
int change_threshold(int *threshold);
void write_to_csv_in_range(void);
/* 関数のプロトタイプ宣言 END */

/**
 *  要素の確保を行う関数
 */
void init(void)
{
    // ある程度の大きさで確保
    document = (char **)malloc(sizeof(char *) * n_docs_max);
    words = (ARRAY **)malloc(sizeof(ARRAY *) * n_words_max);
    results = (RESULTS **)malloc(sizeof(RESULTS *) * n_results_max);
    if (document == NULL || words == NULL || results == NULL)
    {
        fprintf(stderr, "Memory Allocation Error\n");
        exit(1);
    }
}

/**
 *  要素の解放を行う関数
 */
void free_all(void)
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
        fprintf(stderr, "File Open Error: %s\n", file_name);
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
                    associate_words(pre_word_id, cur_word_id, n_docs); // 関連付け

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

/**
 *  指定した範囲の結果を表示する関数
 */
void show_results_in_range(void)
{
    int offset, n_display;          // 結果のオフセットと表示数
    
    printf("\nHow many results do you want to display?\n");
    printf("Offset > ");
    offset = read_input_as_int();       // offsetを入力
    if (offset < 0) offset = 0;
    
    printf("How many > ");
    n_display = read_input_as_int();    // 表示数を入力
    if (n_display < 0)  n_display = 0;
    
    show_results(offset, n_display);    // 結果の表示
    putchar('\n');
}

/**
 *  強度の計算方法を変更する関数
 *  @param CALC_MODE *mode 計算方法
 *  @return int 変更の有無(0:変更なし, 1:変更あり)
 */
int change_mode(CALC_MODE *mode)
{
    int in;
    int i;

    printf("\nChoose Indicator");
    for (i = 0; i < CM_NUM_MODE; i++)
    {
        if (i % 2 == 0)
            putchar('\n');
        
        printf("%d: %s\t", i, mode_string[i]);
    }
    printf("\n> ");
    
    in = read_input_as_int();   // 計算方式を選択    
    if (in < 0 || in >= CM_NUM_MODE || in == *mode)   // 変更なし
    {
        printf("No change\n");
        return 0;
    }
    else
    {
        *mode = in;
        printf("\nIndicator changed: %s\n", mode_string[*mode]);
        return 1;
    }
}

/**
 *  計算に用いる閾値(頻度)を変更する関数
 *  @return int 変更の有無(0:変更なし, 1:変更あり)
 */
int change_threshold(int *threshold)
{
    int in;
    
    printf("\nChange Indicator Precision by Changing Threshold of Frequency\n");
    printf("(Data will be used whose frequency is more than the threshold)\n");
    printf("Current Threshold = %d\n", *threshold);
    printf("New Threshold > ");
    in = read_input_as_int();   // 新しい閾値の入力
    if (in < 1) in = 1;
    
    if (in == *threshold)    // 変更なし
    {
        printf("No Change\n");
        return 0;

    }
    else                    // 変更あり
    {
        *threshold = in;
        return 1;
    }
}

/**
 *  指定した範囲の結果をCSV形式でファイルに書き出す関数
 */
void write_to_csv_in_range(void)
{
    char *file_name;
    int offset, n_display;          // 結果のオフセットと表示数
    
    printf("\nFile Name > ");
    file_name = read_input();
    
    printf("How many results do you want to write?\n");
    printf("Offset > ");
    offset = read_input_as_int();       // offsetを入力
    if (offset < 0) offset = 0;
    
    printf("How many > ");
    n_display = read_input_as_int();    // 表示数を入力
    if (n_display < 0)  n_display = 0;
    
    write_to_csv(file_name, offset, n_display);
    free(file_name);
    
    printf("Finished Writting\n");
}

int main(int argc, char *argv[])
{
    char *file_name = " Usage: -f filename";
    CALC_MODE mode = CM_OVERLAP;    // 計算する方式
    int is_chaged = 1;              // 計算方法に変更があったか
    ACTION act;                     // 操作を選択
    int threshold = 2;       // 計算に用いる出現頻度の最小値
    int i;
    
    init(); // 要素の確保
    
    // コマンドライン引数の読み込み
    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) // 次の文字列があれば
            file_name = argv[i + 1];    // 開くファイルを読み込む
    }

    read_file(file_name);   // ファイル読み込み
    printf("\nFinished Reading File\n");
    printf("Total number of documents read = %d\n", n_docs);
    printf("Total number of words in the index = %d\n", n_words);
    fflush(stdout);
    
    printf("\nCurrent Indicator Type: %s, Threshold: %d\n",
           mode_string[mode], threshold);
    
    while (1)
    {
        if (is_chaged)  // 変更があった場合
        {
            free_results(); // 前回の結果の消去
            // 次回の計算用
            results = (RESULTS **)malloc(sizeof(RESULTS *) * n_results_max);
            if (results == NULL)
            {
                fprintf(stderr, "Memory Allocation Error\n");
                exit(1);
            }
            
            calc_indicator(mode, threshold);    // 強度の計算
            printf("\nCalculation Finished\n");
            fflush(stdout);
            
            sort_results();     // 結果のソート
            printf("\nSort Finished\n");
            fflush(stdout);
        }
        
        
        printf("\nCurrent Indicator Type: %s, Threshold: %d\n",
               mode_string[mode], threshold);
        
        printf("Choose Action (-1:Quit)\n");
        printf("0:Show Results     1:Change Indicator \n");
        printf("2:Change Precision 3:Write to Csv \n> ");
        act = read_input_as_int();  // 操作を選択
        
        switch (act)
        {
            case A_QUIT:    // 終了
                free_all();
                printf("Quit\n");
                return 0;
                
            case A_SHOW_RESULTS:
                show_results_in_range();        // 結果の表示
                is_chaged = 0;
                break;
                
            case A_CHANGE_INDICATOR:            // 計算方法の変更
                is_chaged = change_mode(&mode);
                break;
            
            case A_CHANGE_PRECISION:
                is_chaged = change_threshold(&threshold); // 計算に用いるときの閾値(頻度)の変更
                break;
                
            case A_WRITE_CSV:
                write_to_csv_in_range();        // CSV形式で書き出し
                is_chaged = 0;
                break;
                
            default:
                break;
        }
        
    }
    
    return 0;
}