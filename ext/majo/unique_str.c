// This file is copied from https://github.com/ruby/ruby and modified for this project.
//
// ----
//
// Copyright (C) 1993-2013 Yukihiro Matsumoto.
// Copyright (C) 2024 Masataka Kuwabara.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

#include "majo.h"

const char *
majo_make_unique_str(st_table *tbl, const char *str, long len)
{
  if (!str) {
    return NULL;
  }
  else {
    st_data_t n;
    char *result;

    if (st_lookup(tbl, (st_data_t)str, &n)) {
      st_insert(tbl, (st_data_t)str, n+1);
      st_get_key(tbl, (st_data_t)str, &n);
      result = (char *)n;
    }
    else {
      result = (char *)malloc(len+1);
      strncpy(result, str, len);
      result[len] = 0;
      st_add_direct(tbl, (st_data_t)result, 1);
    }
    return result;
  }
}

void
majo_delete_unique_str(st_table *tbl, const char *str)
{
  if (str) {
    st_data_t n;

    st_lookup(tbl, (st_data_t)str, &n);
    if (n == 1) {
      n = (st_data_t)str;
      st_delete(tbl, &n, 0);
      free((char *)n);
    }
    else {
      st_insert(tbl, (st_data_t)str, n-1);
    }
  }
}
