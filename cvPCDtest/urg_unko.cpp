#define _USE_MATH_DEFINES

#include "urg_unko.h"
#include "open_urg_sensor.c"


using namespace std;

// pcimageの引数．画像のサイズと解像度
// main.cppでコマンドライン引数から受け取る
extern int imgWidth, imgHeight, imgResolution;

PCImage urg_unko::pcimage;

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
urg_unko::urg_unko() //:pcimage(::imgWidth, ::imgHeight, ::imgResolution)
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
int urg_unko::getData4URG(float& dist,float& old, float& rad){
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

		//積算した距離,回転角を格納
		distance_old = old;
		distance = dist;
		radian = rad;

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
void urg_unko::set_3D_surface(int data_n)
{
	//printf(" set_3D_surface data_n = %d \n", data_n);

	(void)time_stamp;

	long min_distance;
	long max_distance;

	if (abs(distance - distance_old) >= scaninterval){//位置が設定値以上動いたとき

		// 全てのデータの X-Y の位置を取得
		//正常に取得されるデータの最大値，最小値を取得
		urg_distance_min_max(&urg, &min_distance, &max_distance);

		//pcdファイルの初期化
		pcdinit();

		float droidOrientation[3] = {};
		float droidGPS[3] = {};
		//rcvDroid.getOrientationData(droidOrientation);
		//rcvDroid.getGPSData(droidGPS);

		//データの数だけ実際の座標を計算してマップ，pcdファイルに書き込む
		for (int i = 0; i < data_n; ++i) {
			long l = data[i];	//取得した点までの距離
			double radian;
			float x, y, z;
			float pointpos[3];

			//異常値ならとばす
			if ((l <= min_distance) || (l >= max_distance)) {
				pcdWrite(0, 0, currentCoord_x / 1000, currentCoord_y / 1000, droidOrientation, droidGPS);
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
			//pointpos[0] = +cos(radian + urgpos[3]) * (x + distance - distance_old + urgpos[1]) + sin(radian + urgpos[3]) * (y + urgpos[2]) + currentCoord_x;
			//pointpos[1] = -sin(radian + urgpos[3]) * (x + distance - distance_old + urgpos[1]) + cos(radian + urgpos[3]) * (y + urgpos[2]) + currentCoord_y;

			pointpos[0] = +cos(this->radian) * x + sin(this->radian) * y + cos(this->radian) * (distance - distance_old + urgpos[1]) + currentCoord_x;
			pointpos[1] = -sin(this->radian) * x + cos(this->radian) * y - sin(this->radian) * (distance - distance_old + urgpos[1]) + currentCoord_y;

			pointpos[2] = z;

			// 座標を保存
#ifndef DEBUG_WRITELINE
			// 点のみ書き込む
			pcimage.writePoint(pointpos[0] / 1000, pointpos[1] / 1000);
			pcdWrite(pointpos[0] / 1000, pointpos[1] / 1000);
#else
			// 点を書き込んで現在地からの直線を引く
			pcimage.writePoint(pointpos[0] / 1000, pointpos[1] / 1000, currentCoord_x / 1000, currentCoord_y / 1000);
			pcdWrite(pointpos[0] / 1000, pointpos[1] / 1000, currentCoord_x / 1000, currentCoord_y / 1000, droidOrientation, droidGPS);
#endif

		}
		//１スキャン分のpcdファイルを保存
		pcdSave();
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
	ofs.open("./" + pcimage.getDirname() + "/pointcloud_" + std::to_string(pcdnum) + ".pcd");

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

void urg_unko::pcdWrite(float x, float y, float pos_x, float pos_y, float droidAngle[], float droidGPS[])
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
void urg_unko::updateCurrentCoord(float coord_x, float coord_y)
{
	currentCoord_x = coord_x;
	currentCoord_y = coord_y;
}
void urg_unko::updateCurrentCoord(float coordXY[])
{
	currentCoord_x = coordXY[0];
	currentCoord_y = coordXY[1];
}

void urg_unko::initPCImage(PCImage& pci)
{
	pcimage = pci;
}

void urg_unko::setPCImageColor(PCImage::BGR bgr)
{
	pcimage.setColor(bgr);
}