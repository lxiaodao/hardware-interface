#pragma once
#include "mwrf32.h"
#include "string"
using namespace std;
class MemberCardReader
{

public:
	static HANDLE icdev;
	static int isConnected; //是否连接好了硬件
	static int excuteWriteAction(string *array, unsigned char *data);//第二个参数data可以是返回的错误信息
	static int excuteReadWriteAction(unsigned char *data,int flag);//0,read;1,write
	static int read_content(HANDLE icdev, unsigned char adr, unsigned char *data);
	static int write_content(HANDLE icdev, unsigned char adr, unsigned char *data);
private:
	static int connectHardware();
	

};

