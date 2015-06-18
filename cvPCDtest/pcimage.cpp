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


//現在の時刻を文字列で取得する
void getNowTime(string& nowstr){
	SYSTEMTIME st;
	GetSystemTime(&st);
	char szTime[25] = { 0 };
	// wHourを９時間足して、日本時間にする
	wsprintf(szTime, "%04d%02d%02d%02d%02d%02d",
		st.wYear, st.wMonth, st.wDay,
		st.wHour + 9, st.wMinute, st.wSecond);
	nowstr = szTime;
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
		pcimage.writePoint(x_val, y_val);

	}

	cout << "complete" << endl;
	pcimage.savePCImage(0);

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
PCImage::PCImage(int width, int height, int resolution) :pcimage(4 , *this)
{
	//-----メンバの初期化-----
	img_name[0] = "0_0";
	img_width = width;
	img_height = height;
	coefficient = 100 / resolution;
	imgval_increment = 80;
	limit = 10;
	limitpix = limit * coefficient;

	for (auto& image : pcimage)
	{
		image = Mat(Size(width, height), CV_8U, Scalar::all(0));
	}
	for (auto& icondition : imageCondition)
	{
		icondition = NONE;
	}

	//まずpci0を参照する
	pcimage_ptr = &pcimage[0];
	img_name_ptr = &img_name[0];
	nowimage = ZERO;
	imageCondition[0] = CENTER;

	//年月日時分秒で命名したディレクトリを作成
	getNowTime(dirname);
	if (_mkdir(dirname.c_str()) == 0){
		cout << "Made a directory named:" << dirname << endl << endl;
	}
	else{
		cout << "Failed to make the directory" << endl;
	}


	cout << "Image name : " << *img_name_ptr << endl;
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
	//x,yの値を指定した解像度に合わせる
	x_val *= coefficient;
	y_val *= -coefficient;

	//取得した[x,y]の画素値を増加させる
	if (pcimage[nowimage].data[(pcimage[nowimage].rows / 2 + (int)y_val) * pcimage[nowimage].cols + (int)x_val + limitpix] < (imgval_increment * (255 / imgval_increment))){
		pcimage[nowimage].data[(pcimage[nowimage].rows / 2 + (int)y_val) * pcimage[nowimage].cols + (int)x_val + limitpix] += imgval_increment;
	}
	else pcimage[nowimage].data[(pcimage[nowimage].rows / 2 + (int)y_val) * pcimage[nowimage].cols + (int)x_val + limitpix] = 255;
	

	//imshow("pci", pcimage);
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

	////画像位置に合わせて書き込む座標を補正する必要あり

	this->writePoint(x_val, y_val);
	this->checkPosition(pos_x, pos_y);

}

/*
*　概要：画像内の自己位置をチェックして必要な処理を行う
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
*　	int imgNum　画像番号
*　返り値:
*	0
*/
int PCImage::savePCImage( int imgNum )
{
	imwrite(dirname + "/" + *img_name_ptr + ".jpg", pcimage[imgNum]);
	pcimage[imgNum].release();
	img_name[imgNum] = "";
	imageCondition[imgNum] = NONE;

	return 0;
}
/*
*　概要：画像を保存
*　引数:
*　	int imgNum　画像番号
*　返り値:
*	0
*/
void PCImage::savePCImage(Direction direction)
{
	for (int i = 0; i < imageNum ; i++)
	{
		if (imageCondition[i] == direction) this->savePCImage(i);
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
	(Mat)pcimage[emptyImageNum] = imread(img_name[emptyImageNum]);
	if (pcimage[emptyImageNum].empty())
	{
		(Mat)pcimage[emptyImageNum] = Mat(Size(img_width, img_height), CV_8U, Scalar::all(0));
	}

	return 0;
}

/*
*　概要：次の画像を用意する
*　引数:
*	Direction direction　近付いている方向
*　返り値:
*	なし
*/
int PCImage::prepareImage(Direction direction)
{
	int emptyImageNum;
	int xy[2];

	getImageNumber(xy);		//中心画像のx,y番号を取得

	switch (direction)
	{
	case TOP:
		emptyImageNum = getEmptyImage();											//空いている画像の番号を取得
		img_name[emptyImageNum] = to_string(xy[0]) + "_" + to_string(xy[1] + 1);	//用意する画像名を作成
		imageCondition[emptyImageNum] = TOP;										//画像の状態を変更
		loadPCImage(emptyImageNum);													//既に作成されている場合を読み込む
		return 0;

	case RIGHT:
		emptyImageNum = getEmptyImage();
		img_name[emptyImageNum] = to_string(xy[0] + 1) + "_" + to_string(xy[1]);
		imageCondition[emptyImageNum] = RIGHT;
		loadPCImage(emptyImageNum);
		return 0;

	case BOTTOM:
		emptyImageNum = getEmptyImage();
		img_name[emptyImageNum] = to_string(xy[0]) + "_" + to_string(xy[1] - 1);
		imageCondition[emptyImageNum] = BOTTOM;
		loadPCImage(emptyImageNum);
		return 0;

	case LEFT:
		emptyImageNum = getEmptyImage();
		img_name[emptyImageNum] = to_string(xy[0] - 1) + "_" + to_string(xy[1]);
		imageCondition[emptyImageNum] = LEFT;
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
		if (imageCondition[i] == NONE) return i;
	return -1;
}


/*
*　概要：現在の中心画像の番号(x,y)を返す
*　引数:
*	int xy[]　x,y座標を格納する配列
*　返り値:
*	なし
*/
void PCImage::getImageNumber( int xy[] )
{
	string::size_type x_pos;
	
	x_pos = img_name[nowimage].find("_");
	if (x_pos != string::npos){
		xy[0] = stoi(img_name[nowimage].substr(0, x_pos));
		xy[1] = stoi(img_name[nowimage].substr(x_pos + 1));
	}
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

	return 0;
}

/*------------------------------
*----↓--PCIクラスの定義--↓----
*-------------------------------*/
PCImage::PCI& PCImage::PCI::operator=(cv::Mat& mat)
{
	Mat::operator=(mat);
	return *this;
}
void PCImage::PCI::writePoint(float x_val, float y_val)
{
	//x,yの値を指定した解像度に合わせる
	x_val *= pciOut.coefficient;
	y_val *= -pciOut.coefficient;

	//取得した[x,y]の画素値を増加させる
	if (data[(rows / 2 + (int)y_val) * cols + (int)x_val + pciOut.limitpix] < (pciOut.imgval_increment * (255 / pciOut.imgval_increment))){
		data[(rows / 2 + (int)y_val) * cols + (int)x_val + pciOut.limitpix] += pciOut.imgval_increment;
	}
	else data[(rows / 2 + (int)y_val) * cols + (int)x_val + pciOut.limitpix] = 255;


	//imshow("pci", pcimage);
}