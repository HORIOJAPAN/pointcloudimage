#include "pcimage.h"

#include <fstream>
#include <iostream>
#include <direct.h>
#include <windows.h>

using namespace cv;
using namespace std;

/*
*　コンストラクタ(引数有) 
*引数:
*	string name 保存時のファイル名
*	int width 　縦
*	int height　横
*	int resolution　1pix何cm四方にするか
*/
PCImage::PCImage(int width, int height, int resolution)
{
	//-----メンバの初期化-----
	img_name = "0_0";
	img_width = width;
	img_height = height;
	coefficient = 100 / resolution;
	imgval_increment = 80;
	limit = 10;
	limitpix = limit * coefficient;

	pcimage = Mat(Size(width, height), CV_8U, Scalar::all(0));

	//年月日時分秒で命名したディレクトリを作成
	getNowTime(dirname);
	if (_mkdir(dirname.c_str()) == 0){
		cout << "Made a directory named:" << dirname << endl << endl;
	}
	else{
		cout << "Failed to make the directory" << endl;
	}


	cout << "Image name : " << img_name << endl;
	cout << "Width:" << pcimage.cols
		<< "\nHeight:" << pcimage.rows << endl;
	Sleep(2000);

}

//コンストラクタのオーバーロード
PCImage::PCImage()
{
	this->PCImage::PCImage( 1000, 1000, 5);

}
PCImage::PCImage(int resolution )
{
	this->PCImage::PCImage( 1000, 1000, resolution);

}

PCImage::~PCImage()
{
}

/*
*　概要：指定座標に点を書き込む
*　引数:
*	float x_val x座標(m)
*	float y_val y座標(m)
*　返り値:
*	なし
*/
void PCImage::writePoint(float x_val, float y_val)
{
	//x,yの値を指定した解像度に合わせる
	x_val *= coefficient;
	y_val *= -coefficient;

	//取得した[x,y]の画素値を増加させる.オーバーフローしないように分岐.
	if (pcimage.data[(pcimage.rows / 2 + (int)y_val) * pcimage.cols + (int)x_val + limitpix] < (imgval_increment * (255 / imgval_increment))){
		pcimage.data[(pcimage.rows / 2 + (int)y_val) * pcimage.cols + (int)x_val + limitpix] += imgval_increment;
	}
	else pcimage.data[(pcimage.rows / 2 + (int)y_val) * pcimage.cols + (int)x_val + limitpix] = 255;
	
}

void PCImage::writeLine(float x_val, float y_val, float pos_x, float pos_y)
{
	//x,yの値を指定した解像度に合わせる
	x_val *= coefficient;
	y_val *= -coefficient;
	x_val = (int)x_val + limitpix;
	y_val = pcimage.rows / 2 + (int)y_val;


	pos_x *= coefficient;
	pos_y *= -coefficient;
	pos_x = (int)pos_x + limitpix;
	pos_y = pcimage.rows / 2 + (int)pos_y;

	//取得した[x,y]と現在地を線で結ぶ
	line(pcimage, Point(x_val, y_val), Point(pos_x, pos_y), 100);

}

/*
*　概要：指定座標(絶対座標)に点を書き込む
*　引数:
*	float x_val 書き込む点のx座標(m)
*	float y_val 書き込む点のy座標(m)
*	float pos_x	自己位置のx座標(m)
*	float pos_y	自己位置のy座標(m)
*　返り値:
*	なし
*/
void PCImage::writePoint(float x_val, float y_val, float pos_x, float pos_y)
{
	
	this->writeLine(x_val, y_val , pos_x ,pos_y );
	this->writePoint(x_val, y_val);

}

int PCImage::readPoint(int x_val, int y_val)
{
	//指定した[x,y]の画素値を返す
	return pcimage.data[y_val * pcimage.cols + x_val];
}

/*
*　概要：画像を保存
*　引数:
*　	なし
*　返り値:
*	0
*/
int PCImage::savePCImage()
{
	imwrite(dirname + "/" + dirname + ".jpg", pcimage);
	//resize(*pcimage_ptr, *pcimage_ptr, Size(1000, 1000));
	//imshow(*img_name_ptr, *pcimage_ptr);
	//waitKey(1);
	return 0;
}

std::string PCImage::getDirname()
{
	return dirname;
}

//現在の時刻を文字列で取得する
void PCImage::getNowTime(string& nowstr){
	SYSTEMTIME st;
	GetSystemTime(&st);
	char szTime[25] = { 0 };
	// wHourを９時間足して、日本時間にする
	wsprintf(szTime, "%04d%02d%02d%02d%02d%02d",
		st.wYear, st.wMonth, st.wDay,
		st.wHour + 9, st.wMinute, st.wSecond);
	nowstr = szTime;
}
