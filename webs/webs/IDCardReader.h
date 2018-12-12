#pragma once

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ�ļ����ų�����ʹ�õ�����
// Windows ͷ�ļ�
#include <windows.h>

// C ����ʱͷ�ļ�
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
	static int isConnected; //�Ƿ����Ӻ���Ӳ��,��ʼ������
	//���������֤��Ϣ
	static int readIdCardData(unsigned char *data);

	static void string_replace(string &s1, const string s2, const string s3);
	
	static int connectHardware();

};

