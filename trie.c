#include "trie.h"
#include "stdio.h"

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
    if (c < TRIE_FIRST_CHAR || c > TRIE_LAST_CHAR) {
        return; // invalid key
    } 
    if (trie->children[c - TRIE_FIRST_CHAR] == NULL) {
        trie->children[c - TRIE_FIRST_CHAR] = malloc(sizeof(Trie));
        if (trie->children[c - TRIE_FIRST_CHAR] == NULL) {
            fprintf(stderr, "memory allocation failed\n");
            exit(1);
        }
        initTrie(trie->children[c - TRIE_FIRST_CHAR]);
    }
    insertTrie(trie->children[c - TRIE_FIRST_CHAR], key + 1, value);
}

void * searchTrie(Trie *trie, const char * key) {
    if (key[0] == '\0') {
        return trie->is_key ? trie->value : NULL;
    } else {
        char c = key[0];
        if (c < TRIE_FIRST_CHAR || c > TRIE_LAST_CHAR) {
            return NULL; // invalid key
        }
        void * result = trie->children[c - TRIE_FIRST_CHAR] != NULL 
                ? searchTrie(trie->children[c - TRIE_FIRST_CHAR], key + 1)
                : NULL;
        return result;
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