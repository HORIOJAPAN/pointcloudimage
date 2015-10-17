#include "pcimage.h"

#include <fstream>
#include <iostream>
#include <direct.h>
#include <windows.h>

using namespace cv;
using namespace std;

/*------------------------------
*--↓--PCImageクラスの定義--↓--
*-------------------------------*/

/*
*　コンストラクタ(引数有) 
*引数:
*	string name 保存時のファイル名
*	int width 　縦
*	int height　横
*	int resolution　1pix何cm四方にするか
*/
PCImage::PCImage(int width, int height, int resolution) : pcimage(imageNum, *this)
{
	//-----メンバの初期化-----
	img_width = width;
	img_height = height;
	coefficient = 100 / resolution;
	imgval_increment = 80;
	limit = 10;
	limitpix = limit * coefficient;

	//年月日時分秒で命名したディレクトリを作成
	getNowTime(dirname);
	if (_mkdir(dirname.c_str()) == 0){
		cout << "Made a directory named:" << dirname << endl << endl;
	}
	else{
		cout << "Failed to make the directory" << endl;
	}

	//pcimage[0]を準備する
	nowimage = 0;
	pcimage[nowimage].setPCI(0, 0);
	pcimage[nowimage] = Mat(Size(width, height), CV_8U, Scalar::all(0));


	cout << "Width:" << pcimage[nowimage].cols
		<< "\nHeight:" << pcimage[nowimage].rows << endl;

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

//デストラクタ
//解放されていない画像領域があれば念のため保存しておく
PCImage::~PCImage()
{
	for (int i = 0; i < imageNum; i++)
		if (!pcimage[i].empty())
		{
			pcimage[i].savePCImage();
}
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
void PCImage::writeLine(float x_val, float y_val, float pos_x, float pos_y)
{
	int XY[2];
	pcimage[nowimage].getImageNumber(XY);		//中心画像のX,Y番号を取得

	//x,yの値を指定した解像度に合わせる
	x_val *= coefficient;
	y_val *= coefficient;
	x_val = (int)x_val - XY[0] * img_width + limitpix;
	y_val = (int)y_val - XY[1] * img_height + img_height / 2;

	pos_x *= coefficient;
	pos_y *= coefficient;
	pos_x = (int)pos_x - XY[0] * img_width + limitpix;
	pos_y = (int)pos_y - XY[1] * img_height + img_height / 2;

	//取得した[x,y]と現在地を線で結ぶ
	line(pcimage[nowimage], Point(x_val, y_val), Point(pos_x, pos_y), 100);

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

	int ret;
	ret = pcimage[nowimage].writePoint(x_val, y_val);
	if (ret)
	{
		pcimage[ret - 1].writePoint(x_val, y_val);
}

	// 自己位置が変化していなければ処理を返す
	if (selfPos_x == pos_x && selfPos_y == pos_y) return;

	// 自己位置に応じた処理を行う
	this->checkPosition(pos_x, pos_y);

	imshow(dirname, pcimage[nowimage]);
	waitKey(1);

	selfPos_x = pos_x;
	selfPos_y = pos_y;

}

int PCImage::readPoint(int x_val, int y_val)
{
	//指定した[x,y]の画素値を返す
	return pcimage[nowimage].data[y_val * pcimage[nowimage].cols + x_val];
}

/*
*　概要：画像を保存
*　引数:
*　	
*　返り値:
*	0
*/
void PCImage::savePCImage(int x, int y)
{
	for (int i = 0; i < imageNum; i++)
	{
		if (pcimage[i].isCoordinates(x, y)) pcimage[i].savePCImage();
}
}

std::string PCImage::getDirname()
{
	return dirname;
}

//⊂二二二（ ＾ω＾）二⊃ ﾌﾞｰﾝ
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


/*
*
*  ⊂二二二（ ＾ω＾）二⊃     画像領域管理の中枢     ⊂二（＾ω＾ ）二二二⊃
*
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
	//x,yの値を指定した解像度に合わせる
	int xi = int(pos_x * coefficient);
	int yi = int(pos_y * coefficient);

	//現在の画像のXY
	int XY[2];

	pcimage[nowimage].getImageNumber(XY);		//中心画像のX,Y番号を取得

	// 座標を画像内に合わせる
	xi = xi - XY[0] * img_width + limitpix;
	yi = yi - XY[1] * img_height + img_height / 2;

	// 自己位置が指定範囲を超えていたら...
	if (xi < limitpix || img_width - limitpix < xi || yi < limitpix || img_height - limitpix < yi){
		this->outsideProcess(xi, yi, XY);
	}
	// 指定範囲を超えていないのに解放されていない画像があれば解放
	else
	{
		for (int i = 0; i < imageNum; i++)
		{
			if (i == nowimage) continue;
			if (!pcimage[i].isEmpty()) pcimage[i].release();
		}
	}

	return 0;
}

void PCImage::outsideProcess(int pos_x, int pos_y, int XY[2])
{
	int flag_x = 0;
	int flag_y = 0;

	//↑上方向のリミットチェック↑
	if (pos_y < limitpix)
	{
		// リミットを超えていた場合
		flag_y = -1;
	}
	//↓下方向のリミットチェック↓
	else if (img_width - limitpix < pos_y)
	{
		//リミットを超えていた場合
		flag_y = 1;
	}

	//→右方向のリミットチェック→
	if (img_height - limitpix < pos_x)
	{
		// リミットを超えていた場合
		flag_x = 1;
	}
	//←左方向のリミットチェック←
	else if (pos_x < limitpix)
	{
		// リミットを超えていた場合
		flag_x = -1;
	}

	// X,Y共にリミット外
	if (flag_x && flag_y)
	{
		prepareImage(XY[0] + flag_x, XY[1] + flag_y);
		prepareImage(XY[0] + flag_x, XY[1]);
		prepareImage(XY[0], XY[1] + flag_y);
	}
	// どっちかだけリミット
	else
	{
		prepareImage(XY[0] + flag_x, XY[1] + flag_y);

		for (int i = 0; i < imageNum; i++)
		{
			if (i == nowimage) continue;
			if (!pcimage[i].isEmpty() && !pcimage[i].isCoordinates(XY[0] + flag_x, XY[1] + flag_y)) pcimage[i].release();
		}
	}

	// 画像外ならそっちの画像にシフトする(メソッドの分け方を変えるとスッキリすると思われるけど動いたからいいや)
	flag_x = 0;
	flag_y = 0;
	//↑上方向のリミットチェック↑
	if (pos_y < 0)
	{
		// リミットを超えていた場合
		flag_y = -1;
	}
	//↓下方向のリミットチェック↓
	else if (pcimage[nowimage].rows  < pos_y)
	{
		//リミットを超えていた場合
		flag_y = 1;
	}

	//→右方向のリミットチェック→
	if (pcimage[nowimage].cols < pos_x)
	{
		// リミットを超えていた場合
		flag_x = 1;
	}
	//←左方向のリミットチェック←
	else if (pos_x < 0)
	{
		// リミットを超えていた場合
		flag_x = -1;
	}
	//flag_xもしくはflag_yが0以外なら画像シフトを実行
	if (flag_x || flag_y) shiftCenterImage(flag_x, flag_y);
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
	// 画像を読み込む
	pcimage[emptyImageNum] = imread(pcimage[emptyImageNum].getName());
	// 画像が存在していなかった場合は新規作成
	if (pcimage[emptyImageNum].empty())
	{
		pcimage[emptyImageNum] = Mat(Size(img_width, img_height), CV_8U, Scalar::all(0));
	}
	return 0;
}

/*
*　概要：次の画像を用意する
*　引数:
*
*　返り値:
*	成功　0
*	失敗　-1
*/
int PCImage::prepareImage(int X, int Y)
{
	// 既に用意されていたら処理を返す
	if (checkPrepare(X, Y)) return 1;

	int emptyImageNum;
	int xy[2];

	pcimage[nowimage].getImageNumber(xy);		//中心画像のX,Y番号を取得

	emptyImageNum = getEmptyImage();						//空いている画像の番号を取得
	pcimage[emptyImageNum].setPCI(X, Y);					//画像を用意
	loadPCImage(emptyImageNum);								//既に作成されている場合は読み込む
	return 0;

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
	for (int i = 0; i < imageNum; i++){
		cout << i << " isEmpty:" << pcimage[i].isEmpty() << endl;
		if (pcimage[i].isEmpty()) return i;
	}
	return -1;
}

/*
*　概要：中心画像を指定方向にシフトする
*　引数:
*	int x　画像領域座標における現在画像からのx方向変位(-1~1)
*	int y　画像領域座標における現在画像からのy方向変位(-1~1)
*　返り値:
*	なし
*/
int PCImage::shiftCenterImage(int X, int Y)
{
	cout << "Shift center image" << endl;

	int nowXY[2];
	pcimage[nowimage].getImageNumber(nowXY);	//現在画像の座標を取得

	//指定した座標の画像があれば画像番号をnowimageに代入
	for (int i = 0; i < imageNum; i++)
	{
		if (pcimage[i].isCoordinates(nowXY[0] + X, nowXY[1] + Y))
		{
			nowimage = i;
			return 0;
		}
	}
	return -1;
}

/*
*　概要：指定した画像領域座標の画像が用意されていると真を返す
*　引数:
*	int X　画像領域座標における現在画像からのx方向変位(-1~1)
*	int Y　画像領域座標における現在画像からのy方向変位(-1~1)
*　返り値:
*	True or False
*/
bool PCImage::checkPrepare(int X, int Y)
{
	for (int i = 0; i < imageNum; i++)
	{
		if (pcimage[i].isCoordinates(X, Y)) return true;
	}
	return false;
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

PCImage::PCI::PCI(PCImage& pcimage_outer) : pciOut(pcimage_outer)
{
	//念のため領域を解放しておく
	Mat::release();
	name = "";
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
void PCImage::PCI::setPCI(int X, int Y)
{
	imageNumXY[0] = X;
	imageNumXY[1] = Y;
	name = "./" + pciOut.dirname + "/" + to_string(imageNumXY[0]) + "_" + to_string(imageNumXY[1]) + ".jpg";
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
int PCImage::PCI::writePoint(float x_val, float y_val)
{
	//x,yの値を指定した解像度に合わせる
	x_val *= pciOut.coefficient;
	y_val *= pciOut.coefficient;

	//x,yの値を画像の位置に合わせる
	x_val = x_val - imageNumXY[0] * pciOut.img_width + pciOut.limitpix;
	y_val = y_val - imageNumXY[1] * pciOut.img_height + rows / 2;

	//当画像領域内の点か確認して当画像領域外の場合は該当領域のIDを返す
	int x_coord = 0;
	int y_coord = 0;
	if (x_val < 0)
	{
		x_coord = -1;
	}
	else if (x_val >= cols)
	{
		x_coord = 1;
	}
	if (y_val < 0)
	{
		y_coord = -1;
	}
	else if (y_val >= rows)
	{
		y_coord = 1;
	}
	//cout << x_coord << "," << y_coord << endl;
	//cout << x_val << "," << y_val << endl;
	//cout << imageNumXY[0] << "," << imageNumXY[1] << endl;
	if (x_coord || y_coord)
	{
		for (int i = 0; i < imageNum; i++)
		{
			if (pciOut.pcimage[i].isCoordinates(imageNumXY[0] + x_coord, imageNumXY[1] + y_coord)) return i + 1;
		}
		return -1;
	}

	//取得した[x,y]の画素値を増加させる
	if (data[(int)y_val * cols + (int)x_val] < (pciOut.imgval_increment * (255 / pciOut.imgval_increment))){
		data[(int)y_val * cols + (int)x_val] += pciOut.imgval_increment;
	}
	else data[(int)y_val * cols + (int)x_val] = 255;

	return 0;
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
	imwrite(name, *this);
}
void PCImage::PCI::release()
{
	imwrite(name, *this);
	this->Mat::release();
	name = "";
}

bool PCImage::PCI::isCoordinates(int x, int y)
{
	if (imageNumXY[0] == x && imageNumXY[1] == y) return true;
	return false;
}
bool PCImage::PCI::isCoordinates(int xy[])
{
	if (imageNumXY[0] == xy[0] && imageNumXY[1] == xy[1]) return true;
	return false;
}

bool PCImage::PCI::isEmpty()
{
	if (this->Mat::empty()) return true;
	else return false;
}