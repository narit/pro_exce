/*
 * ファイルから1000個の点を読み出し，第一象限のものを表示する
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉
 */

#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "points.bin"    // 開くファイル
#define POINT_NUM 1000  // 点の数

/* 構造体の定義 */
typedef struct _xy {
    double x;
    double y;
} XY;

int main(void)
{
    FILE *fp;   // ファイルポインタ
    XY point[POINT_NUM];
    int count = 0, i;
    
    // バイナリ読み出しモードで開く
    if ((fp = fopen(FILE_NAME, "rb")) == NULL)
    {
        printf("File Open Error: %s\n", FILE_NAME);
        return 1;
    }
    
    fread(point, sizeof(XY), POINT_NUM, fp);  // ファイルから読み出し
    
    // 点を表示
    for (i = 0; i < POINT_NUM; i++)
    {
        if (point[i].x >= 0.0 && point[i].y >= 0.0)
            printf("%4d : (%f, %f) \n", ++count, point[i].x, point[i].y);
    }
    
    fclose(fp); // 閉じる

    return 0;
}
