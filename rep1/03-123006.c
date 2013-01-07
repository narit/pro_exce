/**
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉
 *
 * 文書検索
 *  工夫した点
 *      ・単語の文字数や行の文字数，Querryが長いときも処理できるようにした
 *      ・OR検索ができるようにした
 *      ・NOT検索ができるようにした
 *      ・部分一致検索ができるようにした
 *      ・前方一致検索ができるようにした
 *      ・後方一致検索ができるようにした
 *      ・絞り込み検索ができるようにした
 *      ・小文字大文字関係なく検索できるようにした
 *  参考文献
 *      [1]2分木への要素の
 *          挿入(insert_tree)
 *          表示(show_tree)
 *          検索(search_tree)[insert_treeをもとに]
 *          削除(free_tree)[show_treeをもとに]
 *      「C言語アルゴリズム-２分木」,
 *      http://masudahp.web.fc2.com/cl/algo/btree01.html
 *      [2]関数のリファレンス
 *      「ライブラリ関数一覧」,
 *      http://www9.plala.or.jp/sgwr-t/lib/libtop.html
 *  あとは授業の資料と辞書以外の文献は見てはいません
 */

#include <stdio.h>
#include <ctype.h>  // isspace()のため
#include <stdlib.h> // malloc()のため
#include <string.h> // strcmp()のため

#define DOCUMENT_SIZE_UNIT 100  // ドキュメント数の初期単位
#define WORD_LENGTH_UNIT 10     // 単語の文字数の初期単位
#define LINE_LENGTH_UNIT 100    // 行の文字数の初期単位

#define BUFFER_SIZE_UNIT 100    // バッファの大きさの初期単位

#define SEARCH_FUNC_NUM 6       // 検索する関数の種類

// 読み取り時の状態変数
typedef enum {S_SPACE, S_WORD} STATE;

// 検索方法
typedef enum {M_AND, M_OR, M_NOT, M_PARTIAL, M_PREFIX, M_SUFFIX} MODE;
// モードを表す文字列
char *mode_string[SEARCH_FUNC_NUM] = {
    "AND",
    "OR",
    "NOT",
    "PARTIAL MATCH",
    "PREFIX",
    "SUFFIX",
};

/* 索引の線形リスト用構造体 */
typedef struct _index
{
    int id;
    struct _index *next;
} INDEX;

/* 転置索引の2分木用構造体 */
typedef struct _tree_node
{
    char *word;     // 単語
    INDEX indices;  // 転置索引
    struct _tree_node *left;
    struct _tree_node *right;
} TREE;


/* 関数のプロトタイプ宣言 START */
void insert_document(char *);
void show_document(void);
void free_document(void);

int insert_list(INDEX *, int);
void show_list(INDEX *);
void free_list(INDEX *);

int insert_tree(char *);
void show_tree(TREE *);
TREE *search_tree(char *);
void free_tree(TREE *);

void set_character(char **, char, int *, int *);
void read_file(char *);
char *read_input(void);

void conjunction(INDEX *, INDEX *);
INDEX *and_search(char *, INDEX *);

void disjunction(INDEX *, INDEX *);
INDEX *or_search(char *, INDEX *);

void negation(INDEX *);
INDEX *not_search(char *, INDEX *);

void full_search_by_func(TREE *, INDEX *, const char *,
                         int (const char *, const char *));
INDEX *full_search(char *, INDEX *, int (const char *, const char *));

int is_hit_with_partial(const char *, const char *);
INDEX *partial_match_search(char *, INDEX *);

int is_hit_with_prefix(const char *, const char *);
INDEX *prefix_search(char *, INDEX *);

int is_hit_with_suffix(const char *, const char *);
INDEX *suffix_search(char *, INDEX *);

int change_mode(MODE *);

void show_result(INDEX *);
/* 関数のプロトタイプ宣言 END */

TREE root = {"", {-1, NULL}, &root, &root};  // 転置索引のroot

