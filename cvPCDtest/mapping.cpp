#define _USE_MATH_DEFINES

#include "urg_unko.h"
#include "open_urg_sensor.c"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

float scaninterval = 0.0;//計測を実施する最低間隔[mm]

//URG関係のグローバル変数
enum {
	CAPTURE_TIMES = 1,
};

float chairpos_old = 0.0;//車いすの車輪の直前の進行 
float chairpos = 0.0;//車いすの現在の進行 
float DIS_old = 0.0;

HANDLE hComm;

int CommClose(HANDLE hComm)
{
	if (hComm){
		CloseHandle(hComm);
	}

	return 1;
}

int Encoder(HANDLE hComm, float& dist, float& rad)
{
	unsigned char	sendbuf[1];
	unsigned char	receive_data[2];
	int				ret;
	float			DL, DR, DIS, ANG;
	unsigned long	len;

	// ハンドルチェック
	/*if( !hComm ){
	return -1;
	}*/


	// バッファクリア
	memset(sendbuf, 0x00, sizeof(sendbuf));

	// パケット作成
	sendbuf[0] = (unsigned char)1;				// ヘッダー1

	// 通信バッファクリア
	PurgeComm(hComm, PURGE_RXCLEAR);

	// 送信
	ret = WriteFile(hComm, &sendbuf, 1, &len, NULL);

	memset(receive_data, 0x00, sizeof(receive_data));

	PurgeComm(hComm, PURGE_RXCLEAR);

	ret = ReadFile(hComm, &receive_data, 2, &len, NULL);
	//cout << static_cast<bitset<8>>(receive_data[0]) << "," << static_cast<bitset<8>>(receive_data[1] )<< endl;

	int data1 = 0, data2 = 0;

	if ((int)receive_data[0] == 255)
	{
		data1 = -256 | receive_data[0];
	}
	else data1 = receive_data[0];
	if ((int)receive_data[1] == 255)
	{
		data2 = -256 | receive_data[1];
	}
	else data2 = receive_data[1];

	//DL = receive_data[0] * 2.5;
	//DR = receive_data[1] * 2.5;
	DL = (signed int)data1 * 24.78367538;
	DR = (signed int)data2 * 24.78367538;

	DIS = (DL + DR) / 2;
	ANG = (DL - DR) / 526 ;

	dist += DIS;
	rad += ANG;

	printf("Distance = %d , Angle = %f \n", (int)DIS, ANG);

	return ret;
}

/*
 *	概要:
 *		Arduinoとシリアル通信を行うためのハンドルを取得する
 *	引数：
 *		HANDLE&	hComm	ハンドル変数への参照
 *	返り値:
 *		なし
 */
void getArduinoHandle(HANDLE& hComm)
{
	hComm = CreateFile("\\\\.\\COM10", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hComm == INVALID_HANDLE_VALUE){
		printf("シリアルポートを開くことができませんでした。");
		char z;
		z = getchar();
		return;
	}
	else
	{
		DCB lpTest;
		GetCommState(hComm, &lpTest);
		lpTest.BaudRate = 9600;
		lpTest.ByteSize = 8;
		lpTest.Parity = NOPARITY;
		lpTest.StopBits = ONESTOPBIT;
		SetCommState(hComm, &lpTest);
	}
	return;
}

/*
*	概要:
*		urg_unkoのmainループ
*	引数：
*		HANDLE&	hComm	ハンドル変数への参照
*	返り値:
*		なし
*/
int getDataUNKO(int URG_COM[], int ARDUINO_COM)
{
	HANDLE handle_ARDUINO;

	urg_unko unkoArray[URGCOUNT];

	float dist = 0;
	float rad = 0;

	float urgPOS[][3] = { 1100.0, 285.0, 0.0,
		20.0, 443.0, 0.0 };

	getArduinoHandle(handle_ARDUINO);

	Sleep(2000);

	cout << sizeof(URG_COM) / sizeof(int) << endl;

	for (int i = 0; i < URGCOUNT; i++)
	{
		unkoArray[i].init(URG_COM[i], urgPOS[i]);

	}

	cv::namedWindow("q");

	while (true){

		Encoder(handle_ARDUINO, dist, rad);
		cout << "\n\n dist = " << dist << ", rad = " << rad << endl << endl;

		for (int i = 0; i < URGCOUNT; i++)
		{
			unkoArray[i].getData4URG(dist, rad);
		}



		DIS_old = chairpos;

		if (cv::waitKey(1) == 'q')
		{
			break;
		}

	}

	CommClose(hComm);

	return 0;
}

/*
*
*	こっからurg_unkoの実装部分
*
*/
urg_unko::urg_unko() :pcimage(5000, 5000, 5)
{
	COMport = 0;
	pcdnum = 0;

}
void urg_unko::init(int COM , float pos[])
{
	COMport = COM;
	urg_unko::connectURG();

	startpos[0] = 0.0;
	startpos[1] = 0.0;

	startpos_old[0] = 0.0;
	startpos_old[1] = 0.0;

	for (int i = 0; i < 3; i++)
	{
		urgpos[i] = pos[i];
	}

}

