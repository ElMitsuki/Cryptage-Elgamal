#include "utils.h"
#include <stdlib.h>
#include <time.h>

/*
  Fonction qui choisit un nombre aléatoire entre min (inclus) et max (exclus)
   - min : borne inferieure
   - max : borne superieure
  Code de retour : le nombre aléatoire choisit par la fonction
*/
uint32_t random_number(uint32_t min, uint32_t max){
  int r = rand()%(max-min)+min;
  return (uint32_t)r;
}

/*
  Fonction calculant la puissance d'un nombre passé en paramètre et dont l'exposant est egalement passé en paramètre.
   - value : la valeur qui sera mis à la puissance
   - exponent : valeur de l'exposant
  Code de retour : le resultat de la puissance de value par exponent
*/
uint32_t power(uint32_t value, uint32_t exponent){
  uint32_t res = value;
  if(exponent == 0){
    return 1;
  }
  for(uint32_t i = 1; i<exponent; i++){
    res = res * value;
  }
  return res;
}

/*
  Fonction calculant la puissance modulaire d'un nombre passé en paramètre et dont l'exposant ainsi que le modulo sont egalement passés en paramètres
   - value : la valeur qui sera mis à la puissance
   - exponent : valeur de l'exposant
   - mod : le modulo
  Code de retour : le resultat de la puissance de value par exponent modulo mod
*/
uint32_t power_modulo(uint64_t value, uint64_t exponent, uint64_t mod) {

  uint64_t res = value;
  if(exponent == 0){
    res = 1;
  }
  for(uint64_t i = 1; i<exponent; i++){
    res = (res*value) % mod;
  }
  return (uint32_t) res;
}

/*
   Fonction calculant les coefficients de bezout par rapport à des nombres a et b passés en paramètres. La formule est, pour rappel, au + bv = pgcd(a,b). L'utilisation de cette fonction necessite d'utiliser
   des __int64 car les coefficients de bezout peuvent être négatifs et les multiplications effectuées peuvent exceder le codage sur 32 bits
    - a : premier nombre
    - b : deuxieme nombre
  Code de retour : une structure de type coef_bezout. Cette structure contient les coefficients u et v permettant de résoudre l'equation cité en haut et elle contient aussi le reste finale de l'equation qui correspond au pgcd(a,b)
*/
struct coef_bezout bezout(__int64_t a, __int64_t b) {

  struct coef_bezout coef;
  __int64_t pgcdAB = pgcd(a, b);
  __int64_t r0 = a;
  __int64_t r1 = b;
  __int64_t q = 0;
  __int64_t u0 = 1;
  __int64_t v0 = 0;
  __int64_t u1 = 0;
  __int64_t v1 = 1;
  __int64_t tempR1;
  __int64_t tempU0;
  __int64_t tempV0;

  while(r1 != pgcdAB){
    q = r0 / r1;

    tempR1 = r1;
    r1 = r0 - q*r1;
    r0 = tempR1;

    tempU0 = u0;
    tempV0 = v0;
    u0 = u1;
    v0 = v1;
    u1 = tempU0 - q*u1;
    v1 = tempV0 - q*v1;
  }

  coef.r = r1;
  coef.u = u1;
  coef.v = v1;

  return coef;
}

/*
  Fonction determinant l'inverse d'un nombre modulo un autre nombre
   - mod : le modulo. Ce nombre correspond à l'anneau dans lequel ou souhaite connaitre l'inverse de l'autre nombre passé en paramètre
   - b : le nombre dont on cherche l'inverse dans Z/modZ
  Code de retour : l'inverse de b modulo mod
*/
uint32_t inverse(__int64_t mod, __int64_t b) {
  struct coef_bezout coef = bezout(mod, b);
  __int64_t inv = -1;

  if(coef.r == 1) {
    inv = coef.v;
    while(inv < 0){
      inv = inv + mod;
    }
  }

  return (uint32_t)inv;
}

