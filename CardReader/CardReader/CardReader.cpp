// CardReader.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "mwrf32.h"
#include <string>
using namespace std;
/**
//调用操作流程//
 
函数调用应遵循如下规则：
（ 1 ）程序开始，首先要调用 rf_init( ) 函数打开读写器，建立读写器与 PC 之间的连接。
（ 2 ）用 rf_load_key()将卡中某一扇区密码输入到读写器中。如果使用直接密码认证，这步可以省
略。
（ 3 ）调用 rf_card() 函数或者连续调用 rf_request()、anticoll()、select()三个函数，成功可返回卡的序
列号， 并且卡片进入激活状态。
（ 4 ）如果使用装载密码认证并且已装载密码，对于需要密码验证的卡片调用 rf_authentication()
函数验证卡片的密码，一次只能验证一个扇区。
如果使用直接密码认证，则调用 rf_authentication_key()函数验证卡片的密码，用于验证的密
码由函数参数中给定。
（5）对已验证过的扇区可进行读、写、初始化值、加值、减值等功能操作。对其它扇区的读、
写操作必须重复上述（4）过程。
（6）由于高级函数集成了若干低级函数，所以调用前可不必运行（3）、（4）过程。  //神一样的描述，没有明确说明哪些是高级函数；所以为了保险起见还是逐一调用为佳。
（ 7 ）对某张卡操作完成后，应用 rf_halt()函数结束对该卡的操作。
（ 8 ）程序正常退出或因错误退出之前，要用 rf_exit() 函数关闭读写器，断开读写器与 PC 之间的
连接；
（ 9 ）有关调用各种函数库的具体方法，请参考 Examples目录下的使用范例。


*/


int read_content(HANDLE icdev, unsigned char adr,unsigned char *data) {

	//读取
	int flag;
	//unsigned char data[16];

	
	flag = rf_read(icdev, adr, data);  //读M1卡块0的数据
    if (flag != 0) {
		 std::cout << "Read error:" << flag << data << "\r\n";
									 return -1;
	}


	std::cout << "Read the data from block: " << int(adr) << " "<< data  << "\r\n";
	
	return 0;
}
int write_content(HANDLE icdev, unsigned char adr, unsigned char *data) {

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
int main()
{
    

	// 初始化
	//int icdev = (int)rf_init(1000, 1000);//115200
	HANDLE  icdev = rf_init(0, 9600);
	unsigned char ver[20];
	int status = rf_get_status(icdev, ver);
	if (status != 0) {
		std::cout << "Card Reader init failed!" << "\r\n";
		return -1;
	}
	else {
		std::cout << "------初始化成功Card Reader init SUCCESS------"<< ver<<"!\n" ;
	}

	rf_beep(icdev, (short)30);
	/*
	for (short i = 0; i<16; i++)
	{
		byte[] key = new byte[]{ (byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff };
		st = epen.rf_load_key(icdev, (short)0, i, key);
		if (st != 0)
		{
			System.out.println("加载 " + i + " 扇区密码失败!");
		}
	}
	*/
	unsigned char key[6] = { (byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff };
	for (short i = 1; i<2; i++) {
		
		int flag = rf_load_key(icdev, (short)0, i, key);
		if (flag != 0)
		{
			std::cout << "------------------------加载扇区密码失败-------------------------" << i << "\r\n" ;
		}
		else {
			std::cout << "------------------------OK，加载扇区密码成功-------------------------" << i << " "<<key<< "\r\n";
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
	unsigned long noofcard=0;
	st=rf_card(icdev, (short)0, &noofcard);  //IDLE模式操作，单卡操作
	if (st == 0) {
		std::cout << "------------------------寻卡成功-------------------------" << "\r\n";
	}
	//验证
	short sector = 1; //扇区1
	st = rf_authentication(icdev, (short)0, sector);
	if (st != 0)
	{
		
		std::cout << "------------------------扇区密码验证 错误!-------------------------" <<sector << "\r\n";
	}
	else {
		std::cout << "------------------------OK,扇区密码验证成功!-------------------------" << sector << "\r\n";
	}

	

	// 增加读写卡片内容的部分
	
	//酒店id保存,cardno保存，减少数据的写入
	//返回数据格式"{\"hotelNo\":\"H201810000012\",\"cardNo\":\"20181000000001\"}"
	//unsigned char data1[] = "{\"name\":\"杨天林\",\"hotelNo\":\"8976\",\"memberNo\":\"22\",\"cardNo\":\"21\"}";
	//write_content(icdev, (short)(sector * 4),data1);
	string arr[] = {"H201810000012","20181000000001"};
	/*
	for (int i = 4; i < 6; i++) {
		char hotelno[16];
		strcpy_s(hotelno,  arr[i-4].c_str());
		unsigned char *conbuf = (unsigned char *)hotelno;
		write_content(icdev, (short)i, conbuf);
	}
	*/

	

	for (int i = 4; i < 6; i++) {
		unsigned char data[16];
		read_content(icdev, (short)i, data);
	}
    
	
	
	Sleep(1000); //停留1秒 

	rf_halt(icdev); //结束卡的操作
	std::cout << "------------------------结束卡的操作-------------------------" << icdev << "\r\n";


	// 退出
	st = rf_exit((HANDLE)icdev);
	std::cout << "Exit，result: " << st << "." << std::endl;
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
