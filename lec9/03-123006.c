/**
 * ハッシュテーブルを使った検索
 * 都市名と人口のデータを都市名を使って検索する
 * (小文字でも大文字でも検索できるようにした)
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉
 */

#include <stdio.h>
#include <ctype.h>  // isspace()のため
#include <stdlib.h> // malloc()のため
#include <string.h> // strcmp()のため

#define HASH_SIZE   500    // ハッシュテーブルの大きさ
#define BUFFER_SIZE 1000    // 入力文字列の大きさ
#define FILE_NAME "cities.csv"    // 開くファイル

/* リスト構造の定義　*/
typedef struct list
{
    char *name;
    int pop;
    struct list *next;
} LIST;

// ハッシュテーブル
LIST hash_table[HASH_SIZE] = {0};

/* プロトタイプ宣言 START */
unsigned calc_hash(char *str);
void add(char *str, int num);
LIST* search_by_name(char *str);
void free_list();
int strcmp_i(char *s1, char *s2);
/* プロトタイプ宣言 END */

/* ハッシュ関数の定義 */
unsigned calc_hash(char *str)
{
    unsigned sum = 0;
    int i;
    
    // 大文字,小文字の区別をなく
    for (sum = 0; *str != '\0'; str++) {
        sum = (sum * 31) + tolower(*str);
    }
    
    return sum % HASH_SIZE;
}

/* テーブルへ追加 */
void add(char *str, int num)
{
    LIST *p, *q, *new_ele;
    unsigned hash_val;
    
    hash_val = calc_hash(str);  // ハッシュ値の計算
    p = hash_table[hash_val].next;
    q = &hash_table[hash_val];
    
    // 同じ物があるかどうかチェックしながら検索
    while (p != NULL) {
        if (strcmp_i(p->name, str) == 0) {
            p->pop = num;   // 人口の更新
            break;
        }
        q = p;
        p = p->next;
    }
    
    // ヒットしなかったら追加
    if (p == NULL) {
        // 新しく構造体を確保
        new_ele = (LIST *)malloc(sizeof(LIST));
        if (new_ele == NULL) {
            printf("Memory Allocate Error\n");
            exit(1);
        }
        
        // 都市名の領域を確保
        new_ele->name = (char *)malloc(sizeof(char) * (strlen(str) + 1));
        if (new_ele->name == NULL) {
            printf("Memory Allocate Error\n");
            exit(1);
        }
        strcpy(new_ele->name, str); // 都市名の代入
        new_ele->pop = num;         // 人口の代入
        q->next = new_ele;          // 末尾に追加
        new_ele->next = NULL;
    }
}

/* 名前で検索する */
LIST *search_by_name(char *str)
{
    LIST *p, *q;
    unsigned hash_val;
    
    hash_val = calc_hash(str);  // ハッシュ値の計算
    p = hash_table[hash_val].next;
    q = &hash_table[hash_val];
    
    // ヒットするまで検索
    while (p != NULL) {
        if (strcmp_i(p->name, str) == 0) {
            break;
        }
        q = p;
        p = p->next;
    }
    
    return p;
}

/* リストの表示 */
void show_list()
{
    LIST *p, *q;
    int i;
    
    for (i = 0; i < HASH_SIZE; i++) {
        p = hash_table[i].next;
        q = &hash_table[i];
        printf("table[%d] :", i);
        while (p != NULL) {
            printf("%s, %d ->", p->name, p->pop);
            q = p;
            p = p->next;
        }
        printf("\n");
    }
}

/* リストの解放 */
void free_list()
{
    LIST *p, *q;
    int i;
    
    for (i = 0; i < HASH_SIZE; i++) {
        p = hash_table[i].next;
        q = &hash_table[i];
        while (p != NULL) {
            q = p;
            p = p->next;
            free(q->name);  // 文字列の解放
            free(q);        // 要素の解放
        }
        // printf("\n");
    }
}

/* ファイルから読み込み */
int read_file()
{
    FILE *fp;
    char buf[BUFFER_SIZE];
    char *name;
    int pop;
    
    if ((fp = fopen(FILE_NAME, "r")) == NULL)
    {
        printf("File Open Error: %s\n", FILE_NAME);
        exit(1);
    }
    
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        name = strtok(buf, ",");
        pop = atoi(strtok(NULL, "\n"));
        add(name, pop);
    }
}

/* 小文字に変換して判定 */
int strcmp_i(char *s1, char *s2)
{
    // 前から順に見ていく
    for (; tolower(*s1) == tolower(*s2) && *s1 != '\0'; s1++, s2++) {
    }

    return *s1 - *s2; // 全く同じか，違うか
}

int main(void)
{
    
    char input[BUFFER_SIZE];
    LIST *res;
    int i;
    
    read_file();    // ファイルの読み込み
    //show_list();    // リストの内容表示
    
    // 検索する都市名の入力
    printf("検索する都市名 > ");
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0';    // 改行を消す
    
    res = search_by_name(input);    // 検索する
    
    // 結果の表示
    if (res == NULL) {
        printf("そのような都市はありません.\n");
    }
    else {
        printf("--- 結果 ---\n");
        printf("都市名: %s, 人口: %d\n", res->name, res->pop);
    }
    
    free_list();    // リストの解放
    
    return 0;
}