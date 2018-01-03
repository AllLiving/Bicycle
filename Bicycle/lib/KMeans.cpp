#include"KMeans.h"

AroyaKMeans::AroyaKMeans() {
	//nothing to do yet
	bord = 0.05;
}

void AroyaKMeans::setClusters(const int&clusters_) {
	clusters = clusters_;
	centre.clear();
	vector<double>empty;
	for (int i = 0; i < clusters; i++)centre.push_back(empty);
}
void AroyaKMeans::setData(const vector<vector<double>>&yourData) {
	data.clear();
	for (unsigned int dpin = 0; dpin < yourData.size(); dpin++) {
		vector<double> crtv = yourData[dpin];
		vector<vector<double>>::iterator it;
		it = find(data.begin(), data.end(), crtv);
		if (it == data.end()) {
			data.push_back(crtv);
		}
	}
	rows = data.size();
	columns = data[0].size();
	cluster = new int[rows];
}

void AroyaKMeans::run() {
	int i, j, k;
	double min;
	int changed = rows;
	//设置初始聚类点
	for (i = 0; i < clusters; i++) {
		for (j = 0; j < columns; j++) {
			centre[i].push_back(data[i][j]);
		}
	}
	//分配空间
	distance = new double*[clusters];
	for (i = 0; i < clusters; i++)distance[i] = new double[rows];

	double *thisCluster = new double[clusters];

	for (i = 0; i < rows; i++) {
		cluster[i] = -1;
	}


	//开始聚类
	//小于bord的点变化则收敛
	while (double(changed) / double(rows) > bord) {
		changed = 0;

		//初始化空间
		for (i = 0; i < clusters; i++) {
			for (j = 0; j < rows; j++) {
				distance[i][j] = 0;
			}
			thisCluster[i] = 0;
		}

		//先计算所有距离
		for (i = 0; i < clusters; i++) {
			for (j = 0; j < rows; j++) {
				for (k = 0; k < columns; k++) {
					distance[i][j] += pow(centre[i][k] - data[j][k], 2);
				}
			}
		}
#ifdef AROYA_DEBUG
		cout << "distance:\n";
		for (i = 0; i < clusters; i++) {
			cout << "cluster:" << i << endl;
			for (j = 0; j < rows; j++) {
				cout << "point " << j << "\t\t" << distance[i][j] << endl;
			}
		}
		system("pause");
#endif

		//清除聚类点
		for (i = 0; i < clusters; i++) {
			for (j = 0; j < columns; j++) {
				centre[i][j] = 0;
			}
		}

		//寻找最短路径 并记录
		for (i = 0; i < rows; i++) {
			min = distance[0][i];
			k = 0;
			for (j = 1; j < clusters; j++) {
				if (distance[j][i] < min) {
					min = distance[j][i];
					k = j;
				}
			}
			if (cluster[i] != k)changed++;
			cluster[i] = k;
			for (j = 0; j < columns; j++) {
				centre[k][j] += data[i][j];
			}
			thisCluster[k]++;
		}

#ifdef AROYA_DEBUG
		cout << "point to cluster:\n";
		for (i = 0; i < rows; i++) {
			cout << "point " << i << "\t\t" << cluster[i] << endl;
		}
		cout << "Changed points:" << "\t" << changed << endl;
		system("pause");
#endif

		//得到新质心
		for (i = 0; i < clusters; i++) {
			k = thisCluster[i];
			for (j = 0; j < columns; j++) {
				centre[i][j] /= k;
			}
		}
#ifdef AROYA_DEBUG
		cout << "centre:\n";
		for (i = 0; i < clusters; i++) {
			cout << "centre " << i;
			for (j = 0; j < columns; j++) {
				cout << "\t" << centre[i][j];
			}
			cout << endl;
		}
		cout << endl;
		system("pause");
#endif
	}

	//清除空间
	for (i = 0; i < clusters; i++)delete[]distance[i];
	delete[]distance;
	delete[]thisCluster;
}
void AroyaKMeans::setBord(const double&t) {
	bord = t;
}

void AroyaKMeans::writeFile(const char*fileName) {
	ofstream fout;
	fout.open(fileName);
	for (int i = 0; i < rows; i++) {
		fout << cluster[i] << endl;
	}
	fout.close();
}

vector<vector<double>> AroyaKMeans::getCentre() {
	return centre;
}
void AroyaKMeans::writeCentre(const char*fileName) {
	ofstream fout;
	fout.open(fileName);
	int i, j, centres = centre.size();
	for (i = 0; i < centres; i++) {
		fout << centre[i][0];
		for (j = 1; j < columns; j++) {
			fout << ',' << centre[i][j];
		}
		fout << endl;
	}
	fout.close();
}