#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

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
        printf("Memory Allocation Error\n");
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