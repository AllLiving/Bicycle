#include"Predict.h"

AroyaPredict::AroyaPredict(){
	Rho1 = 0.1;
	Rho2 = 0.1;
}

bool AroyaPredict::weekday(const int&weekdayA, const int&weekdayB,
	const int&Friday) {
	if (weekdayA > Friday&&weekdayB > Friday)return 1;
	int Saturday = Friday + 1;
	if (weekdayA < Saturday && weekdayB < Saturday)return 1;
	return 0;
}
double AroyaPredict::FunctionR(const double&A, const double&B) {
	int temp;
	if (A > B) {
		temp = A - double(int(FunctionDeltaD(A, B))) * 24;
		if (temp > B)return temp - B;
		return B - temp;
	}
	temp = B - double(int(FunctionDeltaD(A, B))) * 24;
	if (temp > A)return temp - A;
	return A - temp;
}
double AroyaPredict::FunctionDeltaH(const double&A, const double&B) {
	double temp = FunctionR(A, B);
	double temp2 = 24.0 - temp;
	return temp > temp2 ? temp2 : temp;
}
double AroyaPredict::FunctionDeltaD(const double&A, const double&B) {
	if (A > B)return (A - B) / 24.0;
	return(B - A) / 24.0;
}
void AroyaPredict::setRho1(const double&t) {
	Rho1 = t;
}
void AroyaPredict::setRho2(const double&t) {
	Rho2 = t;
}
double AroyaPredict::getAns(const bool&weekday, const double&A, const double&B) {
	return weekday*pow(Rho1, FunctionDeltaH(A, B))*pow(Rho2, FunctionDeltaD(A, B));
}