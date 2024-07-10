#ifndef MAJO_H
#define MAJO_H 1

#include <stdbool.h>

#include "ruby.h"
#include "ruby/debug.h"
#include "ruby/internal/gc.h"

#include "darray.h"

#include "allocation_info.h"
#include "result.h"
#include "unique_str.h"

extern VALUE rb_mMajo;
extern VALUE rb_cMajo_Result;
extern VALUE rb_cMajo_AllocationInfo;

// Exposing the internal functions
size_t rb_obj_memsize_of(VALUE);
VALUE rb_gc_disable_no_rest(void);

#endif