/*
  Fonction calculant le pgcd d'un nombre a avec un nombre b
    - a : premier nombre
    - b : deuxieme nombre
  Code de retour : le pgcd de a et de b
*/
__int64_t pgcd(__int64_t a, __int64_t b){
  __int64_t reste;

  reste = a % b;
  if (reste == 0){
    return b;
  }
  else{
    return pgcd(b, reste);
  }
}

/*
  Fonction determinant si oui ou non un nombre se trouve dans un tableau
   - tab : adresse du tableau
   - size_tab : taille du tableau pointé par tab
   - a : le nombre où l'on souhaite savoir si il se trouve dans le tableau tab
  Code de retour : 0 si le nombre a ne se trouve pas dans le tableau, 1 sinon.
*/
uint8_t is_in_array(uint64_t* tab, uint32_t size_tab, uint32_t a){
  for(uint32_t i = 0; i<size_tab; i++){
    if(a == tab[i]){
      return 1;
    }
  }
  return 0;
}

/*
  La fonction witness (temoin en français) determine si un nombre a est temoin d'un nombre n, c'est-à-dire qu'elle determine si n peut être decomposé par a, ce qui voudrait dire que n ne serait alors pas premier.
  Si a n'est pas temoin de n alors n est potentiellement premier car il faudrait tester avec plusieurs valeurs de a différentes pour être pratiquement sûr que n est premier. Pour cela, la fonction utilise l'algorithme
  d'exponentiation rapide afin de calculer a^n-1 modulo n car si a^(n-1) != 1 modulo n alors on est sûr que n n'est pas premier.
   - a : le nombre temoin
   - n : le nombre où l'on souhaite savoir si il est composé ou pas
  Code de retour : 0 si n n'est pas composé, 1 sinon.
*/
uint8_t witness(uint32_t a, uint32_t n){
  uint32_t m = n-1;
  uint32_t y = 1;
  while(m != 0){
    if(m%2 == 1){
      y = (a*y) % n;
      m = m-1;
    }
    else{
      uint32_t temp = a;
      a = (a*a) % n;
      if(a == 1 && temp!=1 && temp!=n-1){
        return 1; //n est composé, c'est-à-dire que n peut être décomposé en utilisant a donc que n n'est pas premier
      }
      m = m/2;
    }
  }
  if(y != 1){
    return 1; //n est composé, c'est-à-dire que n peut être décomposé en utilisant a donc que n n'est pas premier
  }
  else{
    return 0;
  }
}

/*
  Fonction determinant si oui ou non un nombre est premier grâce au principe de l'algorithme de miller rabin. Lorsque la fonction pense avoir trouvé un nombre premier, le pourcentage de chance que ce nombre
  soit réellement premier dépend fortement du nombre d'essai.
   - n : le nombre où l'on souhaite savoir si il est premier ou pas
   - t : le nombre d'essai, à savoir le nombre de témoin testé
  Code de retour : 0 si n n'est pas premier, 1 sinon
*/
uint8_t miller_rabin(uint64_t n, uint32_t t){
  for(uint8_t i = 0; i<t; i++){
    uint32_t a = random_number(2, n);
    if(witness(a, n)==1){
      return 1; //n est composé, c'est-à-dire que n peut être décomposé en utilisant a donc que n n'est pas premier
    }
  }
  return 0; //n est fortement pseudo-premier en base a pour t entiers
}

/*
  Fonction choisissant aléatoirement un nombre premier
   - k : le nombre de bits sur lequel doit être codé le nombre premier. Ce nombre k doit être inferieure ou égal à 32
   - t : le nombre d'essai, à savoir le nombre de témoin testé
*/
uint32_t prime_number_rand(uint8_t k, uint32_t t){
  if(k<=32){
    uint32_t p = 0;
    uint32_t boolean = 0;
    do{
      p = random_number(60000, power(2,k)-1);
      boolean = miller_rabin(p, t);
    }while((boolean!=0) && (p>2));
    return p;
  }
  else{
    return -1;
  }
}
