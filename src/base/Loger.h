/*
 * @Author: yesky a316606581@gmail.com
 * @LastEditors: yesky a316606581@gmail.com
 * @LastEditTime: 2024-04-23 16:08:04
 * @Date: 2024-04-21 13:36:15
 * @FilePath: Loger.h
 * @Description: a simple logger
 * Copyright (c) 2024 by yesky email: a316606581@gmail.com, All Rights Reserved.
 */
#pragma once

#include <chrono>
#include <condition_variable>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>

namespace LOG_BASE {
typedef void* HANDLE;
/**
 * @description: log level
 * @return {*}
 */
enum LOGLEVEL {
  LOG_LEVEL_NONE,
  LOG_LEVEL_ERROR,    // error
  LOG_LEVEL_WARNING,  // warning
  LOG_LEVEL_DEBUG,    // debug
  LOG_LEVEL_INFO,     // info
};

/**
 * @description: log file target console, file etc..
 * @return {*}
 */
enum LOGTARGET {
  LOG_TARGET_NONE = 0x00,
  LOG_TARGET_CONSOLE = 0x01,
  LOG_TARGET_FILE = 0x10
};

/**
 * @description: single pattern
 * @return {*}
 */

class LOG {
 public:
  // init
  void init(LOGLEVEL loglevel, LOGTARGET logtarget, const std::string& path);

  //
  void uninit();

  static LOG* getInstance();

  // log level
  LOGLEVEL getLogLevel();
  void setLogLevel(LOGLEVEL loglevel);

  // log target
  LOGTARGET getLogTarget();
  void setLogTarget(LOGTARGET logtarget);

  void set_file_name(const std::string& path);

  // callback
  void set_callback(
      const std::function<void(std::pair<LOGLEVEL, std::string>)>& func);

  void add_log(LOGLEVEL loglevel, const std::string& message);

  static std::string level2string(LOGLEVEL loglevel);

 private:
  void worker();
  std::string get_current_timestamp();
  void stop();

  LOG();
  LOG(const std::string& path);
  ~LOG();
  static LOG* Log;

  // mutex
  static std::mutex log_mutex;

  std::condition_variable _condition;

  std::queue<std::pair<LOGLEVEL, std::string> > _queue;

  std::thread _worker;

  std::string _path;

  std::ofstream _file;

  // log level
  LOGLEVEL loglevel;

  // log target
  LOGTARGET logtarget;

  bool _stop;

  // handle
  std::function<void(std::pair<LOGLEVEL, std::string>)> _handle;
};
}  // namespace LOG_BASE

#define LOG_DEBUG(format, ...) { LOG_BASE::LOG::getInstance()->add_log(format, ##__VA_ARGS__); }
#define LOG_INFO(format, ...)  { LOG_BASE::LOG::getInstance()->add_log(format, ##__VA_ARGS__); }
#define LOG_WARN(format, ...)  { LOG_BASE::LOG::getInstance()->add_log(format, ##__VA_ARGS__); }
#define LOG_ERROR(format, ...) { LOG_BASE::LOG::getInstance()->add_log(format, ##__VA_ARGS__); }