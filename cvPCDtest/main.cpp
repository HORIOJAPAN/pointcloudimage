#include "urg_unko.h"
using namespace std;

int imgWidth, imgHeight, imgResolution;

int main(int argc, char* argv[])
{
	char z;
	//URGのCOMポートを指定
	int URG_COM[] = { 21, 22 };

	//URGの位置を指定
	float urgPOS[][3] = { 1100.0, 285.0, 0.0,
		20.0, 443.0, 0.0 };

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
		//ArduinoのCOMポートを指定
		ARDUINO_COM = atoi(argv[3]);

		//URGの位置を指定
		urgPOS[0][0] = atof(argv[4]);
		urgPOS[0][1] = atof(argv[5]);
		urgPOS[1][0] = atof(argv[6]);
		urgPOS[1][1] = atof(argv[7]);

		//pcimageの引数
		imgWidth = atoi(argv[8]);
		imgHeight = atoi(argv[9]);
		imgResolution = atoi(argv[10]);

		cout << "csFormから起動" << endl;
	}


	cout << "\n　接続したURGの個数：" << sizeof(URG_COM) / sizeof(URG_COM[0]) << endl << endl;

	getDataUNKO(URG_COM, urgPOS, ARDUINO_COM);

	//z = getchar();

	return 0;
}