/*
  libco
  license: public domain
*/

#if defined(__clang__) || defined(__GNUC__)
  #if defined(LIBCO_NO_INLINE_ASM)
    #include "amd64.c"
  #else
    #include "amd64_gcc.c"
  #endif
#else
  #error "libco: unsupported processor, compiler or operating system"
#endif
