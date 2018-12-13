
#include "stdafx.h"

#include <iostream>
#include <string>
#include <map>
#include "FileReader.h"
std::map<std::string, std::string> FileReader::configIni;

string FileReader::getvalue(string &key) {
	std::string value;
	string filepath = "config.ini";
	std::map<std::string, std::string> map = FileReader::loadFile(filepath);
	std::map<std::string, std::string>::const_iterator pos = map.find(key);
	if (pos == map.end()) {
		std::cout << key+"-------cant find value of the key-------\n";
	}
	else {
		value= pos->second;
		std::cout << value + "-------find the key and value-------\n";
	}
	return value;
}

unsigned int FileReader::getServerPort() {
	//crsf_url=http://localhost:3000
	//server_port = 9020
	string key = "server_port";
	string port = getvalue(key);	
	unsigned int need_port = atoi(port.c_str());
	std::cout << "-------���÷������˿�-------" << need_port <<"\n";
	return need_port;
}
string FileReader::Access_Control_Allow_Origin() {
	string key = "crsf_url";
	string crsf_allow= getvalue(key);
	std::cout << "-------���ÿ���������Զ��-------" << crsf_allow << "\n";
	return crsf_allow;

}


