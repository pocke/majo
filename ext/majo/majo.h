#ifndef MAJO_H
#define MAJO_H 1

#include <stdbool.h>

#include "ruby.h"
#include "ruby/version.h"
#include "ruby/debug.h"
#include "ruby/internal/gc.h"

#include "darray.h"

#include "allocation_info.h"
#include "result.h"
#include "unique_str.h"
#include "attached_object.h"

extern VALUE rb_mMajo;
extern VALUE rb_cMajo_Result;
extern VALUE rb_cMajo_AllocationInfo;

// Exposing the internal functions
size_t rb_obj_memsize_of(VALUE);

#endif
