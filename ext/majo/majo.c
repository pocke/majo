#include "majo.h"

VALUE rb_mMajo;
VALUE rb_cMajo_Result;
VALUE rb_cMajo_AllocationInfo;

ID running_tracer_stack;

static bool
internal_object_p(VALUE obj)
{
  switch (TYPE(obj)) {
    case T_OBJECT:
    case T_CLASS:
    case T_MODULE:
    case T_FLOAT:
    case T_STRING:
    case T_REGEXP:
    case T_ARRAY:
    case T_HASH:
    case T_STRUCT:
    case T_BIGNUM:
    case T_FILE:
    case T_DATA:
    case T_MATCH:
    case T_COMPLEX:
    case T_RATIONAL:
    case T_NIL:
    case T_TRUE:
    case T_FALSE:
    case T_SYMBOL:
    case T_FIXNUM:
      return false;
    default:
      return true;
  }
}

static const char*
to_class_path(VALUE klass, st_table *str_table)
{
  if (FL_TEST(klass, FL_SINGLETON)) {
    return to_class_path(majo_attached_object(klass), str_table);
  }

  VALUE class_path = (RTEST(klass) && !OBJ_FROZEN(klass)) ? rb_class_path_cached(klass) : Qnil;
  const char *class_path_cstr = RTEST(class_path) ? majo_make_unique_str(str_table, RSTRING_PTR(class_path), RSTRING_LEN(class_path)) : 0;
  return class_path_cstr;
}

static void
newobj_i(VALUE tpval, void *data)
{
  VALUE res = (VALUE)data;
  majo_result *arg = majo_check_result(res);

  rb_trace_arg_t *tparg = rb_tracearg_from_tracepoint(tpval);
  VALUE obj = rb_tracearg_object(tparg);

  if (internal_object_p(obj)) {
    return;
  }

  VALUE path = rb_tracearg_path(tparg);
  VALUE line = rb_tracearg_lineno(tparg);
  VALUE mid = rb_tracearg_method_id(tparg);
  VALUE klass = rb_tracearg_defined_class(tparg);

  majo_allocation_info *info = (majo_allocation_info *)malloc(sizeof(majo_allocation_info));

  const char *path_cstr = RTEST(path) ? majo_make_unique_str(arg->str_table, RSTRING_PTR(path), RSTRING_LEN(path)) : 0;
  const char *class_path_cstr = to_class_path(klass, arg->str_table);

  VALUE obj_class = rb_obj_class(obj);
  const char *obj_class_path_cstr = to_class_path(obj_class, arg->str_table);

  info->result = res;
  info->path = path_cstr;
  info->line = NUM2INT(line);
  info->mid = mid;
  info->singleton_p = FL_TEST(klass, FL_SINGLETON);
  info->object_class_path = obj_class_path_cstr;

  info->class_path = class_path_cstr;
  info->alloc_generation = rb_gc_count();
  info->free_generation = 0;
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
    size_t gc_count = rb_gc_count();
    int lifetime = (int)(gc_count - info->alloc_generation - 1);
    if (
      (NIL_P(arg->upper_lifetime) || lifetime <= NUM2INT(arg->upper_lifetime)) &&
      (NIL_P(arg->lower_lifetime) || NUM2INT(arg->lower_lifetime) <= lifetime)
    ) {
      info->memsize = rb_obj_memsize_of((VALUE)obj);
      info->free_generation = gc_count;

      VALUE obj = rb_tracearg_object(tparg);
      if (!internal_object_p(obj)) {
        majo_result_append_info(arg, *info);
      }
    }
    free(info);
  }
}

static VALUE
start(VALUE self, VALUE upper_lifetime, VALUE lower_lifetime) {
  VALUE res = majo_new_result();
  majo_result *arg = majo_check_result(res);
  arg->upper_lifetime = upper_lifetime;
  arg->lower_lifetime = lower_lifetime;

  VALUE stack = rb_ivar_get(rb_mMajo, running_tracer_stack);
  rb_ary_push(stack, res);

  arg->newobj_trace = rb_tracepoint_new(0, RUBY_INTERNAL_EVENT_NEWOBJ, newobj_i, (void *)res);
  arg->freeobj_trace = rb_tracepoint_new(0, RUBY_INTERNAL_EVENT_FREEOBJ, freeobj_i, arg);

  rb_tracepoint_enable(arg->newobj_trace);
  rb_tracepoint_enable(arg->freeobj_trace);

  return res;
}

static VALUE
stop(VALUE self)
{
  VALUE stack = rb_ivar_get(rb_mMajo, running_tracer_stack);
  VALUE res = rb_ary_pop(stack);
  majo_result *arg = majo_check_result(res);

  rb_tracepoint_disable(arg->newobj_trace);
  rb_tracepoint_disable(arg->freeobj_trace);

  return res;
}

void
Init_majo(void)
{
  rb_mMajo = rb_define_module("Majo");

  rb_define_module_function(rb_mMajo, "__start", start, 2);
  rb_define_module_function(rb_mMajo, "__stop", stop, 0);

  running_tracer_stack = rb_intern("running_tracer_stack");
  rb_ivar_set(rb_mMajo, running_tracer_stack, rb_ary_new());

  majo_init_result();
  majo_init_allocation_info();
  majo_init_attached_object();
}
