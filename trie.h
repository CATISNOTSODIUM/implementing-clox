#ifndef clox_trie_h
#define clox_trie_h

#define TRIE_NUM_KEY 26
#include "common.h"

/**
 * A trie data structure with small alphabetical letter as a key
 * with the assumption that all keys are unique
 */
typedef struct Trie Trie;
struct Trie {
    bool is_key;
    void * value;
    Trie * children[TRIE_NUM_KEY];
};

void initTrie(Trie * trie);
void insertTrie(Trie * trie, const char * key, void * value);
void * searchTrie(Trie *trie, const char *key);
void freeTrie(Trie * trie);
#endif