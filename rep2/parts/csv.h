#ifndef _CSV_H_
#define _CSV_H_

#include "result.h"

/**
 *  CSV形式のファイルを作成する関数
 *  @param char *file_name ファイル名
 *  @param int offset オフセット
 *  @param int n_display 出力する数
 */
void write_to_csv(char *file_name, int offset, int n_display);

#endif