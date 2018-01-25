#include "Trans.h"

Centre::Centre(){
	flow.push_back(0);
}

Centre::Centre(const vector<double> &location) {
	flow.push_back(0);
	this->location = location;
}

void Centre::reset() {
	satellite.clear();
	neighbor.clear();
	flow.clear();
	flow.push_back(0);
	vastFlow = 0;
	destination = -1;
}

void Centre::setLocation(const vector<double> &locate) {
	location = locate;
}

void Centre::setFlow(const double &flux, const int &clust_index) {
	if (flow.size() == 0) {
		flow.clear();
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
		if (flow.size() == 0) {
			printf("[Centre]orient:Lack flux information.\n");
			system("pause");
		}
		if (flow[npin] > mstFlow) {
			mstFlow = flow[npin];
			destin = npin;
		}
	}
	if (destin == -1) {
		printf("[Centre]orient:Undefined destination.\n");
		system("pause");
	}
	destination = destin;
	vastFlow = mstFlow;
}

vector<double> Centre::getLocation() {
	return location;
}

int Centre::getDestination() {	return destination; }
double Centre::getVastFlow() { return vastFlow; }

// add a new neightbor;
void Centre::addNeighbor(const vector<double> &NEIGHBOR_LOCATE) {
	neighbor.push_back(NEIGHBOR_LOCATE);
	flow.push_back(0);
}

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
}

// condense a point station into one cluster;
int Trans::Clust(const vector<double> &point) {
	double dist = 0;
	int clust = 0;
	// compare the distance between the station and centers;
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

// add a new centre;
void Trans::pushCentre(const vector<double> &CENTRE_LOCATION) {
	Centre tmp(CENTRE_LOCATION);
	centre.push_back(tmp);
}

void Trans::pushCentre(const vector<vector<double>> &CENTRE_POS) {
	for (unsigned int cpin = 0; cpin < CENTRE_POS.size(); cpin++) {
		Centre tmp(CENTRE_POS[cpin]);
		centre.push_back(tmp);
	}
}

// erase special centre;
void Trans::eraseCentre(const int& CENTRE_INDEX) {
	if (CENTRE_INDEX < 0 || CENTRE_INDEX > centre.size()) {
		printf("[Trans]eraseCentre: Erase Fail, because centre index is out of range.\n");
		system("pause");
	}
	centre.erase(centre.begin() + CENTRE_INDEX);
}

// collect flux for trans system;
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

// T cluster, return merged statellites cluster matrix;
vector<vector<double>> Trans::TransCluster(const vector<vector<double>> &stt,
						const vector<vector<double>> &end, const vector<int> &flux){
	if (stt.size() != end.size() || stt.size() != flux.size() || end.size() != flux.size()) {
		printf("[Trans]TransCluster:Parameter is not a matrix.\n");
		printf("[Trans]:department count=%d, destination count=%d, flow count=%d\n",
			stt.size(), end.size(), flux.size());
		system("pause");
	}

	// initialize centre information;
	// in this case, we should give up all set for each centre;
	// because some information has changed probably;
	for (unsigned int cpin = 0; cpin < centre.size(); cpin++) {
		centre[cpin].reset();
		Centre crtc = centre[cpin];
		for (unsigned int npin = 0; npin < centre.size(); npin++) {
			Centre crtn = centre[npin];
			if(npin == cpin){}
			else {
				crtc.addNeighbor(crtn.getLocation());
			}
		}
		centre[cpin] = crtc;
	}

	int size = stt.size();
	// collect and analyze flow information;
	for (unsigned int i = 0; i < size; i++) {
		statFlow(stt[i], end[i], flux[i]);
	}
	// log flux information between stations;
	int destin = -1;
	int *fluxLog = new int[centre.size()]; // designed for log flux to centre[fluxlog]
	for (unsigned int fpin = 0; fpin < centre.size(); fpin++) {
		fluxLog[fpin] = 0;
	}
	for (unsigned int cpin = 0; cpin < centre.size(); cpin++) {

		Centre crtc = centre[cpin];
		crtc.orient();
		fluxLog[crtc.getDestination()] += crtc.getVastFlow();
		centre[cpin] = crtc;
	}
	// choose the heaviest flux as destination;
	int mstFlux = fluxLog[0];
	for (unsigned int fpin = 0; fpin < centre.size(); fpin++) {
		if (fluxLog[fpin] > mstFlux) {
			mstFlux = fluxLog[fpin];
			destin = fpin;
		}
	}
	delete[] fluxLog;
	// cluster all stations having same flow to the destination;
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
	if (entries.size() == 0) {
		printf("[Trans]TransCluster:cannot find max flux data.\n");
		system("pause");
	}

	// collect cluster centres to merge;
	vector<vector<double>> statellite_merge;
	for (unsigned int epin = 0; epin < entries.size(); epin++) {
		int crt_index = entries[epin];
		vector<vector<double>> stl_mrg = centre[crt_index].satellite;
		for (unsigned int spin = 0; spin < stl_mrg.size(); spin++) {
			statellite_merge.push_back(stl_mrg[spin]);
		}
	}
	for (int epin = (entries.size()-1); epin >= 0; epin--) {
		int crt_index = entries[epin];
		eraseCentre(crt_index);
	}
	
	return statellite_merge;
}

// write centre information into file;
void Trans::writeTransCentre(const string &FILENAME) {
	string file = FILENAME;
	ofstream fout(file);
	fout << "latitude" << "," << "longitude" << endl;
	for (unsigned int cpin = 0; cpin < centre.size(); cpin++) {
		Centre crtc = centre[cpin];
		vector<double> pos = crtc.getLocation();
		for (unsigned int pin = 0; pin < (pos.size()-1); pin++) {
			fout << pos[pin] << ",";
		}
		fout << pos[pos.size() - 1] << endl;
	}
}

// get stations having same destination;
vector<vector<double>> Trans::getPt_End(const vector<vector<double>> &DEPART,
	const vector<vector<double>> &DESTIN, const int &CLUSTER_INDEX) {
	if (DEPART.size() != DESTIN.size()) {
		printf("[Trans]TransCluster:Parameter isn't a matrix.\n");
		system("pause");
	}
	vector<vector<double>> stations;
	for (unsigned int dpin = 0; dpin < DESTIN.size(); dpin++) {
		vector<double> crtp = DESTIN[dpin];
		int destin = Clust(crtp);
		if (destin == CLUSTER_INDEX) {
			stations.push_back(DEPART[dpin]);
		}
	}
	return stations;
}

// get trans cluster's centre;
vector<double> Trans::getTransCentre(int pos) { return centre[pos].getLocation();  }

// get centre of the trans object;
vector<Centre> Trans::getCentre() {
	return centre;
}

int Trans::getCentreSize() {
	return centre.size();
}

vector<vector<double>> Trans::getCluster(const int &CENTRE_INDEX) {
	if (CENTRE_INDEX < 0 || CENTRE_INDEX > centre.size()) {
		printf("[Trans]getCluster:cannot find %dth centre.\n", CENTRE_INDEX);
		system("pause");
	}
	vector<Centre> clst = getCentre();
	Centre crtc = clst[CENTRE_INDEX];
	return crtc.satellite;
}