#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash_table.h"
#include "prime.h"


static ht_item HT_DELETED_ITEM = {NULL, NULL};

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
// ht_hash_table* ht_new(){
//   // allocate memory for the hashtable
//   ht_hash_table* ht = malloc(sizeof(ht_hash_table));
//
//   // set the size of the ht
//   ht->size = 53;
//   // current count of items in the ht
//   ht->count = 0;
//
//   // initialize arary of items
//   // 1st input to calloc is number of elements we want to allocate memory for
//   // 2nd input to calloc is size of each element in bytes
//   ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
//   return ht;
// }

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

// method to insert into the hash table
void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
  // add the key and value in the memory
  ht_item* item = ht_new_item(key,value);
  // generate the index in the hashtable to store the value at
  int index = ht_get_hash(key, ht->size,0);
  // to check if there is already a value at the given index
  ht_item* curr_item = ht->items[index];
  int i = 1;
  // until we find a memory location in hashtable that is null or we find a deleted node, we run the loop
  while (curr_item != NULL && curr_item != &HT_DELETED_ITEM) {
    // if an item with key at the location is found
    if (strcmp(curr_item->key, key) == 0) {
      // delete this item
      ht_del_item(curr_item);
      // add the new item at the index
      ht->items[index] = item;
      return;
    }
    index = ht_get_hash(item->key,ht->size,i);
    curr_item = ht->items[index];
    i++;
  }
  // store the item in the hash table at index
  ht->items[index] = item;
  // increase the count of items in the hashtable
  ht->count++;
}

// search
char* ht_search(ht_hash_table* ht, const char*  key) {
  // generate index from the key
  int index = ht_get_hash(key, ht->size,0);
  // get item stored at the generated index
  ht_item* item = ht->items[index];
  int i = 1;
  // if item isnt NULL we check if we have found the matching key
  while (item!= NULL) {
    // if the provided key matches to the key stored in hash table, return the value
    // checking if the item at the index isnt deleted item
    if (item != &HT_DELETED_ITEM){
      if (strcmp(item->key, key) == 0) {
        return item-> value;
      }
    }
    // else we try to generate hash again, as the data mightve be stored somewhere else due to collision
    index = ht_get_hash(item->key, ht->size,i);
    // get the item
    item = ht->items[index];
    //increase the attempt for hashing
    i++;
  }

  // if we reach here, it means the key isnt available in the hashtable so return null
  return NULL;
}

// delete
void ht_delete(ht_hash_table* ht, const char* key) {
  int index = ht_get_hash(key, ht->size,0);
  ht_item* item = ht->items[index];
  int i =1;
  while (item!=NULL) {
    // checking if the item at index isnt already deleted
    if (item != &HT_DELETED_ITEM) {
      if(strcmp(item->key, key) == 0) {
        // if we found the item, delete it
        ht_del_item(item);
        ht->items[index] = &HT_DELETED_ITEM;
      }
    }
    index = ht_get_hash(item->key, ht->size,i);
    item = ht->items[index];;
    i++;
  }
  // decrease the count of item in the hashtable
  ht->count--;
}

//
static ht_hash_table* ht_new_sized(const int base_size) {
  ht_hash_table* ht = malloc(sizeof(ht_hash_table));

  // calculate the next prime after base_size
  ht->size = next_prime(base_size);
  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
  return ht;
}

ht_hash_table* ht_new() {
  return ht_new_sized(HT_INITIAL_BASE_SIZE);
}