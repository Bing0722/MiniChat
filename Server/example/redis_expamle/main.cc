#include <cstring>
#include <hiredis/hiredis.h>
#include <hiredis/read.h>
#include <string>

int main() {
  // 连接到 127.0.0.1:6380 的redis服务器
  redisContext *c = redisConnect("127.0.0.1", 6380);
  if (c->err) {
    printf("Connection to redisServer faile:%s\n", c->errstr);
    redisFree(c);
    return -1;
  }
  printf("Connection to redisServer Success\n");

  // redis 认证
  std::string redis_password = "123456";
  redisReply *r =
      (redisReply *)redisCommand(c, "AUTH %s", redis_password.c_str());

  if (r->type == REDIS_REPLY_ERROR) {
    printf("Redis认证失败!\n");
  } else {
    printf("Redis认证成功!\n");
  }

  // set
  const char *command1 = "set stest1 value1";

  // 执行set命令
  r = (redisReply *)redisCommand(c, command1);

  if (NULL == r) {
    printf("Execut command1 failure\n");
    redisFree(c);
    return -1;
  }

  if (!(r->type == REDIS_REPLY_STATUS &&
        (strcmp(r->str, "OK") == 0 || strcmp(r->str, "ok") == 0))) {
    printf("Failed to execute command[%s]\n", command1);
    freeReplyObject(r);
    redisFree(c);
    return -1;
  }

  freeReplyObject(r);
  printf("Succeed to execute command[%s]\n", command1);

  // strlen
  const char *command2 = "strlen stest1";
  r = (redisReply *)redisCommand(c, command2);

  if (r->type != REDIS_REPLY_INTEGER) {
    printf("Failed to execute command[%s]\n", command2);
    freeReplyObject(r);
    redisFree(c);
    return -1;
  }

  int length = r->integer;
  freeReplyObject(r);
  printf("The length of 'stest1' is %d.\n", length);
  printf("Succeed to execute command[%s]\n", command2);

  // get
  const char *command3 = "get stest1";
  r = (redisReply *)redisCommand(c, command3);
  if (r->type != REDIS_REPLY_STRING) {
    printf("Failed to execute command[%s]\n", command3);
    freeReplyObject(r);
    redisFree(c);
    return -1;
  }

  printf("The value of 'stest1' is %s\n", r->str);
  freeReplyObject(r);
  printf("Succeed to execute command[%s]\n", command3);

  // get
  const char *command4 = "get stest2";
  r = (redisReply *)redisCommand(c, command4);
  if (r->type != REDIS_REPLY_NIL) {
    printf("Failed to execute command[%s]\n", command4);
    freeReplyObject(r);
    redisFree(c);
    return -1;
  }
  freeReplyObject(r);
  printf("Succeed to execute command[%s]\n", command4);

  // del
  const char *command5 = "del stest1";
  r = (redisReply *)redisCommand(c, command5);
  if (r->type != REDIS_REPLY_INTEGER) {
    printf("Failed to execute command[%s]\n", command4);
    freeReplyObject(r);
    redisFree(c);
    return -1;
  }
  freeReplyObject(r);
  printf("Succeed to execute command[%s]\n", command5);

  redisFree(c);
}
