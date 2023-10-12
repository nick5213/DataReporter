//
//  dc_report.cc
//
//  Created by nick5213 on 2022/11/28.
//  Copyright (c) 2022 nick5213. All rights reserved.
//

#include "dc_report.h"

namespace dc {

DCReport::DCReport(std::string uuid, std::string path, std::string key)
    : uuid_(std::move(uuid)), path_(std::move(path)), key_(std::move(key)) {
  using namespace std::placeholders;

  ReportFunction callback = std::bind(&::dc::DCReport::Upload, this, _1, _2);
  reporter_ = std::make_shared<future::Reporter>(uuid_, path_, key_, callback);
  reporter_->SetUploadItemCount(10);
  reporter_->SetReportingInterval(10 * 1000L);
  reporter_->Start();
}

std::shared_ptr<future::Reporter> DCReport::GetReporter() {
  return reporter_;
}

void DCReport::Upload(int64_t key, std::list<std::shared_ptr<future::CacheItem>> &data) {
  if (callback_ == nullptr) {
    return;
  }
  std::vector<std::string> data_info;
  for (const std::shared_ptr<future::CacheItem> &item: data) {
    if (item->pbEncodeItem.data.Length() == 0) {
      continue;
    }
    data_info.emplace_back((char *) item->pbEncodeItem.data.GetBegin(), item->pbEncodeItem.data.Length());
  }
  callback_(key, data_info);
}

void DCReport::SetReportCallback(const ReportCallback &callback) {
  callback_ = callback;
}

void DCReport::Push(const std::vector<unsigned char> &data) {
  reporter_->Push(data);
}

void DCReport::UploadSuccess(int64_t key) {
  reporter_->UoloadSuccess(key);
}

void DCReport::UploadFailed(int64_t key) {
  reporter_->UploadFailed(key);
}

void DCReport::SetUploadItemCount(std::size_t itemSize) {
  reporter_->SetUploadItemCount(itemSize);
}

void DCReport::SetFileMaxSize(std::size_t fileMaxSize) {
  reporter_->SetFileMaxSize(fileMaxSize);
}

void DCReport::SetExpiredTime(std::int64_t expiredTime) {
  reporter_->SetExpiredTime(expiredTime);
}

void DCReport::SetReportingInterval(std::int64_t reportingInterval) {
  reporter_->SetReportingInterval(reportingInterval);
}

void DCReport::SetRetryInterval(std::int64_t reportingInterval) {
  reporter_->SetRetryInterval(reportingInterval);
}

}  // namespace dc