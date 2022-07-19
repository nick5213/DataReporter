//
//  xdart_channel.cc
//
//  Created by nick5213 on 2021/7/25.
//  Copyright (c) 2023 nick5213. All rights reserved.
//

#include "dart/xdart_channel.h"

#define TAG "XDartChannel" // NO_LINT

namespace dc {
namespace ffi {

/**
 * 绑定 dart 侧的 Dart_PostCObject 函数指针
 *
 * @param object_type Dart_PostCObject 函数指针
 */
DART_EXPORT void DCReport_BindDartPostCObject(Dart_PostCObject object_type) {
  dc::ffi::XDartChannel::BindDartPostCObject(object_type);
}

/**
 * 设置全局 Dart_Port
 *
 * @param port_id dart port id
 */
DART_EXPORT void XDart_SetGlobalDartPort(Dart_Port port_id) {
  dc::ffi::XDartChannel::SetGlobalDartPort(port_id);
}

Dart_PostCObject XDartChannel::dart_post_c_object_ = nullptr;
Dart_Port XDartChannel::global_dart_port_ = ILLEGAL_PORT;

/**
 * 绑定 Dart VM 的 Dart_PostCObject
 *
 * @param object Dart_PostCObject 函数指针
 */
void XDartChannel::BindDartPostCObject(Dart_PostCObject object) {
  dart_post_c_object_ = object;
}

/**
 * 设置全局 Dart ReceivePort
 *
 * @param port_id dart UI ReceivePort
 */
void XDartChannel::SetGlobalDartPort(Dart_Port port_id) {
  global_dart_port_ = port_id;
}

/**
 * 获取全局端口
 *
 * @return 全局端口
 */
Dart_Port XDartChannel::GetGlobalDartPort() {
  return global_dart_port_;
}

/**
 * Posts a message on some port.
 *
 * @param port_id 指定接收的目标端口 ReceivePort
 * @param message Dart_CObject 消息
 * @return True if the message was posted.
 */
bool XDartChannel::PostCObject(Dart_Port port_id, Dart_CObject *message) {
  if (dart_post_c_object_ == nullptr) {
    return false;
  }
  return dart_post_c_object_(port_id, message);
}

/**
 * Posts a message on global dart port.
 *
 * @param message Dart_CObject 消息
 * @return True if the message was posted.
 */
bool XDartChannel::PostCObject(Dart_CObject *message) {
  if (dart_post_c_object_ == nullptr) {
    return false;
  }
  if (ILLEGAL_PORT == global_dart_port_) {
    return false;
  }
  return dart_post_c_object_(global_dart_port_, message);
}

}  // namespace ffi
}  // namespace dc


