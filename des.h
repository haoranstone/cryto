#ifndef DES_H
#define DES_H

#include "types.h"


extern int des_encrypt(U8* data_in, U8* data_out, U8* key);

extern int des_decrypt(U8* data_in, U8* data_out, U8* key); 



#endif
