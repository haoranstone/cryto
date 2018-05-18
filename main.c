#include <stdio.h>
#include "des.h"


void main(void)
{
  U8 in[8] ={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
  U8 key[8] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
  U8 out[8];

  des_encrypt(in,out,key);
  des_decrypt(out,in,key);


}