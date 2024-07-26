#if RUBY_API_VERSION_MAJOR == 3 && RUBY_API_VERSION_MINOR == 1

VALUE
majo_attached_object(VALUE klass);

#else

#define majo_attached_object(klass) rb_class_attached_object(klass)

#endif

void
majo_init_attached_object();
