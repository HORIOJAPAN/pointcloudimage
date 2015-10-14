#include "../cvPCDtest/pcimage.h"

#include <fstream>
#include <iostream>
#include <direct.h>

using namespace cv;
using namespace std;

/*
*pcdファイル名を指定すると画像に変換して保存
*引数:
*	string fileName ファイル名
*	string imgName 保存する画像名
*	int resolution 1pix何cm四方にするか
*返り値:
*	成功 0  失敗　1
*/
int save_pcdtoimg(string fileName, string imgName, int resolution)
{
	const int img_width = 1000;					//用意する画像の幅
	const int img_height = 1000;				//用意する画像の高さ
	const int coefficient = 100 / resolution;	//データを解像度に合わせる係数
	const int imgval_increment = 80;			//画素値の増加量

	//点群座標から画像の座標に変換した値
	int	x_val, y_val;

	string str, x_str, y_str;
	string searchLine("nan");
	string searchWord(" ");
	string::size_type x_pos, y_pos;

	Mat pcd_img(Size(img_width, img_height), CV_8U, Scalar::all(0));

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
			x_val = int(stof(x_str) * coefficient);
		}

		//xの値の後ろから半角スペースまでの文字列に係数を掛けてint型で取得
		y_pos = str.find(searchWord, x_pos + 1);
		if (y_pos != string::npos){
			y_str = str.substr(x_pos + 1, y_pos);
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

	Mat src_img = imread(src_imgName, 0);
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

	PCImage pcimage(3);

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
	pcimage.savePCImage();

	return 0;
}