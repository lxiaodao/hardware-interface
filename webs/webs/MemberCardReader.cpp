#include "stdafx.h"

#include <windows.h>
#include "MemberCardReader.h"
#include <iostream>
#include "mwrf32.h"
#include <string>
using namespace std;

int MemberCardReader::isConnected = 0;//0��û�У�1��ok
HANDLE MemberCardReader::icdev = 0;
int MemberCardReader::connectHardware()
{
	// ��ʼ��
	//int icdev = (int)rf_init(1000, 1000);//115200
	if (MemberCardReader::isConnected == 1) {
		return 0;
	}
	icdev = rf_init(0, 9600);
	unsigned char ver[20];
	int status = rf_get_status(icdev, ver);
	if (status != 0) {
		std::cout << "��ʼ�����Ӷ�����Ӳ��ʧ��Card Reader init failed!" << "\r\n";
		return -1;
	}
	else {
		std::cout << "------��ʼ�����Ӷ�����Ӳ���ɹ�Card Reader init SUCCESS------" << ver << "!\n";
	}

	rf_beep(icdev, (short)10);
	MemberCardReader::isConnected = 1;//��ʼ��
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
			std::cout << "------------------------������������ʧ��-------------------------" << i << "\r\n";
		}
		else {
			std::cout << "------------------------OK��������������ɹ�-------------------------" << i << " " << key << "\r\n";
		}
	}

	//Ѱ��

	int st;
	/**
	ע �⣺ѡ�� IDLE
	ģʽѰ��ʱ����ɶԿ�Ƭ�Ĳ�������� rf_halt ������ֹͣ���� �� �� ���д�������ҵ���Ƭ������
	��Ƭ�뿪���������ٴ����½��롣
	ѡ�� ALL ģʽѰ��ʱ����ɶԿ�Ƭ�Ĳ�������� rf_halt ������ֹͣ���� �� �˺��д��������
	���ÿ�Ƭ�������뿪���������ٴ����½���
	*/
	unsigned long noofcard = 0;
	st = rf_card(icdev, (short)0, &noofcard);  //IDLEģʽ��������������
	if (st == 0) {
		std::cout << "------------------------Ѱ���ɹ�-------------------------" << "\r\n";
	}
	//��֤
	short sector = 1; //����1
	st = rf_authentication(icdev, (short)0, sector);
	if (st != 0)
	{

		std::cout << "------------------------����������֤ ����!-------------------------" << sector << "\r\n";
	}
	else {
		std::cout << "------------------------OK,����������֤�ɹ�!-------------------------" << sector << "\r\n";
	}



	// ���Ӷ�д��Ƭ���ݵĲ���

	//���ٲ�������ʱ�䣬ֻ�����������ݡ��Ƶ�id����,cardno���棬�������ݵ�д��
	//�������ݸ�ʽ"{\"hotelNo\":\"H201810000012\",\"cardNo\":\"20181000000001\"}"
	
	if (flag == 1) {
		//д����
		for (int i = 4; i < 6; i++) {
			char hotelno[16];
			//strcpy_s(hotelno, needwrite[i - 4].c_str());
			//unsigned char *conbuf = (unsigned char *)hotelno;
			//TODO: �ַ����������ת��
			write_content(icdev, (short)i, needwrite);
		}
	}else{
		//������
		std::cout << "------------------------�����ݲ���-------------------------" << icdev << "\r\n";
		for (int i = 4; i < 6; i++) {
			unsigned char data[16];
			read_content(icdev, (short)i, data);
			
			strcat((char *)needwrite, (char *)data);

		}
	}
	

	Sleep(100); //ͣ��100���� 

	rf_halt(icdev); //�������Ĳ���
	std::cout << "------------------------�������Ĳ���-------------------------" << icdev << "\r\n";


	// �˿ڶ�����Ӳ����PC������
	//st = rf_exit((HANDLE)icdev);
	
	return 0;
}

int MemberCardReader::read_content(HANDLE icdev, unsigned char adr, unsigned char * data)
{
	//��ȡ
	int flag;
	//unsigned char data[16];
	flag = rf_read(icdev, adr, data);  //��M1����0������
	if (flag != 0) {
		std::cout << "Read error:" << flag << data << "\r\n";
		return -1;
	}
	std::cout << "Read the data from block: " << int(adr) << " " << data << "\r\n";

	return 0;
}

int MemberCardReader::write_content(HANDLE icdev, unsigned char adr, unsigned char * data)
{
	//д��
	int flag;
	flag = rf_write(icdev, adr, data);
	if (flag != 0) {
		std::cout << "Card write error:" << flag << data << "\r\n";
		return -1;
	}
	std::cout << "д��ɹ�,Write the data into block: " << int(adr) << "\r\n";

	return 0;
}
