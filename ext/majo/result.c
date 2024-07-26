#include "majo.h"

static void majo_result_mark(void *ptr)
{
  majo_result *arg = (majo_result*)ptr;
  rb_gc_mark(arg->newobj_trace);
  rb_gc_mark(arg->freeobj_trace);
  rb_gc_mark(arg->retained);
}

static int
free_st_key(st_data_t key, st_data_t value, st_data_t data)
{
    free((void *)key);
    return ST_CONTINUE;
}

static int
free_st_value(st_data_t key, st_data_t value, st_data_t data)
{
    free((void *)value);
    return ST_CONTINUE;
}

static void majo_result_free(majo_result *arg) {
  st_foreach(arg->object_table, free_st_value, 0);
  st_free_table(arg->object_table);

  st_foreach(arg->str_table, free_st_key, 0);
  st_free_table(arg->str_table);

  rb_darray_free(arg->olds);

  free(arg);
}

static size_t majo_result_memsize(const void *ptr) {
  majo_result *res = (majo_result*)ptr;
  return sizeof(majo_result) + st_memsize(res->object_table) + st_memsize(res->str_table) + rb_darray_capa(res->olds) * sizeof(majo_allocation_info);
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
  arg->str_table = st_init_strtable();
  arg->olds = NULL;
  arg->retained = rb_ary_new();

  return obj;
}

VALUE
majo_new_result() {
  return rb_class_new_instance(0, NULL, rb_cMajo_Result);
}

majo_result *
majo_check_result(VALUE obj) {
  return rb_check_typeddata(obj, &result_type);
}

void
majo_result_append_info(majo_result *res, majo_allocation_info info) {
  rb_darray_append(&res->olds, info);
}

static VALUE
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

static VALUE
majo_result_retained(VALUE self) {
  majo_result *res = majo_check_result(self);
  return res->retained;
}

static VALUE
majo_result_store_retained_object(VALUE self, VALUE obj) {
  majo_result *res = majo_check_result(self);

  st_data_t value;
  if (st_lookup(res->object_table, (st_data_t)obj, &value)) {
    majo_allocation_info *info = (majo_allocation_info *)value;
    info->memsize = rb_obj_memsize_of(obj);

    VALUE info_r = majo_new_allocation_info(info);
    rb_ary_push(res->retained, info_r);
  }

  return Qnil;
}

void
majo_init_result() {
  rb_cMajo_Result = rb_define_class_under(rb_mMajo, "Result", rb_cObject);
  rb_define_alloc_func(rb_cMajo_Result, result_alloc);

  rb_define_method(rb_cMajo_Result, "allocations", majo_result_allocations, 0);
  rb_define_method(rb_cMajo_Result, "retained", majo_result_retained, 0);
  rb_define_method(rb_cMajo_Result, "store_retained_object", majo_result_store_retained_object, 1);
}
