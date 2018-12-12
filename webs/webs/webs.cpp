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

		for (std::map<std::string, std::string>::const_iterator i = r->params_.begin();
			i != r->params_.end();
			i++) {

			//读取请求参数
			//TODO:赋值

		}
		//string arr[] = { "H201810000012","20181000000001" };
		unsigned char data[32] = "";
		int ret=MemberCardReader::excuteReadWriteAction(data, 1);
	
		//body += "<br>" + i->first + " = " + i->second;
		string str = string((char*)data);
		//TODO:返回格式{"hotelNo":"H201810000012","memberNo":"20181000000001"}
		body += "contentis:" + str;

	}
	else if (r->path_ == "/readMCard") {
		
		
		//string arr[] = { "H201810000012","20181000000001" };
		unsigned char data[32] = "";
		MemberCardReader::excuteReadWriteAction(data, 0);
		
		string str = string((char*)data);
		//TODO:返回格式{"hotelNo":"H201810000012","memberNo":"20181000000001"}
		body += "contentis:" + str;
		
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
	webserver(9020, Json_Request_Handler);
}


