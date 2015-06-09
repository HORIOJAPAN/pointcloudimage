#include "pcdtoimg.h"

using namespace std;

int main()
{
	string fileName = "./pointcloud.pcd";
	string imgName = "pcd.jpg";

	char z;

	save_pcdtoimg(fileName, imgName);
	
	z = getchar();

	return 0;
}