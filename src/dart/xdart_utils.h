//
//  xdart_utils.h
//  支持构造不同类型参数的 Dart_CObject 模板
//
//  Created by nick5213 on 2021/7/26.
//  Copyright (c) 2021 nick5213. All rights reserved.
//

#ifndef DATA_REPORTER_DART_X_DART_UTILS_H_
#define DATA_REPORTER_DART_X_DART_UTILS_H_

#include <vector>

#include "dart/xdart_native_api.h"

namespace dc {
namespace ffi {

class XDartUtils {

 public:

  template<typename T>
  inline static Dart_CObject *NewDartCObject(const T &);

  static void PushDartCObjectToTarget(std::vector<Dart_CObject *> &vector) {
  }

  template<typename First, typename... Remaining>
  inline static void PushDartCObjectToTarget(std::vector<Dart_CObject *> &target,
                                             const First &first,
                                             const Remaining &...remaining);

  /**
   * new Dart_CObject with type
   *
   * @param type Dart_CObject_Type
   * @return Dart_CObject
   */
  static Dart_CObject *New(Dart_CObject_Type type) {
    auto *cobject = new Dart_CObject();
    cobject->type = type;
    return cobject;
  }
};

/**
 * 推入参数列表到目标 Dart_CObject * 数组
 *
 * @tparam First     参数列表第一个
 * @tparam Remaining 剩余参数列表
 * @param target     存放的目标 Dart_CObject * 数组
 * @param first      参数列表第一个
 * @param remaining  剩余参数列表
 */
template<typename First, typename... Remaining>
inline void XDartUtils::PushDartCObjectToTarget(std::vector<Dart_CObject *> &target,
                                                const First &first,
                                                const Remaining &... remaining) {
  auto cobject = XDartUtils::NewDartCObject<First>(first);
  if (cobject) {
    target.push_back(cobject);
  }
  XDartUtils::PushDartCObjectToTarget(target, remaining...);
}

template<typename T>
inline Dart_CObject *XDartUtils::NewDartCObject(const T &) {
  return nullptr;
}

/**
 * 构建 Dart C 对象
 * int32 类型模板
 *
 * @param value int32 value
 * @return Dart C 对象
 */
template<>
inline Dart_CObject *XDartUtils::NewDartCObject(const int32_t &value) {
  auto *cobject = New(Dart_CObject_kInt32);
  cobject->value.as_int32 = value;
  return cobject;
}

/**
 * 构建 Dart C 对象
 * int64 类型模板
 *
 * @param value int64 value
 * @return Dart C 对象
 */
template<>
inline Dart_CObject *XDartUtils::NewDartCObject(const int64_t &value) {
  auto *cobject = New(Dart_CObject_kInt64);
  cobject->value.as_int64 = value;
  return cobject;
}

/**
 * 构建 Dart C 对象
 * double 类型模板
 *
 * @param value double value
 * @return Dart C 对象
 */
template<>
inline Dart_CObject *XDartUtils::NewDartCObject(const double &value) {
  auto *cobject = New(Dart_CObject_kDouble);
  cobject->value.as_int64 = value;
  return cobject;
}

/**
 * 构建 Dart C 对象
 * std::string 类型模板
 *
 * @param value std::string value
 * @return Dart C 对象
 */
template<>
inline Dart_CObject *XDartUtils::NewDartCObject(const std::string &value) {
  auto *cobject = New(Dart_CObject_kString);
  cobject->value.as_string = const_cast<char *>(value.c_str());;
  return cobject;
}

/**
 * 构建 Dart C 对象
 * char * 类型模板
 *
 * @param value char * value
 * @return Dart C 对象
 */
template<>
inline Dart_CObject *XDartUtils::NewDartCObject(char *const &value) {
  auto *cobject = New(Dart_CObject_kString);
  cobject->value.as_string = value;
  return cobject;
}

/**
 * 构建 Dart C 对象
 * std::vector<Dart_CObject *> 类型模板
 *
 * @param value std::vector<Dart_CObject *> value
 * @return Dart C 对象
 */
template<>
inline Dart_CObject *XDartUtils::NewDartCObject(const std::vector<Dart_CObject *> &value) {
  auto *cobject = New(Dart_CObject_kArray);
  cobject->value.as_array.length = value.size();
  return cobject;
}

}  // namespace ffi
}  // namespace dc

#endif //DATA_REPORTER_DART_X_DART_UTILS_H_
