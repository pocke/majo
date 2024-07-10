#ifndef MAJO_UNIQUE_STR_H
#define MAJO_UNIQUE_STR_H

const char *
majo_make_unique_str(st_table *tbl, const char *str, long len);

void
majo_delete_unique_str(st_table *tbl, const char *str);

#endif
