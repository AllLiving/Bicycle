#include"KMeansHelper.h"

AroyaKMeansHelper::AroyaKMeansHelper() {
	//nothing to do yet
}

//clear the buffer
bool AroyaKMeansHelper::HelperClear() {
	if (table.size() > 0)	table.clear();
	if (buffer.size() > 0)	buffer.clear();
	return true;
}

//insert table
void AroyaKMeansHelper::insert(AroyaReader&reader, const char*tableName, const char*myTableName) {
	if (myTableName == nullptr)myTableName = tableName;
	int t = reader.findTable(tableName);	//find table index
	int length = reader.getRows();			//get data size
	int myPosition = findTable(myTableName);//find myTableName index

	if (myPosition < 0) {					//not found create new table
		myPosition = buffer.size();			//point to end of buffer
		buffer.push_back(empty);			//new buffer
		table.push_back(myTableName);		//new table
	}
	for (int i = 1; i < length; i++) {		//push to buffer
		buffer[myPosition].push_back(reader.getDoubleData(i, t));
	}
}
int AroyaKMeansHelper::findTable(const char*tableName) {
	int i, j;
	i = table.size();
	for (j = 0; j < i; j++) {
		if (table[j] == tableName)return j;
	}
	return -1;
}

vector<vector<double>> AroyaKMeansHelper::getData() {
	vector<vector<double>>temp;
	vector<double>item;
	int i, j, rows, columns;
	rows = buffer[0].size();
	columns = buffer.size();
	for (i = 0; i < rows; i++) {
		item.clear();
		for (j = 0; j < columns; j++) {
			item.push_back(buffer[j][i]);
		}
		temp.push_back(item);
	}
	return temp;
}

// collect flow information between two station;
vector<vector<double>> AroyaKMeansHelper::getData(vector<int> &flow) {
	vector<vector<double>>temp;
	vector<double>item;
	int i, j, rows, columns;
	rows = buffer[0].size();
	columns = buffer.size();
	flow.clear();

	for (i = 0; i < rows; i++) {
		item.clear();
		for (j = 0; j < columns; j++) {
			item.push_back(buffer[j][i]);
		}
		// delete duplication;
		bool exist = false;
		unsigned int tpin;
		for (tpin = 0; tpin < temp.size(); tpin++) {
			if (temp[tpin] == item) {
				exist = true;
				break;
			}
		}
		if (!exist) {
			temp.push_back(item);
			flow.push_back(1);
		}else {
			int tmp_flux = ++flow[tpin];
			flow.push_back(tmp_flux);
		}
	}
	return temp;
}