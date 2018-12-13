#include "stdafx.h"

#include <windows.h>
#include "MemberCardReader.h"
#include <iostream>
#include "mwrf32.h"
#include <string>
#include "StringUtil.h"
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

int MemberCardReader::excuteWriteAction(string * array, unsigned char * data)
{   
	if (MemberCardReader::connectHardware() != 0) {

		strcat((char *)data, "初始化连接读卡器硬件失败");
		return -1;
	}

	unsigned char key[6] = { (byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff };
	for (short i = 1; i<2; i++) {

		int flag = rf_load_key(icdev, (short)0, i, key);
		if (flag != 0)
		{
			std::cout << "------------------------加载扇区密码失败-------------------------" << i << "\r\n";
			
			strcat((char *)data, "加载扇区1密码失败!");
			return -1;
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
	else {
		std::cout << "------------------------IDLE模式寻卡失败!-------------------------" << "\r\n";
		strcat((char *)data, "IDLE模式寻卡失败!");
		return -1;
	}
	//验证
	short sector = 1; //扇区1
	st = rf_authentication(icdev, (short)0, sector);
	if (st != 0)
	{

		std::cout << "------------------------扇区1密码验证错误!-------------------------" << sector << "\r\n";
		strcat((char *)data, "扇区1密码验证错误!");
		return -1;
	}
	else {
		std::cout << "------------------------OK,扇区密码验证成功!-------------------------" << sector << "\r\n";
	}
	//写操作
	//写第4，5，6块数据
	//boolean flag_array[3] = {true,true,true};
	//string msg[3];
	for (int i = 4; i < 7; i++) {
	
		
		unsigned char *conbuf = (unsigned char *)array[i - 4].c_str();
		
		int ret=write_content(icdev, (short)i, conbuf);
		if (ret != 0) {
			//要么全成功，要么全失败。写入失败
			
			strcpy((char*)data, (char*)conbuf);
			return -1;
		}
	}
	
	string backjson = "{\"hotelNo\":\":hotelNo\",\"memberNo\":\":memberNo\",\"cardNo\":\":cardNo\"}";
	StringUtil::string_replaceAll(backjson,":hotelNo", array[0]);
	StringUtil::string_replaceAll(backjson, ":memberNo", array[1]);
	StringUtil::string_replaceAll(backjson, ":cardNo", array[2]);

	strcpy((char*)data, backjson.c_str());

	rf_beep(icdev, (short)10);//鸣笛，写入成功

	Sleep(100); //停留100豪秒 

	rf_halt(icdev); //结束卡的操作
	std::cout << "------------------------结束卡的操作-------------------------" << icdev << "\r\n";

	return 0;
}

int MemberCardReader::excuteReadWriteAction(unsigned char *needwrite,int flag )
{
	
	
	if (MemberCardReader::connectHardware() != 0) {

		strcat((char *)needwrite, "初始化连接读卡器硬件失败");
		return -1;
	}
	
	unsigned char key[6] = { (byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff };
	for (short i = 1; i<2; i++) {

		int flag = rf_load_key(icdev, (short)0, i, key);
		if (flag != 0)
		{
			std::cout << "------------------------加载扇区密码失败-------------------------" << i << "\r\n";
			strcat((char *)needwrite, "加载扇区1密码失败!");
			return -1;
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
	else {
		std::cout << "------------------------IDLE模式寻卡失败!-------------------------" << "\r\n";
		strcat((char *)needwrite, "IDLE模式寻卡失败!");
		return -1;
	}
	//验证
	short sector = 1; //扇区1
	st = rf_authentication(icdev, (short)0, sector);
	if (st != 0)
	{

		std::cout << "------------------------扇区密码验证错误!-------------------------" << sector << "\r\n";
		strcat((char *)needwrite, "扇区1密码验证错误!");
		return -1;
	}
	else {
		std::cout << "------------------------OK,扇区密码验证成功!-------------------------" << sector << "\r\n";
	}



	// 增加读写卡片内容的部分

	//减少操作卡的时间，只保存三种数据。酒店id保存,memberNo保存，cardno保存，减少数据的写入
	//返回数据格式
	string backjson = "{\"hotelNo\":\":hotelNo\",\"memberNo\":\":memberNo\",\"cardNo\":\":cardNo\"}";
	
	
	
	//读操作，读取第4，5，6块数据
	std::cout << "------------------------读数据操作-------------------------" << icdev << "\r\n";
	unsigned char data1[50];
	int ret=read_content(icdev, 4, data1);
	if (ret != 0) {
		strcat((char *)needwrite, (char *)data1);
		return -1;
	}

	StringUtil::string_replaceAll(backjson, ":hotelNo", (char *)data1);

	unsigned char data2[50];
	ret = read_content(icdev, 5, data2);
	if (ret != 0) {
		strcat((char *)needwrite, (char *)data2);
		return -1;
	}
	StringUtil::string_replaceAll(backjson, ":memberNo", (char *)data2);

	unsigned char data3[50];
	ret = read_content(icdev, 6, data3);
	if (ret != 0) {
		strcat((char *)needwrite, (char *)data3);
		return -1;
	}
	StringUtil::string_replaceAll(backjson, ":cardNo", (char *)data3);

	strcpy((char*)needwrite, backjson.c_str());//返回的

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
		strcat((char *)data, adr + "数据块读取失败" + flag);
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
		strcat((char *)data, adr+"数据块写入失败"+flag);
		return -1;
	}
	std::cout << "写入成功,Write the data into block: " << int(adr) << "\r\n";

	return 0;
}
