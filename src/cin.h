/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LIBCIN_CIN_H
#define LIBCIN_CIN_H

struct CinData {
  char* filename;
  char* engine;
  char* ename;
  char* cname;
  char* selkey;
  struct Trie* keymap;
  struct Trie* charmap;
};

struct CinData* libcin_open_cin(char* cin_filename);
void libcin_load_data(struct CinData* cin_data);
void libcin_unload_data(struct CinData* cin_data);
void libcin_free_cin(struct CinData* cin_data);

#endif /* LIBCIN_CIN_H */
