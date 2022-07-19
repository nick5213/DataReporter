//
//  xdart_channel.h
//
//  Created by nick5213 on 2021/7/25.
//  Copyright (c) 2021 nick5213. All rights reserved.
//

#ifndef DATA_REPORTER_DART_X_DART_CHANNEL_H_
#define DATA_REPORTER_DART_X_DART_CHANNEL_H_

#include "xdart_native_api.h"

namespace dc {
namespace ffi {

DART_EXPORT void DCReport_BindDartPostCObject(Dart_PostCObject object_type);

class XDartChannel {
 public:
  static void BindDartPostCObject(Dart_PostCObject object);
  static void SetGlobalDartPort(Dart_Port port_id);
  static Dart_Port GetGlobalDartPort();

  static bool PostCObject(Dart_Port port_id, Dart_CObject *message);
  static bool PostCObject(Dart_CObject *message);

 private:
  static Dart_PostCObject dart_post_c_object_;
  static Dart_Port global_dart_port_;
};

}  // namespace ffi
}  // namespace dc

#endif //DATA_REPORTER_DART_X_DART_CHANNEL_H_
