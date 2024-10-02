#pragma once
#define HT_PRIME_1 163
#define HT_PRIME_2 157


// This struct will store items containing key and value
typedef struct {
  // using pointers for key and value allows dynamic memory allocation
    char *key;
    char *value;
} ht_item;

// this struct will define a hashtable of having total size of "size" and
// current number of items in hash table, which helps in resizing
typedef struct {
  int size;
  int count;
  // pointer to an array of pointers to "ht_item"
  // this allows dynamic memory allocation
  ht_item **items;
} ht_hash_table;

// Function to create a new hash table
ht_hash_table* ht_new();

// Function to delete an existing hash table
void ht_del_hash_table(ht_hash_table* ht);