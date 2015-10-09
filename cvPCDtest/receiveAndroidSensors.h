#ifndef _INC_RCVDROIDSENSORS
#define _INC_RCVDROIDSENSORS

// 2015/10/09

#include <fstream>
#include <Windows.h>
#include "Timer.h"
#include "SharedMemory.h"

class rcvAndroidSensors
{
private:
	// メンバ変数
	// シリアル通信系
	int		COM;	//COMポート番号
	HANDLE	hComm;	//シリアルポートのハンドル

	// GPS系
	bool	isGetGPS = false;
	bool	isSaveGPSCSV = false;
	bool	isPreparationGPSofs = false;
	float	mLatitude , mLongitude , mAccuracy;	//	GPSから取得する緯度経度精度
	Timer	timerGPS;
	ofstream	ofsGPS;
	int		timeCountGPS;

	// 方位角系
	bool	isGetOrientation = false;
	bool	isSaveOrientationCSV  = false;
	bool	isPreparationOrientationofs = false;
	float	mAzimuth, mPitch, mRoll;	// 姿勢
	Timer	timerOrientation;
	ofstream	ofsOrientation;
	int		timeCountOrientation;

	// 共有メモリ系
	bool	isSaveSharedMemory;
	const string shMemName = "AndroidSensors";
	SharedMemory<float> shMem;
	enum {ISSAVE , LATITUDE , LONGITUDE , ACCURACY , AZIMUTH , PITCH , ROLL};

	// その他
	int		minSaveInterval;

	// privateメソッド
	// シリアルポートを開く
	void	comOpen();
	// ポートを閉じる
	void	comClose();

	void	PreparationGPSCSV();
	void	PreparationOrientationCSV();

public:
	//publicメソッド
	rcvAndroidSensors( int comport );
	~rcvAndroidSensors();

	// データの更新
	void	getSensorData();

	// データを取得するか設定
	void	setIsGetGPS(bool isGetdata);
	void	setIsSaveGPSCSV(bool isSaveCSV);
	void	setIsGetOrientation(bool isGetdata);
	void	setIsSaveOrientationCSV(bool isSaveCSV);
	void	setIsSaveSharedMemory(bool isSaveSharedMemory);

	// 保存の最低間隔[msec]
	void	setSaveMinInterval(int interval);

	// データを取得
	void	getGPSData(float retArray[3]);
	void	getOrientationData(float retArray[3]);
	
};

#endif