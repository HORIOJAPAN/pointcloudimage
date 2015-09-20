#include "urg_unko.h"

using namespace std;


int main()
{
	char z;

	int URG_COM[] = {7, 11 };
	int ARDUINO_COM = 0;

	cout << "\nunko" << sizeof(URG_COM) / sizeof(int) << endl << endl;

	getDataUNKO(URG_COM, ARDUINO_COM);

	z = getchar();

	return 0;
}