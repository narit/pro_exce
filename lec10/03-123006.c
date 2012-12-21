/*
 * 木構造を使ってnumbers.txtを昇順にソート
 * 学生番号: 03-123006
 * 氏名: 岩成達哉
 */

#include <stdio.h>
#include <stdlib.h> // malloc()のため

#define FILE_NAME "numbers.txt"    // 開くファイル

/* 構造体の定義 */
typedef struct tree {
    int number;
    struct tree *left;
    struct tree *right;
} TREE;
TREE root = {0, &root, &root};  // root

/* 関数のプロトタイプ宣言 START */
int insert_tree(int num);
void show_tree(TREE *p);
void free_tree(TREE *p);
/* 関数のプロトタイプ宣言 END */

int main(void)
{
    FILE *fp;       // ファイルポインタ
    int num;
    
    if ((fp = fopen(FILE_NAME, "r")) == NULL)
    {
        printf("File Open Error: %s\n", FILE_NAME);
        return 1;
    }
    
    // 終わりまで1つずつ取り込む
    while (fscanf(fp, "%d", &num) != EOF)
        insert_tree(num);
    
    fclose(fp);
    
    show_tree(&root);   // 表示
    
    free_tree(&root);   // 木の解放

    return 0;
}

/* 木に要素を追加する関数 */
int insert_tree(int num)
{
    TREE *p, *new;
    
    p = &root; // ルートの番地を格納
    
    // 最初ならデータをそのまま格納
    if(p->left == &root || p->right == &root)
    {
        p->number = num;
        p->left = p->right = NULL;
        return 1;
    }
    
    // 領域確保(新しい節を作成)
    if((new = (TREE *)malloc(sizeof(TREE))) == NULL)
    {
        printf("領域を確保できません\n");
        exit(1);
    }
    
    // 枝をたどって同じ値を持つ節を探す
    while(p->number != num)
    {
        if(p->number > num)
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
    
    // 登録済みなら終わり
    if(p->number == num)
    {
        free(new);
        return 0;
    }
    
    // 登録済みでないなら新しい節にデータを登録
    new->number = num;
    new->left = new->right = NULL;
    return 1;
}

/* 木の内容を表示する関数(帰りがけ順なので昇順になる) */
void show_tree(TREE *p)
{
    // 左へ
    if(p->left != NULL)
        show_tree(p->left);
    
    printf("%d\n", p->number);
    
    // 右へ
    if(p->right != NULL)
        show_tree(p->right);
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