char **document;    // ドキュメントを表す動的配列
int n_docs = 0;     // 振ったID = ドキュメント数
int n_docs_max = DOCUMENT_SIZE_UNIT;    // 最初の大きさ
int n_words = 0;    // 単語数

/**
 * ドキュメントを保存する関数
 * @param char *str 追加するドキュメントの文字列
 */
void insert_document(char *str)
{
    char **tmp;
    
    // ドキュメント格納配列の大きさが足りないとき
    if (n_docs >= n_docs_max)
    {
        n_docs_max += DOCUMENT_SIZE_UNIT; // 少し大きくする
        // メモリを再度割り当てる
        if ((tmp = (char **)realloc(document, sizeof(char *) * n_docs_max)) == NULL)
        {
            printf("Memory Reallocation Error\n");
            free(document);
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
        printf("Memory Allocation Error\n");
        exit(1);
    }
    // 文字列のコピー
    memcpy(document[n_docs], str, sizeof(char) * (strlen(str) + 1));
    n_docs++;   // ドキュメント数の更新
}

/**
 * ドキュメントの一覧を表示する関数
 *      for Debug
 */
void show_document(void)
{
    int i;
    
    for (i = 0; i < n_docs; i++)
        printf("doc[%3d]: %s\n", i, document[i]);
}

/**
 * ドキュメントのメモリ解放をする関数
 */
void free_document(void)
{
    int i;
    
    for (i = 0; i < n_docs; i++)    // ドキュメント数だけ解放
        free(document[i]);
    
    free(document);
}

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

/**
 * 木に要素を追加する関数
 * @param char *word 追加する文字列
 * @return int 格納できたかどうか(0:重複有り, 1:成功)
 */
int insert_tree(char *word)
{
    TREE *p, *new_ele;
    
    p = &root; // ルートのアドレスを格納
    
    // 最初ならデータ格納をそのまま格納
    if(p->left == &root || p->right == &root)
    {
        // 文字列用の領域を確保してコピー
        p->word = (char *)malloc(sizeof(char) * (strlen(word) + 1));
        memcpy(p->word, word, sizeof(char) * (strlen(word) + 1));
        insert_list(&(p->indices), n_docs); // 索引の追加
        p->left = NULL;
        p->right = NULL;
        n_words++;  // 単語数をカウント
        return 1;
    }
    
    // 領域確保(新しい節を作成)
    if((new_ele = (TREE *)calloc(1, sizeof(TREE))) == NULL)
    {
        printf("Memory Allocation Error\n");
        exit(1);
    }
    
    // 枝をたどって節を探す
    while(strcmp(p->word, word))
    {
        // 辞書順で前
        if(strcmp(p->word, word) > 0)
        {
            if(p->left == NULL)
            {
                p->left = new_ele;
                break;
            }
            
            // 左の節へ
            p = p->left;
        }
        // 辞書順で後
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
    if(strcmp(p->word, word) == 0)
    {
        insert_list(&(p->indices), n_docs);
        free(new_ele);
        return 0;
    }
    
    // 登録済みでないなら新しい節にデータを登録
    new_ele->word = (char *)calloc((strlen(word) + 1), sizeof(char));
    memcpy(new_ele->word, word, sizeof(char) * (strlen(word) + 1));
    insert_list(&(new_ele->indices), n_docs); // 索引の追加
    n_words++;  // 単語数をカウント
    return 1;
}

/**
 * 木の内容を表示する関数(帰りがけ順)
 * @param TREE *p 表示したい部分木
 *      for Debug
 */
void show_tree(TREE *p)
{
    // 左へ
    if(p->left != NULL)
        show_tree(p->left);
    
    printf("word: %s\n", p->word);
    
    // 右へ
    if(p->right != NULL)
        show_tree(p->right);
}

/**
 * 木を検索する関数
 * @param char *word 検索のキー
 * @return TREE * ヒットした節(ヒットしなかった場合はNULL)
 */
TREE *search_tree(char *word)
{
    TREE *p;
    
    p = &root; // ルートのアドレスを格納
    
    // 枝をたどって節を探す
    while(strcmp(p->word, word))
    {
        if(strcmp(p->word, word) > 0)
        {
            if(p->left == NULL)
                return NULL;    // NOT FOUND
            
            // 左の節へ
            p = p->left;
        }
        else
        {
            if(p->right == NULL)
                return NULL;    // NOT FOUND
            
            // 右の節へ
            p = p->right;
        }
    }
    
    // 発見
    return p;
    
}

/**
 * 木を解放する関数(通りがけ順で削除)
 * @param TREE *p 解放する部分木の根
 */
void free_tree(TREE *p)
{
    // 左へ
    if (p->left != NULL)
        free_tree(p->left);
    
    // 右へ
    if (p->right != NULL)
        free_tree(p->right);
    
    // rootでないなら解放していく
    if (p != &root)
    {
        free_list(p->indices.next);
        free(p);
    }
}

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
            printf("Memory Reallocation Error\n");
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
 * ファイルを読み込んでドキュメントと転置索引を作る関数
 * @param char *file_name 開くファイル名
 */
void read_file(char *file_name)
{
    FILE *fp;
    int c;
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
    document = (char **)calloc(n_docs_max, sizeof(char *));
    word = (char *)calloc(word_len_max, sizeof(char));
    line = (char *)calloc(line_len_max, sizeof(char));
    
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
                insert_tree(word);  // 木に追加
                word_len = 0;       // 単語の文字数の初期化
            }
            
            if (c == '\n' || c == '\r' || c == EOF)  // 改行かファイルの終わりなら
            {
                
                // 行にNULL文字を追加
                set_character(&line, '\0', &line_len, &line_len_max);
                
                // 行をドキュメントとして保存
                if (strlen(line) > 0)
                    insert_document(line);  // 配列に追加
                
                line_len = 0;           // 行の文字数の初期化
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
 * 索引の集合の積を作る関数
 * @param INDEX *result 結果の格納先
 * @param INDEX *indices 比較対象
 */
void conjunction(INDEX *result, INDEX *indices)
{
    INDEX *p, *q, *r;
    
    // 最初なら比較対象をそのまま代入する
    if (result->next == NULL)
    {
        r = indices->next;
        while (r != NULL)
        {
            insert_list(result, r->id);
            r = r->next;
        }
    }
    else
    {
        // resultにあるもので比較をしていく
        for (p = result->next, q = result; p != NULL; q = p, p = p->next)
        {
            // 比較対象の先頭を入れる
            r = indices->next;
            while (r != NULL)
            {
                if (p->id == r->id) // 同じ索引があるなら抜ける
                    break;
                r = r->next;
            }
            
            // 共通のものでなければ消す
            if (r == NULL)
            {
                q->next = p->next;
                free(p);
                p = q;
            }
        }
    }
}

/**
 * AND検索をする関数
 * @param char *input 解析する入力文字列
 * @param INDEX *before 前回の結果
 * @return INDEX * 結果の線形リスト(NOT FOUNDはNULL)
 */
INDEX *and_search(char *input, INDEX *before)
{
    char *token;    // 取得文字列
    TREE *node;     // 文字列をキーにしてヒットした節
    INDEX result = {-1, NULL};   // 結果
    INDEX tmp = {-1, NULL};     // 前回の結果を使う場合
    
    token = strtok(input, " ");
    while (token != NULL)
    {
        node = search_tree(token);  // 木を探す
        
        if (node == NULL)   // 同じ物がないならNOT FOUND
            return NULL;
        
        conjunction(&result, &(node->indices));   // 集合の積
        
        if (result.next == NULL) // 共通の物がなくなった時点でNOT FOUND
            return NULL;
        
        token = strtok(NULL, " ");
    }
    
    if (before != NULL) // 前回の結果とのANDを取る場合
    {
        tmp.next = before;
        conjunction(&result, &tmp);
        free_list(tmp.next);    //  前回のリストを消去
    }
    
    return result.next;  // 結果を返す
}

/**
 * 索引の集合の和を作る関数
 * @param INDEX *result 結果の格納先
 * @param INDEX *indices 比較対象
 */
void disjunction(INDEX *result, INDEX *indices)
{
    INDEX *p, *q, *r;
    
    // 最初なら比較対象をそのまま代入する
    if (result->next == NULL)
    {
        r = indices->next;
        while (r != NULL)
        {
            insert_list(result, r->id);
            r = r->next;
        }
    }
    else
    {
        // indicesにあるもので比較をしていく
        for (p = indices->next, q = indices; p != NULL; q = p, p = p->next)
        {
            // 比較対象の先頭を入れる
            r = result->next;
            while (r != NULL)
            {
                if (p->id == r->id) // 同じ索引があるなら抜ける
                    break;
                r = r->next;
            }
            
            // 共通のものでなければ追加する
            if (r == NULL)
                insert_list(result, p->id);
        }
    }
}

/**
 * OR検索をする関数
 * @param char *input 解析する入力文字列
 * @param INDEX *before 前回の結果
 * @return INDEX * 結果の線形リスト(NOT FOUNDはNULL)
 */
INDEX *or_search(char *input, INDEX *before)
{
    char *token;    // 取得文字列
    TREE *node;     // 文字列をキーにしてヒットした節
    INDEX result = {-1, NULL};  // 結果
    INDEX tmp = {-1, NULL};     // 前回の結果を使う場合
    
    token = strtok(input, " ");
    while (token != NULL)
    {
        node = search_tree(token);  // 木を探す
        
        if (node != NULL)   // 空でないとき
            disjunction(&result, &(node->indices));   // 集合の和
        
        token = strtok(NULL, " ");
    }
    
    if (before != NULL) // 前回の結果とのANDを取る場合
    {
        tmp.next = before;
        conjunction(&result, &tmp);
        free_list(tmp.next);    //  前回のリストを消去
    }
    
    return result.next;  // 結果を返す
}


/**
 * 索引の集合の否定を作る関数
 * @param INDEX *result 結果の格納先
 */
void negation(INDEX *result)
{
    INDEX *p;
    INDEX *tmp;
    int i;
    
    tmp = (INDEX *)calloc(1, sizeof(INDEX));
    
    for (i = 0; i < n_docs; i++)
    {
        // resultにあるもので比較をしていく
        for (p = result->next; p != NULL; p = p->next)
        {
            if (p->id == i) // 同じ索引があるなら追加しない
                break;
        }
        
        if (p == NULL)  // 無いものなら追加
            insert_list(tmp, i);
    }
    free_list(result->next);  //  前の結果を消しておく
    result->next = tmp->next;   // 結果の格納
}

/**
 * NOT検索をする関数
 * @param char *input 解析する入力文字列
 * @param INDEX *before 前回の結果
 * @return INDEX * 結果の線形リスト(NOT FOUNDはNULL)
 */
INDEX *not_search(char *input, INDEX *before)
{
    char *token;    // 取得文字列
    TREE *node;     // 文字列をキーにしてヒットした節
    INDEX result = {-1, NULL};  // 結果
    INDEX tmp = {-1, NULL};     // 前回の結果を使う場合
    
    token = strtok(input, " ");
    while (token != NULL)
    {
        node = search_tree(token);  // 木を探す
        
        if (node != NULL)   // 空でないとき
            disjunction(&result, &(node->indices));   // 集合の和
        
        token = strtok(NULL, " ");
    }
    
    negation(&result);  // 否定を取る
    
    if (before != NULL) // 前回の結果とのANDを取る場合
    {
        tmp.next = before;
        conjunction(&result, &tmp);
        free_list(tmp.next);    //  前回のリストを消去
    }
    
    return result.next;  // 結果を返す
}

/**
 * 木を全探索をする関数
 * @param TREE *p 表示したい部分木
 * @param INDEX *result 結果の格納先
 * @param const char *str 検索のキー
 * @param int is_hit(const char *, const char *)    一致の判定をする関数
 */
void full_search_by_func(TREE *p, INDEX *result, const char *str,
                         int is_hit(const char *, const char *))
{
    // 左へ
    if(p->left != NULL)
        full_search_by_func(p->left, result, str, is_hit);
    
    // 右へ
    if(p->right != NULL)
        full_search_by_func(p->right, result, str, is_hit);
    
    if (is_hit(p->word, str))        // 一致したかどうか
        disjunction(result, &(p->indices)); // 索引の論理和をとっておく
}

/**
 * 検索をする関数
 * @param char *input 解析する入力文字列
 * @param INDEX *before 前回の結果
 * @param int is_hit(const char *, const char *)    一致の判定をする関数
 * @return INDEX * 結果の線形リスト(NOT FOUNDはNULL)
 */
INDEX *full_search(char *input, INDEX *before,
                   int is_hit(const char *, const char *))
{
    char *token;    // 取得文字列
    INDEX result = {-1, NULL};  // 結果
    INDEX tmp = {-1, NULL};     // 単語ごとの結果
    
    token = strtok(input, " ");
    while (token != NULL)
    {
        full_search_by_func(&root, &tmp, token, is_hit);  // 木を探す
        
        conjunction(&result, &tmp);   // 集合の和
        
        free_list(tmp.next);    // 一時データを削除
        tmp.next = NULL;        // 初期化しておく
        token = strtok(NULL, " ");
    }
    
    if (before != NULL) // 前回の結果とのANDを取る場合
    {
        tmp.next = before;
        conjunction(&result, &tmp);
        free_list(tmp.next);    //  前回のリストを消去
    }
    
    return result.next;  // 結果を返す
}

/**
 * 部分一致の判定をする関数
 * @param const char *str1 比較対象
 * @param const char *str2 検索のキー
 * @return int 一致したかどうか(0:一致していない, 1:一致した)
 */
int is_hit_with_partial(const char *str1, const char *str2)
{
    if (strstr(str1, str2) != NULL) // 部分一致したら真
        return 1;
    else
        return 0;
}

/**
 * 部分一致検索をする関数
 * @param char *input 解析する入力文字列
 * @param INDEX *before 前回の結果
 * @return INDEX * 結果の線形リスト(NOT FOUNDはNULL)
 */
INDEX *partial_match_search(char *input, INDEX *before)
{
    return full_search(input, before, is_hit_with_partial);  // 結果を返す
}

/**
 * 前方一致の判定をする関数
 * @param const char *str1 比較対象
 * @param const char *str2 検索のキー
 * @return int 一致したかどうか(0:一致していない, 1:一致した)
 */
int is_hit_with_prefix(const char *str1, const char *str2)
{
    // 返ってきたポインタが先頭を指していたら前方一致
    if (strstr(str1, str2) == str1)
        return 1;
    else
        return 0;
}

/**
 * 前方一致検索をする関数
 * @param char *input 解析する入力文字列
 * @param INDEX *before 前回の結果
 * @return INDEX * 結果の線形リスト(NOT FOUNDはNULL)
 */
INDEX *prefix_search(char *input, INDEX *before)
{
   return full_search(input, before, is_hit_with_prefix);
}

/**
 * 後方一致の判定をする関数
 * @param const char *str1 比較対象
 * @param const char *str2 検索のキー
 * @return int 一致したかどうか(0:一致していない, 1:一致した)
 */
int is_hit_with_suffix(const char *str1, const char *str2)
{
    char *tmp;
    
    if ((tmp = strstr(str1, str2)) != NULL  // ヒットして
        && *(tmp + strlen(str2)) == '\0')     // 検索文字列の次が\0なら後方一致
        return 1;
    else
        return 0;
}

/**
 * 後方一致検索をする関数
 * @param char *input 解析する入力文字列
 * @param INDEX *before 前回の結果
 * @return INDEX * 結果の線形リスト(NOT FOUNDはNULL)
 */
INDEX *suffix_search(char *input, INDEX *before)
{    
    return full_search(input, before, is_hit_with_suffix);
}

/**
 * 検索方法を切り替える関数
 * @param MODE *mode 切り替える変数
 * @return int 終了かどうか(0:継続, 1:終了)
 */
int change_mode(MODE *mode)
{
    char *reply;
    
    printf("--- Please Choose Search Mode ---\n");
    printf("a: AND, o: OR, n: NOT, p: PARTIAL MATCH, r: PREFIX, s: SUFFIX, q: Quit\n");
    printf("Mode: ");
    reply = read_input();  // モード選択文字列
    
    // モードを変更
    switch (reply[0])
    {
        case 'a':   // AND
            *mode = M_AND;
            printf("AND Search Mode\n");
            break;
        case 'o':   // OR
            *mode = M_OR;
            printf("OR Search Mode\n");
            break;
        case 'n':   // NOT
            *mode = M_NOT;
            printf("NOT Search Mode\n");
            break;
        case 'p':   // PARTIAL MATCH
            *mode = M_PARTIAL;
            printf("PARTIAL MATCH Search Mode\n");
            break;
        case 'r':   // PREFIX
            *mode = M_PREFIX;
            printf("PREFIX Search Mode\n");
            break;
        case 's':   // SUFFIX
            *mode = M_SUFFIX;
            printf("SUFFIX Search Mode\n");
            break;
        case 'q':   // Quit
            free(reply);    // モード選択文字列の消去
            printf("Search Finished\n");
            return 1;
        default:
            printf("No Change\n");
            break;
    }
    printf("\n");
    
    free(reply);    // モード選択文字列の消去
    
    return 0;
}

/**
 * 結果を表示する関数
 * @param INDEX *result 表示したい索引のリスト
 */
void show_result(INDEX *result)
{
    INDEX *p;
    int count = 0;  // ヒットした数
    
    printf("\n------- result -------\n");
    
    if (result == NULL) // 見つからなかった時
        printf("NOT FOUND\n");
    
    p = result;
    while (p != NULL)   // 結果の表示
    {
        printf("[%3d] : %s\n", p->id, document[p->id]);
        p = p->next;
        count++;
    }
    printf("-- %4d document(s) --\n\n", count);
}

int main(int argc, char *argv[])
{
    char *file_name = " Usage: -f filename";
    char *input, *reply;
    INDEX *result = NULL;
    int i;
    MODE mode = M_AND;  // 検索方法(初期はAND検索)
    // 検索の関数へのポインタの配列
    INDEX* (*search[])(char *, INDEX *) = {
        and_search,
        or_search,
        not_search,
        partial_match_search,
        prefix_search,
        suffix_search,
    };
    
    // コマンドライン引数の読み込み
    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) // 次の文字列があれば
            file_name = argv[i + 1];    // 開くファイルを読み込む
    }

    read_file(file_name);   // ファイル読み込み
    printf("Total number of documents read = %d\n", n_docs);
    printf("Total number of words in the index = %d\n\n", n_words);
    
    while (1)
    {
        printf("Enter Querry(Enter -1 to Choose Mode)[%s]: ", mode_string[mode]);
        input = read_input();   // 入力を受付
        
        if (strcmp(input, "-1") == 0)   // モード選択に
        {
            free(input);        // クエリ文字列の解放
            
            if (change_mode(&mode))   // モードを選択して終了のとき
            {
                free_list(result);  // 結果の消去
                break;
            }
            continue;
        }
        
        result = search[mode](input, result); // 検索(searchは関数へのポインタの配列)
        show_result(result);    // 結果の表示
        
        // --- 絞り込み検索に対応 START ---
        printf("Refine?(y or other): ");
        reply = read_input();
        if (reply[0] != 'y')    // 絞り込みじゃないなら元のデータを消す
        {
            free_list(result);
            result = NULL;
        }
        free(reply);
        printf("\n");
        // --- 絞り込み検索に対応 END ---
        
        free(input);        // クエリ文字列の解放
    }
    
    free_tree(&root);   // 木の解放
    free_document();    // ドキュメントの解放
    
    return 0;
}