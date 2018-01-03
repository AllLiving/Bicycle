#ifndef	TRANS
#define TRANS
#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<bits\stdc++.h>
#include<sstream>

using namespace std;

class Centre {
public:
	Centre();
	Centre(const vector<double> &);
	void setLocation(const vector<double> &);
	vector<double> getLocation();
	void setFlow(const double &, const int &);
	void orient();
	int getDestination();
	double getVastFlow();

	// every state node in this cluster;
	vector<vector<double>> satellite;
	// other clusters;
	vector<vector<double>> neighbor;
private:
	// where the centre locate;
	vector<double> location;
	// flows to neighbors
	vector<double> flow;
	double vastFlow;
	int destination;
};

class Trans {
public:
	void setCentre(const vector<vector<double>> &);
	void statFlow(const vector<double> &, const vector<double> &, const double &);
	vector<vector<double>> TransCluster(const vector<vector<double>>&, const vector<vector<double>> &, const vector<double> &);
private:
	// manage clusters
	vector<Centre> centre;

	int Clust(const vector<double> &);
};

#endif