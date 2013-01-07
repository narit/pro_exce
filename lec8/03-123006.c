/*
 * 構造体を使ってnews.txtから最大頻度の単語とその出現度を求める
 * 学生番号: 03-123006
 * 氏名: 岩成達哉
 */

#include <stdio.h>
#include <ctype.h>  // isspace()のため
#include <stdlib.h> // malloc()のため
#include <string.h> // strcmp()のため

#define S_SPACE 0
#define S_WORD 1

#define WRD_LEN 32              // 最大文字数
#define FILE_NAME "news.txt"    // 開くファイル

/* 構造体の定義 */
typedef struct tree {
    char wrd[WRD_LEN];
    int freq;
    struct tree *left;
    struct tree *right;
} TREE;
TREE root = {"", 0, &root, &root};  // root
TREE max = {"", 0, NULL, NULL};     // 最大頻度の単語

/* 関数のプロトタイプ宣言 START */
int insert_tree(char *wrd);
void show_tree(TREE *p);
void find_max(TREE *p);
void free_tree(TREE *p);
/* 関数のプロトタイプ宣言 END */

int main(void)
{
    FILE *fp;       // ファイルポインタ
    int c;          // 入力文字列
    char tmp[WRD_LEN];  // 格納文字列
    int len = 0;        // 文字の長さ
    int state = S_SPACE;    // 状態
    
    if ((fp = fopen(FILE_NAME, "r")) == NULL)
    {
        printf("File Open Error: %s\n", FILE_NAME);
        return 1;
    }
    
    // 終わりまで1文字ずつ取得
    while ((c = getc(fp)))
    {
        // 空白文字か判別
        if (isspace(c) || c == EOF)
        {
            if (state == S_WORD)    // 文字が入力されていたなら
            {
                tmp[len] = '\0';    // ヌル文字の挿入
                insert_tree(tmp);   // 木に追加
            }
            
            if (c == EOF)   // 繰り返しを抜ける
                break;
            
            len = 0;            // 文字数の初期化
            state = S_SPACE;    // 状態の変更
        }
        // 空白文字でない場合(単語の場合)
        else
        {
            tmp[len++] = c; // 文字の保存
            state = S_WORD; // 状態の変更
        }
    }
    
    fclose(fp);
    
    //show_tree(&root);
    
    find_max(&root);    // 最大頻度単語を探す
    printf("最大頻度単語:%s, 頻度:%d\n", max.wrd, max.freq);
    
    free_tree(&root);   // 木の解放

    return 0;
}

/* 木に要素を追加する関数 */
int insert_tree(char *wrd)
{
    TREE *p, *new;
    
    p = &root; // ルートの番地を格納
    
    // 最初ならデータ格納をそのまま格納
    if(p->left == &root || p->right == &root)
    {
        strcpy(p->wrd, wrd);
        p->freq = 1;
        p->left = NULL;
        p->right = NULL;
        return 1;
    }
    
    // 領域確保(新しい節を作成)
    if((new = (TREE *)malloc(sizeof(TREE))) == NULL)
    {
        printf("領域を確保できません\n");
        exit(1);
    }
    
    // 枝をたどって節を探す
    while(strcmp(p->wrd, wrd))
    {
        if(strcmp(p->wrd, wrd) > 0)
        {
            if(p->left == NULL)
            {
                p->left = new;
                break;
            }
            // 左の節へ
            p = p->left;
        }
        else
        {
            if(p->right == NULL)
            {
                p->right = new;
                break;
            }
            // 右の節へ
            p = p->right;
        }
    }
    
    // 登録済みなら頻度数をカウントして終わり
    if(strcmp(p->wrd, wrd) == 0)
    {
        (p->freq)++;
        free(new);
        return 0;
    }
    
    // 登録済みでないなら新しい節にデータを登録
    strcpy(new->wrd, wrd);
    new->freq = 1;
    new->left = NULL;
    new->right = NULL;
    return 1;
}

/* 木の内容を表示する関数(帰りがけ順) */
void show_tree(TREE *p)
{
    // 左へ
    if(p->left != NULL)
        show_tree(p->left);
    
    printf("%s : %d\n", p->wrd, p->freq);
    
    // 右へ
    if(p->right != NULL)
        show_tree(p->right);
}

/* 頻度の最大を探す関数(帰りがけ順) */
void find_max(TREE *p)
{
    // 左へ
    if(p->left != NULL)
        find_max(p->left);
    
    // 値の更新
    if (max.freq < p->freq)
    {
        strcpy(max.wrd, p->wrd);
        max.freq = p->freq;
    }
    
    // 右へ
    if(p->right != NULL)
        find_max(p->right);
}

/* 木を解放する関数 */
void free_tree(TREE *p)
{
    // 左へ
    if (p != NULL && p->left != NULL)
    {
        free_tree(p->left);
        free(p->left);
        p->left = NULL;
    }
    
    // 右へ
    if (p != NULL && p->right != NULL)
    {
        free_tree(p->right);
        free(p->right);
        p->right = NULL;
    }
}
