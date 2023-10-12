//
//  main.cc
//
//  Created by nick5213 on 2022/11/28.
//  Copyright (c) 2022 nick5213. All rights reserved.
//

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <utility>

#include "dc/dc_report.h"
#include "Reporter.h"
#include "flutter/flutter_data_report.h"
#include "dart/xdart_channel.h"

static void *TestDCReport(const std::string &uuid, const std::string &path, const std::string &key) {
  using namespace future;
  using namespace dc;
  auto *dc_report = new DCReport(uuid, path, key);
  dc_report->SetUploadItemCount(100);

  {
    // dart 回调示例
    using namespace std::placeholders;
    int port = 8099;
    dc_report->SetReportCallback([dc_report, port](int64_t key, std::vector<std::string> &data) {
      std::cout << ">>> DartCallback with: " << port << ", key:" << key << std::endl;
      for (const auto &v : data) {
        std::cout << ">>> DartCallback data: " << v << std::endl;
      }
      dc_report->GetReporter()->UoloadSuccess(key);
    });
  }

  {
    // 添加上报数据示例
    std::string msg1 = "Hello world!";
    std::string msg2 = "1008611001";
    std::string msg3 = "你好，世界";
    dc_report->Push(std::vector<unsigned char>(msg1.begin(), msg1.end()));
    dc_report->Push(std::vector<unsigned char>(msg2.begin(), msg2.end()));
    dc_report->Push(std::vector<unsigned char>(msg3.begin(), msg3.end()));
  }

  return dc_report;
}

static void *TestFlutter(const std::string &uuid, const std::string &path, const std::string &key) {
  using namespace std::placeholders;
  auto *reporter = DCReportInit(uuid.c_str(), path.c_str(), key.c_str());

  // 模拟绑定 dart 函数指针
  dc::ffi::DCReport_BindDartPostCObject([](Dart_Port port_id, Dart_CObject *message) -> bool {
    std::cout << ">>> DartPostCObject Callback with: " << port_id << ", msg: " << message << std::endl;
    auto type = message->type;
    if (type == Dart_CObject_kArray) {
      auto v = message->value.as_array;
      auto l = v.length;
      auto s = v.values;

      dc::DCReport *token;
      int64_t key;
      std::vector<std::string> data;

      for (int i = 0; i < l; ++i) {
        auto item = s[i];
        if (i == 0 && item->type == Dart_CObject_kInt64) {
          token = reinterpret_cast<dc::DCReport *>(item->value.as_int64);
        } else if (i == 1 && item->type == Dart_CObject_kInt64) {
          key = item->value.as_int64;
        } else if (item->type == Dart_CObject_kString) {
          data.emplace_back(item->value.as_string);
          std::cout << ">>> DartPostCObject Callback key: " << key << ", value: " << item->value.as_string << std::endl;
        }
      }

      // upload data
      if (token) {
        token->UploadSuccess(key);
        std::cout << ">>> DartPostCObject upload success with: " << key << std::endl;
      }
    }
  });

  DCReportSetCallback(reporter, 8199);
  DCReportSetItemCount(reporter, 2);
  DCReportSetReportInterval(reporter, 1 * 1000L);

  DCReportPush(reporter, "Hello Flutter");
  DCReportPush(reporter, "Dart-FFi");
  DCReportPush(reporter, "Dart-Channel");

  return reporter;
}

int main(int argc, char *argv[]) {
  std::string target = argv[0];
  std::string path;
  std::string uuid = "10086";
  std::string key;

  size_t found = target.find_last_of("/\\");
  if (found != std::string::npos) {
    path = target.substr(0, found + 1);
  }
  std::cout << ">>> path: " << path << uuid << "_data.buf" << std::endl;

  // 测试1
  // auto *token = TestDCReport(uuid, path, key);

  // 测试2
  auto *token = TestFlutter(uuid, path, key);

  std::this_thread::sleep_for(std::chrono::milliseconds(5 * 1000L));

  // release
  DCReportRelease(token);
  std::cout << ">>> reporter finish." << std::endl;

  return 0;
}
