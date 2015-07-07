#ifndef _INC_TIMER
#define _INC_TIMER

#include <string>
#include <chrono>
#include <vector>

using namespace std;

class Timer
{
public:
	//単位の種類
	enum tUnit { sec, millisec, microsec, nanosec };

private:
	//計測開始点，終了点を格納する配列
	vector<std::chrono::time_point<std::chrono::system_clock>> start, end;

	//指定した単位で時間を返す
	int retTime(std::chrono::time_point<std::chrono::system_clock> start, std::chrono::time_point<std::chrono::system_clock> end, tUnit unit);

	//単位の名前を返す
	string unitname( tUnit unit);

public:
	//計測開始点
	//複数保存する場合は番号(int型の整数)を指定
	//番号が連続でないとうまく測れないので注意
	void Start( int timeNum );

	//計測結果保存
	//引数は番号，単位，ラベル，保存時のファイル名
	//番号以外は省略可能(オーバーロードされている形のみ)
	//保存時のファイル名を指定するとそのファイルの最後尾に追記する
	//ファイル名指定なしの場合は保存しない
	void End(int timeNum , string filename , string label, tUnit unit);
	void End(int timeNum , string filename);
	void End(int timeNum , string filename, string label);
	void End(int timeNum , tUnit unit);
	void End(int timeNum);

};

#endif