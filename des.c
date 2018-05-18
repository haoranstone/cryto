#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "des.h"

// initial permutation table
U8 IP[]={  58, 50, 42, 34, 26, 18, 10, 2,
           60, 52, 44, 36, 28, 20, 12, 4,
           62, 54, 46, 38, 30, 22, 14, 6,
           64, 56, 48, 40, 32, 24, 16, 8,
           57, 49, 41, 33, 25, 17, 9, 1,
           59, 51, 43, 35, 27, 19, 11, 3,
           61, 53, 45, 37, 29, 21, 13, 5,
           63, 55, 47, 39, 31, 23, 15, 7
         };

// Inverse permutation table
U8 InvIP[]={ 40, 8, 48, 16, 56, 24, 64, 32,
	           39, 7, 47, 15, 55, 23, 63, 31,
	           38, 6, 46, 14, 54, 22, 62, 30,
	           37, 5, 45, 13, 53, 21, 61, 29,
	           36, 4, 44, 12, 52, 20, 60, 28,
	           35, 3, 43, 11, 51, 19, 59, 27,
	           34, 2, 42, 10, 50, 18, 58, 26,
	           33, 1, 41, 9,  49, 17, 57, 25
	         };

//Expand function
U8 E[] = {   32, 1 , 2 , 3,  4,  5,
	           4 , 5 , 6 , 7,  8,  9,
	           8 , 9 , 10, 11, 12, 13,
	           12, 13, 14, 15, 16, 17,
	           16, 17, 18, 19, 20, 21,
	           20, 21, 22, 23, 24, 25,
	           24, 25, 26, 27, 28, 29,
	           28, 29, 30, 31, 32, 1
	        };

// Permutation
U8 P[] ={   16, 7 , 20, 21,
	          29, 12, 28, 17,
	           1, 15, 23, 26,
	           5, 18, 31, 10,
	           2, 8 , 24, 14,
            32, 27, 3 , 9 ,
	          19, 13, 30, 6 ,
	          22, 11, 4 , 25
   	};

// Key Schedule PC1 table
U8 PC1[]= {
	   57, 49, 41, 33, 25, 17, 9,
	   1 ,58, 50, 42, 34, 26, 18,
	   10, 2, 59, 51, 43, 35, 27,
	   19, 11, 3, 60, 52, 44, 36,
	   63, 55, 47, 39, 31, 23, 15,
	   7 ,62 ,54, 46, 38, 30, 22,
	   14, 6 ,61, 53, 45, 37, 29,
	   21, 13, 5, 28, 20, 12, 4
	};

// Key Schedule PC2 table
U8 PC2[] = {
	   14, 17, 11, 24, 1, 5,
	   3 ,28, 15, 6 ,21, 10,
	   23, 19, 12, 4 ,26, 8,
	   16, 7 ,27, 20 ,13, 2,
	   41, 52, 31, 37, 47, 55,
	   30, 40, 51, 45, 33, 48,
	   44, 49, 39, 56, 34, 53,
	   46, 42, 50, 36, 29, 32
	};

