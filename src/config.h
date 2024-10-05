// config.h
// Copyright 2024/10/5 Robin.Rowe@CinePaint.org
// License Open Source MIT

#ifndef config_h
#define config_h

#define _GL_CONFIG_H_INCLUDED 1
#define _GL_ATTRIBUTE_MALLOC 
#define _GL_ATTRIBUTE_DEALLOC_FREE
#define _GL_ATTRIBUTE_SPEC_PRINTF_STANDARD 
#define _GL_ATTRIBUTE_FORMAT(a))
#define _GL_ATTRIBUTE_RETURNS_NONNULL
#define _GL_ATTRIBUTE_CONST
#define _GL_ATTRIBUTE_PURE
#define _GL_INLINE_HEADER_BEGIN
#define _GL_INLINE_HEADER_END
#define _GL_ATTRIBUTE_FORMAT_PRINTF(a,b)
#define GLOB_TILDE_CHECK 1
#define xstrdup strdup
#define BINARY_IO_INLINE
#define PACKAGE_BUGREPORT "NONE"
#define VERSION "1"
#define PACKAGE_VERSION "0"

#include <stdio.h>
//typedef int FILE;
size_t getline(char** lineptr, size_t* n, FILE* stream);

#define STDOUT_FILENO _fileno(stdout)
#define STDIN_FILENO _fileno(stdin)
#define STDERR_FILENO _fileno(stderr)

#endif