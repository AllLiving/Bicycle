#include"lib\Reader.h"
#include"lib\KMeansHelper.h"
#include"lib\KMeans.h"
#include"lib\Trans.h"

using namespace std;

int main() {
	AroyaReader reader;
	reader.read("example\\test1.csv");

	AroyaKMeansHelper KMeansHelper;
	KMeansHelper.insert(reader, "start station latitude");
	KMeansHelper.insert(reader, "start station longitude");

	AroyaKMeans KMeans;
	KMeans.setClusters(6);
	vector<vector<double>> data = KMeansHelper.getData();


	KMeans.setData(KMeansHelper.getData());
	KMeans.run();
	//KMeans.writeFile("output.csv");
	//KMeans.writeCentre("centre.csv");

	Trans t;
	t.setCentre(KMeans.getCentre());
	int a = 0;
	int b = 1;
	int c = 2;
	int d = 3;

	return 0;
}