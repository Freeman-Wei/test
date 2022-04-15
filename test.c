/*
    2022.04.15
    laiwei.lv
    功能：
*/
#include <stdio.h>

#define LEN 1000

int main()
{
    int arr[32] ;
    int carry_bit = 1, flag = 0;
    int n = 3, num = 0;
    int i = 0, j = 0;
    int carry_bit_tem = 0;
    int true_bit = 0;

    for (i = 0; i < 32; i++){
        arr[i] = 0xffffffff;
    }
    
    while(1){
        n = 2;
        num = 0;
        for( i = 0; i < 32; i++){
            for(j = 0; j < 32 && num < LEN; j++){

                num ++;

                if (((carry_bit << j) & arr[i])){

                    n++;
                    
                    if (n == 3){
                        true_bit ++;
                        n = 0;
                        carry_bit_tem = carry_bit << j;
                        arr[i] = arr[i] ^ carry_bit_tem;
                        //printf("%d: %x, %x\n", j, arr[i], carry_bit_tem);
                    }
                }
                
                if(true_bit == 999){
                    break;
                }
            }
            
            if(true_bit == 999){
                    break;
                }
        }

        if(true_bit == 999){
                    break;
                }
    }

    for (i = 0; i < 32; i++){
        printf("%d: %x\n", i, arr[i]);
    }

    printf("%d, %d\n",num, true_bit);
    ptintf("test\n");

    return 0;
}
