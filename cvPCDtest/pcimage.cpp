#include "pcimage.h"

#include <fstream>
#include <iostream>
#include <direct.h>
#include <windows.h>

using namespace cv;
using namespace std;

/*
*　概要:pcdファイル名を指定すると画像に変換して保存
*　引数:
*	string fileName ファイル名
*	string imgName 保存する画像名
*	int resolution 1pix何cm四方にするか
*　返り値:
*	成功 0  失敗　1
*/
int save_pcdtoimg(string fileName, string imgName ,int resolution )
{
	const int img_width = 1000;					//用意する画像の幅
	const int img_height = 1000;				//用意する画像の高さ
	const int coefficient = 100 / resolution;	//データを解像度に合わせる係数
	const int imgval_increment = 80;			//画素値の増加量

	//点群座標から画像の座標に変換した値
	int	x_val, y_val;

	string str ,x_str ,y_str;
	string searchLine("nan");
	string searchWord(" ");
	string::size_type x_pos , y_pos;

	Mat pcd_img( Size( img_width, img_height ) , CV_8U, Scalar::all(0) );

	//pcdファイルを読み込む
	ifstream ifs( fileName );
	if (ifs.fail())
	{
		cerr << "False" << endl;
		return EXIT_FAILURE;
	}

	//ヘッダ部分をとばすためのループ
	for (int i = 0 ; i <= 11; i++){
		getline(ifs, str);
	}

	while (getline(ifs, str))
	{
		//nanの列ならスルー
		if ( str.find(searchLine) != string::npos ) continue;

		//先頭から半角スペースまでの文字列に係数を掛けてint型で取得
		x_pos = str.find(searchWord);
		if (x_pos != string::npos){
			x_str = str.substr(0, x_pos);
			x_val = int(stof(x_str) * coefficient);
		}

		//xの値の後ろから半角スペースまでの文字列に係数を掛けてint型で取得
		y_pos = str.find(searchWord, x_pos + 1);
		if (y_pos != string::npos){
			y_str = str.substr( x_pos + 1 , y_pos );
			y_val = int(stof(y_str) * -coefficient);
		}

		//取得した[x,y]の画素値を増加させる
		pcd_img.data[(pcd_img.rows / 2 + y_val) * pcd_img.cols + x_val + pcd_img.cols / 5] += imgval_increment;

	}

	cout << "complete" << endl;

	//jpgで保存
	imwrite(imgName, pcd_img);

	return 0;
}

int save_floorimg(string src_imgName, string dst_imgName)
{
	string str;
	string searchLine("nan");
	string searchWord(" ");

	Mat src_img = imread( src_imgName , 0 );
	Mat dst_img = src_img.clone();

	for (int y = 0; y < src_img.rows - 1; y++){
		for (int x = 0; x < src_img.cols - 1; x++){
			if (src_img.data[y * src_img.cols + x] > 0){
				line(dst_img, Point(x, y), Point(dst_img.cols / 5, dst_img.rows / 2), 80);
			}
		}
	}
	for (int y = 0; y < src_img.rows - 1; y++){
		for (int x = 0; x < src_img.cols - 1; x++){
			if (src_img.data[y * src_img.cols + x] > 0){
				dst_img.data[y * dst_img.cols + x] = src_img.data[y * src_img.cols + x];
			}
		}
	}


	cout << "complete" << endl;

	//jpgで保存
	imwrite(dst_imgName, dst_img);

	return 0;
}

int PCIclasstest(){
	
	int img_width = 1000;
	int	img_height = 1000;
	float x_val, y_val;

	PCImage pcimage(5);

	string str, x_str, y_str;
	string searchLine("nan");
	string searchWord(" ");
	string::size_type x_pos, y_pos;
	string fileName = "./pointcloud.pcd";

	//pcdファイルを読み込む
	ifstream ifs(fileName);
	if (ifs.fail())
	{
		cerr << "False" << endl;
		return EXIT_FAILURE;
	}

	//ヘッダ部分をとばすためのループ
	for (int i = 0; i <= 11; i++){
		getline(ifs, str);
	}

	while (getline(ifs, str))
	{
		//nanの列ならスルー
		if (str.find(searchLine) != string::npos) continue;

		//先頭から半角スペースまでの文字列に係数を掛けてint型で取得
		x_pos = str.find(searchWord);
		if (x_pos != string::npos){
			x_str = str.substr(0, x_pos);
			x_val = stof(x_str);
		}

		//xの値の後ろから半角スペースまでの文字列に係数を掛けてint型で取得
		y_pos = str.find(searchWord, x_pos + 1);
		if (y_pos != string::npos){
			y_str = str.substr(x_pos + 1, y_pos);
			y_val = stof(y_str);
		}
		pcimage.writePoint( x_val, y_val );

	}

	cout << "complete" << endl;
	pcimage.savePCImage(PCImage::CENTER);

	return 0;
}

