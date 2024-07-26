#include "majo.h"

#if RUBY_API_VERSION_MAJOR == 3 && RUBY_API_VERSION_MINOR == 1

static ID id_attached;

VALUE
majo_attached_object(VALUE klass) {
  if (!FL_TEST(klass, FL_SINGLETON)) {
    rb_raise(rb_eTypeError, "`%"PRIsVALUE"' is not a singleton class", klass);
  }

  return rb_attr_get(klass, id_attached);
}

void
majo_init_attached_object() {
  id_attached = rb_intern_const("__attached__");
}

#else

void
majo_init_attached_object() {}

#endif
