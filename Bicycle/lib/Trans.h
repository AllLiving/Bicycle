#ifndef	TRANS
#define TRANS
#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<bits\stdc++.h>
#include<sstream>

using namespace std;

// designed for managing cluster center;
class Centre {
public:
	Centre();
	Centre(const vector<double> &);
	void reset();
	void setLocation(const vector<double> &);
	void setFlow(const double &, const int &);
	void orient();
	void addNeighbor(const vector<double> &NEIGHBOR_LOCATE);

	vector<double> getLocation();
	int getDestination();
	double getVastFlow();

	// every state node in this cluster;
	vector<vector<double>> satellite;
private:
	// other clusters;
	vector<vector<double>> neighbor;
	// where the centre locate;
	vector<double> location;
	// flows to neighbors
	vector<double> flow;
	double vastFlow; 
	int destination; 
};

// manage the whole transport system;
class Trans {
public:
	void setCentre(const vector<vector<double>> &);
	vector<vector<double>> TransCluster(const vector<vector<double>>& START_STATION_SEQ, 
										const vector<vector<double>> & DESTINATION_SEQ, 
										const vector<int> &FLOW_BETWEEN_STATION);
	void pushCentre(const vector<double> &CENTRE_LOC);
	void pushCentre(const vector<vector<double>> &CENTRE_POS);

	void writeTransCentre(const string &FILENAME);
	vector<vector<double>> getPt_End(const vector<vector<double>> &DEPARTMENTS,
		const vector<vector<double>> &DESTINATIONS, const int &CLUSTER_INDEX);
	vector<double> getTransCentre(int pos);
	vector<Centre> getCentre();
	int getCentreSize();
	vector<vector<double>> getCluster(const int &);
private:
	// manage clusters
	vector<Centre> centre;

	void eraseCentre(const int& CENTRE_INDEX);
	void statFlow(const vector<double> &, const vector<double> &, const double &);
	int Clust(const vector<double> &);
};

#endif