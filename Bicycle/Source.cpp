#include"lib\Reader.h"
#include"lib\KMeansHelper.h"
#include"lib\KMeans.h"
#include"lib\Trans.h"
#include"lib\GauseSim.h"

using namespace std;
#define T

int main() {
	unsigned int GeoClustersNum = 10;
	string centreFileName = "transCentre.csv";

	AroyaReader reader;
	reader.read("example\\test1.csv");

	AroyaKMeansHelper KMeansHelper;
	KMeansHelper.HelperClear();
	KMeansHelper.insert(reader, "start station latitude");
	KMeansHelper.insert(reader, "start station longitude");

	AroyaKMeans KMeans;
	KMeans.setClusters(GeoClustersNum);
	vector<vector<double>> data = KMeansHelper.getData();

	//KMeans.setData(KMeansHelper.getData());
	KMeans.setData(data);
	KMeans.run();
	//KMeans.writeFile("output.csv");
	//KMeans.writeCentre("centre.csv");

	vector<vector<double>> transCentre = KMeans.getCentre();
	KMeans.initialize();
	KMeans.setData(transCentre);
	KMeans.setClusters(GeoClustersNum / 2);
	KMeans.run();
	transCentre = KMeans.getCentre();

	for (unsigned int itpin = 0; itpin < 5; itpin++) {
#ifdef T
		Trans t;
		t.setCentre(transCentre);
		transCentre.clear();
		// Trans Cluster
		// collect start station sequence;
		KMeansHelper.HelperClear();
		KMeansHelper.insert(reader, "start station latitude");
		KMeansHelper.insert(reader, "start station longitude");
		vector<vector<double>> depart = KMeansHelper.getData();

		// collect end station sequence;
		KMeansHelper.HelperClear();
		KMeansHelper.insert(reader, "end station latitude");
		KMeansHelper.insert(reader, "end station longitude");
		vector<vector<double>> destin = KMeansHelper.getData();

		for (unsigned int destin_index = 0; destin_index < t.getCentre().size(); destin_index++) {
			// get stations with same destination;
			vector<vector<double>> forCluster = t.getPt_End(depart, destin, destin_index);
			t.writeTransCentre(centreFileName);
			
			// Kmeans to stations above;
			double proportion = (double)forCluster.size() / depart.size();
			int clusterCnt = GeoClustersNum * proportion;
			KMeans.initialize();
			KMeans.setData(forCluster);
			if (clusterCnt == 0)	continue;
			KMeans.setClusters(clusterCnt);
			KMeans.run();

			// collect centres;
			vector<vector<double>> sec_kmeans = KMeans.getCentre();
			for (unsigned int cpin = 0; cpin < sec_kmeans.size(); cpin++) {
				transCentre.push_back(sec_kmeans[cpin]);
			}
		}
		vector<vector<double>> raw_centre = transCentre;
		KMeans.initialize();
		KMeans.setData(raw_centre);
		KMeans.setClusters(raw_centre.size());
		KMeans.run();
		KMeans.writeCentre("centre.csv");
		KMeans.setClusters(GeoClustersNum / 2);
		KMeans.run();
		transCentre = KMeans.getCentre();
#endif // T
		printf("[Main]:%dth Iterator done.\n", itpin);
	}
	printf("\n\n\n\n\n");	system("pause");
	return 0;
}