#include "stdafx.h"

#include <windows.h>
#include "MemberCardReader.h"
#include <iostream>
#include "mwrf32.h"
#include <string>
using namespace std;

int MemberCardReader::isConnected = 0;//0，没有；1，ok
HANDLE MemberCardReader::icdev = 0;
int MemberCardReader::connectHardware()
{
	// 初始化
	//int icdev = (int)rf_init(1000, 1000);//115200
	if (MemberCardReader::isConnected == 1) {
		return 0;
	}
	icdev = rf_init(0, 9600);
	unsigned char ver[20];
	int status = rf_get_status(icdev, ver);
	if (status != 0) {
		std::cout << "初始化连接读卡器硬件失败Card Reader init failed!" << "\r\n";
		return -1;
	}
	else {
		std::cout << "------初始化连接读卡器硬件成功Card Reader init SUCCESS------" << ver << "!\n";
	}

	rf_beep(icdev, (short)10);
	MemberCardReader::isConnected = 1;//初始化
	return 0;
}

int MemberCardReader::excuteReadWriteAction(unsigned char *needwrite,int flag )
{
	
	
    MemberCardReader::connectHardware();
	
	unsigned char key[6] = { (byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff };
	for (short i = 1; i<2; i++) {

		int flag = rf_load_key(icdev, (short)0, i, key);
		if (flag != 0)
		{
			std::cout << "------------------------加载扇区密码失败-------------------------" << i << "\r\n";
		}
		else {
			std::cout << "------------------------OK，加载扇区密码成功-------------------------" << i << " " << key << "\r\n";
		}
	}

	//寻卡

	int st;
	/**
	注 意：选用 IDLE
	模式寻卡时，完成对卡片的操作后调用 rf_halt 函数来停止操作 ， 此 后读写器不能找到卡片，除非
	卡片离开操作区域并再次重新进入。
	选用 ALL 模式寻卡时，完成对卡片的操作后调用 rf_halt 函数来停止操作 ， 此后读写器仍能找
	到该卡片，无须离开操作区域并再次重新进入
	*/
	unsigned long noofcard = 0;
	st = rf_card(icdev, (short)0, &noofcard);  //IDLE模式操作，单卡操作
	if (st == 0) {
		std::cout << "------------------------寻卡成功-------------------------" << "\r\n";
	}
	//验证
	short sector = 1; //扇区1
	st = rf_authentication(icdev, (short)0, sector);
	if (st != 0)
	{

		std::cout << "------------------------扇区密码验证 错误!-------------------------" << sector << "\r\n";
	}
	else {
		std::cout << "------------------------OK,扇区密码验证成功!-------------------------" << sector << "\r\n";
	}



	// 增加读写卡片内容的部分

	//减少操作卡的时间，只保存两种数据。酒店id保存,cardno保存，减少数据的写入
	//返回数据格式"{\"hotelNo\":\"H201810000012\",\"cardNo\":\"20181000000001\"}"
	
	if (flag == 1) {
		//写操作
		for (int i = 4; i < 6; i++) {
			char hotelno[16];
			//strcpy_s(hotelno, needwrite[i - 4].c_str());
			//unsigned char *conbuf = (unsigned char *)hotelno;
			//TODO: 字符串和数组的转换
			write_content(icdev, (short)i, needwrite);
		}
	}else{
		//读操作
		std::cout << "------------------------读数据操作-------------------------" << icdev << "\r\n";
		for (int i = 4; i < 6; i++) {
			unsigned char data[16];
			read_content(icdev, (short)i, data);
			
			strcat((char *)needwrite, (char *)data);

		}
	}
	

	Sleep(100); //停留100豪秒 

	rf_halt(icdev); //结束卡的操作
	std::cout << "------------------------结束卡的操作-------------------------" << icdev << "\r\n";


	// 端口读卡器硬件和PC的链接
	//st = rf_exit((HANDLE)icdev);
	
	return 0;
}

int MemberCardReader::read_content(HANDLE icdev, unsigned char adr, unsigned char * data)
{
	//读取
	int flag;
	//unsigned char data[16];
	flag = rf_read(icdev, adr, data);  //读M1卡块0的数据
	if (flag != 0) {
		std::cout << "Read error:" << flag << data << "\r\n";
		return -1;
	}
	std::cout << "Read the data from block: " << int(adr) << " " << data << "\r\n";

	return 0;
}

int MemberCardReader::write_content(HANDLE icdev, unsigned char adr, unsigned char * data)
{
	//写入
	int flag;
	flag = rf_write(icdev, adr, data);
	if (flag != 0) {
		std::cout << "Card write error:" << flag << data << "\r\n";
		return -1;
	}
	std::cout << "写入成功,Write the data into block: " << int(adr) << "\r\n";

	return 0;
}
