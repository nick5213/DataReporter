//
//  scope_dart_obj_array.h
//  RAAI 构造管理 Dart_CObject Array，支持添加不同类型参数列表
//
//  Created by nick5213 on 2021/7/26.
//  Copyright (c) 2023 nick5213. All rights reserved.
//

#ifndef DATA_REPORTER_DART_SCOPE_DART_OBJ_ARRAY_H_
#define DATA_REPORTER_DART_SCOPE_DART_OBJ_ARRAY_H_

#include "dart/macros.h"
#include "dart/xdart_utils.h"
#include "dart/xdart_native_api.h"

namespace dc {
namespace ffi {

//
// RAAI 构造管理 Dart_CObject Array
// Example:
//    xdart::ffi::ScopeDartObjectArray objs;
//    objs.Push<int, std::string>(200, message);
//    // convert to Dart_CObject *
//    auto *cobject = objs.AsCObject()
//    // just get Dart_CObject *, should convert first
//    auto *cobject = objs.Get()
//
// 析构函数会自动释放，无需手动 `Release`
//
class ScopeDartObjectArray {

 public:
  explicit ScopeDartObjectArray() : dart_c_object_(nullptr), dart_cobj_array_({}) {
  }

  ~ScopeDartObjectArray() {
    Release();
  }

  /**
   * 添加指定类型的参数列表
   *
   * @param args 参数列表
   */
  template<typename... Args>
  void Push(const Args &... args) {
    size_t size = std::tuple_size<decltype(std::make_tuple(args...))>::value;
    if (size > 0) {
      XDartUtils::PushDartCObjectToTarget<Args...>(dart_cobj_array_, args...);
    }
  }

  /**
   * 转换 Dart CObject
   *
   * @return 返回 Dart_CObject 结构体指针
   */
  Dart_CObject *AsCObject() {
    dart_c_object_ = XDartUtils::NewDartCObject<std::vector<Dart_CObject *>>(dart_cobj_array_);
    dart_c_object_->value.as_array.values = dart_cobj_array_.data();
    return dart_c_object_;
  }

  /**
   * 检查 Dart_CObject 结构体指针是否有效
   *
   * @return 有效返回 true
   */
  bool Valid() {
    return dart_c_object_ != nullptr;
  }

  /**
   * Just get Dart_CObject*
   *
   * @return Dart_CObject*
   */
  Dart_CObject *Get() {
    return dart_c_object_;
  }

  /**
   * 释放 Dart CObject 内存
   */
  void Release() {
    ReleaseDartCObject(dart_c_object_);
  }

 private:
  /**
   * 释放 Dart_CObject
   * - 如果类型为 `Dart_CObject_kArray`，则遍历释放数组下的 Dart_CObject
   *
   * @param cobject  dart object 结构体指针
   */
  void ReleaseDartCObject(Dart_CObject *cobject) {
    if (cobject != nullptr) {
      if (cobject->type == Dart_CObject_kArray) {
        for (int i = 0; i < dart_c_object_->value.as_array.length; ++i) {
          auto sub_obj = dart_c_object_->value.as_array.values[i];
          if (sub_obj != nullptr) {
            if (sub_obj->type == Dart_CObject_kArray) {
              ReleaseDartCObject(sub_obj);
            } else {
              delete sub_obj;
            }
          }
        }
      }
      delete cobject;
    }
  }

 private:
  Dart_CObject *dart_c_object_;
  std::vector<Dart_CObject *> dart_cobj_array_;

  DISALLOW_COPY_AND_ASSIGN(ScopeDartObjectArray);
};

}  // namespace ffi
}  // namespace dc

#endif //DATA_REPORTER_DART_SCOPE_DART_OBJ_ARRAY_H_
