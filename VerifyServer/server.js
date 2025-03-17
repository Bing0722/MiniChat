const grpc = require("@grpc/grpc-js");
const message_proto = require("./proto");
const const_module = require("./global");
const { v4: uuidv4 } = require("uuid");
const emailModule = require("./email");
const redis_module = require("./redis");

async function GetVerifyCode(call, callback) {
  console.log("email is ", call.request.email);
  try {
    let query_res = await redis_module.GetRedis(
      const_module.code_preifx + call.request.email,
    );

    console.log("query_res is ", query_res);

    if (query_res == null) {
    }

    let uniqueld = query_res;
    if (query_res == null) {
      uniqueld = uuidv4();
      if (uniqueld.length > 6) {
        uniqueld = uniqueld.substring(0, 6);
      }
      let bres = await redis_module.SetRedisExpire(
        const_module.code_preifx + call.request.email,
        uniqueld,
        600,
      );

      if (!bres) {
        callback(null, {
          email: call.request.email,
          error: const_module.Errors.REDIS_ERROR,
        });
        return;
      }
    }

    console.log("uniqueld is ", uniqueld);
    let text_str = "你的验证码为: " + uniqueld + "\n请在3分钟内完成注册";
    // 发送邮件
    let mailOptions = {
      from: "cbzerok24885@163.com",
      to: call.request.email,
      subject: "验证码",
      text: text_str,
    };

    let send_res = await emailModule.SendMail(mailOptions);
    console.log("send_res is ", send_res);

    callback(null, {
      email: call.request.email,
      error: const_module.Errors.SUCCESS,
    });
  } catch (error) {
    console.log("catch error is ", error);

    callback(null, {
      email: call.request.email,
      error: const_module.Errors.EXCEPTION,
    });
  }
}

function main() {
  var server = new grpc.Server();
  server.addService(message_proto.VerifyService.service, {
    GetVerifyCode: GetVerifyCode,
  });
  server.bindAsync(
    "0.0.0.0:50051",
    grpc.ServerCredentials.createInsecure(),
    () => {
      server.start();
      console.log("gprc server start");
    },
  );
}

main();
