#include "majo.h"

VALUE rb_mMajo;

void
Init_majo(void)
{
  rb_mMajo = rb_define_module("Majo");
}
