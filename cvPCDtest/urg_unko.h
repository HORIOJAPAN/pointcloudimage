#ifndef _INC_URG_UNKO
#define _INC_URG_UNKO

#include "urg_sensor.h"
#include "urg_utils.h"
#include "urg_open.h"
#include "pcimage.h"

#include <Windows.h>
#include <fstream>

#define URGCOUNT 2

int getDataUNKO(int URG_COM[], int ARDUINO_COM );

class urg_unko
{
private:
	int COMport;

	std::ofstream ofs;
	int pcdnum;
	int pcdcount;

	urg_t urg;
	long *data = NULL;
	long time_stamp;
	PCImage pcimage;

	int disconnectURG();
	int connectURG();
	void set_3D_surface(int data_n);

	void pcdWrite(float x, float y);
	void pcdSave();

public:
	urg_unko();
	~urg_unko();

	void init(int COM);
	int getData4URG(float& dist, float& rad);

	void pcdinit();

};


#endif