#ifndef _INC_PCIMAGE
#define _INC_PCIMAGE

#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>


const int imageNum = 4;		//事前に用意する画像領域の数


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
	enum Direction { NONE, TOP, RIGHT, BOTTOM, LEFT, CENTER };		//方向を表す列挙型
	enum NowNumber { ZERO, ONE, TWO, THREE };
	int coefficient;						//データを解像度に合わせる係数

	//Matクラスを継承した点群画像クラス
	//画像番号を考慮した処理を行う
	class PCI : public cv::Mat
	{
	private:
		Direction			imageCondition;
		std::string			name;
		int					imageNumXY[2];
		PCImage&			pciOut;

	public:
		PCI();
		PCI& operator=(cv::Mat& mat);
		//PCI& operator=(PCImage& pciOut);
		void setOuter(PCImage& pciOut);

		//画像に点を書き込む
		void writePoint(float x_val, float y_val);
	};

private:
	PCI pcimage[imageNum];					//画像領域の配列
	PCI* pcimage_ptr;						//現在参照している画像へのポインタ

	std::string dirname;					//作成するディレクトリ名
	std::string img_name[imageNum];			//保存時の画像名の配列
	std::string* img_name_ptr;

	int img_width;							//用意する画像の幅
	int img_height;							//用意する画像の高さ

	int imgval_increment;					//画素値の増加量
	int limit , limitpix;					//次の画像を読み込むボーダーライン(m)(pix)

	bool isPrepareTOP = false, isPrepareRIGHT = false, isPrepareBOTTOM = false, isPrepareLEFT = false;		//上下左右について次の画像が用意されているかどうかのフラグ

	Direction imageCondition[imageNum];		//画像領域の状態
	NowNumber nowimage;						//現在参照している画像の番号

public:
	//コンストラクタ
	PCImage();
	PCImage( int resolution );
	PCImage( int width, int height, int resolution);
	//デストラクタ
	~PCImage();
	PCImage& operator=(PCImage& pc);

	//画像に点を書き込む
	void writePoint(float x_val, float y_val);
	void writePoint(float x_val, float y_val , float pos_x , float pos_y);

	//指定した点の画素を読み取る
	int readPoint(int x_val, int y_val);

	//自己位置に応じて画像の用意などの処理をする
	int checkPosition(float pos_x, float pos_y);

	//次の画像を用意する
	int prepareImage(Direction direction);

	//使われていない画像の番号を返す
	int getEmptyImage();

	//画像の位置を返す
	void getImageNumber(int xy[] );

	//画像を保存して領域を解放する
	int savePCImage( int imgNum);
	void savePCImage(Direction direction);

	//画像を読み込む
	int loadPCImage(int emptyImageNum);

	//中心画像を指定方向にシフトする
	int shiftCenterImage(Direction direction);

};



#endif