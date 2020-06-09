#include <stdint.h>

struct coef_bezout{
  __int64_t r;
  __int64_t u;
  __int64_t v;
};

uint32_t random_number(uint32_t min, uint32_t max);
struct coef_bezout bezout(__int64_t a, __int64_t b);
uint32_t power(uint32_t value, uint32_t exponent);
uint32_t power_modulo(uint64_t value, uint64_t exponent, uint64_t mod);
uint32_t inverse(__int64_t mod, __int64_t b);
__int64_t pgcd(__int64_t a, __int64_t b);
uint8_t is_in_array(uint64_t* tab, uint32_t size_tab, uint32_t a);
uint8_t witness(uint32_t a, uint32_t n);
uint8_t miller_rabin(uint64_t n, uint32_t t);
uint32_t prime_number_rand(uint8_t k, uint32_t t);
