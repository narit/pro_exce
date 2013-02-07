#include <stdio.h>
#include <stdlib.h>
#include "csv.h"

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