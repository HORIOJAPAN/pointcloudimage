#include "urg_unko.h"
#include "SharedMemory.h"
#include "Timer.h"
#include "receiveAndroidSensors.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#define PI 3.14159265359

//#define KAISUU 10

using namespace std;
using namespace cv;

rcvAndroidSensors rcvDroid(24);
float defaultOrientation[3];

//数値表示用
Mat picture;
Mat arrowpic;
Mat rotatepic;
Mat affine_mat;

bool isInitialized = false;


//左右輪のエンコーダ生データ積算用
int data_L = 0, data_R = 0;

//数値の表示
void meter(Mat pic, float data[] , string name[], int NumOfData)
{
	int baseline = 0;
	pic = Mat::zeros(500, 1000, CV_8UC3);
	Size textSize = getTextSize("OpenCV ", FONT_HERSHEY_SIMPLEX, 2, 2, &baseline);
	for (int i = 0; i < NumOfData; i++){
		putText(pic, name[i] + " : " + to_string(data[i]), cv::Point(50, 50 + textSize.height*i), FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0, 200,0 ), 2, CV_AA);
	}
	
	imshow("meter", pic);
}

//回転角を矢印で表示
void showDirection(float radian , string showName)
{
	affine_mat = getRotationMatrix2D(Point(arrowpic.cols / 2, arrowpic.rows / 2), -radian / PI * 180, 1);
	warpAffine(arrowpic, rotatepic, affine_mat, arrowpic.size());
	putText(rotatepic, to_string((int)(-radian / (2 * PI))), cv::Point(20, 50), FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(100, 0, 230), 2, CV_AA);
	imshow("direction" + showName, rotatepic);
}

int CommClose(HANDLE hComm)
{
	if (hComm){
		CloseHandle(hComm);
	}

	return 1;
}

