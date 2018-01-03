#include "Trans.h"

Centre::Centre(){}

Centre::Centre(const vector<double> &location) {
	this->location = location;
}

void Centre::setLocation(const vector<double> &locate) {
	location = locate;
}

vector<double> Centre::getLocation() {
	return location;
}

void Centre::setFlow(const double &flux, const int &clust_index) {
	if (flow.size() == 0) {
		for (unsigned int cpin = 0; cpin < neighbor.size()+1; cpin++) {
			flow.push_back(0);
		}
	}
	flow[clust_index] += flux;
}

// define the destination from this cluster;
// according flow;
void Centre::orient() {
	int destin = -1;
	double mstFlow = 0;
	for (unsigned int npin = 0; npin < neighbor.size(); npin++) {
		if (flow[npin] > mstFlow) {
			mstFlow = flow[npin];
			destination = npin;
		}
	}
	destination = destin;
	vastFlow = mstFlow;
}

int Centre::getDestination() {	return destination; }
double Centre::getVastFlow() { return vastFlow; }

// what are centres in this trans system;
void Trans::setCentre(const vector<vector<double>> &center){
	Centre emptyCentre;
	centre.clear();
	// set locatiion for each cluster center;
	for (unsigned int cpin = 0; cpin < center.size(); cpin++) {
		vector<double> crtpos = center[cpin];
		centre.push_back(emptyCentre);
		centre[cpin].setLocation(crtpos);
	}
	// value each cluster's neighbors;
	for (unsigned int cpin = 0; cpin < center.size(); cpin++) {
		for (unsigned npin = 0; npin < center.size(); npin++) {
			vector<double> crtp = center[npin];
			if(npin == cpin){}
			else {
				centre[cpin].neighbor.push_back(crtp);
			}
		}
	}
}

// condense a point state into one cluster;
int Trans::Clust(const vector<double> &point) {
	double dist = 0;
	int clust = 0;
	for (unsigned int cpin = 0; cpin < centre.size(); cpin++) {
		Centre crtc = centre[cpin];
		vector<double> pos = crtc.getLocation();
		double dst = 0;
		for (unsigned int i = 0; i < pos.size(); i++) {
			dst += (pos[i] - point[i])*(pos[i] - point[i]);
		}
		dst = sqrt(dst);
		if (cpin == 0)	dist = dst;
		else if (dst < dist) {
			dist = dst;
			clust = cpin;
		}
	}
	return clust;
}

// statistic flux for trans system;
void Trans::statFlow(const vector<double> &stt, const vector<double> &end, const double &flow) {
	int depart = Clust(stt);
	int destin = Clust(end);
	if(depart == destin){}
	else {
		Centre dptc = centre[depart];
		// statistic satellites of the centre;
		dptc.satellite.push_back(stt);
		// collect flow information between centres;
		dptc.setFlow(flow, destin);
		
		centre[depart] = dptc;
	}
}

// T cluster
vector<vector<double>> Trans::TransCluster(const vector<vector<double>> &stt,
						const vector<vector<double>> &end, const vector<double> &flux){
	if (stt.size() != end.size() || stt.size() != flux.size() || end.size() != flux.size()) {
		printf("[Trans]statistic:Parameter is not a matrix.\n");
		system("pause");
	}
	int size = stt.size();
	for (unsigned int i = 0; i < size; i++) {
		statFlow(stt[i], end[i], flux[i]);
	}
	// choose the heaviest flow as destination;
	double mstFlow = 0;
	int destin = -1;
	for (unsigned int cpin = 0; cpin < centre.size(); cpin++) {
		Centre crtc = centre[cpin];
		crtc.orient();
		if (crtc.getVastFlow() > mstFlow) {
			mstFlow = crtc.getVastFlow();
			destin = cpin;
		}
		centre[cpin] = crtc;
	}
	// cluster all station having same flow to the destination;
	vector<int> entries;
	vector<int> orignal;
	for (unsigned int cpin = 0; cpin < centre.size(); cpin++) {
		Centre crtc = centre[cpin];
		if (crtc.getDestination() == destin) {
			entries.push_back(cpin);
		}
		else {
			orignal.push_back(cpin);
		}
	}

	// collect cluster centres to merge;
	vector<vector<double>> cluster;
	for (unsigned int epin = 0; epin < centre.size(); epin++) {
		vector<double> crtl = centre[epin].getLocation();
		cluster.push_back(crtl);
	}
	// merge stations with same destination;
	vector<double> avrg;
	for (unsigned int cpin = 0; cpin < cluster[0].size(); cpin++) {
		double sum = 0;
		for (unsigned int i = 0; i < cluster.size(); i++) {
			sum += cluster[cpin][i];
		}
		sum /= cluster.size();
		avrg.push_back(sum);
	}
	
	cluster.clear();
	cluster.push_back(avrg);
	for (unsigned int opin = 0; opin < orignal.size(); opin++) {
		cluster.push_back(centre[opin].getLocation());
	}
	
	return cluster;
}