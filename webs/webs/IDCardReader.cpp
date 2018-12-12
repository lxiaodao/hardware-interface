#include "stdafx.h"
#include "IDCardReader.h"
#include "Sdtapi.h"
#include <iostream>
#include <string>
using namespace std;

int IDCardReader::isConnected = 0;//0,表示未初始化；1，表示初始化，连接了读卡器硬件。
int IDCardReader::connectHardware()
{
	if (IDCardReader::isConnected == 1) {
		return 0;
	}
	// set default port 1001
	// connect the com interface
	int ret = InitComm(1001);
	if (!ret) {
		std::cout << "Init communication failed, port: 1001"  << "\r\n";
		return -1;
	}
	else {
		std::cout << "Init communication success, port: 1001" << "\r\n";
	}
	IDCardReader::isConnected = 1;//已经连接
	return 0;
}
IDCardReader::~IDCardReader()
{

	// close communication
	int ret = CloseComm();
	std::cout << "================关闭身份证读卡器连接===================\r\n";
}
int IDCardReader::readIdCardData(unsigned char * data)
{
	if (IDCardReader::connectHardware()==-1) {
		//data = (unsigned char *)"连接身份证读卡器失败";
		string authfail = "连接身份证读卡器失败400!";
		strcpy((char*)data, authfail.c_str());
		return -1;
	}
	int ret;
	

		std::cout << "================等待读取身份证卡片===================\r\n";
		
		ret = Authenticate();
		if (!ret) {
			std::cout << "Authenticate ID card failed:"<<ret<<"\r\n";
			string authfail="验证身份证失败!";
			strcpy((char*)data, authfail.c_str());
			return -1;
			
		}

		// read card info
		char name[32] = { 0 };
		char gender[4] = { 0 };
		char folk[16] = { 0 };
		char birthDay[16] = { 0 };
		char code[32] = { 0 };
		char address[96] = { 0 };
		char agency[32] = { 0 };
		char expireStart[16] = { 0 };
		char expireEnd[16] = { 0 };

		// read card info and photo
		ret = ReadBaseInfos(name, gender, folk, birthDay, code, address, agency, expireStart, expireEnd);
		if (ret <= 0) {
			std::cout << "Read ID card info failed, error: " << ret << "\r\n";
			data = (unsigned char *)"读取身份证基础信息数据失败!";
			return -1;
			
		}

		string content_json = "{\"name\":\":name\",\"gender\":\":gender\",\"folk\":\":folk\",\"birthDay\":\":birthDay\",\"code\":\":code\",\"address\":\":address\",\"agency\":\":agency\",\"expireStart\":\":expireStart\",\"expireEnd\":\":expireEnd\"}";

	
	    //ret = fprintf_s(stream, "{\"name\":\"%s\",\"gender\":\"%s\",\"folk\":\"%s\",\"birthDay\":\"%s\",\"code\":\"%s\",\"address\":\"%s\",\"agency\":\"%s\",\"expireStart\":\"%s\",\"expireEnd\":\"%s\"}", name, gender, folk, birthDay, code, address, agency, expireStart, expireEnd);
		
		IDCardReader::string_replace(content_json, ":name", name);
		IDCardReader::string_replace(content_json, ":gender", gender);
		IDCardReader::string_replace(content_json, ":folk", folk);
		IDCardReader::string_replace(content_json, ":birthDay", birthDay);
		IDCardReader::string_replace(content_json, ":code", code);
		IDCardReader::string_replace(content_json, ":address", address);
		IDCardReader::string_replace(content_json, ":agency", agency);
		IDCardReader::string_replace(content_json, ":expireStart", expireStart);
		IDCardReader::string_replace(content_json, ":expireEnd", expireEnd);

		strcpy((char*)data,content_json.c_str());
	
		


	
	return ret;
}


void IDCardReader::string_replace(string &s1, const string s2, const string s3)
{
	string::size_type pos = 0;
	string::size_type a = s2.size();
	string::size_type b = s3.size();
	while ((pos = s1.find(s2, pos)) != string::npos)
	{
		s1.replace(pos, a, s3);
		pos += b;
	}
}



