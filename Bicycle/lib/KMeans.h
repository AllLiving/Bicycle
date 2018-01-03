#ifndef Aroya_KMEANS
#define Aroya_KMEANS

#include<iostream>
#include<vector>
#include<cmath>
#include<fstream>
using namespace std;

//#define AROYA_DEBUG

class AroyaKMeans {
public:
	AroyaKMeans();
	void setClusters(const int&clusters);					//设置聚类点数
	void setData(const vector<vector<double>>&yourData);	//从reader读入信息
	void run();												//进行聚类
	void setBord(const double&newBord);						//设置收敛条件
	void writeFile(const char*fileName);					//聚类情况写出文件

	//质心:一行为一个数据;列为维度
	vector<vector<double>> getCentre();						//获取质心
	void writeCentre(const char*fileName);					//质心信息写出文件
private:
	vector<vector<double>>data;		//data
	int*cluster;					//指明data所在的centre
	vector<vector<double>>centre;	//质心
	double **distance;				//data到质心的距离
	int rows, columns, clusters;
	double bord;					//默认5%以下的变化时已收敛
};


#endif