#include <time.h>
#include <stdlib.h>
#include "SAND128128.h"

typedef uint16_t u16;
typedef uint32_t u32;
#define ROUNDS128128 54

u32 Get_Random_u32(){
    return (u16)rand() ^ ((u32)rand() << 16);
}

u64 Get_Random_u64(){
    return Get_Random_u32() ^ ((u64)Get_Random_u32() << 32);
}

u32 Bit_Flip_u32(u32 x, int position){
    x ^= (u32)1 << position;
    return x;
}

u64 Bit_Flip_u64(u64 x, int position){
    x ^= (u64)1 << position;
    return x;
}

int Check_Bit(int position, int test_rounds){
    int i, test_set = 1000;
    u64 diff_A = 0, diff_B = 0;
    for(i = 0; i < test_set; i++){
        u64 Pt0[2], Pt1[2], Ct0[2], Ct1[2], Mk[2], Rk[ROUNDS128128];
        Pt0[0] = Get_Random_u64(), Pt0[1] = Get_Random_u64();
        Pt1[0] = Pt0[0], Pt1[1] = Pt0[1];
        Mk[0] = Get_Random_u64(), Mk[1] = Get_Random_u64();

        KeySchedule(Mk, Rk, ROUNDS128128, 0);
        Round(Pt0, Ct0, Rk, test_rounds);

        if(position < 64){
            Pt1[0] = Bit_Flip_u64(Pt0[0], position);
        }
        else{
            Pt1[1] = Bit_Flip_u64(Pt0[1], position - 64);
        }

        Round(Pt1, Ct1, Rk, test_rounds);

        u64 d_a, d_b;
        d_a = Ct1[0] ^ Ct0[0];
        d_b = Ct1[1] ^ Ct0[1];
        diff_A |= d_a;
        diff_B |= d_b;
    }
    if(diff_A == 0xFFFFFFFFFFFFFFFFLL && diff_B == 0xFFFFFFFFFFFFFFFFLL){
        return 1;
    }
    return 0;
}

void Fulldiffusion_test(int test_rounds){
    int r;
    for(r = 1; r <= test_rounds; r ++){
        int counter = 0;
        int p = 0;
        for(p = 0; p < 128; p++){
            counter += Check_Bit(p, r);
        }
        printf("Test %2d rounds: with %3d bits achieving full diffusion"
               "\n", r, counter);
    }
}

int main(){
    u64 Pt[2] = {0x7F6F5F4F3F2F1F0F, 0xFFEFDFCFBFAF9F8F}, Ct[2];
    /* Rk0 || Rk1 */
    u64 Mk[2] = {0x1716151413121110, 0x1F1E1D1C1B1A1918};
    u64 Rk[ROUNDS128128];

    printf("Pt: 0x%016lX 0x%016lX\n", Pt[1], Pt[0]);
    printf("MK: 0x%016lX 0x%016lX\n", Mk[1], Mk[0]);
    printf("Process Enc\n");
    KeySchedule(Mk, Rk, ROUNDS128128, 0);
    Round(Pt, Ct, Rk, ROUNDS128128);
    printf("Ct: 0x%016lX 0x%016lX\n", Ct[1], Ct[0]);
    printf("Process Dec\n");
    KeySchedule(Mk, Rk, ROUNDS128128, 1);
    Round(Ct, Pt, Rk, ROUNDS128128);
    printf("Pt: 0x%016lX 0x%016lX\n", Pt[1], Pt[0]);
    printf("\n");

    srand(time(NULL));
    Fulldiffusion_test(10);

    return 0;
}