//key shift numbers
U8 keyshift[]={ 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

// substition SBox
U8 SBoxes[][16]={
    //s0
		14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
		0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
		4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
		15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,

    //s1
		15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
		3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
		0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
		13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,

    //s2
		10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
		13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
		13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
		1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,

    //s3
		7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
		13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
		10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
		3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,

    //s4
		2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
		14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
		4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
		11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,

    //s5
		12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
		10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
		9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
		4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,

    //s6
		4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
		13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
		1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
		6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,

    //s7
    13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
    1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
    7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
    2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
		};

// HEX table
static U8 table[] ="0123456789ABCDEF";

// 16 rounds, each round 48bits
static U32 RK[16][2];

static void debug(U8* str, U8* dt, unsigned int length )
{
   unsigned int i= 0;
   U8 tmp = 0x00;
   fprintf(stdout, "%-40s:", str);
   for (i = 0; i < length; i++)
   {
     fprintf(stdout, "%02X", dt[i]);
   }
   fprintf(stdout, "\r\n");
}


static U8 bitReverse(U8 in)
{
   U8 tmp;
   tmp = ((in & 0x80) >> 7) | ( (in & 0x01) << 7) | ((in & 0x40) >> 5) | ( (in & 0x02) << 5 ) | ( (in & 0x20) >> 3) | ( (in & 0x04)  << 3) | ( (in & 0x10) >> 1 ) | ( (in & 0x08) << 1);
return tmp;
}

//PC1 -> C0,D0 each one 28bits
static void pc1_permutation(U8* key,U32* out)
{
  unsigned int length = sizeof(PC1)/sizeof(U8);
  unsigned int i = 0,index,bit;
  unsigned int byteindex, bitindex;
  memset((U8*)out,0,8);
  for ( i = 0; i < length; i ++)
  {
     index = PC1[i]-1; // bit index in bit sequence (starting from 1)
     byteindex = index>>3;
     bitindex  = 7- index&0x07;
     bit =(key[byteindex] & (1<< bitindex))>>(bitindex);
     out[i/28] |= ( bit << (i%28));
  }
}

// left rotate actually right shift in U32 word
static U32 rotate_bits(U32 C,unsigned int bits)
{
   unsigned int i = 0;
   U32 tmp =0;
   U32 tp = C;
   for (i = 0; i < bits; i ++)
   {
      tmp = tp&0x01;
      tp = (tp>>1)|(tmp<<27);
   }
   return tp;
}

//56bits -> 48bits;
static void pc2_permutation(U32* in, U32* out)
{
  unsigned int i = 0,idx;
  unsigned int length = sizeof(PC2)/sizeof(U8);
  U8 bit = 0;
  memset((U8*)out,0,8);//clear round key buffer

  for (i = 0; i< length; i++)
  {
    idx = PC2[i]-1;
    bit = (in[idx/28] >> (idx%28))&0x01;
    out[i>>5] |= (bit << (i&0x1F));
  }
}

static void keyshedule(U8* key)
{
  /* all round key are 2*32bits  and only 48bits are available */
   unsigned int i =0 ;
   U32 C[2];
   pc1_permutation(key,C);
   for (i = 0; i < 16; i++)
   {
      C[0] = rotate_bits(C[0],keyshift[i]);
      C[1] = rotate_bits(C[1],keyshift[i]);
      pc2_permutation(C,&RK[i][0]);
   }
}

////////////////////////////////////////////////////////////////////////////////
// input 64bits  permutation to 64bits
static void bitPermutation(U8* bitmaptbl, unsigned int bitlen, U8* in ,U8* out)
{
   unsigned int i,index,byte;
   unsigned int ByteIdx,BitIdx;
   memset(out,0, bitlen>>3);
   for ( i = 0; i < bitlen; i ++)
   {
        index = bitmaptbl[i]-1; // bit index in the entire byte array staring from 1
	ByteIdx = (index >> 3);
	BitIdx  = 7 - (index & 0x07);
        byte  = (in[ByteIdx]>>BitIdx)&0x01; // selected bit and move to rightmost
        out[i>>3] |= (byte << (i&0x07));
   }
}



/**
* mode = 0  initial
* mode = 1  inverse
*/
static void initPermutation(U8* in, U32* out)
{
  //do permutation for data in  OR data out
  U8 temp[8];
  unsigned int bitlen = 0;
  bitlen = sizeof(IP)/sizeof(U8);
  bitPermutation(IP,bitlen,in,temp);
  memcpy((U8*)&out[0],temp,8);
  //debug("INITIAL PERMUTATION(64bits)",(U8*)&temp[0],8);
}

static void inversePermutation(U8* in , U8* out)
{
  U8 temp[8];
  U8 cnt = 0;
  unsigned int bitlen = 0;
  bitlen = sizeof(InvIP)/sizeof(U8);
  bitPermutation(InvIP,bitlen,in,temp);
  
  for (cnt =0 ; cnt < 4; cnt ++)
  {
       temp[cnt]   = bitReverse(temp[cnt]);
       temp[7-cnt] = bitReverse(temp[7-cnt]);
       temp[cnt]  ^= temp[7-cnt];
       temp[7-cnt]^= temp[cnt];
       temp[cnt]  ^= temp[7-cnt];
  }
    memcpy(out,temp,8);
 // printf("%-40s:%08X%08X\r\n","ENCRYPTION OUTPUT",*((U32*)&out[4]),*((U32*)&out[0]));
}

/////////////////////////////////////////////////////////////////////////////////////////////
/*
*  in = 32bits
* out = 48bits;
*/
static void Expand(U32 in ,U32* out)
{
  unsigned int length = sizeof(E)/sizeof(U8);
  unsigned int i,idx;
  memset((U8*)&out[0],0,6);
  for ( i = 0 ; i < length; i ++)
  {
     idx = E[i] - 1;
     out[i>>5] |= (((in &(1<<idx))>>idx)<<(i&0x1F));
  }
  //printf("%-40s:%08X%08X\r\n","EXPAND(48bits)",out[1],out[0]);
}

/*
*  in = 32bits , out 32bits
*/
static void RoundPermutation(U32 in, U32* out)
{
  unsigned int length = sizeof(P)/sizeof(U8);
  unsigned int i,idx;
  U32 temp=0;
  U32 output = 0;
  for ( i = 0 ; i < length; i ++)
  {
     idx = P[i] - 1;
     temp = (in >> idx)&0x01;
     output |= (temp << i);
  }
  *out = output;
  //printf("%-40s:%08X\r\n","ROUND PERMUTATION(32bits) ",output);
}



// input: in        48bits
//        subkey    48bits
//        out       32bits
static void SBox(U32* in, U32* subkey, U32* out)
{
   unsigned int i =0 ;
   U32 output     = 0;
   U8  preSbox ,j,k,t;
   U32 postSbox;

   //XOR first
   in[0] = in[0]^subkey[0];
   in[1] = in[1]^subkey[1];
   //SBox substition
   for (i = 0; i < 8; i++)
   {
      //first find 6bits . 32-6 = 26
      preSbox = in[0] & 0x3F;
      in[0] = (in[0]>>6) | ((in[1]&0x3F) << 26);
      in[1] = (in[1]>>6);
      // calc row index and column index
      t=preSbox = bitReverse(preSbox<<2);
      j = ((preSbox>>4)&0x02)|(preSbox&0x01);
      k = (preSbox>>1)&0x0F;
      // calc SBox table index; SBox[][16]
      preSbox =  SBoxes[4*i+j][k];
      postSbox = bitReverse(preSbox << 4) ;
      output  |= (postSbox << (4*i));
   }
   *out = output;
   //printf("%-40s:%08X\r\n","SBOX OUTPUT ",output);
}

int des_encrypt(U8* data_in, U8* data_out, U8* key)
{
  debug("ENCRYPTION INPUT(64bits)",(U8*)&data_in[0],8);
  debug("ENCRYPTION KEY(64bits)",(U8*)&key[0],8);
  U32 C[2],i;
  U32 preSbox[2];
  U32 postSbox;
  U8  output[8];
  memset((U8*)&C[0],0,8);
  memset((U8*)&preSbox[0],0,8);
  // key schedule first
  keyshedule(key);
  //initial permutation
  initPermutation(data_in,&C[0]);
  // 16 rounds SBox caculation
  for ( i = 0; i < 16; i++){
      Expand(C[1],&preSbox[0]);               // right half expand to 48bits
      SBox(&preSbox[0],&RK[i][0],&postSbox);  // SBOX transoform
      RoundPermutation(postSbox,&postSbox);   // permutation after SBoxoutput;
      postSbox ^= C[0];                       // Left Xor output from permutation.
      C[0] = C[1];                            // swap
      C[1] = postSbox;                        // swap
  }
  // before output from 16 rounds SBOX. swap the last round data
  C[0] ^= C[1];
  C[1] ^= C[0];
  C[0] ^= C[1];
  // Inverse permutation after swap
 inversePermutation((U8*)&C[0],&data_out[0]);
 debug("ENCRYPTION OUTPUT(64bits)",(U8*)&data_out[0],8);
  return 0;

}


int des_decrypt(U8* data_in,U8* data_out, U8* key)
{
  debug("DECRYPTION INPUT(64bits)",(U8*)&data_in[0],8);
  debug("DECRYPTION KEY(64bits)",(U8*)&key[0],8);
  U32 C[2],i;
  U32 preSbox[2];
  U32 postSbox;
  U8  output[8];
  memset((U8*)&C[0],0,8);
  memset((U8*)&preSbox[0],0,8);
  // key schedule first
  keyshedule(key);
  //initial permutation
  initPermutation(data_in,&C[0]);
  // 16 rounds SBox caculation first sub-key is the 16-th round key of encryption
  for ( i = 0; i < 16; i++){
      Expand(C[1],&preSbox[0]);                   // right half expand to 48bits
      SBox(&preSbox[0],&RK[15-i][0],&postSbox);   // SBOX transoform
      RoundPermutation(postSbox,&postSbox);       // permutation after SBoxoutput;
      postSbox ^= C[0];                           // Left Xor output from permutation.
      C[0] = C[1];                                // swap
      C[1] = postSbox;                            // swap
  }
  // before output from 16 rounds SBOX. swap the last round data
  C[0] ^= C[1];
  C[1] ^= C[0];
  C[0] ^= C[1];
  // Inverse permutation after swap
 inversePermutation((U8*)&C[0],&data_out[0]);
 debug("DECRYPTION OUTPUT(64bits)",(U8*)&data_out[0],8);
  return 0;

}
