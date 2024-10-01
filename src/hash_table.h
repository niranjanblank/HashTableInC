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