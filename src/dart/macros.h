//
//  macros.h
//  常用宏，部分摘自 Android 源码
//
//  Created by nick5213 on 2020/7/12.
//  Copyright © 2023 nick5213. All rights reserved.
//

#ifndef DATA_REPORTER_BASE_MACROS_H_
#define DATA_REPORTER_BASE_MACROS_H_

// 数组元素个数
#define NELEM(array_name) (sizeof(array_name) / sizeof((array_name)[0]))

// DISALLOW_COPY_AND_ASSIGN disallows the copy and operator= functions.
// It goes in the private: declarations in a class.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the private: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName();                                    \
  DISALLOW_COPY_AND_ASSIGN(TypeName)

#if defined(__clang__) || defined(__GNUC__)
#define LIKELY(x)       __builtin_expect((x), true)
#define UNLIKELY(x)     __builtin_expect((x), false)

#else
#define LIKELY(x)       (x)
#define UNLIKELY(x)     (x)

#endif

#ifndef NDEBUG
#define ALWAYS_INLINE
#else
#define ALWAYS_INLINE  __attribute__ ((always_inline))
#endif

#ifdef __cplusplus
#define EXTERN_EXPORTED         extern "C" __attribute__((visibility ("default")))
#define EXTERN_NON_EXPORTED     extern "C" __attribute__((visibility ("hidden")))
#else
#define EXTERN_EXPORTED         extern __attribute__((visibility ("default")))
#define EXTERN_NON_EXPORTED     extern __attribute__((visibility ("default")))
#endif

#define VISIBILITY_HIDDEN       __attribute__((visibility("hidden")))
#define VISIBILITY_PROTECTED    __attribute__((visibility("protected")))
#define VISIBILITY_DEFAULT      __attribute__((visibility("default")))

#define UNUSED __attribute__((unused))

#endif //DATA_REPORTER_BASE_MACROS_H_