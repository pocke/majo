#include "majo.h"

static void majo_allocation_info_mark(void *ptr)
{
  // TODO
  majo_allocation_info *info = (majo_allocation_info*)ptr;
}

static void majo_allocation_info_free(majo_allocation_info *info) {
  // TODO
  ruby_xfree(info);
}

static size_t majo_allocation_info_memsize(const void *ptr) {
  return sizeof(majo_allocation_info);
}

static rb_data_type_t allocation_info_type = {
  "Majo::AllocationInfo",
  {majo_allocation_info_mark, (RUBY_DATA_FUNC)majo_allocation_info_free, majo_allocation_info_memsize},
  0, 0, RUBY_TYPED_FREE_IMMEDIATELY
};


static VALUE allocation_info_alloc(VALUE klass) {
  majo_allocation_info *info;
  VALUE obj = TypedData_Make_Struct(klass, majo_allocation_info, &allocation_info_type, info);
  return obj;
}

VALUE
majo_new_allocation_info(majo_allocation_info *info) {
  VALUE obj = TypedData_Make_Struct(rb_cMajo_AllocationInfo, majo_allocation_info, &allocation_info_type, info);
  return obj;
}

void
majo_init_allocation_info() {
  rb_cMajo_AllocationInfo = rb_define_class_under(rb_mMajo, "AllocationInfo", rb_cObject);
  rb_define_alloc_func(rb_cMajo_AllocationInfo, allocation_info_alloc);
}
