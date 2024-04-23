/*
 * @Author: yesky a316606581@gmail.com
 * @LastEditors: yesky a316606581@gmail.com
 * @LastEditTime: 2024-04-23 15:47:46
 * @Date: 2024-04-21 12:41:12
 * @FilePath: Loger.cpp
 * @Description:
 * Copyright (c) 2024 by yesky email: a316606581@gmail.com, All Rights Reserved.
 */

#include "Loger.h"

LOG_BASE::LOG* LOG_BASE::LOG::Log = NULL;
std::mutex LOG_BASE::LOG::log_mutex;

void LOG_BASE::LOG::init(LOGLEVEL loglevel, LOGTARGET logtarget,
                         const std::string &path) {
  setLogLevel(loglevel);
  setLogTarget(logtarget);
  _path = path;
  _stop = false;
  _worker = std::thread(&LOG_BASE::LOG::worker, this);

  _file.open(_path, std::ios_base::app);
}

void LOG_BASE::LOG::uninit() {
  stop();
  _worker.join();
  if (_file.is_open()) _file.close();
}

LOG_BASE::LOG *LOG_BASE::LOG::getInstance() {
  if (NULL == Log) {
    std::lock_guard<std::mutex> lock_guard(log_mutex);
    if (NULL == Log) {
      Log = new LOG("/home/yesky/code/OpenGLProjects/OpenGL_Render/OpenGLRender/src/base/log.txt");
    }
  }
  return Log;
}

LOG_BASE::LOGLEVEL LOG_BASE::LOG::getLogLevel() { return this->loglevel; }

void LOG_BASE::LOG::setLogLevel(LOGLEVEL loglevel) {
  this->loglevel = loglevel;
}

LOG_BASE::LOGTARGET LOG_BASE::LOG::getLogTarget() { return this->logtarget; }

void LOG_BASE::LOG::setLogTarget(LOGTARGET logtarget) {
  this->logtarget = logtarget;
}

void LOG_BASE::LOG::set_file_name(const std::string &path) {
  _path = path;
  _file.open(_path, std::ios_base::app);
}

void LOG_BASE::LOG::set_callback(
    const std::function<void(std::pair<LOGLEVEL, std::string>)> &func) {
  _handle = func;
}

void LOG_BASE::LOG::add_log(LOGLEVEL loglevel, const std::string &message) {
  std::lock_guard<std::mutex> lock(log_mutex);
  _queue.emplace(loglevel, message);
  _condition.notify_one();
}

std::string LOG_BASE::LOG::level2string(LOGLEVEL loglevel) {
  switch (loglevel) {
    case LOG_BASE::LOGLEVEL::LOG_LEVEL_DEBUG:
      return "Debug";
      break;
    case LOG_BASE::LOGLEVEL::LOG_LEVEL_ERROR:
      return "Error";
      break;
    case LOG_BASE::LOGLEVEL::LOG_LEVEL_WARNING:
      return "Warning";
      break;
    case LOG_BASE::LOGLEVEL::LOG_LEVEL_INFO:
      return "Info";
      break;
    default:
      return "Unknown";
      break;
  }
}

void LOG_BASE::LOG::worker() {
  for (;;) {
    std::pair<LOG_BASE::LOGLEVEL, std::string> pair;
    {
      std::unique_lock<std::mutex> lock(log_mutex);
      _condition.wait(lock, [this] { return _stop || !_queue.empty(); });
      if (_stop && _queue.empty()) {
        break;
      }
      pair = _queue.front();
      _queue.pop();
    }

    //输出到文件
    std::ofstream out(_path, std::ios_base::app);
    if (!out.good())
    {
        continue;
    }
    out << get_current_timestamp() <<"  "<<level2string(pair.first) <<":"<<
    pair.second << std::endl; out.close();

    // 如果文件流不可用，则重新尝试打开
    if (!_file.is_open()) {
      _file.open(_path, std::ios_base::app);
      if (!_file.is_open()) {
        // 打开文件失败，继续下一个循环
        continue;
      }
    }

    // 输出日志消息到文件流
    _file << get_current_timestamp() << "  " << level2string(pair.first) << ":"
          << pair.second << std::endl;
    _file.flush();  // 刷新缓冲区
    if (_handle) {
      _handle(pair);
    }
  }
}

std::string LOG_BASE::LOG::get_current_timestamp() {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  auto ms = std::chrono::duration_cast<std::chrono::microseconds>(
                now.time_since_epoch()) %
            1000;
  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << '.'
     << std::setfill('0') << std::setw(3) << ms.count();
  return ss.str();
}

void LOG_BASE::LOG::stop() {
  std::lock_guard<std::mutex> lock(log_mutex);
  _stop = true;
  _condition.notify_one();
}

LOG_BASE::LOG::LOG(const std::string &path) {
  init(LOG_LEVEL_NONE, LOG_TARGET_FILE, path);
}

LOG_BASE::LOG::~LOG() {
  stop();
  _worker.join();
  if (_file.is_open()) _file.close();
}