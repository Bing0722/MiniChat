#pragma once

#include "global.h"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

class LogicSystem;

class MsgNode {
public:
  MsgNode(short max_len);
  ~MsgNode();
  void Clear();

  char *data_;

  short cur_len_;
  short total_len_;
};

class RecvNode : public MsgNode {
  friend class LogicSystem;

public:
  RecvNode(short max_len, short msg_id);

private:
  short msg_id_;
};

class SendNode : public MsgNode {
  friend class LogicSystem;

public:
  SendNode(const char *msg, short max_len, short mas_id);

private:
  short msg_id_;
};