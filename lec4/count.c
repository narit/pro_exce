#include <stdio.h>

int main(){
  int A[26];
  int i, c;

  for(i=0; i<26; i++){
    A[i] = 0;
  }

  while( (c = getchar()) != EOF){
    if ((c >='a') && (c <= 'z')){
      A[c - 'a'] += 1;
    }
  }

  for(i=0; i<26; i++){
    printf("A[%c] = %d\n", i + 'a', A[i]);
    }

  return 0;
}
