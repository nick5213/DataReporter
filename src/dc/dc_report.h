//
//  dc_report.h
//
//  Created by nick5213 on 2022/11/28.
//  Copyright (c) 2022 nick5213. All rights reserved.
//

#ifndef DATA_REPORTER_DC_DC_REPORT_H_
#define DATA_REPORTER_DC_DC_REPORT_H_

#include <string>
#include <memory>
#include <iostream>

#include "Reporter.h"

namespace dc {

/**
 * 数据上报回调
 */
using ReportCallback = std::function<void(int64_t key, std::vector<std::string> &data)>;

/**
 * DataReport 回调
 */
using ReportFunction = std::function<void(int64_t key, std::list<std::shared_ptr<future::CacheItem>> &data)>;

class DCReport : public std::enable_shared_from_this<DCReport> {

 public:
  DCReport(std::string uuid, std::string path, std::string key);

  ~DCReport() = default;

  std::shared_ptr<DCReport> GetPtr() {
    return shared_from_this();
  }

  std::shared_ptr<future::Reporter> operator()() const {
    return reporter_;
  }

  std::shared_ptr<future::Reporter> GetReporter();
  void Upload(int64_t key, std::list<std::shared_ptr<future::CacheItem>> &data);
  void SetReportCallback(const ReportCallback &callback);
  void Push(const std::vector<unsigned char> &data);
  void UploadSuccess(int64_t key);
  void UploadFailed(int64_t key);
  void SetUploadItemCount(std::size_t itemSize);
  void SetFileMaxSize(std::size_t fileMaxSize);
  void SetExpiredTime(std::int64_t expiredTime);
  void SetReportingInterval(std::int64_t reportingInterval);
  void SetRetryInterval(std::int64_t reportingInterval);

 private:
  /**
   * uuid, 关联对应文件名
   */
  std::string uuid_;

  /**
   * 文件路径
   */
  std::string path_;

  /**
   * 文件加密 key
   */
  std::string key_;

  /**
   * 上报对象
   */
  std::shared_ptr<future::Reporter> reporter_;

  /**
   * 上报回调
   */
  ReportCallback callback_;
};

}  // namespace dc

#endif //DATA_REPORTER_DC_DC_REPORT_H_
