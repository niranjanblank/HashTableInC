#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash_table.h"



// This functions returns pointer to the ht_item
// This is static beacause it will only be called by code intrenal to the hash table
static ht_item* ht_new_item(const char *key, const char *value){
  // allocate memory for the item
  ht_item* item = malloc(sizeof(ht_item));

  // allocate and copy the key and value into the new item
  item->key = strdup(key);
  item->value = strdup(value);

// return the pointer to the new item
  return item;
}

// This function initializes the hashtable
ht_hash_table* ht_new(){
  // allocate memory for the hashtable
  ht_hash_table* ht = malloc(sizeof(ht_hash_table));

  // set the size of the ht
  ht->size = 53;
  // current count of items in the ht
  ht->count = 0;

  // initialize arary of items
  // 1st input to calloc is number of elements we want to allocate memory for
  // 2nd input to calloc is size of each element in bytes
  ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
  return ht;
}

// delete hash table item
static void ht_del_item(ht_item* item){
  // frees the memory associated with the key and values in an item of hashtable
  free(item->key);
  free(item->value);
  // frees item to avoid dangling pointers
  free(item);
}

// delete entire hash table
void ht_del_hash_table(ht_hash_table* ht){
	// free all the items inside the hash table
    for(int i=0; i < ht->size; i++){
    	ht_item* item = ht->items[i];
    	if(item != NULL){
      		ht_del_item(item);
    	}
    free(ht->items);
    free(ht);
    }
}

// hashing function
static int ht_hash(const char* s, const int a, const int m) {
  // hash is initially set to lent to handle potential integer overflow
  long hash = 0;
  int string_len = strlen(s);
  for(int i =0; i < string_len;i++) {
    // calculate the hash
    hash += (long)pow(a,string_len - (i+1)) * s[i];
  // keep the hash value within the desired range
    hash %= m;
  }
  return (int)hash;
}

// handling collisions using open addressing with double hashing
// double hashing uses two hash functions to calculate the index an item should be stored at after i collisions

// the index to be used after collision is given by
// to avoid hash_b() from returning 0 and causing insertion into same bucket over again, we add 1 to it
// index = (hash_a(string) + i * (hash_b(string) + 1)) % num_buckets

static int ht_get_hash(const char* s, const int num_buckets, const int attempt) {
  const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
  int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
  if (hash_b == num_buckets - 1) {
    hash_b = 1;
  }
  return (hash_a + attempt*(hash_b+1)) % num_buckets;
}

