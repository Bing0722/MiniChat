#pragma once

#include "global.h"

struct SectionInfo {
  SectionInfo() {}

  ~SectionInfo() { section_datas_.clear(); }

  // 拷贝构造
  SectionInfo(const SectionInfo &src) { section_datas_ = src.section_datas_; }

  // 重载赋值操作
  SectionInfo &operator=(const SectionInfo &src) {
    if (&src == this) {
      return *this;
    }
    this->section_datas_ = src.section_datas_;

    return *this;
  }

  // 重载 [] 运算符
  std::string operator[](const std::string &key) {
    if (section_datas_.find(key) == section_datas_.end()) {
      return "";
    }

    // 这里可以添加一些边界检查
    return section_datas_[key];
  }

  std::map<std::string, std::string> section_datas_;
};

class ConfigMgr {
public:
  ~ConfigMgr() { config_map_.clear(); }

  // 线程安全 单例
  static ConfigMgr &Instance() {
    static ConfigMgr cfg_mgr;
    return cfg_mgr;
  }

  ConfigMgr(const ConfigMgr &src) { this->config_map_ = src.config_map_; }

  SectionInfo operator[](const std::string &section) {
    if (config_map_.find(section) == config_map_.end()) {
      return SectionInfo();
    }
    return config_map_[section];
  }

  ConfigMgr &operator=(const ConfigMgr &src) {
    if (&src == this) {
      return *this;
    }
    this->config_map_ = src.config_map_;
    return *this;
  }

private:
  ConfigMgr();

  std::map<std::string, SectionInfo> config_map_;
};
