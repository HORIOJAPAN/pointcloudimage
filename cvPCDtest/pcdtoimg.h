#ifndef _INC_PCDTOIMG
#define _INC_PCDTOIMG

#include <fstream>
#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

//pcdファイル名を指定すると画像に変換して保存
int save_pcdtoimg(std::string fileName, std::string imgName, int resolution = 1);

//点群画像に床を入れる(未完成)
int save_floorimg(std::string src_imgName, std::string dst_imgName);

int PCIclasstest();

//点群画像を作成するクラス
class PCImage
{
private:
	cv::Mat pcimage;
	std::string img_name;
	int img_width;					//用意する画像の幅
	int img_height;				//用意する画像の高さ
	int coefficient;	//データを解像度に合わせる係数
	int imgval_increment;			//画素値の増加量

public:
	PCImage(std::string name, int width, int height, int resolution);
	~PCImage();
	void writePoint(float x_val, float y_val);
	int readPoint(int x_val, int y_val);
	
	int savePCImage();

};




#endif