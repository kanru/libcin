/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LIBCIN_TRIE_H
#define LIBCIN_TRIE_H

struct ListNode {
  struct ListNode* next;
  char* value;
};

struct TrieNode {
  struct TrieNode* next;
  struct TrieNode* sibling;
  struct ListNode* values;
  char key;
};

struct Trie {
  struct TrieNode* root;
};

struct Trie* trie_alloc();
void trie_free(struct Trie* trie);
void trie_insert(struct Trie* trie, char* keys, char* value);
struct ListNode* trie_search(struct Trie* trie, char* keys);

struct TrieNode* trienode_alloc();
void trienode_free(struct TrieNode* trie_node);

struct ListNode* listnode_alloc();
void listnode_append(struct ListNode* head, struct ListNode* new_node);
void listnode_free(struct ListNode* head);

#endif /* LIBCIN_TRIE_H */
