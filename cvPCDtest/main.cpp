#include "pcdtoimg.h"

using namespace std;

int main()
{
	string fileName = "./pointcloud.pcd";
	string imgName = "pcd.jpg";
	string floorimgName = "pcd_floor.jpg";

	char z;

	//save_pcdtoimg(fileName, imgName,1);
	//save_floorimg(imgName, floorimgName);

	PCIclasstest();
	
	z = getchar();

	return 0;
}