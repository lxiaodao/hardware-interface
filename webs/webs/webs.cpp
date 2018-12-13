// webs.cpp : Defines the entry point for the console application.
// 需要支持中文

#include "stdafx.h"
#include "webserver.h"
#include "Socket.h"
#include <iostream>
#include "MemberCardReader.h"

#include <string>
#include "mwrf32.h"
#include "IDCardReader.h"
#include "FileReader.h"
using namespace std;


void Json_Request_Handler(webserver::http_request* r) {
	Socket s = *(r->s_);

	std::string title;
	std::string body;
	std::string bgcolor = "#ffffff";
	std::string links ="";

	//std::map<std::string, std::string> req_params = r->params_;
	

	if (r->path_ == "/") {
		title = "Web Server Example";
		body = "<h1>Welcome to Rene's Web Server</h1>"
			"I wonder what you're going to click" + links;
	}
	else if (r->path_ == "/writeMCard") {
		//hotelNo=H201810000012&memberNo=20181000000001&cardNo=30089000
		string hotelNo="";
		string memberNo = "";
		string cardNo = "";

		for (std::map<std::string, std::string>::const_iterator i = r->params_.begin();
			i != r->params_.end();
			i++) {

			//读取请求参数
			//TODO:赋值
			string name=i->first;
			if (name == "hotelNo") {
				hotelNo = i->second;
			}
			else if (name == "memberNo") {
				memberNo = i->second;
			}
			else if (name == "cardNo") {
				cardNo = i->second;
			}

		}
		boolean isAllInvalidParam = hotelNo == ""&&memberNo == ""&&cardNo == "";
		if (!isAllInvalidParam) {
			//参数都为空，非法			
			unsigned char data[120] = "";
			string dataArray[] = { hotelNo, memberNo,cardNo };
			int ret = MemberCardReader::excuteWriteAction(dataArray, data);
			string backjson_or_msg = string((char*)data);
			//返回格式{"hotelNo":"H201810000012","memberNo":"20181000000001","cardNo":"30089000"}
			body += backjson_or_msg;
		}
		else {
			body += "请求参数非法！";
		}
		
		

	}
	else if (r->path_ == "/readMCard") {
		
		unsigned char data[120] = "";
		MemberCardReader::excuteReadWriteAction(data, 0);//读操作
		
		string backjson_or_msg = string((char*)data);
		//返回格式{"hotelNo":"H201810000012","memberNo":"20181000000001","cardNo":"30089000"}
		body += backjson_or_msg;
		
	}	
	else if (r->path_ == "/auth") {
		if (r->authentication_given_) {
			if (r->username_ == "rene" && r->password_ == "secretGarden") {
				body = "<h1>Successfully authenticated</h1>" + links;
			}
			else {
				body = "<h1>Wrong username or password</h1>" + links;
				r->auth_realm_ = "Private Stuff";
			}
		}
		else {
			r->auth_realm_ = "Private Stuff";
		}
	}
	else if (r->path_ == "/readIDCard") {
		unsigned char json_data[1000] = "";
		IDCardReader::readIdCardData(json_data);
		string result = string((char*)json_data);
		body += result;
		
	}
	else {
		r->status_ = "404 Not Found";
		title = "Wrong URL";
		body = "<h1>Wrong URL</h1>";
		body += "Path is : &gt;" + r->path_ + "&lt;";
	}

	
	r->answer_ += body;
	
}

int main() {
	unsigned int port_ = FileReader::getServerPort();//add by ly 2018-12-13
	webserver(port_, Json_Request_Handler);
}


