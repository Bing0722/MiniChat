#pragma once

#include "Singleton.h"
#include "WIN32/message.grpc.pb.h"
#include "WIN32/message.pb.h"
#include "data.h"
#include "global.h"
#include <grpcpp/grpcpp.h>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <queue>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using message::GetChatServerRsp;
using message::LoginReq;
using message::LoginRsp;

class ChatConPool {
public:
private:
};

class ChatGrpcClient : public Singleton<ChatGrpcClient> {
  friend class Singleton<ChatGrpcClient>;

public:
private:
};