#ifndef _INC_URG_UNKO
#define _INC_URG_UNKO

#include "urg_sensor.h"
#include "urg_utils.h"
#include "urg_open.h"
#include "pcimage.h"

#include <Windows.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

//接続したURGの個数を自動で判断するようにしたマクロ
#define getDataUNKO(aURGCOM , aURGPOS , ARDUINOCOM) getDataUNKOOrigin( (aURGCOM),(aURGPOS),(ARDUINOCOM),sizeof((aURGCOM))/sizeof(aURGCOM[0])) 

//指定したCOMポートを閉じる
int CommClose(HANDLE hComm);
//Arduinoのハンドルを取得
void getArduinoHandle(int arduinoCOM, HANDLE& hComm);
//urg_unkoのmainループ
void getDataUNKOOrigin(int URG_COM[], float URGPOS[][3], int ARDUINO_COM, int NumOfURG);

/*
*
*　URGでデータを取得してマッピングを行うクラス
*
*/
class urg_unko{
private:
	/*
	*	メンバ変数
	*/
	int COMport;	//URGのCOMポート

	std::ofstream ofs;	//ファイルストリームオブジェクト．pcdファイル作成に用いる
	int pcdnum;			//pcdファイルの番号をカウントする変数
	int pcdcount;		//pcdファイルに書き込む点の数をカウントする変数

	float urgpos[3];	//NCWCの回転中心から見たURGの位置．センサの地面からの高さ，センサの基準位置からの距離，および水平面からの俯角
	
	urg_t urg;			//URGオブジェクト
	long *data = NULL;	
	long time_stamp;

	PCImage pcimage;	//マップ画像作成用クラス

	/*
	*	privateなメソッド
	*/

	//URGとの接続を切断
	int disconnectURG();
	//URGと接続
	int connectURG();

	//取得したデータから実際の二次元情報を計算してマップ，pcdファイルへの書き込みを行う
	void set_3D_surface(int data_n);

	//pcdファイルを作成して書き込む準備を行う
	void pcdinit();
	//pcdファイルへ点を書き込む
	void pcdWrite(float x, float y);
	void pcdWrite(float x, float y, float pos_x, float pos_y);
	//pcdファイルへの書き込みを終了して保存する
	void pcdSave();

public:
	/*
	*	publicなメソッド
	*/

	//コンストラクタ
	urg_unko();
	urg_unko(int imgWidth, int imgHeight, int imgResolution);
	//デストラクタ
	~urg_unko();

	//自身の初期化処理を行う
	void init(int COM , float pos[]);
	//URGからデータを取得するメソッド
	int getData4URG(float& dist, float& rad);

	std::string	getDirName();
};


#endif