#include "urg_unko.h"
using namespace std;

int imgWidth, imgHeight, imgResolution;

int main(int argc, char* argv[])
{
	//URGのCOMポートを指定
	int URG_COM[] = { 6, 27, 23 };

	//URGの位置を指定
	//地面からの高さ,中心からの横,縦方向の距離,回転角(ラジアン)
	float urgPOS[][4] = { 685.0, -325.0, 110.0, -1.5708,
		685.0, 325.0, 110.0, 1.5708,
		430.0, 0.0, 310.0, 0.0
	};

	//ArduinoのCOMポートを指定
	int ARDUINO_COM = 10;

	//pcimageの引数
	imgWidth = 1000;
	imgHeight = 1000;
	imgResolution = 5;

	
	cout << argc << endl;
	for (int i = 0; i < argc; i++)
	{
		cout << argv[i] << endl;
	}
	

	if (argc == 11)
	{
		//URGのCOMポートを指定
		URG_COM[0] = atoi(argv[1]);
		URG_COM[1] = atoi(argv[2]);
		URG_COM[2] = atoi(argv[3]);
		//ArduinoのCOMポートを指定
		ARDUINO_COM = atoi(argv[4]);

		//URGの位置を指定
		urgPOS[0][0] = atof(argv[5]);
		urgPOS[0][1] = atof(argv[6]);
		urgPOS[1][0] = atof(argv[7]);
		urgPOS[1][1] = atof(argv[8]);
		urgPOS[2][0] = atof(argv[9]);
		urgPOS[2][1] = atof(argv[10]);

		//pcimageの引数
		imgWidth = atoi(argv[11]);
		imgHeight = atoi(argv[12]);
		imgResolution = atoi(argv[13]);

		cout << "csFormから起動" << endl;
	}


	cout << "\n　接続したURGの個数：" << sizeof(URG_COM) / sizeof(URG_COM[0]) << endl << endl;

	getDataUNKO(URG_COM, urgPOS, ARDUINO_COM);

	//z = getchar();

	return 0;
}