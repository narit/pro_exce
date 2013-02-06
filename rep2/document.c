#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "document.h"

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
        n_docs_max += DOCUMENT_SIZE_UNIT; // 少し大きくする
        // メモリを再度割り当てる
        if ((tmp = (char **)realloc(document, sizeof(char *) * n_docs_max)) == NULL)
        {
            printf("Memory Reallocation Error\n");
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
        printf("Memory Allocation Error\n");
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
