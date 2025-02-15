const nodeMailer = require("nodemailer");
const config_module = require("./config");

/**
 * 创建发送邮箱代理
 */
let transport = nodeMailer.createTransport({
	host: "smtp.163.com",
	port: 465,
	secure: true,
	auth: {
		user: config_module.email_user, // 发送邮箱的地址
		pass: config_module.email_pass, // 邮箱授权码或者秘密
	},
});

/**
 * 发送邮件数
 * @param {*} mailOptions_ 发送邮件的参数
 * @returns
 */
function SendMail(mailOptions_) {
	return new Promise(function (resolve, reject) {
		transport.sendMail(mailOptions_, function (error, info) {
			if (error) {
				console.log(error);
				reject(error);
			} else {
				console.log("邮件已发送成功: " + info.response);
				resolve(info.response);
			}
		});
	});
}

module.exports.SendMail = SendMail;