int urg_unko::disconnectURG(){

	//切断
	free(this->data);
	urg_close(&this->urg);

	printf("URG disconnected \n");
	return 0;

}
urg_unko::~urg_unko()
{
	disconnectURG();
	pcimage.savePCImage();
}

int urg_unko::connectURG(){
	if (open_urg_sensor(&this->urg, COMport) < 0) {
		return 1;
	}

	this->data = (long *)malloc(urg_max_data_size(&this->urg) * sizeof(this->data[0]));
	if (!this->data) {
		perror("urg_max_index()");
		return 1;
	}

	printf("URG connected : urg_max_data_size =  %d \n", urg_max_data_size(&this->urg));
	return 0;
}

int urg_unko::getData4URG(float& dist, float& rad){

	int n;
	int i;

	//データ取得
#if 0
	//データの取得範囲を変更する場合
	urg_set_scanning_parameter(&this->urg,
		urg_deg2step(&this->urg, -90),
		urg_deg2step(&this->urg, +90), 0);
#endif

	startpos_old[0] = startpos[0];
	startpos_old[1] = startpos[1];

	urg_start_measurement(&this->urg, URG_DISTANCE, 1, 0);

	for (i = 0; i < CAPTURE_TIMES; ++i) {
		n = urg_get_distance(&this->urg, this->data, &this->time_stamp);
		if (n <= 0) {
			printf("urg_get_distance: %s\n", urg_error(&this->urg));
			free(this->data);
			urg_close(&this->urg);
			return 1;
		}

		set_3D_surface( n );

		chairpos = dist;
		urgpos[2] = rad;

	}
	startpos[0] = + cos(urgpos[2]) * (chairpos - DIS_old) + startpos_old[0];
	startpos[1] = - sin(urgpos[2]) * (chairpos - DIS_old) + startpos_old[1];
	printf("startpos[0] = %f , startpos[1] = %f\n", startpos[0], startpos[1]);

	return 0;

}

void urg_unko::set_3D_surface( int data_n)
{
	printf(" set_3D_surface data_n = %d \n", data_n);

	(void)time_stamp;

	int i;
	long min_distance;
	long max_distance;

	if (chairpos >= 0){//位置の取得が成功したとき

		if (abs(chairpos - chairpos_old) >= scaninterval){//椅子の位置が設定値以上動いたとき

			// 全てのデータの X-Y の位置を取得
			urg_distance_min_max(&urg, &min_distance, &max_distance);

			pcdinit();

			for (i = 0; i < data_n; ++i) {
				long l = data[i];
				double radian;
				float x, y, z;
				float pointpos[3];

				if ((l <= min_distance) || (l >= max_distance)) {
					continue;
				}

				radian = urg_index2rad(&urg, i);
				x = (float)(l * cos(radian));
				y = (float)(l * sin(radian));
				z = 120.0;

				//2次元平面の座標変換
				pointpos[0] = + cos(urgpos[2]) * x + sin(urgpos[2]) * y + cos(urgpos[2]) * (chairpos - DIS_old + urgpos[1]) + startpos[0];
				pointpos[1] = -sin(urgpos[2]) * x + cos(urgpos[2]) * y - sin(urgpos[2]) * (chairpos - DIS_old + urgpos[1]) + startpos[1];
				pointpos[2] = z;

				//座標を保存
				pcimage.writePoint(pointpos[0] / 1000, pointpos[1] / 1000);

				this->pcdWrite(pointpos[0] / 1000, pointpos[1] / 1000);

			}
			this->pcdSave();
		}
	}
}

void urg_unko::pcdinit()
{
	ofs.open( "./" + pcimage.getDirname() + "/pointcloud_" + std::to_string(pcdnum) + ".pcd");

	pcdnum++;
	pcdcount = 0;
	ofs << "# .PCD v.7 - Point Cloud Data file format\n"
		<< "VERSION .7\n"
		<< "FIELDS x y z rgb\n"
		<< "SIZE 4 4 4 4\n"
		<< "TYPE F F F F\n"
		<< "COUNT 1 1 1 1\n"
		<< "WIDTH\n"
		<< "HEIGHT\n"
		<< "VIEWPOINT 0 0 0 1 0 0 0\n"
		<< "POINTS 400\n"
		<< "DATA ascii" << endl;
}

void urg_unko::pcdWrite(float x, float y)
{
	ofs << x << " " << y << " " << "0.0" << endl;
	pcdcount++;
}
void urg_unko::pcdSave()
{
	ofs.seekp(0, ios_base::beg);

	ofs << "# .PCD v.7 - Point Cloud Data file format\n"
		<< "VERSION .7\n"
		<< "FIELDS x y z rgb\n"
		<< "SIZE 4 4 4 4\n"
		<< "TYPE F F F F\n"
		<< "COUNT 1 1 1 1\n"
		<< "WIDTH\n"
		<< "HEIGHT\n"
		<< "VIEWPOINT 0 0 0 1 0 0 0\n"
		<< "POINTS " + std::to_string(pcdcount) + "\n"
		<< "DATA ascii" << endl;

	ofs.close();
	ofs.flush();
}
