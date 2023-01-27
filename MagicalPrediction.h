#ifndef __MAGICALPREDICTION_H
#define __MAGICALPREDICTION_H

// Directives and and Definitions

#define ALPHA_SIZE 26
#define MAX_WORD_LENGTH 2097151

typedef struct TrieNode
{
  int frequency; // Stores node frequency
  // flags the last node in a string
  bool is_word;
  struct TrieNode *children[ALPHA_SIZE];
} TrieNode;


// Functional Prototypes

TrieNode *createTrieNode(void);

TrieNode *insert(TrieNode *root, int frequency, char *str);

void printTrie(TrieNode *root);

TrieNode *commands(char *filename);


#endif
