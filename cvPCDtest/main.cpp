#include "urg_unko.h"
using namespace std;

int main()
{
	char z;

	//URGのCOMポートを指定
	int URG_COM[] = {4, 11 };

	//URGの位置を指定
	float urgPOS[][3] = { 1100.0, 285.0, 0.0,
							20.0, 443.0, 0.0 };

	//ArduinoのCOMポートを指定
	int ARDUINO_COM = 0;

	cout << "\n　接続したURGの個数：" << sizeof(URG_COM) / sizeof(URG_COM[0]) << endl << endl;

	getDataUNKO(URG_COM, urgPOS, ARDUINO_COM);

	z = getchar();

	return 0;
}