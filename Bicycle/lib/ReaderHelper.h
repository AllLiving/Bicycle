#ifndef Aroya_READER_HELPER
#define Aroya_READER_HELPER

#include<vector>
#include<fstream>
using namespace std;
#include"Reader.h"

class AroyaReaderHelper {
public:
	AroyaReaderHelper();
	//clear reader
	void clear();
	//insert table
	//if not input myTableName, will set it as same as tableName
	void insert(AroyaReader&reader, const char*tableName, const char*myTableName=nullptr);
	//insert by column position
	void insert(AroyaReader&reader, const int&col);
	//delete table
	bool erase(const char*tableName);
	
	//归一化
	void normalization();
	//写出数据
	void writeFile(const char*fileName);

	//get
	int getRows();
	int getColumns();

	//读取不带table
	//默认0行优先读取
	//1为列优先读取
	vector<vector<double>> getData(int dataType=0);
	//for bpnn to load
	double** getDataPointer();

	//记录table
	void saveTable(const char*fileName);
	//更变为记录的table顺序
	void transformTable(const char*fileName);
private:
	int findTable(const char*tableName);
	vector<string>table;
	vector<vector<double>>buffer;
	vector<double>empty;
};



#endif