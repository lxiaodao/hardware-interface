#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
using namespace std;

class IDCardReader
{
public:
	~IDCardReader();
	static int isConnected; //是否连接好了硬件,初始化连接
	//读二代身份证信息
	static int readIdCardData(unsigned char *data);

	static void string_replace(string &s1, const string s2, const string s3);
	
	static int connectHardware();

};

