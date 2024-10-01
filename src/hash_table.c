#include <stdlib.h>
#include <string.h>
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