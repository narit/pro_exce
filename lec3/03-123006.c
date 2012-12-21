/*
 * 2001年1月1日以降の日付を与えられるとその日の曜日を返す
 * 学籍番号: 03-123006
 * 氏名: 岩成達哉(Iwanari Tatsuya)
 */

#include <stdio.h>

/* 関数のプロトタイプ宣言 */
int getDay(unsigned long int, int, int);

int main() {
  unsigned long int year;  // 年
  int month;               // 月
  int day;                 // 日
  char WEEK[7][10] = {"Monday", "Tuseday", "Wednesday", "Thursday", // 表示用
		      "Friday", "Saturday", "Sunday"};              //
  int index;  // 曜日

  // 入力
  printf("2001年1月1日以降の日を入力するとその日の曜日を求めます\n");
  printf("年: ");
  scanf("%lu", &year);
  printf("月: ");
  scanf("%d", &month);
  printf("日: ");
  scanf("%d", &day);

  // 曜日を求め, 入力が不正ならエラー終了
  if ( (index = getDay(year, month, day)) == -1) {
    printf("2001年1月1日以降の正しい形式の日ではありません\n");
    return 1;
  }
  
  // 曜日を表示(1年1月1日は月曜日)
  printf("%lu-%d-%d is a %s\n", year, month, day, WEEK[index]);
  
  return 0;
}



/* 2001年1月1日からの経過日数を求める関数
 * 引数
 *   year  : 入力された年
 *   month : 入力された月
 *   day   : 入力された日
 * 戻り値
 *   0:月, 1:火, 2:水, 3:木, 4:金, 5:土, 6:日
 */
int getDay(unsigned long int year, int month, int day) {
  int total = 0;  // 日数の合計
  int remainder;  // 判定に用いる
  const int DOM[] = {31, 28, 31, 30, 31, 30,  // 月の日数
		     31, 31, 30, 31, 30, 31}; //
  int leap = 0;  // うるう年の判定フラグ
  int i;
 
  // 入力された年がうるう年か判定
  if ( ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0) ) {
    leap = 1;
  }

  // 正しい値か判定
  if (year < 2001
      || (month < 1 || month > 12)
      || (day < 1 || day > DOM[month - 1])) {
    
    // 入力された日がうるう年かつ2月なら29日も許す
    // それ以外ならエラーを返す
    if (! (leap && month == 2 && day == 29) ) {
      return -1;
    }
  }
  
  // 400の倍数を足したり引いたりした年の曜日は同じ
  remainder = year % 400 + 400;

  // うるう年を考えずに計算
  total += (remainder - 1) * 365;
  
  // うるう年を考慮
  total += remainder / 4 - remainder / 100 + remainder / 400;
  
  // 入力された日がうるう年であるが, まだ2月29日を迎えていないなら1日多く足している
  if (leap && month < 3) {
    total -= 1;
  }

  // 月の日数を足す
  for (i = 0; i < month - 1; ++i) {
    total += DOM[i];
  }
  
  // 日数をたす
  total += day - 1;

  return total % 7; // 曜日を返す
}
