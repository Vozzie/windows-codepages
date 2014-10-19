#pragma once
#ifndef __CODEPAGES_HEADER__
#define __CODEPAGES_HEADER__

#include <stddef.h>

typedef unsigned char  ANSIChar;
typedef unsigned char  UTF8Char;
typedef unsigned short UTF16Char;
typedef unsigned int   UTF32Char;

typedef ANSIChar  * ANSIPtr;
typedef UTF8Char  * UTF8Ptr;
typedef UTF16Char * UTF16Ptr;
typedef UTF32Char * UTF32Ptr;

#define CODEPAGE_874  874
#define CODEPAGE_932  932
#define CODEPAGE_936  936
#define CODEPAGE_949  949
#define CODEPAGE_950  950
#define CODEPAGE_1250 1250
#define CODEPAGE_1251 1251
#define CODEPAGE_1252 1252
#define CODEPAGE_1253 1253
#define CODEPAGE_1254 1254
#define CODEPAGE_1255 1255
#define CODEPAGE_1256 1256
#define CODEPAGE_1257 1257
#define CODEPAGE_1258 1258

#define CODEPAGE_037  37
#define CODEPAGE_500  500
#define CODEPAGE_875  875
#define CODEPAGE_1026 1026

#define CODEPAGE_ERROR_INVALID_CODEPAGE ((size_t)-1)
#define CODEPAGE_ERROR_INVALID_ARGUMENTS ((size_t)-2)
#define CODEPAGE_ERROR_INVALID_BUFFSIZE ((size_t)-3)

size_t codepage_to_utf8(int codepage, ANSIPtr source, size_t source_length, UTF8Ptr target, size_t target_length, size_t * invalid_chars);
size_t codepage_to_utf8_size(int codepage, ANSIPtr source, size_t source_length, size_t * invalid_chars);
size_t codepage_to_utf16(int codepage, ANSIPtr source, size_t source_length, UTF16Ptr target, size_t target_length, size_t * invalid_chars);

#endif//__CODEPAGES_HEADER__
