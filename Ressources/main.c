#include "elgamal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//si on lance le programme comme ça, cela peut prendre 1min
int main(int argc,char**argv){
  //test de Haristoy
  /*bloc b;
  b.i64 = 0x000005be00000624;

  struct private_key privK_test;
  privK_test.p = 2579;
  privK_test.a = 765;

  message m = bloc64_decrypton(b, &privK_test);

  printf("nombre = %u\n", m.i32);*/


  //test des fonctions
  srand(time(NULL));

  //rangement du message dans un tableau de uint8_t
  char* msg = "Kraemer == Haristoy";
  uint32_t msg_size = strlen(msg);
  uint8_t mess[msg_size];
  memset(mess, 0, sizeof(mess));
  for(uint8_t i = 0; i<msg_size; i++){
    mess[i] = msg[i];
  }

  //encodage du message : conversion en un tableau de 32 bits
  uint32_t res_size = msg_size/4;
  res_size++;
  uint32_t encode_msg[res_size];
  bloc32_encode(mess, encode_msg, msg_size, res_size);
  for(int i = 0; i<res_size; i++){
    printf("encode_msg[%d] : %u\n", i, encode_msg[i]);
  }

  //generation de la clé publique et de la clé privée
  /*struct public_key pubK;
  struct private_key privK;
  key_generator(&pubK, &privK);
  printf("public key : p = %u, g = %u, A = %u / private key : p = %u, a = %u\n", pubK.p, pubK.g, pubK.A, privK.p, privK.a);*/

  struct public_key pubK;
  pubK.p = 2725838081;
  pubK.g = 6;
  pubK.A = 1247564666;
  struct private_key privK;
  privK.p = 2725838081;
  privK.a = 16807;

  //cryptage du message à l'aide d'elgamal
  uint64_t encrypted_msg[res_size];
  memset(encrypted_msg, 0, sizeof(encrypted_msg));
  bloc b_enc;
  message m_enc;
  for(uint32_t i = 0; i<res_size; i++){
    m_enc.i32 = encode_msg[i];
    b_enc = bloc32_encrypton(m_enc, &pubK);
    printf("message crypté = %lu\n", b_enc.i64);
    printf("y2 = %u, y1 = %u\n", b_enc.i32[0], b_enc.i32[1]);
    encrypted_msg[i] = b_enc.i64;
  }

  //decryptage du message
  uint32_t decrypted_msg[res_size];
  memset(decrypted_msg, 0, sizeof(decrypted_msg));
  bloc b_dec;
  message m_dec;
  for(uint32_t i = 0; i<res_size; i++){
    b_dec.i64 = encrypted_msg[i];
    m_dec = bloc64_decrypton(b_dec, &privK);
    printf("message decrypté = %u\n", m_dec.i32);
    decrypted_msg[i] = m_dec.i32;
  }

  //decodage du message
  uint8_t decode_msg[msg_size];
  bloc32_decode(decrypted_msg, decode_msg, res_size);
  for(uint8_t i = 0; i<sizeof(decode_msg); i++){
    printf("%c", decode_msg[i]);
  }
  printf("\n");

  /*struct public_key pubK;
  struct private_key privK;
  key_generator(&pubK, &privK);
  printf("public key : p = %u, g = %u, A = %u / private key : p = %u, a = %u\n", pubK.p, pubK.g, pubK.A, privK.p, privK.a);*/

  return 0;
}
