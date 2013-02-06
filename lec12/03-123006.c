/*
 * ファイルを逆順に読み出す
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉
 */

#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "news.txt"    // 開くファイル


int main(void)
{
    FILE *fp;   // ファイルポインタ
    char c;
    int i;
    
    // バイナリ読み出しモードで開く
    if ((fp = fopen(FILE_NAME, "rb")) == NULL)
    {
        fprintf(stderr, "File Open Error: %s\n", FILE_NAME);
        return 1;
    }
    
    // 後ろから少しずつずらす
    // 先頭まで来たら戻せなくなるので0以外がかえる
    for (i = 1; fseek(fp, - i * sizeof(c), SEEK_END) == 0; i++)
    {
        fread(&c, sizeof(c), 1, fp);    // 読み出し
        putchar(c);
    }
    putchar('\n');
    
    fclose(fp); // 閉じる

    return 0;
}