#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <map>


using namespace std;
class FileReader {


public:

	static string getvalue(string &key);
	static unsigned int getServerPort();
	static string Access_Control_Allow_Origin();
	static std::map<std::string, std::string> configIni;
private:
	

	static std::map<std::string, std::string> loadFile(string &filepath) {
		if (!configIni.empty()) {
			return configIni;//已经初始化
		}
		
		std::ifstream input(filepath);
		std::string line;

		int number_of_line = 0;
		while (std::getline(input, line))
		{
			//分割字符串=
			string key;
			string value;
			std::size_t pos = line.find_first_of("=");
			key = line.substr(0, pos);
			value = line.substr(pos + 1);
			std::cout << "-------substring-------" << key + " and " + value + "\r\n";
			number_of_line++;
			configIni.insert(std::map<std::string, std::string>::value_type(key, value));
		}

		input.close();

		return configIni;

	};
};
