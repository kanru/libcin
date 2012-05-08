/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cin.h"
#include "trie.h"

static void
print_list(struct List* values)
{
  if (!values)
    return;

  struct ListNode* node;
  for (node = values->head; node; node = node->next)
    {
      printf("%s\n", node->value);
    }
}

int
main(int argc, char* argv[])
{
  if (argc < 2)
    {
      printf("Usage: t <test.cin>\n");
      return 1;
    }

  printf("Open %s...\n", argv[1]);

  struct CinData* cin_data = libcin_open_cin(argv[1]);

  printf("Find cin [%s, %s]\n", cin_data->ename, cin_data->cname);
  printf("Loading...\n");

  libcin_load_data(cin_data);

  char buf[255];
  printf("> ");
  while (scanf("%s", &buf[0]) != EOF)
    {
      print_list(trie_search(cin_data->charmap, buf));
      printf("> ");
    }

  libcin_free_cin(cin_data);
  return 0;
}
