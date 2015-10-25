#define _USE_MATH_DEFINES

#include "urg_unko.h"
#include "SharedMemory.h"
#include "open_urg_sensor.c"
#include "Timer.h"
#include "receiveAndroidSensors.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#define PI 3.14159265359

#define DEBUG_WRITELINE

//#define KAISUU 10

using namespace std;
using namespace cv;

float scaninterval = 0.0;//計測を実施する最低間隔[mm]

//URG関係のグローバル変数
enum {
	CAPTURE_TIMES = 1,
};

rcvAndroidSensors rcvDroid(24);
float defaultOrientation[3];

float chairpos_old = 0.0;//車いすの車輪の直前の進行 
float chairpos = 0.0;//車いすの現在の進行 
float DIS_old = 0.0;

float startpos[2] = {};	//測定開始位置から見た現在の位置
float startpos_old[2] = {};	//直前の位置を保存する変数

//数値表示用
Mat picture;
Mat arrowpic;
Mat rotatepic;
Mat affine_mat;

bool isInitialized = false;


//左右輪のエンコーダ生データ積算用
int data_L = 0, data_R = 0;

// pcimageの引数．画像のサイズと解像度
// main.cppでコマンドライン
extern int imgWidth, imgHeight, imgResolution;


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

	//cout << "\n\n\ndata1:" << data1 << " ,  data2:" << data2 << endl << endl;

	//char型を整数値として表示
	//cout << "\n\n\ndata1:" << std::showbase << std::dec << static_cast<int>(receive_char1) << " ,  data2:" << std::showbase << std::dec << static_cast<int>(receive_char2) << endl << endl;

	// データを積算
	data_L += static_cast<int>(receive_char1);
	data_R += static_cast<int>(receive_char2);
	//cout << "\n\n\ndata_L:" << data_L << " ,  data_R:" << data_R << endl << endl;

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

	//printf("Distance = %d , Angle = %f \n", (int)dist, rad);

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
	HANDLE handle_ARDUINO;	//Arduino用ハンドル

	urg_unko *unkoArray = new urg_unko[NumOfURG];	//urg_unko型変数の配列

	Timer	timer; //ループの間隔調整用タイマー
	int		interval;
	timer.Start();

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

	//マップ作成を行うループ
	//'q'を入力するとループを抜ける
#ifndef KAISUU
	while (true){
#else
	for (int i = 0; i < 10; i++){
#endif
		// 処理の間隔を指定時間あける
		if (timer.getLapTime(1, Timer::millisec, false) < shMemInt.getShMemData(INTERVALTIME)) continue;
		interval = timer.getLapTime();

		//エンコーダから移動量，回転量を取得
		Encoder(handle_ARDUINO, dist, rad);
		//cout << "\n\n dist = " << dist << ", rad = " << rad << endl << endl;

		//積算した距離を格納
		chairpos = dist;

		//URGからデータを取得し，エンコーダの値を基にマップ，pcdファイルを作成
		for (int i = 0; i < NumOfURG; i++)
		{
			unkoArray[i].getData4URG(dist, rad);
		}

		//現在の位置を保存
		startpos_old[0] = startpos[0];
		startpos_old[1] = startpos[1];

		//現在の位置を更新
		//測定開始時点を基準に
		//		xの正：前
		//		yの正：左
		startpos[0] += cos(rad) * (chairpos - DIS_old);
		startpos[1] -= sin(rad) * (chairpos - DIS_old);
		//printf("startpos[0] = %f , startpos[1] = %f\n", startpos[0], startpos[1]);

		//現在の移動量を保存
		DIS_old = chairpos;

		//'q'が入力されたらループを抜ける
		// もしくは共有メモリの0番地に0が入力さ(ry
		if (cv::waitKey(1) == 'q' || shMemInt.getShMemData(0))
		{
			//Newで確保した配列の解放
			delete[] unkoArray;

			// 表示している画像を閉じる
			destroyAllWindows();

			break;
		}

		// メーターの表示を設定
		{
			meterData[0] = data_L;
			meterData[1] = data_R;
			meterData[2] = data_L - data_R;
			if (data_R)	meterData[3] = (float)data_L / (float)data_R * 100;
			else meterData[3] = 0;
			meterData[4] = startpos[0];
			meterData[5] = startpos[1];
			meterData[6] = dist;
			meterData[7] = rad / PI *180;
			meterData[8] = interval;

			meter(picture, meterData, meterName, 9);
			showDirection( -rad , ":Encoder" );
		}

	}
	for (int i = 0; i < NumOfURG; i++)
	{
		unkoArray[i].save();
	}

	//Arduinoのハンドルを解放
	CommClose(handle_ARDUINO);

	return;
}

