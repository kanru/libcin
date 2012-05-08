/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

struct List*
list_alloc()
{
  return calloc(1, sizeof(struct List));
}

void
list_free(struct List* list)
{
  if (list)
    {
      listnode_free(list->head);
      free(list);
    }
}

void
list_append(struct List* list, struct ListNode* node)
{
  if (list->head)
    {
      list->tail->next = node;
      list->tail = node;
    }
  else
    {
      list->head = node;
      list->tail = node;
    }
}

struct ListNode*
listnode_alloc(char* value)
{
  struct ListNode* node = calloc(1, sizeof(struct ListNode));
  node->value = strdup(value);
  return node;
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

  list_free(node->values);
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

struct List*
trie_search(struct Trie* trie, char* keys)
{
  unsigned short key_idx = 0;
  unsigned short key_len = strlen(keys);
  struct TrieNode* node = trie->root;

  for (;;)
    {
      if (!node)
        break;

      if (node->key == tolower(keys[key_idx]))
        {
          key_idx++;
          if (key_idx >= key_len)
            break;
          else
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
      node->key = tolower(keys[0]);
    }

  for (;;)
    {
      struct TrieNode** next_node;
      if (node->key == tolower(keys[key_idx]))
        {
          key_idx++;
          if (key_idx >= key_len)
            break;
          else
            next_node = &node->next;
        }
      else
        next_node = &node->sibling;

      if (!*next_node)
        {
          *next_node = trienode_alloc();
          (*next_node)->key = tolower(keys[key_idx]);
        }

      node = *next_node;
    }

  if (!node->values)
    node->values = list_alloc();
  struct ListNode* new_value = listnode_alloc(value);
  list_append(node->values, new_value);

  return;
}
