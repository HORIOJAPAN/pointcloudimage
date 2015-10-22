#include "receiveAndroidSensors.h"
#include <iostream>
#include <string>

using namespace std;

// 2015/10/09

rcvAndroidSensors::rcvAndroidSensors(int comport) : shMem(shMemName)
{
	COM = comport;

	comOpen();

	minSaveInterval = 5000;

}
rcvAndroidSensors::~rcvAndroidSensors()
{
	comClose();
}
void rcvAndroidSensors::comOpen()
{
	DCB				cDcb;		// 通信設定用
	COMMTIMEOUTS	cTimeouts;	// 通信ポートタイムアウト用

	// 通信ポートを開く
	string com = "\\\\.\\COM" + to_string(COM);
	hComm = CreateFile(com.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	// ハンドルの確認
	if (hComm == INVALID_HANDLE_VALUE){
		hComm = NULL;
		LPVOID lpMsgBuf;
		FormatMessage(				//エラー表示文字列作成
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);

		MessageBox(NULL, (const char*)lpMsgBuf, NULL, MB_OK);	//メッセージ表示
		return;
	}


	// 通信設定
	if (!GetCommState(hComm, &cDcb)){
		// 通信設定エラーの場合
		printf("ERROR:GetCommState error\n");
		comClose();
		hComm = NULL;
	}
	//cDcb.BaudRate = 115200;				// 通信速度
	cDcb.BaudRate = 9600;				// 通信速度
	cDcb.ByteSize = 8;					// データビット長
	cDcb.fParity = TRUE;				// パリティチェック
	cDcb.Parity = NOPARITY;			// ノーパリティ
	cDcb.StopBits = ONESTOPBIT;			// 1ストップビット

	if (!SetCommState(hComm, &cDcb)){
		// 通信設定エラーの場合
		printf("ERROR:GetCommState error\n");
		comClose();
		hComm = NULL;
	}


	// 通信設定(通信タイムアウト設定)
	// 文字の読み込み待ち時間(ms)
	cTimeouts.ReadIntervalTimeout = 50;
	// 読み込みの１文字あたりの時間(ms)
	cTimeouts.ReadTotalTimeoutMultiplier = 50;
	// 読み込みの定数時間(ms)
	cTimeouts.ReadTotalTimeoutConstant = 50;
	// 書き込みの１文字あたりの時間(ms)
	cTimeouts.WriteTotalTimeoutMultiplier = 0;

	if (!SetCommTimeouts(hComm, &cTimeouts)){
		// 通信設定エラーの場合
		printf("ERROR:SetCommTimeouts error\n");
		comClose();
		hComm = NULL;
	}

	// 通信バッファクリア
	PurgeComm(hComm, PURGE_RXCLEAR);
}

void rcvAndroidSensors::comClose()
{
	if (hComm){
		CloseHandle(hComm);
	}
	cout << "Port closed" << endl;
}

void rcvAndroidSensors::PreparationGPSCSV()
{
	timerGPS.Start();
	timeCountGPS = 0;

	ofsGPS.open("GPSdata_" + timerGPS.getNowTime() + ".csv");
	ofsOrientation << timerGPS.getNowTime() << ","
		<< "Latitude,"
		<< "Longitude,"
		<< "Accuracy," << endl;

	isPreparationGPSofs = true;
}
void rcvAndroidSensors::PreparationOrientationCSV()
{
	timerOrientation.Start();
	timeCountOrientation = 0;

	ofsOrientation.open("OrientationData_" + timerOrientation.getNowTime() + ".csv");
	ofsOrientation << timerOrientation.getNowTime() << ","
		<< "Azimuth,"
		<< "Pitch,"
		<< "Roll," << endl;

	isPreparationOrientationofs = true;
}

void rcvAndroidSensors::setIsSaveGPSCSV(bool isSaveCSV)
{
	isSaveGPSCSV = isSaveCSV;

	if (isSaveGPSCSV && !isPreparationGPSofs)
	{
		PreparationGPSCSV();
	}
}
void rcvAndroidSensors::setIsSaveOrientationCSV(bool isSaveCSV)
{
	isSaveOrientationCSV = isSaveCSV;

	if (isSaveOrientationCSV && !isPreparationOrientationofs)
	{
		PreparationOrientationCSV();
	}
}


void rcvAndroidSensors::getSensorData()
{
	unsigned char	sendbuf[128] = { (byte)0x01, (unsigned char)1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, };
	unsigned char	readbuf[128] = {};
	unsigned char	sum;
	int				i;
	int				ret;
	unsigned long	len, readlen;
	float			dLatitude, dLongitude;


	// ハンドルチェック
	if (!hComm){
		return;
	}
	{
		// バッファクリア
		//memset(sendbuf, 0x01, sizeof(sendbuf));

		// 通信バッファクリア
		PurgeComm(hComm, PURGE_RXCLEAR);

		// 送信
		ret = WriteFile(hComm, &sendbuf, 10, &len, NULL);
	}
	// 読み込み
	memset(readbuf, 0x00, sizeof(readbuf));
	readlen = 11;
	len = 0;

	ret = ReadFile(hComm, readbuf, readlen, &len, NULL);

	// GPSのデータ
	if (readbuf[0] == 1){
		//データ復元
		dLatitude = (readbuf[1] << 16) + (readbuf[2] << 8) + readbuf[3];
		dLongitude = (readbuf[4] << 16) + (readbuf[5] << 8) + readbuf[6];
		mAccuracy = (readbuf[7] << 8) + readbuf[8];

		//符号合わせ
		if ((readbuf[9] & 1) > 0) dLatitude = -dLatitude;
		if ((readbuf[9] & 2) > 0) dLongitude = -dLongitude;

		//桁合わせ
		dLatitude /= 100000;
		dLongitude /= 100000;
		mAccuracy /= 10;

		// 返ってくるのは変位なので積算
		mLatitude += dLatitude;
		mLongitude += dLongitude;

		printf("--GPS変位--\n %.6f , %.6f \n", dLatitude, dLongitude);
		printf("--GPS絶対値--\n %.6f , %.6f , %.1f \n\n", mLatitude, mLongitude, mAccuracy);

		// 指定された間隔で保存
		timeCountGPS += timerGPS.getLapTime();
		if (timeCountGPS > minSaveInterval && isSaveGPSCSV)
		{
			ofsGPS << timerGPS.getNowTime() << ","
				<< mLatitude << ","
				<< mLongitude << ","
				<< mAccuracy << "," << endl;
		}

	}
	// 姿勢のデータ
	else if (readbuf[0] == 2){
		// データの復元
		mAzimuth = (readbuf[1] << 8) + readbuf[2];
		mPitch = (readbuf[3] << 8) + readbuf[4];
		mRoll = (readbuf[5] << 8) + readbuf[6];

		// 桁合わせと-180～180に変換
		mAzimuth = mAzimuth / 100 - 180;
		mPitch = mPitch / 100 - 180;
		mRoll = mRoll / 100 - 180;

		//printf("--orientation--\n %.2f , %.2f , %.2f \n", mAzimuth, mPitch, mRoll);

		// 指定間隔で保存
		timeCountOrientation += timerOrientation.getLapTime();
		if (timeCountOrientation > minSaveInterval && isSaveOrientationCSV)
		{
			ofsOrientation << timerOrientation.getNowTime() << ","
				<< mAzimuth	<< ","
				<< mPitch	<< ","
				<< mRoll	<< "," << endl;
			cout << "⊂二二二（ ＾ω＾）二⊃ 保存したﾌﾞｰﾝ" << endl;
		}

	}
	// GPSの初期値
	else if (readbuf[0] == 3){
		mLatitude = (readbuf[1] << 24) + (readbuf[2] << 16) + (readbuf[3] << 8) + readbuf[4];
		mLongitude = (readbuf[5] << 24) + (readbuf[6] << 16) + (readbuf[7] << 8) + readbuf[8];
		mAccuracy = (readbuf[9] << 8) + readbuf[10];

		mLatitude = mLatitude / 1000000;
		mLongitude = mLongitude / 1000000;
		mAccuracy = mAccuracy / 10;

		printf("--GPS--\n %.6f , %.6f , %.1f \n\n", mLatitude, mLongitude, mAccuracy);
		timeCountGPS += timerGPS.getLapTime();
		if (timeCountGPS > minSaveInterval && isSaveGPSCSV)
		{
			ofsGPS << timerGPS.getNowTime() << ","
				<< mLatitude	<< ","
				<< mLongitude	<< ","
				<< mAccuracy	<< "," << endl;
		}

	}
}

void rcvAndroidSensors::setSaveMinInterval(int interval)
{
	minSaveInterval = interval;
}

void	rcvAndroidSensors::getOrientationData(float retArray[3])
{
	unsigned char	sendbuf[1];
	unsigned char	readbuf[20];
	int				ret;
	unsigned long	len, readlen;

	// ハンドルチェック
	if (!hComm){
		cout << "No handle" << endl;
		return;
	}
	
	// バッファクリア
	memset(sendbuf, 0x02, sizeof(sendbuf));

	// 通信バッファクリア
	PurgeComm(hComm, PURGE_RXCLEAR);

	// 送信
	ret = WriteFile(hComm, &sendbuf, 1, &len, NULL);

	// 読み込み
	memset(readbuf, 0x00, sizeof(readbuf));
	readlen = 11;
	len = 0;

	ret = ReadFile(hComm, readbuf, readlen, &len, NULL);

	// 姿勢のデータ
	if (readbuf[0] == 2){
		// データの復元
		mAzimuth = (readbuf[1] << 8) + readbuf[2];
		mPitch = (readbuf[3] << 8) + readbuf[4];
		mRoll = (readbuf[5] << 8) + readbuf[6];

		// 桁合わせと-180～180に変換
		mAzimuth = mAzimuth / 100 - 180;
		mPitch = mPitch / 100 - 180;
		mRoll = mRoll / 100 - 180;

		retArray[0] = mAzimuth;
		retArray[1] = mPitch;
		retArray[2] = mRoll;

		//printf("--orientation--\n %.2f , %.2f , %.2f \n", mAzimuth, mPitch, mRoll);

		// 指定間隔で保存
		if(isSaveOrientationCSV) timeCountOrientation += timerOrientation.getLapTime();
		if (timeCountOrientation > minSaveInterval && isSaveOrientationCSV)
		{
			ofsOrientation << timerOrientation.getNowTime() << ","
				<< mAzimuth << ","
				<< mPitch << ","
				<< mRoll << "," << endl;
			cout << "⊂二二二（ ＾ω＾）二⊃ 保存したﾌﾞｰﾝ" << endl;
		}

		if (isSaveSharedMemory)
		{
			shMem.setShMemData(mAzimuth, AZIMUTH);
			shMem.setShMemData(mPitch, PITCH);
			shMem.setShMemData(mRoll, ROLL);
		}

	}
	else
	{
		cout << "Not Orientation data" << endl;
		retArray[0] = 0;
		retArray[1] = 0;
		retArray[2] = 0;
	}
}

void rcvAndroidSensors::getGPSData(float retArray[3])
{
	unsigned char	sendbuf[1];
	unsigned char	readbuf[20];
	int				ret;
	unsigned long	len, readlen;

	// ハンドルチェック
	if (!hComm){
		cout << "No handle" << endl;
		return;
	}

	// バッファクリア
	memset(sendbuf, 0x01, sizeof(sendbuf));

	// 通信バッファクリア
	PurgeComm(hComm, PURGE_RXCLEAR);

	// 送信
	ret = WriteFile(hComm, &sendbuf, 1, &len, NULL);

	// 読み込み
	memset(readbuf, 0x00, sizeof(readbuf));
	readlen = 11;
	len = 0;

	ret = ReadFile(hComm, readbuf, readlen, &len, NULL);

	// GPSの初期値
	if (readbuf[0] == 3){
		mLatitude = (readbuf[1] << 24) + (readbuf[2] << 16) + (readbuf[3] << 8) + readbuf[4];
		mLongitude = (readbuf[5] << 24) + (readbuf[6] << 16) + (readbuf[7] << 8) + readbuf[8];
		mAccuracy = (readbuf[9] << 8) + readbuf[10];

		mLatitude = mLatitude / 1000000;
		mLongitude = mLongitude / 1000000;
		mAccuracy = mAccuracy / 10;

		retArray[0] = mLatitude;
		retArray[1] = mLongitude;
		retArray[2] = mAccuracy;

		//printf("--GPS--\n %.6f , %.6f , %.1f \n\n", mLatitude, mLongitude, mAccuracy);

		// 指定間隔で保存
		if (isSaveGPSCSV) timeCountGPS += timerGPS.getLapTime();
		if (timeCountGPS > minSaveInterval && isSaveGPSCSV)
		{
			ofsGPS << timerGPS.getNowTime() << ","
				<< mLatitude << ","
				<< mLongitude << ","
				<< mAccuracy << "," << endl;
		}
		if (isSaveSharedMemory)
		{
			shMem.setShMemData(mLatitude, LATITUDE);
			shMem.setShMemData(mLongitude, LONGITUDE);
			shMem.setShMemData(mAccuracy, ACCURACY);
		}

	}
	else
	{
		cout << "Not GPS data" << endl;
		retArray[0] = 0;
		retArray[1] = 0;
		retArray[2] = 0;
	}

}

