#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#define DOCUMENT_SIZE_UNIT 100  // ドキュメント数の初期単位

extern char **document; // ドキュメントを表す動的配列
extern int n_docs;      // 振ったID = ドキュメント数
extern int n_docs_max;  // 最初の大きさ

/**
 *  ドキュメントを保存する関数
 *  @param char *str 追加するドキュメントの文字列
 */
void insert_document(char *str);

/**
 *  ドキュメントの一覧を表示する関数
 *      for Debug
 */
void show_document(void);

/**
 *  ドキュメントのメモリ解放をする関数
 */
void free_document(void);

#endif