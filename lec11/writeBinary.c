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

/* 構造体の定義 */
typedef struct _xy {
    double x;
    double y;
} XY;

XY gen_point(void)
{
    XY tmp;
    
    // [-√3, √3]の座標を得る
    tmp.x = ((rand() % 2) ? 1 : -1) * sqrt(3.0 * (double)rand() / RAND_MAX);
    
    // xに合わせてyを計算
    tmp.y = ((rand() % 2) ? 1 : -1) * sqrt((1.0 - tmp.x * tmp.x / 3.0) * 5.0);
    
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
