#ifndef MAJO_RESULT_H
#define MAJO_RESULT_H

typedef struct {
  st_table *object_table; /* obj (VALUE) -> allocation_info */
  st_table *str_table;    /* cstr -> refcount */
  rb_darray(majo_allocation_info) olds;
  VALUE newobj_trace;
  VALUE freeobj_trace;
} majo_result;

VALUE
majo_new_result();

majo_result *
majo_check_result(VALUE obj);

void
majo_result_append_info(majo_result *res, majo_allocation_info info);

void
majo_init_result();

#endif
