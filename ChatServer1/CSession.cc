#include "CSession.h"
#include "CServer.h"
#include "LogicSystem.h"

CSession::CSession(boost::asio::io_context &ioc, CServer *server)
    : socket_(ioc), server_(server), stop_(false), b_head_parse_(false),
      user_uid_(0) {

  boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
  session_id_ = boost::uuids::to_string(a_uuid);
  recv_head_node_ = std::make_shared<MsgNode>(HEAD_TOTAL_LEN);
}

CSession::~CSession() { std::cout << "~CSession destruct.." << std::endl; }

tcp::socket &CSession::GetSocket() { return socket_; }

std::string &CSession::GetSessionID() { return session_id_; }

void CSession::SetUserID(int uid) { user_uid_ = uid; }

int CSession::GetUserID() { return user_uid_; }

void CSession::Start() { AsyncReadHead(HEAD_TOTAL_LEN); }

void CSession::Send(std::string msg, short msgid) {
  std::lock_guard<std::mutex> lock(send_lock_);
  int send_que_size = send_que_.size();
  if (send_que_size > MAX_SENDQUE) {
    std::cout << "session: " << session_id_ << " send que fulled, size is "
              << MAX_SENDQUE << std::endl;
    return;
  }

  send_que_.push(std::make_shared<SendNode>(msg.c_str(), msg.length(), msgid));
  if (send_que_size > 0) {
    return;
  }

  auto &msgnode = send_que_.front();
  boost::asio::async_write(
      socket_, boost::asio::buffer(msgnode->data_, msgnode->total_len_),
      std::bind(&CSession::HandleWrite, this, std::placeholders::_1,
                SharedSelf()));
}

void CSession::Send(char *msg, short max_len, short msgid) {
  std::lock_guard<std::mutex> lock(send_lock_);
  int send_que_size = send_que_.size();
  if (send_que_size > MAX_SENDQUE) {
    std::cout << "session: " << session_id_ << " send que fulled, size is "
              << MAX_SENDQUE << std::endl;
    return;
  }

  send_que_.push(std::make_shared<SendNode>(msg, max_len, msgid));
  if (send_que_size > 0) {
    return;
  }

  auto &msgnode = send_que_.front();
  boost::asio::async_write(
      socket_, boost::asio::buffer(msgnode->data_, msgnode->total_len_),
      std::bind(&CSession::HandleWrite, this, std::placeholders::_1,
                SharedSelf()));
}

void CSession::Close() {
  socket_.close();
  stop_ = true;
}

std::shared_ptr<CSession> CSession::SharedSelf() { return shared_from_this(); }

void CSession::AsyncReadHead(int total_len) {
  auto self = shared_from_this();

  AsyncReadFull(HEAD_TOTAL_LEN, [self,
                                 this](const boost::system::error_code &ec,
                                       std::size_t bytes_transfered) {
    try {
      if (ec) {
        std::cout << "handle read failed, error is " << ec.what() << std::endl;
        Close();
        server_->ClearSession(session_id_);
        return;
      }

      if (bytes_transfered < HEAD_TOTAL_LEN) {
        std::cout << "read length not match, read [" << bytes_transfered
                  << "] ,tatal [" << HEAD_TOTAL_LEN << "]" << std::endl;

        Close();
        server_->ClearSession(session_id_);
        return;
      }

      recv_head_node_->Clear();
      memcpy(recv_head_node_->data_, data_, bytes_transfered);

      // 获取头部MSGID数据
      short msg_id = 0;
      memcpy(&msg_id, recv_head_node_->data_, HEAD_ID_LEN);
      // 网络字节序转化为本地字节序
      msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);
      std::cout << "msg_id is " << msg_id << std::endl;

      // 消息 id非法
      if (msg_id > MAX_LENGTH) {
        std::cout << "Invalid msg_id is " << msg_id << std::endl;
        server_->ClearSession(session_id_);
        return;
      }

      short msg_len = 0;
      memcpy(&msg_len, recv_head_node_->data_ + HEAD_ID_LEN, HEAD_DATA_LEN);

      msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);
      std::cout << "msg_len is " << msg_len << std::endl;

      // 消息长度 非法
      if (msg_len > MAX_LENGTH) {
        std::cout << "Invalid data length is " << std::endl;
        server_->ClearSession(session_id_);
        return;
      }

      recv_msg_node_ = std::make_shared<RecvNode>(msg_len, msg_id);
      AsyncReadBody(msg_len);

    } catch (const std::exception &e) {
      std::cerr << "Exception code is " << e.what() << std::endl;
    }
  });
}

