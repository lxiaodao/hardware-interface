#pragma once
#include "mwrf32.h"

class MemberCardReader
{

public:
	static HANDLE icdev;
	static int isConnected; //�Ƿ����Ӻ���Ӳ��
	static int excuteReadWriteAction(unsigned char *data,int flag);//0,read;1,write
	static int read_content(HANDLE icdev, unsigned char adr, unsigned char *data);
	static int write_content(HANDLE icdev, unsigned char adr, unsigned char *data);
private:
	static int connectHardware();
	

};

