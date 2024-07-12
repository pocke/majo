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

static void
newobj_i(VALUE tpval, void *data)
{
  majo_result *arg = (majo_result *)data;

  rb_trace_arg_t *tparg = rb_tracearg_from_tracepoint(tpval);
  VALUE obj = rb_tracearg_object(tparg);
  VALUE path = rb_tracearg_path(tparg);
  VALUE line = rb_tracearg_lineno(tparg);
  VALUE mid = rb_tracearg_method_id(tparg);
  VALUE klass = rb_tracearg_defined_class(tparg);

  // TODO: when the st already has an entry for the value
  majo_allocation_info *info = (majo_allocation_info *)malloc(sizeof(majo_allocation_info));

  const char *path_cstr = RTEST(path) ? majo_make_unique_str(arg->str_table, RSTRING_PTR(path), RSTRING_LEN(path)) : 0;

  VALUE class_path = (RTEST(klass) && !OBJ_FROZEN(klass)) ? rb_class_path_cached(klass) : Qnil;
  const char *class_path_cstr = RTEST(class_path) ? majo_make_unique_str(arg->str_table, RSTRING_PTR(class_path), RSTRING_LEN(class_path)) : 0;


  info->path = path_cstr;
  info->line = NUM2INT(line);
  info->mid = mid;

  info->class_path = class_path_cstr;
  info->generation = rb_gc_count();
  st_insert(arg->object_table, (st_data_t)obj, (st_data_t)info);
}

static void
freeobj_i(VALUE tpval, void *data)
{
  // Since freeobj_i is called during GC, it must not trigger another GC.
  VALUE gc_disabled = rb_gc_disable_no_rest();

  majo_result *arg = (majo_result *)data;
  rb_trace_arg_t *tparg = rb_tracearg_from_tracepoint(tpval);
  st_data_t obj = (st_data_t)rb_tracearg_object(tparg);
  st_data_t v;

  // TODO refcount of the strings
  if (st_delete(arg->object_table, &obj, &v)) {
    majo_allocation_info *info = (majo_allocation_info *)v;
    // Reject it for majo
    if (info->generation < rb_gc_count()-1) {
      info->memsize = rb_obj_memsize_of((VALUE)obj);

      VALUE obj = rb_tracearg_object(tparg);
      if (!internal_object_p(obj)) {
        VALUE obj_class = rb_obj_class(obj);
        VALUE obj_class_path = (RTEST(obj_class) && !OBJ_FROZEN(obj_class)) ? rb_class_path_cached(obj_class) : Qnil;
        const char *obj_class_path_cstr = RTEST(obj_class_path) ? majo_make_unique_str(arg->str_table, RSTRING_PTR(obj_class_path), RSTRING_LEN(obj_class_path)) : 0;
        info->object_class_path = obj_class_path_cstr;

        majo_result_append_info(arg, *info);
      }
    }
    free(info);
  }

  if (gc_disabled == Qfalse) rb_gc_enable();
}

static VALUE
start(VALUE self) {
  VALUE res = majo_new_result();
  majo_result *arg = majo_check_result(res);

  VALUE stack = rb_ivar_get(rb_mMajo, running_tracer_stack);
  rb_ary_push(stack, res);

  arg->newobj_trace = rb_tracepoint_new(0, RUBY_INTERNAL_EVENT_NEWOBJ, newobj_i, arg);
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

  rb_define_module_function(rb_mMajo, "__start", start, 0);
  rb_define_module_function(rb_mMajo, "__stop", stop, 0);

  running_tracer_stack = rb_intern("running_tracer_stack");
  rb_ivar_set(rb_mMajo, running_tracer_stack, rb_ary_new());

  majo_init_result();
  majo_init_allocation_info();
}