/*------------------------------
*--↓--PCImageクラスの定義--↓--
*-------------------------------*/

/*
*　概要:コンストラクタ(引数有) 
*　引数:
*	int width 　縦
*	int height　横
*	int resolution　1pix何cm四方にするか
*/
PCImage::PCImage(int width, int height, int resolution) :pcimage(imageNum , *this)
{
	//-----メンバの初期化-----
	img_width = width;
	img_height = height;
	coefficient = 100 / resolution;
	imgval_increment = 80;
	limit = 10;
	limitpix = limit * coefficient;

	//年月日時分秒で命名したディレクトリを作成
	this->getNowTime(dirname);
	if (_mkdir(dirname.c_str()) == 0){
		cout << "Made a directory named:" << dirname << endl << endl;
	}
	else{
		cout << "Failed to make the directory" << endl;
	}

	//pcimage[0]を準備する
	nowimage = ZERO;
	pcimage[nowimage].setPCI(0, 0, CENTER);
	pcimage[nowimage] = Mat(Size(width, height), CV_8U, Scalar::all(0));


	cout << "Width:" << pcimage[nowimage].cols
		<< "\nHeight:" << pcimage[nowimage].rows << endl;

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
	pcimage[nowimage].writePoint(x_val, y_val);
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
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//端に近付いて点が次の画像に入った時に処理を分岐せねば
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


	pcimage[nowimage].writePoint(x_val, y_val);
	this->checkPosition(pos_x, pos_y);

}

/*
*　概要：画像内の自己位置をチェックして必要な処理を行う
*		　画像の用意と保存，中心画像の移行
*　引数:
*	float pos_x	自己位置のx座標(m)
*	float pos_y	自己位置のy座標(m)
*　返り値:
*	0
*/
int PCImage::checkPosition(float pos_x, float pos_y)
{
	int xi = int(pos_x * coefficient);
	int yi = int(pos_y * -coefficient);

	//上下左右のリミットチェック
	//画像の端に近付いたら次の画像を用意し，離れたら保存する
	//上方向のリミットチェック
	if (!isPrepareTOP && yi < limitpix)
	{
		prepareImage(TOP);
		isPrepareTOP = true;		//フラグをtrueにする
	}
	else if (isPrepareTOP && yi > limitpix)
	{
		savePCImage(TOP);
		isPrepareTOP = false;		//フラグをfalseにする
	}
	//右方向のリミットチェック
	if (!isPrepareRIGHT && xi > pcimage[nowimage].cols - limitpix)
	{
		prepareImage(RIGHT);
		isPrepareRIGHT = true;
	}
	else if (isPrepareRIGHT && xi < pcimage[nowimage].cols - limitpix)
	{
		savePCImage(RIGHT);
		isPrepareRIGHT = false;
	}
	//下方向のリミットチェック
	if (!isPrepareBOTTOM && yi >  pcimage[nowimage].rows - limitpix)
	{
		prepareImage(BOTTOM);
		isPrepareBOTTOM = true;

	}
	else if (isPrepareBOTTOM && yi <  pcimage[nowimage].rows - limitpix)
	{
		savePCImage(BOTTOM);
		isPrepareBOTTOM = false;
	}
	//左方向のリミットチェック
	if (!isPrepareLEFT && xi < limitpix)
	{
		prepareImage(LEFT);
		isPrepareLEFT = true;
	}
	else if (isPrepareLEFT && xi > limitpix)
	{
		savePCImage(LEFT);
		isPrepareLEFT = false;
	}


	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//次の画像にシフトする処理 
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	return 0;
}

int PCImage::readPoint(int x_val, int y_val)
{
	//指定した[x,y]の画素値を返す
	return pcimage[nowimage].data[y_val * pcimage[nowimage].cols + x_val];
}

/*
*　概要：画像を保存
*　引数:
*　	Direction direction　保存したい画像の状態
*　返り値:
*	なし
*/
void PCImage::savePCImage(Direction direction)
{
	for (int i = 0; i < imageNum ; i++)
	{
		if (pcimage[i].getCondition() == direction) pcimage[i].savePCImage();
	}

}

/*
*　概要：画像を読み込む
*　引数:
*　	int emptyImageNum 画像番号
*　返り値:
*	0
*/
int PCImage::loadPCImage(int emptyImageNum)
{
	pcimage[emptyImageNum] = imread(pcimage[emptyImageNum].getName());
	if (pcimage[emptyImageNum].empty())
	{
		pcimage[emptyImageNum] = Mat(Size(img_width, img_height), CV_8U, Scalar::all(0));
	}
	return 0;
}

