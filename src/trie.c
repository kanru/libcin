/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <assert.h>
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
      if (dealloc)
        (*dealloc)(node->value);
      free(node);
      node = next_node;
    }
}

struct TrieNode*
trienode_alloc(char key)
{
  struct TrieNode* node = calloc(1, sizeof(struct TrieNode));
  node->key = key;
  return node;
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

struct TrieNode*
trienode_next(struct TrieNode* trie_node)
{
  return trie_node->next;
}

struct TrieNode*
trienode_sibling(struct TrieNode* trie_node)
{
  return trie_node->sibling;
}

char
trienode_key(struct TrieNode* trie_node)
{
  return trie_node->key;
}

struct List*
trienode_values(struct TrieNode* trie_node)
{
  return trie_node->values;
}

void
trienode_set_next(struct TrieNode* trie_node, struct TrieNode* next)
{
  trie_node->next = next;
}

void
trienode_set_sibling(struct TrieNode* trie_node, struct TrieNode* sibling)
{
  trie_node->sibling = sibling;
}

void
trienode_set_values(struct TrieNode* trie_node, struct List* values)
{
  trie_node->values = values;
}

struct TrieNode*
trie_root(struct Trie* trie)
{
  return trie->root;
}

void
trie_set_root(struct Trie* trie, struct TrieNode* node)
{
  trie->root = node;
}

struct Trie*
trie_alloc()
{
  return calloc(1, sizeof(struct Trie));
}

void
trie_free(struct Trie* trie)
{
  if (trie_root(trie) != NULL)
    trienode_free(trie_root(trie));
  free(trie);
}

struct List*
trie_search(struct Trie* trie, char* keys)
{
  unsigned short key_idx = 0;
  unsigned short key_len = strlen(keys);
  struct TrieNode* node = trie_root(trie);
  struct List* result = list_alloc(NULL);

  trie_search1(node, keys, key_idx, key_len, result);
  return result;
}

void
trie_search_star(struct TrieNode* node,
                 char* keys, unsigned short idx, unsigned short max,
                 struct List* result)
{
  struct TrieNode* next;
  int next_idx = idx + 1;

  /* star at the end position */
  /* TODO: unify the logic below */
  if (next_idx == max)
    {
      if (trienode_values(node))
        list_append(result, listnode_alloc(trienode_values(node)));
    }
  else
    {
      /* start at the beginning and middle */
      for (next = node; next; next = trienode_next(next), next_idx++)
        {
          if (trienode_key(next) != keys[next_idx])
            break;
          if (next_idx == max-1)
            {
              if (trienode_values(next))
                list_append(result, listnode_alloc(trienode_values(next)));
              break;
            }
        }
    }

  if (trienode_next(node))
    trie_search_star(trienode_next(node), keys, idx, max, result);
  if (trienode_sibling(node))
    trie_search_star(trienode_sibling(node), keys, idx, max, result);
}

void
trie_search1(struct TrieNode* node,
             char* keys, unsigned short idx, unsigned short max,
             struct List* result)
{
  if (!node)
    return;

  assert(idx < max);

  if (keys[idx] == '*')
    trie_search_star(node, keys, idx, max, result);
  else if (keys[idx] == '?')
    {
      idx++;
      if (idx == max)
        {
          struct TrieNode* sibling;
          for (sibling = node; sibling; sibling = trienode_sibling(sibling))
            {
              struct ListNode* value = listnode_alloc(trienode_values(sibling));
              list_append(result, value);
            }
        }
      else
        {
          struct TrieNode* sibling;
          for (sibling = node; sibling; sibling = trienode_sibling(sibling))
            trie_search1(trienode_next(sibling), keys, idx, max, result);
        }
    }
  else if (trienode_key(node) == tolower(keys[idx]))
    {
      idx++;
      if (idx == max)
        {
          struct ListNode* value = listnode_alloc(trienode_values(node));
          list_append(result, value);
        }
      else
        trie_search1(trienode_next(node), keys, idx, max, result);
    }
  else
    trie_search1(trienode_sibling(node), keys, idx, max, result);
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
  struct TrieNode* node = trie_root(trie);

  if (!node)
    {
      node = trienode_alloc(tolower(keys[0]));
      trie_set_root(trie, node);
    }

  for (;;)
    {
      if (trienode_key(node) == tolower(keys[key_idx]))
        {
          key_idx++;
          if (key_idx >= key_len)
            break;
          else
            {
              if (!trienode_next(node))
                trienode_set_next(node, trienode_alloc(keys[key_idx]));
              node = trienode_next(node);
            }
        }
      else
        {
          if (!trienode_sibling(node))
            trienode_set_sibling(node, trienode_alloc(keys[key_idx]));
          node = trienode_sibling(node);
        }
    }

  if (!trienode_values(node))
    trienode_set_values(node, list_alloc(free));
  struct ListNode* new_value = listnode_alloc(strdup(value));
  list_append(trienode_values(node), new_value);

  return;
}
