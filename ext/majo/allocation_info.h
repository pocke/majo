#ifndef MAJO_ALLOCATION_INFO_H
#define MAJO_ALLOCATION_INFO_H

typedef struct {
  VALUE klass;

  /* allocation info */
  // const char *path;
  // unsigned long line;
  // const char *class_path;
  // VALUE mid;
  size_t generation;
} majo_allocation_info;

VALUE
majo_new_allocation_info(majo_allocation_info *info);

void
majo_init_allocation_info();

#endif
