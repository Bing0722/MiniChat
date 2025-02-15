#include "../../RedisMgr.h"
#include <cassert>

int main() {
  // assert(RedisMgr::GetInstance()->Connect("127.0.0.1", 6380));
  // assert(RedisMgr::GetInstance()->Auth("123456"));
  assert(RedisMgr::GetInstance()->Set("Bing", "C++"));
  std::string value = "";
  assert(RedisMgr::GetInstance()->Get("Bing", value));
  assert(RedisMgr::GetInstance()->Get("Monekey", value) == false);
  assert(RedisMgr::GetInstance()->HSet("web", "C++", "C"));
  assert(RedisMgr::GetInstance()->HGet("web", "C++") != "");
  assert(RedisMgr::GetInstance()->ExistsKey("web"));
  assert(RedisMgr::GetInstance()->Del("web"));
  assert(RedisMgr::GetInstance()->Del("web"));
  assert(RedisMgr::GetInstance()->ExistsKey("web") == false);
  assert(RedisMgr::GetInstance()->LPush("lpk1", "lpv1"));
  assert(RedisMgr::GetInstance()->LPush("lpk1", "lpv2"));

  assert(RedisMgr::GetInstance()->LPush("lpk1", "lpv3"));
  assert(RedisMgr::GetInstance()->RPop("lpk1", value));
  assert(RedisMgr::GetInstance()->RPop("lpk1", value));
  assert(RedisMgr::GetInstance()->LPop("lpk1", value));
  assert(RedisMgr::GetInstance()->LPop("lpk2", value) == false);
  RedisMgr::GetInstance()->Close();

  return 0;
}
