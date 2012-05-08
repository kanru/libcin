/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cin.h"
#include "trie.h"

static char*
key_value_split(char* buf, size_t length)
{
  char* end = buf+length;
  char* c;
  for (c = buf; c != end && !isspace(*c); c++);
  *c = 0;

  if (c == end)
    return NULL;

  char* v;
  for (c++; c != end && isspace(*c); c++);
  if (c != end)
    v = c;

  for (c++; c != end && !isspace(*c); c++);
  *c = 0;

  *end = 0;

  return v;
}

struct CinData*
libcin_open_cin(char* cin_filename)
{
  struct CinData* cin = calloc(1, sizeof(struct CinData));
  char buf[255] = {0};
  FILE* input = fopen(cin_filename, "r");
  while (fgets(&buf[0], sizeof(buf), input) != NULL)
    {
      char* value = key_value_split(buf, sizeof(buf));
      char* key = &buf[0];

      if (!strcmp(key, "%gen_inp"))
        cin->engine = strdup("gen_inp");

      if (!strcmp(key, "%ename"))
        cin->ename = strdup(value);

      if (!strcmp(key, "%cname"))
        cin->cname = strdup(value);

      if (!strcmp(key, "%selkey"))
        cin->selkey = strdup(value);

      if (!strcmp(key, "%keyname") && !strcmp(value, "begin"))
        break;
    }
  fclose(input);

  cin->filename = strdup(cin_filename);
  cin->keymap = NULL;
  cin->charmap = NULL;

  if (!cin->engine)
    cin->engine = strdup("gen_inp");

  if (!cin->ename)
    cin->ename = strdup("Unknown");

  if (!cin->cname)
    cin->cname = strdup("Unknown");

  if (!cin->selkey)
    cin->selkey = strdup("123456789");

  return cin;
}

void
libcin_load_data(struct CinData* cin_data)
{
  if (cin_data->charmap != NULL)
    return;

  cin_data->keymap = trie_alloc();
  cin_data->charmap = trie_alloc();

  unsigned char phase = 0;
  char buf[255] = {0};
  FILE* input = fopen(cin_data->filename, "r");
  while (fgets(&buf[0], sizeof(buf), input) != NULL)
    {
      char* value = key_value_split(buf, sizeof(buf));
      char* key = &buf[0];

      if (!strcmp(key, "%keyname") && !strcmp(value, "begin"))
        phase = 1;

      if (!strcmp(key, "%keyname") && !strcmp(value, "end"))
        phase = 0;

      if (!strcmp(key, "%chardef") && !strcmp(value, "begin"))
        phase = 2;

      if (!strcmp(key, "%chardef") && !strcmp(value, "end"))
        phase = 0;

      if (phase == 1)
        trie_insert(cin_data->keymap, key, value);

      if (phase == 2)
        trie_insert(cin_data->charmap, key, value);
    }
  fclose(input);
}

void
libcin_unload_data(struct CinData* cin_data)
{
  if (cin_data->keymap)
    trie_free(cin_data->keymap);

  if (cin_data->charmap)
    trie_free(cin_data->charmap);

  cin_data->keymap = NULL;
  cin_data->charmap = NULL;
}

void
libcin_free_cin(struct CinData* cin_data)
{
  libcin_unload_data(cin_data);
  free(cin_data->filename);
  free(cin_data->engine);
  free(cin_data->ename);
  free(cin_data->cname);
  free(cin_data->selkey);
  free(cin_data);
}
