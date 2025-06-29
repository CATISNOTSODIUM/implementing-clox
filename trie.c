#include "trie.h"

void initTrie(Trie *trie) {
    for (int i = 0; i < TRIE_NUM_KEY; i++) {
        trie->children[i] = NULL;
    }
    trie->is_key = false;
    trie->value = NULL;
}

void insertTrie(Trie *trie, const char * key, void * value) {
    if (key[0] == '\0') {
        trie->is_key = true;
        trie->value = value;
        return;
    }
    char c = key[0];
    if (c < 'a' || c > 'z') {
        return; // invalid key
    } 
    if (trie->children[c - 'a'] == NULL) {
        trie->children[c - 'a'] = malloc(sizeof(Trie));
    }
    insertTrie(trie->children[c - 'a'], key + 1, value);
}

void * searchTrie(Trie *trie, const char * key) {
    if (key[0] == '\0') {
        return trie->is_key ? trie->value : NULL;
    } else {
        char c = key[0];
        if (c < 'a' || c > 'z') {
            return NULL; // invalid key
        }
        return trie->children[c - 'a'] != NULL 
                ? searchTrie(trie->children[c - 'a'], key + 1)
                : NULL;
    }
}

void freeTrie(Trie *trie) {
    if (trie == NULL) {
        return;
    }
    for (int i = 0; i < TRIE_NUM_KEY; i++) {
        if (trie->children[i] != NULL) {
            freeTrie(trie->children[i]);
            free(trie->children[i]);
        }
    }
}