/********************************************
*
*	こっからurg_unkoの実装部分
*
*********************************************/

/*
*	概要:
*		コンストラクタ
*		pcimage，COMport，pcdnumを初期化する
*	引数：
*		なし
*	返り値:
*		なし
*/
// urg_unkoを配列で宣言したときに引数を渡せないのでpcimageの引数はとりあえずグローバル変数から受け取る
urg_unko::urg_unko() :pcimage(::imgWidth,::imgHeight,::imgResolution)
{
	COMport = 0;
	pcdnum = 0;
}

/*
*	概要:
*		オブジェクトの初期化処理
*	引数：
*		int COM		URGのCOMポート番号
*		float pos[]	NCWCの回転中心から見たURGの位置
*	返り値:
*		なし
*/
void urg_unko::init(int COM, float pos[])
{
	// 引数のCOMをメンバのCOMportに格納
	COMport = COM;

	//指定されたCOMポートのURGと接続
	connectURG();

	//以下，メンバの初期化
	for (int i = 0; i < 3; i++)
	{
		urgpos[i] = pos[i];
	}
}

/*
*	概要:
*		URGの接続を切断
*	引数：
*		なし
*	返り値:
*		0
*/
int urg_unko::disconnectURG(){

	//切断
	free(data);
	urg_close(&urg);

	printf("URG disconnected \n");
	return 0;

}

/*
*	概要:
*		デストラクタ
*		URGの切断，画像の保存を行う
*	引数：
*		なし
*	返り値:
*		なし
*/
urg_unko::~urg_unko()
{
	pcimage.savePCImage();
	disconnectURG();
}

/*
*	概要:
*		URGと接続する
*		init(int COM , float pos[])で指定したURGと接続する
*	引数：
*		なし
*	返り値:
*		0
*/
int urg_unko::connectURG(){

	//urgオブジェクトの取得
	if (open_urg_sensor(&urg, COMport) < 0) {
		return 1;
	}

	//データ取得用のメモリを確保
	data = (long *)malloc(urg_max_data_size(&urg) * sizeof(data[0]));
	if (!data) {
		perror("urg_max_index()");
		return 1;
	}

	printf("URG connected : urg_max_data_size =  %d \n", urg_max_data_size(&urg));
	return 0;
}

/*
*	概要:
*		URGからデータを取得する
*	引数：
*		float& dist	積算する距離データ用変数の参照
*		float& rad	積算する回転角データ用変数の参照
*	返り値:
*		0
*/
int urg_unko::getData4URG(float& dist, float& rad){
	//データ取得
#if 0
	//データの取得範囲を変更する場合
	urg_set_scanning_parameter(&urg,
		urg_deg2step(&urg, -90),
		urg_deg2step(&urg, +90), 0);
#endif

	//測定の開始
	urg_start_measurement(&urg, URG_DISTANCE, 1, 0);

	for (int i = 0; i < CAPTURE_TIMES; ++i) {
		int n;

		//測定データの取得
		n = urg_get_distance(&urg, data, &time_stamp);
		if (n <= 0) {
			printf("urg_get_distance: %s\n", urg_error(&urg));
			free(data);
			urg_close(&urg);
			return 1;
		}

		//積算した回転角を格納
		urgpos[2] = rad;

		//測定データからマップ，pcdファイルを作成
		set_3D_surface(n);
	}

	return 0;

}

