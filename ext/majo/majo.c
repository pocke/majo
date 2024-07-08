#include "majo.h"

VALUE rb_mMajo;
VALUE rb_cMajo_Result;
VALUE rb_cMajo_AllocationInfo;

static void
newobj_i(VALUE tpval, void *data)
{
  majo_result *arg = (majo_result *)data;

  rb_trace_arg_t *tparg = rb_tracearg_from_tracepoint(tpval);
  VALUE obj = rb_tracearg_object(tparg);
  // VALUE path = rb_tracearg_path(tparg);
  // VALUE line = rb_tracearg_lineno(tparg);
  // VALUE mid = rb_tracearg_method_id(tparg);
  // VALUE klass = rb_tracearg_defined_class(tparg);

  // TODO: when the st already has an entry for the value
  majo_allocation_info *info = (majo_allocation_info *)malloc(sizeof(majo_allocation_info));
  info->klass = RBASIC_CLASS(obj);

  // info->path = path_cstr;
  // info->line = NUM2INT(line);
  // info->mid = mid;
  // info->class_path = class_path_cstr;
  info->generation = rb_gc_count();
  st_insert(arg->object_table, (st_data_t)obj, (st_data_t)info);
}

static void
freeobj_i(VALUE tpval, void *data)
{
  majo_result *arg = (majo_result *)data;
  rb_trace_arg_t *tparg = rb_tracearg_from_tracepoint(tpval);
  st_data_t obj = (st_data_t)rb_tracearg_object(tparg);
  st_data_t v;

  if (st_delete(arg->object_table, &obj, &v)) {
    majo_allocation_info *info = (majo_allocation_info *)v;
    majo_result_append_info(arg, *info);
    ruby_xfree(info);
  }
}

static VALUE
start(VALUE self) {
  VALUE res = majo_new_result();
  majo_result *arg = majo_check_result(res);

  arg->newobj_trace = rb_tracepoint_new(0, RUBY_INTERNAL_EVENT_NEWOBJ, newobj_i, arg);
  arg->freeobj_trace = rb_tracepoint_new(0, RUBY_INTERNAL_EVENT_FREEOBJ, freeobj_i, arg);

  rb_tracepoint_enable(arg->newobj_trace);
  rb_tracepoint_enable(arg->freeobj_trace);

  return res;
}

void
Init_majo(void)
{
  rb_mMajo = rb_define_module("Majo");

  rb_define_module_function(rb_mMajo, "start", start, 0);
  majo_init_result();
  majo_init_allocation_info();
}
