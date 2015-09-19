#include "urg_unko.h"

#include <opencv2/nonfree/nonfree.hpp> // SIFT・SURFモジュール用

//#include "Timer.h"

using namespace std;


int main()
{
	string fileName = "./pointcloud.pcd";
	string imgName = "pcd.jpg";
	string floorimgName = "pcd_floor.jpg";

	/*
	Timer t;
	t.Start(0);
	//FeatureMatching("0_0_1.jpg", "0_0_2.jpg", "ORB", "ORB","BruteForce");
	cv::Mat im = cv::imread("0_0_2.jpg");
	cv::Mat tmp = cv::imread("0_0_2_2.jpg");
	cv::Mat match;
	cv::matchTemplate(im, tmp, match, CV_TM_SQDIFF);
	cv::Point pt;
	minMaxLoc(match, 0, 0, 0, &pt);
	rectangle(im, pt, cv::Point(pt.x + tmp.cols, pt.y + tmp.rows), cv::Scalar(0, 0, 255), 2, 8, 0);
	cv::imshow("Show image", im);

	t.End(0,Timer::millisec);

	cv::waitKey(0);*/
	
	char z;

	//save_pcdtoimg(fileName, imgName,1);
	//save_floorimg(imgName, floorimgName);

	//PCIclasstest();

	int URG_COM[] = {7, 11 };
	int ARDUINO_COM = 0;

	cout << "\nunko" << sizeof(URG_COM) / sizeof(int) << endl << endl;

	getDataUNKO(URG_COM, ARDUINO_COM);

	//getURGdata();

	/*
	cv::Mat image(cv::Size(50, 50), CV_8U, cv::Scalar::all(0));
	for (int i = 0; i < 3; i++) image.data[20 * image.cols + 20] += 100;
	cv::imwrite("testimg.jpg", image);
	*/
	
	//z = getchar();

	return 0;
}