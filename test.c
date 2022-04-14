#include <stdio.h>

int main()
{
    int arr[32] ;
    int carry_bit = 1, flag = 0;
    int n = 3, num = 0;
    int i = 0, j = 0;
    int carry_bit_tem = 0;
    int a = 0, b = 4;

    for (i = 0; i < 32; i++){
        arr[i] = 0xffffffff;
    }
    
    while(--b){
        n = 3;
        num = 0;
        for( i = 0; i < 32; i++){
            for(j = 0; j < 32 && num < 1000; j++){
                num ++;

                if (n == 3){
                    a ++;
                    n = 1;
                    carry_bit_tem = carry_bit << j;
                    arr[i] = arr[i] ^ carry_bit_tem;
                    printf("%d: %x, %x\n", j, arr[i], carry_bit_tem);
                }else if (((carry_bit << j) & arr[i])){
                    n++;
                }
                
                if(a == 999){
                    break;
                }
            }
            
            if(a == 999){
                    break;
                }
            printf("%d: %x\n", i, arr[i]);
        }
    }

    printf("%d, %d\n",num, a);

    return 0;
}
