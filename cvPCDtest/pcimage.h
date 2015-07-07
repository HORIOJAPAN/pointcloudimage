#ifndef _INC_PCIMAGE
#define _INC_PCIMAGE

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>


const int imageNum = 4;		//事前に用意する画像領域の数

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
public:
	//enum Direction { NONE, TOP, RIGHT, LEFT, BOTTOM, CENTER, UPPERRIGHT, LOWERRIGHT, UPPERLEFT, LOWERLEFT };		//方向を表す列挙型
	//enum Number { ZERO, ONE, TWO, THREE };							//画像番号

	//Matクラスを継承した点群画像クラス
	//画像位置を考慮した処理を行う
	class PCI;
	std::string dirname;					//作成するディレクトリ名

private:
	std::vector<PCI> pcimage;				//画像領域の配列


	int img_width;							//用意する画像の幅
	int img_height;							//用意する画像の高さ
	int coefficient;						//データを解像度に合わせる係数
	int imgval_increment;					//画素値の増加量
	int limit , limitpix;					//次の画像を読み込むボーダーライン(m),(pix)

	//上下左右について次の画像が用意されているかどうかのフラグ
	bool isPrepareTOP = false, isPrepareRIGHT = false, isPrepareBOTTOM = false, isPrepareLEFT = false;

	int nowimage;						//現在走行している画像の番号

public:
	//コンストラクタ
	PCImage();
	PCImage( int resolution );
	PCImage( int width, int height, int resolution);
	PCImage(PCImage& pcimage_ptr);
	//デストラクタ
	~PCImage();

	//画像に点を書き込む
	void writePoint(float x_val, float y_val);
	void writePoint(float x_val, float y_val , float pos_x , float pos_y);

	//指定した点の画素を読み取る
	int readPoint(int x_val, int y_val);

	//自己位置に応じて画像の用意などの処理をする
	int checkPosition(float pos_x, float pos_y);

	//画像の領域番号を問い合わせると真偽を返す
	bool checkPrepare(int x, int y);

	//次の画像を用意する
	//int prepareImage(Direction direction);
	int prepareImage(int x , int y);

	//使われていない画像の番号を返す
	int getEmptyImage();

	//画像を保存して領域を解放する
	//void savePCImage(Direction direction);
	void savePCImage(int x , int y);

	//画像を読み込む
	int loadPCImage(int emptyImageNum);

	//中心画像を指定方向にシフトする
	//int shiftCenterImage(Direction direction);
	int shiftCenterImage(int x, int y);

	//現在の時刻を文字列で取得する
	static void getNowTime(std::string& nowstr);

};

//Matクラスを継承した点群画像クラス
//画像位置を考慮した処理を行う
class PCImage::PCI : public cv::Mat
{
private:
	PCImage& pciOut;	//PCImageクラスへの参照

	//PCImage::Direction	imageCondition;			//現在の状態
	std::string			name;					//保存時の名前
	int					imageNumXY[2];			//画像の位置

public:
	PCI(PCImage& pcimage_outer);
	PCI& operator=(cv::Mat& mat);


	//画像情報をセットする
	//void setPCI(int x, int y, PCImage::Direction dir);
	void setPCI(int x, int y);

	//画像の状態を返す
	//PCImage::Direction getCondition();
	//画像の状態を変更する
	//void setCondition(PCImage::Direction dir);

	//画像の位置を返す
	void getImageNumber(int xy[]);

	//画像名を返す
	// [./(directoryname)/(filename).jpg]
	std::string getName();

	//画像の領域番号を問い合わせると真偽を返す
	bool isCoordinates( int x , int y);
	bool isCoordinates(int xy[]);

	//画像の領域番号セットする
	void setCoordinates(int x, int y);
	void setCoordinates(int xy[]);

	//画像に点を書き込む
	int writePoint(float x_val, float y_val);

	//画像を保存して領域を解放する
	void release();
	//画像を保存する
	void savePCImage();
};

#endif