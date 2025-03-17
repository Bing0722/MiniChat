#include "ConfigMgr.h"

ConfigMgr::ConfigMgr() {
#ifdef __linux
  // 获取当前文件路径
  boost::filesystem::path current_path = boost::filesystem::current_path();

  // 拼接config.ini 和当前路径
  boost::filesystem::path config_path = current_path / "config.ini";
  if (!boost::filesystem::exists(config_path)) {
    std::cout << "Failed to load configuration!" << std::endl;
    exit(1);
  }

  // 存储配置数据的树形结构
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(config_path.c_str(), pt);
#elif _WIN32
  // 获取当前文件路径
  boost::filesystem::path current_path = boost::filesystem::current_path();

  // 拼接 config.ini 和当前路径
  boost::filesystem::path config_path = current_path / "../../config.ini";
  if (!boost::filesystem::exists(config_path)) {
    std::cout << "Failed to load configuration!" << std::endl;
    exit(1);
  }

  // 存储配置数据的树形结构
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(config_path.string(), pt);
#else
  std::cout << "Unkonw operating system" << std::endl;
  exit(1);
#endif

  // 读取section 信息
  for (const auto &section_pair : pt) {
    const std::string &section_name = section_pair.first;
    const boost::property_tree::ptree &section_tree = section_pair.second;

    // 存储键值对的信息
    std::map<std::string, std::string> section_config;
    for (const auto &key_value_pair : section_tree) {
      const std::string &key = key_value_pair.first;
      const std::string &value = key_value_pair.second.get_value<std::string>();
      section_config[key] = value;
    }
    SectionInfo sectioninfo;
    sectioninfo.section_datas_ = section_config;

    config_map_[section_name] = sectioninfo;
  }

  // 打印信息
  for (const auto &section_entry : config_map_) {
    const std::string &section_name = section_entry.first;
    SectionInfo section_config = section_entry.second;
    std::cout << "[" << section_name << "]" << std::endl;
    for (const auto &key_value_pair : section_config.section_datas_) {
      std::cout << key_value_pair.first << "=" << key_value_pair.second
                << std::endl;
    }
    std::cout << std::endl;
  }
}
