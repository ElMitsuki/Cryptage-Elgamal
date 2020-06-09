#include "elgamal.h"
#include <string.h>
#include <stdio.h>

/*
  Fonction effectuant le chiffrement d'un message passé en paramètre à partir d'une clé publique passé également en paramètre
   - msg : correspond au message de type "message", une union qui peut être interprété comme un bloc de 32 bits ou bien un tableau de 4 blocs de 8 bits
   - pk : adresse de la structure contenant la clé publique à savoir : le nombre premier (pk->p), son élément primitif le plus petit (pk->g) ainsi qu'un autre élément inversible dans Z/pZ (pk->A)
  Code de retour : un bloc contenant le message ciffré. Un bloc est une union qui peut être interprété comme un bloc de 64 bits ou bien un tableau de 2 blocs de 32 bits
*/
bloc bloc32_encrypton(message msg, struct public_key* pk){
  uint32_t k = /*random_number(1, pk->p)*/2;
  uint32_t y1 = power_modulo(pk->g, k, pk->p);
  uint32_t y2 = ((uint64_t)msg.i32 * (uint64_t)(power_modulo(pk->A, k, pk->p))) % (pk->p);

  bloc encrypton;
  encrypton.i32[0] = y2;
  encrypton.i32[1] = y1;

  return encrypton;
}

/*
  Fonction effectuant le dechiffrement d'un bloc contenant le message chiffré à partir d'une clé privée passé également en paramètre
   - enc_msg : le message chiffré, de type bloc
   - pk : adresse de la structure contenant la clé privée à savoir : le nombre premier (pk->p) et le nombre a tel que g^a modulo p soit égal à A (pk->a)
  Code de retour : le message dechiffré de type "message"
*/
message bloc64_decrypton(bloc enc_msg, struct private_key* pk){
  message m;
  m.i32 = ((uint64_t)enc_msg.i32[0] * (uint64_t)(inverse(pk->p, power_modulo(enc_msg.i32[1], pk->a, pk->p)))) % (pk->p);
  return m;
}

/*
  Fonction permettant d'encoder une suite de blocs de 8 bits en une suite de blocs de 32 bits en concatenant les blocs de 8 bits par 4
   - msg : adresse du tableau contenant l'ensemble des blocs de 8 bits
   - msg_result : adresse du tableau qui contiendra la suite des blocs de 32 bits
   - msg_size : taille du tableau pointé par msg
   - res_size : taille du tableau pointé par msg_result
  Code de retour : Aucun retour car on remplit le tableau dont l'adresse est passé en paramètre
*/
void bloc32_encode(uint8_t* msg, uint32_t* msg_result, uint32_t msg_size, uint32_t res_size){
  uint32_t loop = 0;
  for(uint8_t i = 0; i<res_size; i++){
    message msg_encode;
    memset(msg_encode.i8, 0, sizeof(msg_encode.i8));
    if(loop < msg_size){
      msg_encode.i8[loop%4] = msg[loop];
      loop++;
      while(loop%4 != 0){
        if(loop < msg_size){
          msg_encode.i8[loop%4] = msg[loop];
        }
        loop++;
      }
      msg_result[i] = msg_encode.i32;
    }
  }
}

/*
  Fonction effectuant l'operation inverse de l'encodage, à savoir reconvertir la suite de blocs de 32 bits en une suite de blocs de 8 bits grâce au type union
   - msg : adresse du tableau contenant la suite de blocs de 32 bits
   - decode_msg : adresse du tableau qui contiendra la suite de blocs de 8 bits
   - res_size : taille du tableau pointé par msg
  Code de retour : Aucun retour car on remplit le tableau dont l'adresse est passé en paramètre
*/
void bloc32_decode(uint32_t* msg, uint8_t* decode_msg, uint32_t res_size){
  uint32_t loop = 0;

  for(uint8_t i = 0; i<res_size; i++){
    message msg_encode;
    msg_encode.i32 = msg[loop/4];
    for(uint8_t j = 0; j<4; j++){
      if(msg_encode.i8[j] != 0) {
        decode_msg[loop] = msg_encode.i8[j];
      }
      loop++;
    }
  }
}

/*
  Fonction calculant l'élement primitif le plus petit d'un nombre premier p passé en paramètre et choisissant aléatoirement un autre élement inversible de Z/pZ
   - p : nombre premier dont on cherche l'élement primitif
  Code de retour : une structure de type primitive contenant l'élement primitif calculé ainsi que le nombre inversible choisit aleatoirement
*/
struct primitive primitive_elem(uint32_t p){
  uint32_t cardinal = p-1;
  uint8_t trouvee = 0;
  uint32_t g = 2;
  uint32_t k = 0;
  uint64_t tab_reste[cardinal];
  memset(tab_reste, 0, sizeof(tab_reste));
  struct primitive prim;

  while((g<=cardinal) && (trouvee==0)){
    uint64_t reste_courant = 0;
    uint32_t fin = 0;

    while((k < cardinal) && (fin == 0)){
      if(k == 0){
        tab_reste[0] = 1;
      }
      else{
        reste_courant = (tab_reste[k-1] * g) % p;
        if(is_in_array(tab_reste, cardinal, reste_courant)==0){
          tab_reste[k] = reste_courant;
        }
        else{
          fin = 1;
        }
      }
      k++;
    }

    if(fin == 0){
      trouvee = 1;
    }
    else{
      g++;
    }

    memset(tab_reste, 0, sizeof(tab_reste));
    k = 0;
  }

  prim.g = g;
  prim.A = random_number(1, p);

  return prim;
}

/*
  Fonction effectuant le remplissage d'une clé publique et d'une clé privée passé en paramètre
   - pubK : adresse de la structure de la clé publique
   - privK : adresse de la structure de la clé privée
  Code de retour : Aucun car on remplit les structures dont les adresses sont passées en paramètres
*/
void key_generator(struct public_key* pubK, struct private_key* privK){
  uint32_t p = prime_number_rand(16, 200);
  struct primitive prim = primitive_elem(p);
  uint32_t g = prim.g;
  uint32_t A = prim.A;
  uint32_t a = 0;
  while(power_modulo(g,a,p)!=A){
    a++;
  }

  pubK->p = p;
  pubK->g = g;
  pubK->A = A;
  privK->p = p;
  privK->a = a;
}
