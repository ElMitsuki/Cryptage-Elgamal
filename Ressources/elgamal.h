#include <stdint.h>
#include "utils.h"

struct public_key{
  uint32_t p;
  uint32_t g;
  uint32_t A;
};

struct private_key{
  uint32_t p;
  uint32_t a;
};

struct primitive{
  uint32_t g;
  uint32_t A;
};

typedef union{
  uint32_t i32[2];
  uint64_t i64;
} bloc;

typedef union{
  uint8_t i8[4];
  uint32_t i32;
} message;

bloc bloc32_encrypton(message msg, struct public_key* pk);
message bloc64_decrypton(bloc enc_msg, struct private_key* pk);
void bloc32_encode(uint8_t* msg, uint32_t* msg_result, uint32_t msg_size, uint32_t res_size);
void bloc32_decode(uint32_t* msg, uint8_t* decode_msg, uint32_t res_size);
struct primitive primitive_elem(uint32_t p);
void key_generator(struct public_key* pubK, struct private_key* privK);
