#include "../cvPCDtest/pcimage.h"

#include <fstream>
#include <iostream>
#include <filesystem> 
#include <algorithm>
#include <vector>

using namespace cv;
using namespace std;

#define WRITELINE

const string DIRPATH = "C:\\Users\\user\\Documents\\なかむら\\つくばチャレンジ2015\\測定データ\\20151017141744";
//const string DIRPATH = "C:\\Users\\user\\Documents\\Visual Studio 2013\\Projects\\cvPCDtest\\cvPCDFunctions\\20151016165345";

PCImage pcimage(1000, 1000, 5);

void getAllFileName(vector<string>& fileNames, string extension)
{
	namespace sys = std::tr2::sys;
	sys::path p(DIRPATH); // 列挙の起点
	ifstream ifs;
	string fname;

	for_each(sys::directory_iterator(p), sys::directory_iterator(),
		[&](const sys::path& p) {
		if (sys::is_regular_file(p)) { // ファイルなら...
			cout << p.filename() << endl;
			fname = p.filename();
			if (fname.find("." + extension) != string::npos)	fileNames.push_back(p.filename());
		}
		else if (sys::is_directory(p)) { // ディレクトリなら...
			cout << "dir.: " << p.string() << endl;
		}
	});
}

void makePCImageFromPCD(string filename )
{
	float	x_cood, y_cood;
	float	x_pos, y_pos;

	string str, data_str;
	string searchWord(",");
	string::size_type begin, end;

	//pcdファイルを読み込む
	ifstream ifs(DIRPATH + "\\" + filename);
	if (ifs.fail())
	{
		cerr << "False" << endl;
		return ;
	}

	//ヘッダ部分をとばすためのループ
	for (int i = 0; i <= 11; i++){
		getline(ifs, str);
	}

	while (getline(ifs, str))
	{
		//x
		begin = str.find(searchWord);
		if (begin != string::npos){
			data_str = str.substr(0, begin);
			x_cood = stof(data_str);

			//cout << x_cood << endl;
		}

		//y
		end = str.find(searchWord, begin + 1);
		if (end != string::npos){
			data_str = str.substr(begin + 1, end - begin + 1);
			y_cood = stof(data_str);

			//cout << y_cood << endl;
		}

		//x_pos
		begin = end;
		end = str.find(searchWord, begin + 1);
		if (end != string::npos){
			data_str = str.substr(begin + 1, end - begin + 1);
			x_pos = stof(data_str);

			//cout << x_pos << endl;
		}

		//y_pos
		begin = end;
		end = str.find(searchWord, begin + 1);
		if (end != string::npos){
			data_str = str.substr(begin + 1, end - begin + 1);
			y_pos = stof(data_str);

			//cout << y_pos << endl;
		}

		//取得した[x,y]の画素値を増加させる
#ifdef WRITELINE
		pcimage.writePoint(x_cood, y_cood,x_pos,y_pos);
#else
		pcimage.writePoint(x_cood,y_cood);
#endif

	}

	//cout << "complete" << endl;
}

void sortfnames(vector<string>& allFileNames, vector<string>& sortFileNames)
{
	string str, data_str;
	string searchWord1("_");
	string searchWord2(".");
	string::size_type begin, end;
	int num = 0;

	sortFileNames.resize(allFileNames.size());

	for (int i = 0; i < allFileNames.size(); i++)
	{
		begin = allFileNames[i].find(searchWord1);
		end = allFileNames[i].find(searchWord2);
		if (begin != string::npos){
			data_str = allFileNames[i].substr(begin + 1, end - begin + 1);
			num = stoi(data_str);

			sortFileNames[num] = allFileNames[i];
		}
	}
}

void makePcimage()
{
	vector<string> allFileNames;
	vector<string> sortFileNames;
	getAllFileName(allFileNames,"pcd");

	sortfnames(allFileNames, sortFileNames);

	for (int i = 0; i < sortFileNames.size(); i++)
	{
		cout << sortFileNames[i] << endl;
		makePCImageFromPCD(sortFileNames[i]);
	}
}

void minmaxXY(vector<string> allFileNames, int& maxX, int& minX, int& maxY, int& minY)
{

	string str, data_str;
	string searchWord1("_");
	string searchWord2(".");
	string::size_type begin, end;
	int num;

	for (int i = 0; i < allFileNames.size(); i++)
	{
		begin = allFileNames[i].find(searchWord1);
		end = allFileNames[i].find(searchWord2);
		if (begin != string::npos){

			// X
			data_str = allFileNames[i].substr(0,  begin );
			num = stoi(data_str);
			if (num < minX) minX = num;
			else if (num > maxX) maxX = num;

			// Y
			data_str = allFileNames[i].substr(begin + 1 , end - begin + 1);
			num = stoi(data_str);
			if (num < minY) minY = num;
			else if (num > maxY) maxY = num;
		}
		
	}
}

void uniteImage()
{
	vector<string> allFileNames;
	int maxX = 0, minX = 0;
	int maxY = 0, minY = 0;

	getAllFileName(allFileNames, "jpg");
	minmaxXY(allFileNames, maxX, minX, maxY, minY);

	int size_X = maxX - minX + 1;
	int size_Y = maxY - minY + 1;

	Mat** pcimageArray = new Mat*[size_X];
	for (int i = 0; i < size_X; i++)
	{
		pcimageArray[i] = new Mat[size_Y];
	}

	for (int x = 0; x < size_X; x++)
	{
		for (int y = 0; y < size_Y; y++)
		{
			pcimageArray[x][y] = imread(DIRPATH + "\\" + to_string(minX + x) + "_" + to_string(minY + y) + ".jpg" , 0);
			if (pcimageArray[x][y].empty()) pcimageArray[x][y] = Mat(1000, 1000, CV_8U, Scalar::all(100));

			// imshow(DIRPATH + "\\" + to_string(minX + x) + "_" + to_string(minY + y) + ".jpg", pcimageArray[x][y]);
		}
	}

	waitKey();

	for (int i = 0; i < size_X; i++) {
		delete[] pcimageArray[i];
	}
	delete[] pcimageArray;
}

void main()
{
	makePcimage();
	waitKey();
	//uniteImage();
}