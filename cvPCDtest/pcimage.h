#ifndef _INC_PCIMAGE
#define _INC_PCIMAGE


#include <string>


#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

int getURGdata();

//pcdファイル名を指定すると画像に変換して保存
int save_pcdtoimg(std::string fileName, std::string imgName, int resolution = 1);

//点群画像に床を入れる(未完成)
int save_floorimg(std::string src_imgName, std::string dst_imgName);

//PCImageクラスのテストをする関数
int PCIclasstest();

//点群画像を作成するクラス
class PCImage
{
private:
	cv::Mat pcimage[4];				//画像1
	cv::Mat* pcimage_ptr;			//現在参照している画像へのポインタ

	std::string dirname;			//ディレクトリ名
	std::string img_name[4];		//保存時の画像名
	std::string* img_name_ptr;

	int img_width;					//用意する画像の幅
	int img_height;					//用意する画像の高さ
	int coefficient;				//データを解像度に合わせる係数
	int imgval_increment;			//画素値の増加量
	int limit , limitpix;			//次の画像を読み込むボーダーライン(m)(pix)

	enum Direction { NONE , TOP , RIGHT, BOTTOM, LEFT , CENTER  };
	enum NowNumber { ZERO , ONE, TWO, THREE };

	bool isPrepareTOP, isPrepareRIGHT, isPrepareBOTTOM, isPrepareLEFT;

	Direction imageCondition[4];
	NowNumber nowimage;					//現在参照している画像の番号

public:
	PCImage();
	PCImage( int resolution );
	PCImage( int width, int height, int resolution);
	~PCImage();

	void writePoint(float x_val, float y_val);
	void writePoint(float x_val, float y_val , float pos_x , float pos_y);

	int readPoint(int x_val, int y_val);
	int checkPosition(float pos_x, float pos_y);
	int prepareImage(Direction direction);
	int getEmptyImage();
	void getImageNumber(int xy[] );

	int savePCImage();

	std::string getDirname();


};


#endif