# Architecture of Majo

Majo uses internal TracePoint hooks, `RUBY_INTERNAL_EVENT_NEWOBJ` and `RUBY_INTERNAL_EVENT_FREEOBJ`, to trace object allocation and free events.
We can use `ObjectSpace.trace_object_allocations_start` method in the Ruby-level, but this API is not suitable for Majo because it does not provide information about freed objects. Therefore, Majo uses these hooks in the C-level.

## On allocation event

When an object is allocated, Majo records the object into a st table with the path, line number, and so on.

See `newobj_i` function in `majo.c`.

## On free event

When an object is freed, Majo moves the allocation information to an array from the st table if it's a long-lived object.
It determines whether the object is long-lived by checking the GC count. If the object is not swept by a GC sweeping phase, it's a long-lived object.

See `freeobj_i` function in `majo.c`.

## Output format

Majo provides three output formats: `color`, `monochrome`, and `csv`.

`color` and `monochrome` are for human-readable output. They provide overview information about long-lived objects.

`csv` is for machine-readable output. It provides the raw information. You can use this format to analyze the result with other tools.
