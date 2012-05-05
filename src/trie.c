/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

struct ListNode*
listnode_alloc(char* value)
{
  struct ListNode* node = calloc(1, sizeof(struct ListNode));
  node->value = strdup(value);
  return node;
}

void
listnode_append(struct ListNode* head, struct ListNode* new_node)
{
  struct ListNode* node;
  for (node = head; node; node = node->next)
    {
      if (!node->next)
        break;
    }
  node->next = new_node;
}

void
listnode_free(struct ListNode* head)
{
  struct ListNode* node;
  struct ListNode* next_node;

  for (node = head; node;)
    {
      next_node = node->next;
      free(node->value);
      free(node);
      node = next_node;
    }
}

struct TrieNode*
trienode_alloc()
{
  return calloc(1, sizeof(struct TrieNode));
}

void
trienode_free(struct TrieNode* node)
{
  if (node->sibling)
    trienode_free(node->sibling);

  if (node->next)
    trienode_free(node->next);

  listnode_free(node->values);
  free(node);
}

struct Trie*
trie_alloc()
{
  return calloc(1, sizeof(struct Trie));
}

void
trie_free(struct Trie* trie)
{
  if (trie->root != NULL)
    trienode_free(trie->root);
  free(trie);
}

struct ListNode*
trie_search(struct Trie* trie, char* keys)
{
  unsigned short key_idx = 0;
  unsigned short key_len = strlen(keys);
  struct TrieNode* node = trie->root;

  for (;;)
    {
      if (!node || key_idx >= key_len)
        break;

      if (node->key == keys[key_idx])
        {
          key_idx++;
          node = node->next;
        }
      else
        node = node->sibling;
    }

  if (node && key_idx == key_len)
    return node->values;
  else
    return NULL;
}

void
trie_insert(struct Trie* trie, char* keys, char* value)
{
  unsigned short key_idx = 0;
  unsigned short key_len = strlen(keys);
  struct TrieNode* node = trie->root;

  if (!node)
    {
      node = trie->root = trienode_alloc();
      node->key = keys[0];
    }

  for (;;)
    {
      if (key_idx >= key_len)
        break;

      struct TrieNode** next_node;
      if (node->key == keys[key_idx])
        {
          key_idx++;
          next_node = &node->next;
        }
      else
        next_node = &node->sibling;

      if (!*next_node)
        {
          *next_node = trienode_alloc();
          (*next_node)->key = keys[key_idx];
        }

      node = *next_node;
    }

  struct ListNode* new_value = listnode_alloc(value);
  if (node->values)
    listnode_append(node->values, new_value);
  else
    node->values = new_value;

  return;
}

#if 0
static void
print_list(struct ListNode* values)
{
  struct ListNode* node;
  for (node = values; node; node = node->next)
    {
      printf("%s\n", node->value);
    }
}

int
main(int argc, char* argv[])
{
  struct Trie* trie = trie_alloc();

  char buf[255];
  FILE* input = fopen(argv[1], "r");
  while (fgets(&buf[0], sizeof(buf), input) != NULL)
    {
      char* delim = strchr(buf, ' ');
      char* eol = strchr(buf, '\n');
      char* key = &buf[0];
      char* value = delim+1;
      *delim = 0;
      *eol = 0;

      trie_insert(trie, key, value);
    }
  fclose(input);

  printf("> ");
  while (scanf("%s", &buf[0]) != EOF)
    {
      print_list(trie_search(trie, buf));
      printf("> ");
    }

  trie_free(trie);
  return 0;
}
#endif
