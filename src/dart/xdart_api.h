//
//  xdart_api.h
//  from dart-lang/sdk : runtime/include/dart_api.h
//
//  Created by nick5213 on 2021/7/25.
//  Copyright (c) 2023 nick5213. All rights reserved.
//

#ifndef DATA_REPORTER_DART_X_DART_API_H_
#define DATA_REPORTER_DART_X_DART_API_H_

#include <cstdint>

#ifdef __cplusplus
#define DART_EXTERN_C extern "C"
#else
#define DART_EXTERN_C
#endif

#if defined(__CYGWIN__)
#error Tool chain and platform not supported.
#elif defined(_WIN32)
#if defined(DART_SHARED_LIB)
#define DART_EXPORT DART_EXTERN_C __declspec(dllexport)
#else
#define DART_EXPORT DART_EXTERN_C
#endif
#else
#if __GNUC__ >= 4
#if defined(DART_SHARED_LIB)
#define DART_EXPORT                                                            \
  DART_EXTERN_C __attribute__((visibility("default"))) __attribute((used))
#else
#define DART_EXPORT DART_EXTERN_C
#endif
#else
#error Tool chain not supported.
#endif
#endif

typedef void (*Dart_HandleFinalizer)(void *isolate_callback_data, void *peer);

/**
 * A port is used to send or receive inter-isolate messages
 */
typedef int64_t Dart_Port;

/**
 * ILLEGAL_PORT is a port number guaranteed never to be associated with a valid
 * port.
 */
#define ILLEGAL_PORT ((Dart_Port)0)

/*
 * ==========
 * Typed Data
 * ==========
 */

typedef enum {
  Dart_TypedData_kByteData = 0,
  Dart_TypedData_kInt8,
  Dart_TypedData_kUint8,
  Dart_TypedData_kUint8Clamped,
  Dart_TypedData_kInt16,
  Dart_TypedData_kUint16,
  Dart_TypedData_kInt32,
  Dart_TypedData_kUint32,
  Dart_TypedData_kInt64,
  Dart_TypedData_kUint64,
  Dart_TypedData_kFloat32,
  Dart_TypedData_kFloat64,
  Dart_TypedData_kInt32x4,
  Dart_TypedData_kFloat32x4,
  Dart_TypedData_kFloat64x2,
  Dart_TypedData_kInvalid
} Dart_TypedData_Type;

#endif //DATA_REPORTER_DART_X_DART_API_H_
