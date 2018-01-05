#ifndef AROYA_PREDICT
#define AROYA_PREDICT

#include<cmath>
using namespace std;

class AroyaPredict {
public:
	AroyaPredict();
	/*如果只有日期数据，请先获取星期数再传入
	AroyaDate的输入格式为YYYY?MM?DD
	Date.h和Date.cpp
	*/
	bool weekday(const int&weekdayA,const int&weekdayB,
		const int&Friday=5);

	//mod
	double FunctionR(const double&A, const double&B);
	//delta h
	double FunctionDeltaH(const double&A, const double&B);
	//delta d
	double FunctionDeltaD(const double&A, const double&B);
	//setRho
	void setRho1(const double&t);
	void setRho2(const double&t);
	//predict
	double getAns(const bool&weekday,const double&A, const double&B);

private:
	double Rho1, Rho2;
};


#endif // !AROYA_PREDICT