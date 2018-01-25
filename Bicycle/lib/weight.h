#ifndef WEIGHT
#define WEIGHT

#include <iostream>
#include "Date.h"
#include "Predict.h"
#include "GauseSim.h"

using namespace std;

double timeLike(const string &dateStrA, const string&dateStrB, const int &A, const int &B) {
	AroyaDate ada;
	ada.input(dateStrA);
	AroyaDate adb;
	adb.input(dateStrB);
	AroyaPredict ap;
	bool weekday = ap.weekday(ada.getWeekday(), adb.getWeekday());
	return ap.getAns(weekday, A, B);
}

double tempLike() {
	//printf("test Gause function...\n");
	//set Sigma
	double P1 = 1;
	double V1 = 1;
	double P2 = 1;
	double V2 = 1;
	//cout << "Sim ans:" << GauseSim(P1, V1, P2, V2) << endl;
	//defaule is 1
	//printf("Set default function as 1\n");
	GauseSimSetting::setSigma1(1);
	GauseSimSetting::setSigma2(2);
	//cout << "Sim ans:" << GauseSim(P1, V1, P2, V2) << endl << endl << endl;
	return GauseSim(P1, V1, P2, V2);
}

double weight() {
	double weight = tempLike();
	return weight;
}

#endif // !WEIGHT