//
//  flutter_data_report.h
//
//  Created by nick5213 on 2022/11/28.
//  Copyright (c) 2022 nick5213. All rights reserved.
//

#ifndef DATA_REPORTER_FLUTTER_FLUTTER_DATA_REPORT_H_
#define DATA_REPORTER_FLUTTER_FLUTTER_DATA_REPORT_H_

#include <unistd.h>

#ifdef __cplusplus
#define DC_EXTERN_C extern "C"
#else
#define DC_EXTERN_C
#endif

typedef void *DCReportToken;

/**
 * 初始化 data-reporter
 *
 * @param uuid uuid, 可关联用户id, 内部关联文件名
 * @param path 文件路径
 * @param key  文件加密 key
 * @return dc-reporter 指针
 */
DC_EXTERN_C DCReportToken DCReportInit(const char *uuid, const char *path, const char *key);

/**
 * 释放 dc-reporter
 *
 * @param token dc-reporter 指针
 */
DC_EXTERN_C void DCReportRelease(DCReportToken token);

/**
 * 写入上报数据
 *
 * @param token dc-reporter 指针
 * @param data  写入数据
 */
DC_EXTERN_C void DCReportPush(DCReportToken token, const char *data);

/**
 * 设置上报回调
 *
 * @param token dc-reporter 指针
 * @param port  回调端口
 */
DC_EXTERN_C void DCReportSetCallback(DCReportToken token, int64_t port);

/**
 * 标记上报成功
 *
 * @param token dc-reporter 指针
 * @param key   数据 key
 */
DC_EXTERN_C void DCReportUploadSuccess(DCReportToken token, int64_t key);

/**
 * 标记上报失败
 *
 * @param token dc-reporter 指针
 * @param key   数据 key
 */
DC_EXTERN_C void DCReportUploadFailed(DCReportToken token, int64_t key);

/**
 * 设置上报条数
 *
 * @param token dc-reporter 指针
 * @param count 条数
 */
DC_EXTERN_C void DCReportSetItemCount(DCReportToken token, int64_t count);

/**
 * 设置上报间隔
 *
 * @param token    dc-reporter 指针
 * @param interval 上报间隔
 */
DC_EXTERN_C void DCReportSetReportInterval(DCReportToken token, int64_t interval);

/**
 * 设置重试间隔
 *
 * @param token    dc-reporter 指针
 * @param interval 重试间隔
 */
DC_EXTERN_C void DCReportSetRetryInterval(DCReportToken token, int64_t interval);

#endif //DATA_REPORTER_FLUTTER_FLUTTER_DATA_REPORT_H_
