#ifndef MAJO_ALLOCATION_INFO_H
#define MAJO_ALLOCATION_INFO_H

typedef struct {
  const char *path;
  const char *class_path;
  const char *object_class_path;
  unsigned long line;
  VALUE mid;

  size_t generation;
} majo_allocation_info;

VALUE
majo_new_allocation_info(majo_allocation_info *info);

void
majo_init_allocation_info();

#endif
