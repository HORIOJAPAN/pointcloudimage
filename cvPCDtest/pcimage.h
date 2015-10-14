#ifndef _INC_PCIMAGE
#define _INC_PCIMAGE

#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

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
	cv::Mat pcimage;				//画像

	std::string dirname;			//ディレクトリ名
	std::string img_name;			//保存時の画像名

	int img_width;					//用意する画像の幅
	int img_height;					//用意する画像の高さ
	int coefficient;				//データを解像度に合わせる係数
	int imgval_increment;			//画素値の増加量
	int limit , limitpix;			//次の画像を読み込むボーダーライン(m)(pix)

	const int lineVal = 200;		// 線を描画するときの画素値

public:
	PCImage();
	PCImage( int resolution );
	PCImage( int width, int height, int resolution);
	~PCImage();

	// 画像に点を書き込む
	void writePoint(float x_val, float y_val);
	// 点を書き込んだ後に線を描画する
	void writePoint(float x_val, float y_val , float pos_x , float pos_y);

	// 指定した二点を結ぶ直線を描画
	void writeLine(float x_val, float y_val, float pos_x, float pos_y);

	// 画素値を取得する
	int readPoint(int x_val, int y_val);

	// 画像を保存
	int savePCImage();

	// 保存したディレクトリ名を取得
	std::string getDirname();

	//現在の時刻を文字列で取得する
	static void getNowTime(std::string& nowstr);

};


#endif