/* Programming Assignment 5
This program is written by: Johnathon Bulman */

// MagicalPrediction.c
// ===================
// Letter prediction using Trie data structure.

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MagicalPrediction.h"

// Create Trie node.
TrieNode *createTrieNode(void)
{
  return calloc(1, sizeof(TrieNode));
}

// Insert a string into a trie. Returns the root of the tree.
TrieNode *insert(TrieNode *root, int frequency, char *str)
{
	int i, index, len = strlen(str);
	TrieNode *wizard = root;

	if (root == NULL)
		wizard = root = createTrieNode();

	for (i = 0; i < len; i++)
	{
		if (!isalpha(str[i])) // skip non-alphabetic characters
			continue;

		index = tolower(str[i]) - 'a'; // make letters consistent

		if (wizard->children[index] == NULL)
			wizard->children[index] = createTrieNode();

		wizard = wizard->children[index];
    wizard->frequency += frequency;
	}

	wizard->is_word = 1; // end of string
	return root;
}

void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)
		return;

	if (root->is_word)
		printf("%-5s (%d)\n", buffer, root->frequency);

	buffer[k + 1] = '\0';

	for (i = 0; i < ALPHA_SIZE; i++)
	{
		buffer[k] = i + 'a';
		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

void printTrie(TrieNode *root)
{
  if (root == NULL)
  {
    printf("(Empty tree...) :(\n");
    return;
  }
  
  printf("\nTrie!\n");
  printf("=====\n");
	char buffer[MAX_WORD_LENGTH + 1];
	buffer[0] = '\0';

	printTrieHelper(root, buffer, 0);
}

// Predicts the next letter based on the highest frequency of the wizard's children.
void magical_prediction(TrieNode *root, char *str, FILE *ofp)
{
  int i, index, max = 0, len = strlen(str);
  TrieNode *wizard = root;

  if (root == NULL)
    return;

  // Hop to the last node in the string.
  for (i = 0; i < len; i++)
  {
    if ((wizard = wizard->children[str[i] - 'a']) == NULL)
    {
      fprintf(ofp, "unknown word\n");
      fprintf(stdout, "unknown word\n");
      return;
    }
  }

  // find the node with the highest frequency
  for (i = 0; i < ALPHA_SIZE; i++)
  {
    if (wizard->children[i] == NULL)
    {
      continue;
    }

    else if (wizard->children[i]->frequency > max)
    {
      max = wizard->children[i]->frequency;
    }
  }

  // if max is still zero, all children are null
  if (max == 0)
  {
    fprintf(ofp, "unknown word\n");
    fprintf(stdout, "unknown word\n");
    return;
  }

  // printing highest frequency nodes
  for (i = 0; i < ALPHA_SIZE; i++)
  {
    if (wizard->children[i] == NULL)
    {
      continue;
    }

    else if (wizard->children[i]->frequency == max)
    {
      fprintf(ofp, "%c", i + 'a');
      fprintf(stdout, "%c", i + 'a');
    }
  }

  // formatting
  fprintf(ofp, "\n");
  fprintf(stdout, "\n");
}

// Takes in a corpus.txt input file.
// Handles all commands from the input file, and
// creates an output file to print to.
TrieNode *commands(char *filename)
{
	TrieNode *root = NULL;
	char buffer[MAX_WORD_LENGTH + 1];
  FILE *ifp, *ofp;
  int i, j; // command, frequency

	if ((ifp = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "Failed to open \"%s\" in commands().\n", filename);
		return NULL;
	}

  if ((ofp = fopen("out.txt", "w")) == NULL)
  {
    fprintf(stderr, "Failed to create \"out.txt\" in commands().\n");
    return NULL;
  }

  fscanf(ifp, "%*s");

	while (fscanf(ifp, "%d %s", &i, buffer) != EOF)
  {
		if (i == 1)
    {
      fscanf(ifp, "%d", &j);
      root = insert(root, j, buffer);
    }

    else if (i == 2)
    {
      magical_prediction(root, buffer, ofp);
    }
  }

	fclose(ifp);
  fclose(ofp);
	return root;
}

// Burns the tree to the ground.
TrieNode *forest_fire(TrieNode *root)
{
  int i;

  if (root == NULL)
    return NULL;

  for (i = 0; i < ALPHA_SIZE; i++)
    forest_fire(root->children[i]);

  free(root);
  return NULL;
}

int main(void)
{
  TrieNode *root = commands("in.txt");
  printTrie(root);
  root = forest_fire(root);
  return 0;
}
