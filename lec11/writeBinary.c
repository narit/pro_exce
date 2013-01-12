/*
 * (x^2 / 3) + (y^2 / 5) = 1を満たすランダムな点を1000個ファイルへ書き出す
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉
 */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#define FILE_NAME "points.bin"    // 開くファイル
#define POINT_NUM 1000  // 点の数

//#define DEBUG

/* 構造体の定義 */
typedef struct _xy {
    double x;
    double y;
} XY;

XY gen_point(void)
{
    XY tmp;
    
    double rad = 2.0 * M_PI * (double)rand() / RAND_MAX;
    
    // 三角関数を用いて座標を得る
    tmp.x = sqrt(3.0) * cos(rad);
    tmp.y = sqrt(5.0) * sin(rad);
    
#ifdef DEBUG
    printf("x = %+f, y = %+f, cal = %+f\n", tmp.x, tmp.y,
           tmp.x * tmp.x / 3.0 + tmp.y * tmp.y / 5.0);
#endif
    return tmp;
}

int main(void)
{
    FILE *fp;   // ファイルポインタ
    XY point[POINT_NUM];
    int i;
    
    srand((unsigned)time(NULL));    // 乱数列の初期化
    
    // バイナリ書き込みモードで開く
    if ((fp = fopen(FILE_NAME, "wb")) == NULL)
    {
        printf("File Open Error: %s\n", FILE_NAME);
        return 1;
    }
    
    // 点を作成
    for (i = 0; i < POINT_NUM; i++)
        point[i] = gen_point();
    
    fwrite(point, sizeof(XY), POINT_NUM, fp);  // ファイルへ書き出し
    
    fclose(fp); // 閉じる

    return 0;
}