/*
*	概要:
*		取得したデータから実際の二次元情報を計算してマップ，pcdファイルへの書き込みを行う
*	引数：
*		int data_n	取得したデータの数
*	返り値:
*		なし
*/
void urg_unko::set_3D_surface( int data_n)
{
	//printf(" set_3D_surface data_n = %d \n", data_n);

	(void)time_stamp;

	long min_distance;
	long max_distance;

	if (chairpos >= 0){//位置の取得が成功したとき

		if (abs(chairpos - chairpos_old) >= scaninterval){//位置が設定値以上動いたとき

			// 全てのデータの X-Y の位置を取得
			//正常に取得されるデータの最大値，最小値を取得
			urg_distance_min_max(&urg, &min_distance, &max_distance);

			//pcdファイルの初期化
			pcdinit();

			float droidOrientation[3];
			float droidGPS[3];
			rcvDroid.getOrientationData(droidOrientation);
			rcvDroid.getGPSData(droidGPS);

			//データの数だけ実際の座標を計算してマップ，pcdファイルに書き込む
			for (int i = 0; i < data_n; ++i) {
				long l = data[i];	//取得した点までの距離
				double radian;
				float x, y, z;
				float pointpos[3];

				//異常値ならとばす
				if ((l <= min_distance) || (l >= max_distance)) {
					pcdWrite(0, 0, startpos[0] / 1000, startpos[1] / 1000, droidOrientation, droidGPS);
					continue;
					l = max_distance;
				}

				//点までの角度を取得してxyに変換
				//(極座標で取得されるデータをデカルト座標に変換)
				radian = urg_index2rad(&urg, i);
				x = (float)(l * cos(radian));
				y = (float)(l * sin(radian));
				z = 120.0;

				//2次元平面の座標変換
				pointpos[0] = + cos(urgpos[2]) * x + sin(urgpos[2]) * y + cos(urgpos[2]) * (chairpos - DIS_old + urgpos[1]) + startpos[0];
				pointpos[1] = -sin(urgpos[2]) * x + cos(urgpos[2]) * y - sin(urgpos[2]) * (chairpos - DIS_old + urgpos[1]) + startpos[1];
				pointpos[2] = z;

				// 座標を保存
#ifndef DEBUG_WRITELINE
				// 点のみ書き込む
				pcimage.writePoint(pointpos[0] / 1000, pointpos[1] / 1000);
				pcdWrite(pointpos[0] / 1000, pointpos[1] / 1000);
#else
				// 点を書き込んで現在地からの直線を引く
				pcimage.writePoint(pointpos[0] / 1000, pointpos[1] / 1000, startpos[0] / 1000, startpos[1] / 1000);
				pcdWrite(pointpos[0] / 1000, pointpos[1] / 1000, startpos[0] / 1000, startpos[1] / 1000 ,droidOrientation , droidGPS);
#endif

			}
			//１スキャン分のpcdファイルを保存
			pcdSave();
		}
	}
}

/*
*	概要:
*		pcdファイルを作成して初期化する
*	引数：
*		なし
*	返り値:
*		なし
*/
void urg_unko::pcdinit()
{
	//ファイル名を指定してファイルストリームを開く
	ofs.open( "./" + pcimage.getDirname() + "/pointcloud_" + std::to_string(pcdnum) + ".pcd");

	//pcdファイル番号を進めてデータ数カウント用変数を初期化
	pcdnum++;
	pcdcount = 0;

	//ヘッダを記入
	ofs << "# .PCD v.7 - Point Cloud Data file format\n"
		<< "VERSION .7\n"
		<< "FIELDS x y z\n"
		<< "SIZE 4 4 4\n"
		<< "TYPE F F F\n"
		<< "COUNT 1 1 1\n"
		<< "WIDTH 400\n"
		<< "HEIGHT 1\n"
		<< "VIEWPOINT 0 0 0 1 0 0 0\n"
		<< "POINTS 400\n"
		<< "DATA ascii" << endl;
}

/*
*	概要:
*		pcdファイルにデータを書き込む
*	引数：
*		float x	x座標値
*		float y	y座標値
*	返り値:
*		なし
*/
void urg_unko::pcdWrite(float x, float y)
{
	//データを書き込んでデータ数をカウント
	ofs << x << " " << y << " " << "0.0" << endl;
	pcdcount++;
}

void urg_unko::pcdWrite(float x, float y , float pos_x , float pos_y , float droidAngle[] , float droidGPS[])
{
	//データを書き込んでデータ数をカウント
	ofs << x << ", " << y << ", " << pos_x << ", " << pos_y << ", " << droidAngle[0] << ", " << droidAngle[1] << ", " << droidAngle[2] << ", " << droidGPS[0] << ", " << droidGPS[1] << ", " << droidGPS[2] << ", " << endl;
	pcdcount++;
}

/*
*	概要:
*		ファイルストリームを閉じて保存する
*	引数：
*		なし
*	返り値:
*		なし
*/
void urg_unko::pcdSave()
{
	//最終的なデータ数を追記
	ofs.seekp(0, ios_base::beg);

	ofs << "# .PCD v.7 - Point Cloud Data file format\n"
		<< "VERSION .7\n"
		<< "FIELDS x y z\n"
		<< "SIZE 4 4 4\n"
		<< "TYPE F F F\n"
		<< "COUNT 1 1 1\n"
		<< "WIDTH " + std::to_string(pcdcount) + "\n"
		<< "HEIGHT 1\n"
		<< "VIEWPOINT 0 0 0 1 0 0 0\n"
		<< "POINTS " + std::to_string(pcdcount) + "\n"
		<< "DATA ascii" << endl;

	//ファイルストリームを緘
	ofs.close();
	ofs.flush();
}

void urg_unko::setWriteLine(bool isLine)
{
	pcimage.isWriteLine = isLine;
}

std::string	urg_unko::getDirName()
{
	return pcimage.getDirname();
}
void urg_unko::save()
{
	pcimage.savePCImage();
}