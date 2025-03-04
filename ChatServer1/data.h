#pragma once
#include <string>

struct UserInfo {
  UserInfo()
      : name(""), pass(""), uid(0), email(""), nick(""), desc(""), sex(0),
        icon(""), back("") {}

  std::string name;
  std::string pass;
  int uid;
  std::string email;
  std::string nick;
  std::string desc;
  int sex;
  std::string icon;
  std::string back;
};

struct ApplyInfo {
  ApplyInfo(int uid, std::string name, std::string desc, std::string icon,
            std::string nick, int sex, int status)
      : uid_(uid), name_(name), desc_(desc), icon_(icon), nick_(nick),
        sex_(sex), status_(status) {}

  int uid_;
  std::string name_;
  std::string desc_;
  std::string icon_;
  std::string nick_;
  int sex_;
  int status_;
};
