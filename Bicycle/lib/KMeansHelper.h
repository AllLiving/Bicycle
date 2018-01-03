#ifndef Aroya_KMEANS_HELPER
#define Aroya_KMEANS_HELPER

#include<vector>
#include"Reader.h"
using namespace std;

class AroyaKMeansHelper {
public:
	AroyaKMeansHelper();
	//insert table
	//if not input myTableName, will set it as same as tableName
	void insert(AroyaReader&reader, const char*tableName, const char*myTableName=nullptr);
	//for AroyaKMeans to load
	vector<vector<double>> getData();
private:
	int findTable(const char*tableName);
	vector<string>table;
	vector<vector<double>>buffer;
	vector<double>empty;
};

#endif