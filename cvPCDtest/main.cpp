#include "pcimage.h"

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

	/*
	cv::Mat image(cv::Size(50, 50), CV_8U, cv::Scalar::all(0));
	for (int i = 0; i < 3; i++) image.data[20 * image.cols + 20] += 100;
	cv::imwrite("testimg.jpg", image);
	*/
	
	z = getchar();

	return 0;
}