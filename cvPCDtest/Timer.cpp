#include "Timer.h"

#include <iostream>
#include <fstream>


void Timer::Start(int timeNum)
{
	if (start.size() < timeNum + 1)
	{
		start.push_back(std::chrono::system_clock::now());
		end.push_back(std::chrono::system_clock::now());

	}
	else start[timeNum] = std::chrono::system_clock::now();
}

void Timer::End(int timeNum, string filename, string label, tUnit unit)
{
	end[timeNum] = std::chrono::system_clock::now();

	int ret;

	ret = retTime(start[timeNum], end[timeNum], unit);

	if (label.empty()) label = "No." + to_string(timeNum);

	cout << label << " : " << ret << unitname(unit) << endl;

	if (!filename.empty())
	{
		ofstream ofs(filename,ios::app);
		ofs << label << " , " << ret << " , " << unitname(unit) << endl;

	}
}
void Timer::End(int timeNum, string filename)
{
	this->End(timeNum, filename, "", millisec);
}
void Timer::End(int timeNum, string filename, string label)
{
	this->End(timeNum, filename, label, millisec);
}
void Timer::End(int timeNum, tUnit unit)
{
	this->End(timeNum, "", "", unit);
}
void Timer::End(int timeNum)
{
	this->End(timeNum, "", "", millisec);
}

int Timer::retTime(std::chrono::time_point<std::chrono::system_clock> start, std::chrono::time_point<std::chrono::system_clock> end, tUnit unit)
{
	switch (unit)
	{
	case sec:
		return std::chrono::duration_cast<std::chrono::seconds>(end-start).count();
	case millisec:
		return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	case microsec:
		return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	case nanosec:
		return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	default:
		return -1;
	}

}

string Timer::unitname(tUnit unit)
{
	switch (unit)
	{
	case sec:
		return " seconds";
	case millisec:
		return " milliseconds";
	case microsec:
		return " microseconds";
	case nanosec:
		return " nanoseconds";
	default:
		return " ";
	}
}