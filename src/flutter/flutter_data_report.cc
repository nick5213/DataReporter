//
//  flutter_data_report.cc
//
//  Created by nick5213 on 2022/11/28.
//  Copyright (c) 2022 nick5213. All rights reserved.
//

#include "flutter_data_report.h"

#include <cstdint>

#include "dc/dc_report.h"
#include "dart/xdart_channel.h"
#include "dart/scope_dart_obj_array.h"

DCReportToken DCReportInit(const char *uuid, const char *path, const char *key) {
  auto *dc_report = new dc::DCReport(uuid, path, key);
  dc_report->GetReporter()->Start();
  return dc_report;
}

void DCReportRelease(DCReportToken token) {
  auto *dc_report = reinterpret_cast<dc::DCReport *>(token);
  delete dc_report;
}

void DCReportPush(DCReportToken token, const char *data) {
  auto *dc_report = reinterpret_cast<dc::DCReport *>(token);
  if (dc_report) {
    dc_report->Push(std::vector<unsigned char>(data, data + strlen(data)));
  }
}

void DCReportSetCallback(DCReportToken token, int64_t port) {
  auto *dc_report = reinterpret_cast<dc::DCReport *>(token);
  if (dc_report) {
    // set report callback
    dc_report->SetReportCallback([port](int64_t key, std::vector<std::string> &data) {
      // convert
      dc::ffi::ScopeDartObjectArray objs;
      objs.Push<int64_t>(key);
      for (const auto &v: data) {
        objs.Push<std::string>(v);
      }
      // Post to flutter
      dc::ffi::XDartChannel::PostCObject(port, objs.AsCObject());
    });
  }
}

void DCReportUploadSuccess(DCReportToken token, int64_t key) {
  auto *dc_report = reinterpret_cast<dc::DCReport *>(token);
  if (dc_report) {
    dc_report->UploadSuccess(key);
  }
}

void DCReportUploadFailed(DCReportToken token, int64_t key) {
  auto *dc_report = reinterpret_cast<dc::DCReport *>(token);
  if (dc_report) {
    dc_report->UploadFailed(key);
  }
}

void DCReportSetItemCount(DCReportToken token, int64_t count) {
  auto *dc_report = reinterpret_cast<dc::DCReport *>(token);
  if (dc_report) {
    dc_report->SetUploadItemCount(count);
  }
}

void DCReportSetReportInterval(DCReportToken token, int64_t interval) {
  auto *dc_report = reinterpret_cast<dc::DCReport *>(token);
  if (dc_report) {
    dc_report->SetReportingInterval(interval);
  }
}

void DCReportSetRetryInterval(DCReportToken token, int64_t interval) {
  auto *dc_report = reinterpret_cast<dc::DCReport *>(token);
  if (dc_report) {
    dc_report->SetRetryInterval(interval);
  }
}
