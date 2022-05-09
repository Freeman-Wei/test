/*
    2022.04.15
    laiwei.lv
    功能：
*/
#include <stdio.h>

#define L 32
#define LEN 1000
#define N 999
#define BREAK if(true_bit == N){\
                    break;\
                }

int main()
{
    int arr[L] ;
    int carry_bit = 1, flag = 0;
    int n = 3, num = 0;
    int i = 0, j = 0;
    int carry_bit_tem = 0;
    int true_bit = 0;

    for (i = 0; i < 32; i++){
        arr[i] = 0xffffffff;
    }
    
    while(true_bit != N){
        n = 2;
        num = 0;
        for( i = 0; i < L && num < LEN; i++){
            for(j = 0; j < L && num < LEN; j++){

                num ++;

                if (((carry_bit << j) & arr[i])){

                    n++;
                    
                    if (n == 3){
                        true_bit ++;
                        n = 0;
                        carry_bit_tem = carry_bit << j;
                        arr[i] = arr[i] ^ carry_bit_tem;
                    }
                }
                BREAK
            }
            BREAK
        }
    }

    for (i = 0; i < L; i++){

        if(arr[i] && i != (L - 1)){
            printf("%d: %x\n", i, arr[i]);

            for(j = 0; j < L; j++){

                carry_bit_tem = carry_bit << j;

                if(arr[i] & carry_bit_tem){

                    num = 32 * i;
                    num = num + j + 1;
                    printf("%d\n", num);
                    break;
                }
            }
            break;
        }
    }

    return 0;
}