// 读取完整长度
void CSession::AsyncReadFull(
    std::size_t max_len,
    std::function<void(const boost::system::error_code &, std::size_t)>
        handler) {

  ::memset(data_, 0, MAX_LENGTH);
  AsyncReadLen(0, max_len, handler);
}

void CSession::HandleWrite(const boost::system::error_code &error,
                           std::shared_ptr<CSession> shared_self) {
  try {
    if (!error) {
      std::lock_guard<std::mutex> lock(send_lock_);
      send_que_.pop();
      if (!send_que_.empty()) {
        auto &msgnode = send_que_.front();
        boost::asio::async_write(
            socket_, boost::asio::buffer(msgnode->data_, msgnode->total_len_),
            std::bind(&CSession::HandleWrite, this, std::placeholders::_1,
                      shared_self));
      }
    } else {
      std::cerr << "hanle write failed, error is " << error.what() << std::endl;
      Close();
      server_->ClearSession(session_id_);
    }
  } catch (const std::exception &e) {
    std::cerr << "Exception code: " << e.what() << std::endl;
  }
}

void CSession::AsyncReadLen(
    std::size_t read_len, std::size_t total_len,
    std::function<void(const boost::system::error_code &, std::size_t)>
        handler) {

  auto self = shared_from_this();
  socket_.async_read_some(
      boost::asio::buffer(data_ + read_len, total_len - read_len),
      [read_len, total_len, handler, self](const boost::system::error_code &ec,
                                           std::size_t bytes_transfered) {
        if (ec) {
          // 出现错误 调用回调函数
          handler(ec, read_len + bytes_transfered);
          return;
        }

        if (read_len + bytes_transfered >= total_len) {
          // 长度够了就调用回调
          handler(ec, read_len + bytes_transfered);
          return;
        }

        // 没有错误 且长度不足 则继续读取
        self->AsyncReadLen(read_len + bytes_transfered, total_len, handler);
      });
}

void CSession::AsyncReadBody(int total_len) {
  auto self = shared_from_this();
  AsyncReadFull(total_len, [self, this,
                            total_len](const boost::system::error_code &ec,
                                       std::size_t bytes_transfered) {
    try {
      if (ec) {
        std::cerr << "handle read failed, error is " << ec.what() << std::endl;
        Close();
        server_->ClearSession(session_id_);
        return;
      }
      if (bytes_transfered < total_len) {
        std::cerr << "read length not match, read [" << bytes_transfered
                  << "], total [" << total_len << "]" << std::endl;
        Close();
        server_->ClearSession(session_id_);
        return;
      }

      memcpy(recv_msg_node_->data_, data_, bytes_transfered);
      recv_msg_node_->cur_len_ += bytes_transfered;
      recv_msg_node_->data_[recv_msg_node_->total_len_] = '\0';
      std::cout << "receive data is " << recv_msg_node_->data_ << std::endl;
      // 从此处将消息投递到逻辑队列中
      LogicSystem::GetInstance()->PostMsgToQue(
          std::make_shared<LogicNode>(shared_from_this(), recv_msg_node_));
      // 继续监听头部接收事件
      AsyncReadHead(HEAD_TOTAL_LEN);

    } catch (const std::exception &e) {
      std::cout << "Exception code is " << e.what() << std::endl;
    }
  });
}

LogicNode::LogicNode(std::shared_ptr<CSession> session,
                     std::shared_ptr<RecvNode> recvnode)
    : session_(session), recvnode_(recvnode) {}