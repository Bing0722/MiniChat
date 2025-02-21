#include <iostream>
#include <mysqlx/xdevapi.h>

using namespace ::mysqlx;

int main(int argc, char *argv[]) {
  const std::string host = "mysql://127.0.0.1";
  unsigned short port = 3308;
  const std::string user = "root";
  const std::string pass = "123456";
  const std::string db = "test";

  SessionSettings opts(host, port, user, pass, db);
  Session session(opts);
  {
    RowResult res = session.sql("show variables like 'version'").execute();
    std::stringstream version;

    version << res.fetchOne().get(1).get<string>();
    int major_version;
    version >> major_version;

    if (major_version < 8) {
      std::cout << "Can work only with MySQL Server 8 or later" << std::endl;
      std::cout << "Done!" << std::endl;
      return 0;
    }
  }
  return 0;
}