/*
*　概要：次の画像を用意する
*　引数:
*	Direction direction　用意する方向
*　返り値:
*	成功　0
*	失敗　-1
*/
int PCImage::prepareImage(Direction direction)
{
	int emptyImageNum;
	int xy[2];

	pcimage[nowimage].getImageNumber(xy);		//中心画像のx,y番号を取得

	switch (direction)
	{
	case TOP:
		emptyImageNum = getEmptyImage();						//空いている画像の番号を取得
		pcimage[emptyImageNum].setPCI(xy[0], xy[1] + 1, TOP);	//画像を用意
		loadPCImage(emptyImageNum);								//既に作成されている場合は読み込む
		return 0;

	case RIGHT:
		emptyImageNum = getEmptyImage();
		pcimage[emptyImageNum].setPCI(xy[0] + 1, xy[1] , RIGHT);
		loadPCImage(emptyImageNum);
		return 0;

	case BOTTOM:
		emptyImageNum = getEmptyImage();
		pcimage[emptyImageNum].setPCI(xy[0], xy[1] - 1, BOTTOM);
		loadPCImage(emptyImageNum);
		return 0;

	case LEFT:
		emptyImageNum = getEmptyImage();
		pcimage[emptyImageNum].setPCI(xy[0] - 1 , xy[1] , LEFT);
		loadPCImage(emptyImageNum);
		return 0;

	default:
		return -1;
	}
}

/*
*　概要：空いている画像の番号を返す
*　引数:
*	なし
*　返り値:
*	int	i　空いている画像の番号
*	-1		空いている画像無し
*/
int PCImage::getEmptyImage()
{
	for (int i = 0; i < 4; i++)
		if (pcimage[i].getCondition() == NONE) return i;
	return -1;
}

/*
*　概要：中心画像を指定方向にシフトする
*　引数:
*	Direction direction シフトする方向
*　返り値:
*	なし
*/
int PCImage::shiftCenterImage(Direction direction)
{

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	return 0;
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

/*------------------------------
*----↓--PCIクラスの定義--↓----
*-------------------------------*/
/*
*　概要：=演算子のオーバーロード
*		 Matクラスのものを再定義
*　引数:
*	cv::Mat& mat　Matクラスの参照
*　返り値:
*	*this　自身の参照
*/
PCImage::PCI& PCImage::PCI::operator=(cv::Mat& mat)
{
	Mat::operator=(mat);
	return *this;
}

/*
*　概要：メンバの初期化
*　引数:
*	int x	画像位置のx座標
*	int y	画像位置のy座標
*	PCImage::Direction dir	画像の状態(CENTERからみてどの方向か)
*　返り値:
*	なし
*/
void PCImage::PCI::setPCI(int x, int y, PCImage::Direction dir)
{
	imageNumXY[0] = x;
	imageNumXY[1] = y;
	imageCondition = dir;
	name = "./" + pciOut.dirname + "/" +  to_string(imageNumXY[0]) + "_" + to_string(imageNumXY[1]) + ".jpg";
}

/*
*　概要：画像の状態の取得
*　引数:
*	なし
*　返り値:
*	Direction　imageCondition　画像の状態(中心から見てどの方向か，もしくは空か)
*/
PCImage::Direction PCImage::PCI::getCondition()
{
	return imageCondition;
}

/*
*　概要：画像の位置(x,y)を返す
*　引数:
*	int xy[]　x,y座標を格納する配列
*　返り値:
*	なし
*/
void PCImage::PCI::getImageNumber(int xy[])
{
	xy[0] = imageNumXY[0];
	xy[1] = imageNumXY[1];
}

/*
*　概要：画像名を返す[./(directoryname)/(filename).jpg]
*　引数:
*	なし
*　返り値:
*	string　name　画像名
*/
string PCImage::PCI::getName()
{
	return name;
}

/*
*　概要：指定座標に点を書き込む
*　引数:
*	float x_val x座標(m)
*	float y_val y座標(m)
*　返り値:
*	なし
*/
void PCImage::PCI::writePoint(float x_val, float y_val)
{
	//x,yの値を指定した解像度に合わせる
	x_val *= pciOut.coefficient;
	y_val *= -pciOut.coefficient;

	//x,yの値を画像の位置に合わせる
	x_val -= imageNumXY[0] * pciOut.img_width - pciOut.limitpix;
	y_val -= -imageNumXY[1] * pciOut.img_height - rows / 2;

	//取得した[x,y]の画素値を増加させる
	if (data[(int)y_val * cols + (int)x_val] < (pciOut.imgval_increment * (255 / pciOut.imgval_increment))){
		data[(int)y_val * cols + (int)x_val] += pciOut.imgval_increment;
	}
	else data[(int)y_val * cols + (int)x_val] = 255;
}

/*
*　概要：画像を保存して画像領域を解放
*　引数:
*	なし
*　返り値:
*	なし
*/
void PCImage::PCI::savePCImage()
{
	imwrite( name, *this );
	this->release();
	name = "";
	imageCondition = NONE;
}

