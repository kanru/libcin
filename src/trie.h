/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LIBCIN_TRIE_H
#define LIBCIN_TRIE_H

struct ListNode {
  struct ListNode* next;
  void* value;
};

typedef void (listnode_free_method)(void* value);
struct List {
  struct ListNode* head;
  struct ListNode* tail;
  listnode_free_method* dealloc;
};

struct TrieNode {
  struct TrieNode* next;
  struct TrieNode* sibling;
  struct List* values;
  char key;
};

struct Trie {
  struct TrieNode* root;
};

struct Trie* trie_alloc();
void trie_free(struct Trie* trie);
void trie_insert(struct Trie* trie, char* keys, char* value);
struct List* trie_search(struct Trie* trie, char* keys);
void trie_search1(struct TrieNode* node,
                  char* keys, unsigned short idx, unsigned short max,
                  struct List* result);
void trie_search_star(struct TrieNode* node,
                      char* keys, unsigned short idx, unsigned short max,
                      struct List* result);
void trie_search_free(struct List* result);
struct TrieNode* trie_root(struct Trie* trie);
void trie_set_root(struct Trie* trie, struct TrieNode* root);

struct TrieNode* trienode_alloc(char key);
void trienode_free(struct TrieNode* trie_node);
struct TrieNode* trienode_next(struct TrieNode* trie_node);
struct TrieNode* trienode_sibling(struct TrieNode* trie_node);
char trienode_key(struct TrieNode* trie_node);
struct List* trienode_values(struct TrieNode* trie_node);
void trienode_set_next(struct TrieNode* trie_node, struct TrieNode* next);
void trienode_set_sibling(struct TrieNode* trie_node, struct TrieNode* sibling);
void trienode_set_values(struct TrieNode* trie_node, struct List* values);

struct List* list_alloc(listnode_free_method* dealloc);
void list_append(struct List* list, struct ListNode* node);
void list_free(struct List* list);

struct ListNode* listnode_alloc(void* value);
void listnode_free(struct ListNode* head, listnode_free_method* dealloc);

#endif /* LIBCIN_TRIE_H */
