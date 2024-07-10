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


static VALUE
allocation_info_alloc(VALUE klass) {
  majo_allocation_info *info;
  VALUE obj = TypedData_Make_Struct(klass, majo_allocation_info, &allocation_info_type, info);
  return obj;
}

VALUE
majo_new_allocation_info(majo_allocation_info *info) {
  majo_allocation_info *new_info;
  VALUE obj = TypedData_Make_Struct(rb_cMajo_AllocationInfo, majo_allocation_info, &allocation_info_type, new_info);
  *new_info = *info;
  return obj;
}

majo_allocation_info *
majo_check_allocation_info(VALUE obj) {
  return rb_check_typeddata(obj, &allocation_info_type);
}

static VALUE
allocation_info_path(VALUE self) {
  majo_allocation_info *info = majo_check_allocation_info(self);
  if (info->path) {
    return rb_str_new2(info->path);
  } else {
    return Qnil;
  }
}

static VALUE
allocation_info_class_path(VALUE self) {
  majo_allocation_info *info = majo_check_allocation_info(self);
  if (info->class_path) {
    return rb_str_new2(info->class_path);
  } else {
    return Qnil;
  }
}

static VALUE
allocation_info_object_class_path(VALUE self) {
  majo_allocation_info *info = majo_check_allocation_info(self);
  if (info->object_class_path) {
    return rb_str_new2(info->object_class_path);
  } else {
    return Qnil;
  }
}

static VALUE
allocation_info_line(VALUE self) {
  majo_allocation_info *info = majo_check_allocation_info(self);
  return LONG2NUM(info->line);
}


static VALUE
allocation_info_method_id(VALUE self) {
  majo_allocation_info *info = majo_check_allocation_info(self);
  return info->mid;
}

static VALUE
allocation_info_generation(VALUE self) {
  majo_allocation_info *info = majo_check_allocation_info(self);
  return SIZET2NUM(info->generation);
}

static VALUE
allocation_info_memsize(VALUE self) {
  majo_allocation_info *info = majo_check_allocation_info(self);
  return SIZET2NUM(info->memsize);
}

void
majo_init_allocation_info() {
  rb_cMajo_AllocationInfo = rb_define_class_under(rb_mMajo, "AllocationInfo", rb_cObject);
  rb_define_alloc_func(rb_cMajo_AllocationInfo, allocation_info_alloc);

  rb_define_method(rb_cMajo_AllocationInfo, "path", allocation_info_path, 0);
  rb_define_method(rb_cMajo_AllocationInfo, "class_path", allocation_info_class_path, 0);
  rb_define_method(rb_cMajo_AllocationInfo, "method_id", allocation_info_method_id, 0);
  rb_define_method(rb_cMajo_AllocationInfo, "line", allocation_info_line, 0);
  rb_define_method(rb_cMajo_AllocationInfo, "object_class_path", allocation_info_object_class_path, 0);
  rb_define_method(rb_cMajo_AllocationInfo, "generation", allocation_info_generation, 0);
  rb_define_method(rb_cMajo_AllocationInfo, "memsize", allocation_info_memsize, 0);
}