/*
*	概要:
*		Arduinoから左右輪の変化量を取得し，移動量，回転量を積算する
*	引数：
*		HANDLE hComm	Arduinoのハンドル
*		float& dist		移動量を積算する変数への参照
*		float& rad		回転量を積算する変数への参照
*	返り値:
*		int ret	成功したかどうか
*/
int Encoder(HANDLE hComm, float& dist, float& rad)
{
	unsigned char	sendbuf[1];
	unsigned char	receive_data[2];
	int				ret;
	float			DL, DR, DIS, ANG;
	unsigned long	len;

	float			droidOrientation[3];

	// ハンドルチェック
	if( !hComm )	return -1;
	// バッファクリア
	memset(sendbuf, 0x00, sizeof(sendbuf));
	// パケット作成
	sendbuf[0] = (unsigned char)1;
	// 通信バッファクリア
	PurgeComm(hComm, PURGE_RXCLEAR);
	// 送信
	ret = WriteFile(hComm, &sendbuf, 1, &len, NULL);

	// バッファクリア
	memset(receive_data, 0x00, sizeof(receive_data));
	// 通信バッファクリア
	PurgeComm(hComm, PURGE_RXCLEAR);
	// Arduinoからデータを受信
	ret = ReadFile(hComm, &receive_data, 2, &len, NULL);
	//cout << static_cast<bitset<8>>(receive_data[0]) << "," << static_cast<bitset<8>>(receive_data[1] )<< endl;


	//初期化されていなければ初期化(初めのデータを捨てる)
	if (!isInitialized)
	{
		isInitialized = true;
		return 0;
	}

	//取得した値を符号つきに代入
	signed char receive_char1, receive_char2;
	receive_char1 = receive_data[0];
	receive_char2 = receive_data[1];

	// データを積算
	data_L += static_cast<int>(receive_char1);
	data_R += static_cast<int>(receive_char2);

	//左右輪の回転量から移動量を計算
	DL = receive_char1 * 24.0086517664 / 1.005 ;
	DR = receive_char2 * 23.751783167;

	//移動距離，回転量を計算
	DIS = (DL + DR) / 2;
	ANG = -(DL - DR) / 530;	//右回転が正

	//移動量，回転量を積算用変数へ積算
	dist += DIS;
	rad += ANG;
	//rcvDroid.getOrientationData(droidOrientation);
	//rad = droidOrientation[0] - defaultOrientation[0];

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
void getArduinoHandle(int arduinoCOM , HANDLE& hComm)
{
	//シリアルポートを開いてハンドルを取得
	string com = "\\\\.\\COM" + to_string(arduinoCOM);
	hComm = CreateFile( com.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hComm == INVALID_HANDLE_VALUE){
		printf("シリアルポートを開くことができませんでした。");
		char z;
		z = getchar();
		return;
	}
	//ポートを開けていれば通信設定を行う
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
}

/*
*	概要:
*		マップ作成用のメインループ
*		'q'で終了
*		適切に終了しないと画像が保存されない
*	引数：
*		int URG_COM[]	接続したURGのCOMポートの配列
*		int ARDUINO_COM	接続したArduinoのCOMポート

*	返り値:
*		なし
*/
void getDataUNKOOrigin(int URG_COM[], float URGPOS[][3], int ARDUINO_COM, int NumOfURG)
{
	/**********************
	 *　↓　変数の宣言　↓
	 **********************/

	HANDLE handle_ARDUINO;	//Arduino用ハンドル

	urg_unko *unkoArray = new urg_unko[NumOfURG];	//urg_unko型変数の配列

	Timer	timer; //ループの間隔調整&インターバル測定用タイマー
	int		interval;
	timer.Start();

	float currentCoord[2] = {};	//測定開始位置から見た現在の位置

	float chairdist = 0.0;//車いすの移動量
	float chairdist_old = 0.0;

	float dist = 0.0;	//移動距離の積算用変数
	float rad = 0.0;	//回転量の積算用変数

	// 数値表示用の変数達
	string meterName[] = {"dataL","dataR", "Difference of encoder value(L-R)", "Ratio of encoder value(L/R[%])", 
							"Current coordinates X", "Current coordinates Y", "Moving distance[mm]", "Angle variation[deg]",
							"Interval[millisec]"};
	float		meterData[9] = {};

	// csFormとの懸け橋
	// ループ抜けるタイミングとかのやり取り用
	SharedMemory<int> shMemInt("MappingFormInt");
	enum {ISEND , INTERVALTIME};
	shMemInt.setShMemData(false, ISEND);

	/****************************
	*　↓　もろもろの初期化　↓
	*****************************/

	// 姿勢表示用矢印の読み込み
	arrowpic = imread("arrow.jpg");
	if (arrowpic.empty()) cout << "No arrow image" << endl;
	arrowpic = ~arrowpic;

	//rcvDroid.getOrientationData(defaultOrientation);

	//Arduinoとシリアル通信を行うためのハンドルを取得
	getArduinoHandle(ARDUINO_COM,handle_ARDUINO);
	//エンコーダの初期化
	Encoder(handle_ARDUINO, dist, rad);

	//接続したURGの数だけurg_unko型オブジェクトを初期化
	for (int i = 0; i < NumOfURG; i++)
	{
		unkoArray[i].init(URG_COM[i], URGPOS[i]);
		unkoArray[i].setWriteLine(false);
	}

	/*********************
	*　↓　メイン処理　↓
	**********************/

	//マップ作成を行うループ
	//'q'を入力するとループを抜ける
#ifndef KAISUU
	while (true){
#else
	for (int i = 0; i < KAISUU; i++){
#endif
		// 処理の間隔を指定時間あける
		if (timer.getLapTime(1, Timer::millisec, false) < shMemInt.getShMemData(INTERVALTIME)) continue;
		interval = timer.getLapTime();

		//エンコーダから移動量，回転量を取得
		Encoder(handle_ARDUINO, dist, rad);

		//積算した距離を格納
		chairdist = dist;
		
		//URGからデータを取得し，エンコーダの値を基にマップ，pcdファイルを作成
		for (int i = 0; i < NumOfURG; i++)
		{
			unkoArray[i].updateCurrentCoord(currentCoord);
			unkoArray[i].getData4URG(dist,chairdist_old, rad);
		}
		
		//現在の位置を更新
		//測定開始時点を基準に
		//		xの正：前
		//		yの正：左
		currentCoord[0] += cos(rad) * (chairdist - chairdist_old);
		currentCoord[1] -= sin(rad) * (chairdist - chairdist_old);

		//現在の移動量を保存
		chairdist_old = chairdist;

		//'q'が入力されたらループを抜ける
		// もしくは共有メモリの0番地に0が入力さ(ry
		if (cv::waitKey(1) == 'q' || shMemInt.getShMemData(0)) break;

		// メーターの表示を設定
		{
			meterData[0] = data_L;
			meterData[1] = data_R;
			meterData[2] = data_L - data_R;
			if (data_R)	meterData[3] = (float)data_L / (float)data_R * 100;
			else meterData[3] = 0;
			meterData[4] = currentCoord[0];
			meterData[5] = currentCoord[1];
			meterData[6] = dist;
			meterData[7] = rad / PI *180;
			meterData[8] = interval;

			meter(picture, meterData, meterName, 9);
			showDirection( -rad , ":Encoder" );
		}

	}

	//Newで確保した配列の解放
	delete[] unkoArray;
	// 表示している画像を閉じる
	destroyAllWindows();
	//Arduinoのハンドルを解放
	CommClose(handle_ARDUINO);

	return;
}
