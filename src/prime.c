#include <math.h>
#include "prime.h"


// check if a number is prime
int is_prime(const int x){
	// 1 - prime
    // 0 - not prime
    // -1 - undefined (i.e. x < 2)
  	if(x<2) {return -1;}
    if (x<4) {return 1;}
    if((x%2)==0){return 0;}
    for(int i = 3; i <= floor(sqrt((double) x)); i += 2){
      // skipping every even number, as they arer divisible by 2
      if ((x%i)==0){return 0;}
    }
    return 1;
}

// find ext prime afterr x, or x if x is prime
int next_prime(int x){
  while(is_prime(x)!=1){
    x++;
	}
   return x;
}

