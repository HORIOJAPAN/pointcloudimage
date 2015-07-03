#include "pcimage.h"
#include <opencv2/nonfree/nonfree.hpp> // SIFT・SURFモジュール用

#include "Timer.h"

using namespace std;

void FeatureMatching(
	const std::string& filename1,			// 画像１のファイル名
	const std::string& filename2,			// 画像２のファイル名
	const std::string& featureDetectorName,		// detectorType
	const std::string& descriptorExtractorName,	// descriptorExtractorType
	const std::string& descriptorMatcherName,	// descriptorMatcherType
	bool crossCheck = true)				// マッチング結果をクロスチェックするかどうか
{
	// 画像の読み込み
	cv::Mat img1 = cv::imread(filename1);
	cv::Mat img2 = cv::imread(filename2);

	// SIFT・SURFモジュールの初期化
	cv::initModule_nonfree();

	// 特徴点抽出
	cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create(featureDetectorName);
	std::vector<cv::KeyPoint> keypoint1, keypoint2;
	detector->detect(img1, keypoint1);
	detector->detect(img2, keypoint2);

	// 特徴記述
	cv::Ptr<cv::DescriptorExtractor> extractor = cv::DescriptorExtractor::create(descriptorExtractorName);
	cv::Mat descriptor1, descriptor2;
	extractor->compute(img1, keypoint1, descriptor1);
	extractor->compute(img2, keypoint2, descriptor2);

	// マッチング
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(descriptorMatcherName);
	std::vector<cv::DMatch> dmatch;
	if (crossCheck)
	{
		// クロスチェックする場合
		std::vector<cv::DMatch> match12, match21;
		matcher->match(descriptor1, descriptor2, match12);
		matcher->match(descriptor2, descriptor1, match21);
		for (size_t i = 0; i < match12.size(); i++)
		{
			cv::DMatch forward = match12[i];
			cv::DMatch backward = match21[forward.trainIdx];
			if (backward.trainIdx == forward.queryIdx)
				dmatch.push_back(forward);
		}
	}
	else
	{
		// クロスチェックしない場合
		matcher->match(descriptor1, descriptor2, dmatch);
	}

	// マッチング結果の表示
	cv::Mat out;
	cv::drawMatches(img1, keypoint1, img2, keypoint2, dmatch, out);
	cv::imshow("matching", out);
	cv::imwrite("matching.jpg", out);
	while (cv::waitKey(1) == -1);
}





int unkomain()
{
	string fileName = "./pointcloud.pcd";
	string imgName = "pcd.jpg";
	string floorimgName = "pcd_floor.jpg";


	Timer t;
	t.Start(0);
	/*
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

	PCIclasstest();

	/*
	cv::Mat image(cv::Size(50, 50), CV_8U, cv::Scalar::all(0));
	for (int i = 0; i < 3; i++) image.data[20 * image.cols + 20] += 100;
	cv::imwrite("testimg.jpg", image);
	*/
	
	z = getchar();

	return 0;
}