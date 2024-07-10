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
