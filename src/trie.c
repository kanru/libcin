/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

struct List*
list_alloc(listnode_free_method* dealloc)
{
  struct List* list = calloc(1, sizeof(struct List));
  list->dealloc = dealloc;
  return list;
}

void
list_free(struct List* list)
{
  if (list)
    {
      listnode_free(list->head, list->dealloc);
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
listnode_alloc(void* value)
{
  struct ListNode* node = calloc(1, sizeof(struct ListNode));
  node->value = value;
  return node;
}

void
listnode_free(struct ListNode* head, listnode_free_method* dealloc)
{
  struct ListNode* node;
  struct ListNode* next_node;

  for (node = head; node;)
    {
      next_node = node->next;
      (*dealloc)(node->value);
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

static void
noop(void* value)
{}

struct List*
trie_search(struct Trie* trie, char* keys)
{
  unsigned short key_idx = 0;
  unsigned short key_len = strlen(keys);
  struct TrieNode* node = trie->root;
  struct List* result = list_alloc(noop);

  trie_search1(node, keys, key_idx, key_len, result);
  return result;
}

void
trie_search1(struct TrieNode* node,
             char* keys, unsigned short idx, unsigned short max,
             struct List* result)
{
  if (!node)
    return;

  if (node->key == tolower(keys[idx]))
    {
      idx++;
      if (idx > max)
        return;
      else if (idx == max)
        {
          struct ListNode* value = listnode_alloc(node->values);
          list_append(result, value);
        }
      else
        trie_search1(node->next, keys, idx, max, result);
    }
  else
    trie_search1(node->sibling,  keys, idx, max, result);
}

void
trie_search_free(struct List* result)
{
  list_free(result);
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
    node->values = list_alloc(free);
  struct ListNode* new_value = listnode_alloc(strdup(value));
  list_append(node->values, new_value);

  return;
}
