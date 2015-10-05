#ifndef _INC_TIMER
#define _INC_TIMER

// 2015/10/05

#include <string>
#include <chrono>
#include <vector>

using namespace std;

class Timer
{
public:
	//単位の種類
	static enum tUnit { sec, millisec, microsec, nanosec };
	//単位の名前を返す
	static string unitname(tUnit unit);

private:
	//長いので型を短く定義
	typedef std::chrono::time_point<std::chrono::system_clock> _time;

	//計測開始点，中間点を格納する
	vector<_time>	rawLap;
	vector<int>	lapTime;

	//指定した単位で時間を返す
	int getTime(_time start, _time end, tUnit unit);

public:
	//計測開始点
	void Start();

	//計測結果保存
	//引数はファイル名,単位
	//指定したファイルが既に存在したらファイルの最後尾に追記する
	void Save(string filename, tUnit unit = millisec);

	/*
	 * 指定回数前のラップタイムからの経過時間を取得
	 * Criteria：経過時間の基準．
	 *			 getLapTimeを呼んだ時間が配列で保存されているので，いくつ前からの経過時間を取得するのかを指定する．
	 * unit：単位を指定
	 * isSaveLap：次回以降の基準時間として現在を保存するか指定
	 */
	int getLapTime(int Criteria = 1, tUnit unit = millisec, bool isSavaLap = true);

	// 現在時刻を文字列で返す
	string getNowTime();
};

#endif