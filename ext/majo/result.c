#include "majo.h"

static void majo_result_mark(void *ptr)
{
  majo_result *arg = (majo_result*)ptr;
  rb_gc_mark(arg->newobj_trace);
  rb_gc_mark(arg->freeobj_trace);
}

static void majo_result_free(majo_result *arg) {
  // TODO
  ruby_xfree(arg);
}

static size_t majo_result_memsize(const void *ptr) {
  // TODO
  return sizeof(majo_result);
}

static rb_data_type_t result_type = {
  "Majo::Result",
  {majo_result_mark, (RUBY_DATA_FUNC)majo_result_free, majo_result_memsize},
  0, 0, RUBY_TYPED_FREE_IMMEDIATELY
};

static VALUE result_alloc(VALUE klass) {
  majo_result *arg;
  VALUE obj = TypedData_Make_Struct(klass, majo_result, &result_type, arg);
  arg->object_table = st_init_numtable();
  arg->olds = NULL;

  return obj;
}

VALUE
majo_new_result() {
  return rb_class_new_instance(0, NULL, rb_cMajo_Result);
}

majo_result *
majo_check_result(VALUE obj) {
  majo_result *res;
  TypedData_Get_Struct(obj, majo_result, &result_type, res);
  return res;
}

void
majo_result_append_info(majo_result *res, majo_allocation_info info) {
  rb_darray_append(&res->olds, info);
}

VALUE
majo_result_allocations(VALUE self) {
  majo_result *res = majo_check_result(self);
  VALUE ary = rb_ary_new_capa(rb_darray_size(res->olds));

  majo_allocation_info *info_ptr;
  rb_darray_foreach(res->olds, i, info_ptr) {
    VALUE v = majo_new_allocation_info(info_ptr);
    rb_ary_push(ary, v);
  }

  return ary;
}

void
majo_init_result() {
  rb_cMajo_Result = rb_define_class_under(rb_mMajo, "Result", rb_cObject);
  rb_define_alloc_func(rb_cMajo_Result, result_alloc);

  rb_define_method(rb_cMajo_Result, "allocations", majo_result_allocations, 0);
}