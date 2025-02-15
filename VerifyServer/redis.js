const config_module = require("./config");
const Redis = require("ioredis");

// 创建redis客户端实例
const RedisCli = new Redis({
	host: config_module.redis_host,
	port: config_module.redis_port,
	password: config_module.redis_pass,
});

RedisCli.on("error", function (err) {
	console.log("RedisCli connect error: " + err);
	RedisCli.quit();
});

async function GetRedis(key) {
	try {
		const result = await RedisCli.get(key);
		if (result == null) {
			console.log("Result:", "<" + result + ">", "This key cannot be find...");
			return null;
		}
		consile.log("Result:", "<" + result + ">", "Get key success...");
		return result;
	} catch (err) {
		console.log("GetRedis error is ", err);
		return null;
	}
}

async function QueryRedis(key) {
	try {
		const result = await RedisCli.exists(key);
		if (result === 0) {
			console.log("Result:", "<" + result + ">", "This key is null...");
			return null;
		}
		consile.log("Result:", "<" + result + ">", "With this value!...");
		return result;
	} catch (err) {
		console.log("QueryRedis error is ", err);
		return null;
	}
}

async function SetRedisExpire(key, value, exptime) {
	try {
		await RedisCli.set(key, value);
		await RedisCli.expire(key, exptime);
		return true;
	} catch (err) {
		console.log("Set redis expire error is ", err);
	}
}

function Quit() {
	RedisCli.quit();
}

module.exports = { GetRedis, QueryRedis, SetRedisExpire, Quit };
