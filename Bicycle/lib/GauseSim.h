#ifndef GAUSE_SIM
#define GAUSE_SIM

#include<cmath>
using namespace std;

static double Pi = 3.14159265358979323846; // 4*atan(1)
static double sigma1 = 1.0;
static double sigma2 = 1.0;

namespace GauseSimSetting {
	void setSigma1(const double&);
	void setSigma2(const double&);
}

//setSigma functions 
//are in namespace GauseSimSetting
double GauseSim(
	const double& P1,
	const double& V1,
	const double& P2,
	const double& V2)
{
	return
		exp(-(
			pow(P1 - P2, 2) / sigma1
			+ pow(V1 - V2, 2) / sigma2))
		/
		(2 * Pi*sigma1*sigma2);
}

void GauseSimSetting::setSigma1(const double&t) {
	sigma1 = t;
}
void GauseSimSetting::setSigma2(const double&t) {
	sigma2 = t;
}
#endif//Code by Aroya