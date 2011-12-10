# Liz - lizard brain for game entities
Liz is a data-oriented behavior tree library implemented in C.


## Learning about liz' internals
The best way to learn liz works is to first read liz_vm.h and liz_vm.c and 
especailly look into the functions with the liz_vm_step_ prefix.


## Extending liz with new decider or action nodes
- Add a new type enum value and node defining structs to shape_atom in 
  liz_common_internal.
- Extend liz_builder.
- Extend liz_vm to invoke the new node and guard it if it is a decider.


## Cross portability
The primary development environment for liz is Mac OS X 10.7.x. I haven't tried
to port it to another platform yet. However, liz is written in a C99 subset that 
should, for example, be compilable as C++ in MSVC10.

To port liz to a new platform adapt:
- liz_platform_types.h - e.g., provide definitions for bool, bit-sized integer
  types like int32_t or uint16_t, and uintptr_t and intptr_t, etc.
- liz_platform_macros.h - adapt the macros, e.g., LIZ_INLINE or LIZ_RESTRICT to
  your compiler.
- liz_platform_functions.h - adapt these functions, e.g., assert, memmove, etc.
  to your platform, if necessary.

