#include<iostream>
#include<limits.h>
#include"graph.h"
#include"best_route.h"
#include"DFS.h"

using namespace std;
typedef int data_tp;
int INF = INT_MAX;

int main()
{
	typedef Graph<data_tp> G;
	typedef G::node_type node;
	G::mat_type relate_mat_1 = {
		{0,0,23,45,12,33,0,55,33,99},
		{0,0,44,67,34,11,34,78,99,21},
		{23,44,0,44,0,45,33,67,78,44},
		{45,67,44,0,56,66,43,32,0,22},
		{12,34,0,56,0,47,88,55,66,99},
		{33,11,45,66,47,0,57,22,33,44},
		{0,34,33,43,88,57,0,65,55,10},
		{55,78,67,32,55,22,65,0,35,26},
		{33,99,78,0,66,33,55,35,0,78},
		{99,21,44,22,99	,44,10,26,78,0}
	};
	for (int i = 0; i != relate_mat_1.size(); ++i) {
		for (int j = 0; j != relate_mat_1.size(); ++j) {
			if (i != j && relate_mat_1[i][j] <= 70) {
				relate_mat_1[i][j] = 0;
			}
		}
	}
	/*G::mat_type relate_mat_1 = {
		{ 0, 97,  0,  0,  0,  0,  0,  0,  0,  0, 19,  0,  0,  0,  0,  0, 0,  0,  6, 73},    {74,  0,  0,  9,  0,  0,  0,  0,  0,  0,  0,  0, 53,  0,  0,  0,      0, 68,  0,  0},    { 0,  0,  0, 30, 23, 93, 50,  0,  0,  0,  0,  0,  0,  0,  0,  0,      0,  0,  0,  0},    { 0, 62, 44,  0,  0,  0,  0,  0,  0, 51,  0,  0,  0,  0,  0,  0,      8,  0,  0,  0},    { 0,  0, 15,  0,  0, 78,  0,  0,  0, 41,  0,  0, 67,  0,  0,  0,      0,  0,  0,  0},    { 0,  0, 18,  0, 41,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,      0, 49,  0,  0},    { 0,  0, 62,  0,  0,  0,  0,  0, 13,  0,  0, 52,  0,  0,  0,  0,      0,  0,  0, 70},    { 0,  0,  0,  0,  0,  0,  0,  0, 34,  0,  0,  0, 76,  1,  0,  0,      0, 27,  0,  0},    { 0,  0,  0,  0,  0,  0, 54, 92,  0,  0,  0,  0,  0,  0, 20, 39,      0,  0,  0,  0},    { 0,  0,  0, 61, 31,  0,  0,  0,  0,  0,  8,  0,  0,  0,  0,  0,     33,  0,  0,  0},    {60,  0,  0,  0,  0,  0,  0,  0,  0, 17,  0,  0,  0,  0, 87, 46,      0,  0,  0,  0},    { 0,  0,  0,  0,  0,  0, 37,  0,  0,  0,  0,  0,  0,  0, 29,  0,      0,  0, 90, 32},    { 0, 93,  0,  0, 93,  0,  0, 69,  0,  0,  0,  0,  0,  0,  0, 94,      0,  0,  0,  0},    { 0,  0,  0,  0,  0, 98,  0, 17,  0,  0,  0,  0,  0,  0, 79,  0,      0, 78,  0,  0},    { 0,  0,  0,  0,  0,  0,  0,  0, 23,  0, 66, 38,  0,  8,  0,  0,      0,  0,  0,  0},    { 0,  0,  0,  0,  0,  0,  0,  0,  8,  0, 63,  0, 84,  0,  0,  0,      0,  0, 59,  0},    { 0,  0,  0,  1,  0,  0,  0,  0,  0, 34,  0,  0,  0,  0,  0,  0,      0,  0,  9, 92},    { 0, 74,  0,  0,  0, 29,  0, 94,  0,  0,  0,  0,  0, 64,  0,  0,      0,  0,  0,  0},    {34,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19,  0,  0,  0, 49,     96,  0,  0,  0},    {12,  0,  0,  0,  0,  0,  5,  0,  0,  0,  0, 30,  0,  0,  0,  0,     36,  0,  0,  0}
	};*/
	G::mat_type relate_mat(relate_mat_1);
	//G::node_list nodes = { node(0), node(1), node(2), node(3) };
	//G::node_list nodes_1 = { node(0), node(1), node(2), node(3), node(4) };
	G::node_list nodes, nodes_1;
	for (int i = 0; i != relate_mat_1.size(); ++i) {
		nodes_1.push_back(node(i));
		nodes.push_back(node(i));
		for (int j = 0; j!= relate_mat_1.size(); ++j) {
			if (i != j && relate_mat_1[i][j] == 0) {
				relate_mat[i][j] = INF;
			}
		}
	}
	G school_map(relate_mat, nodes);//校园地图
	G school_map_1(relate_mat_1, nodes_1);//校园地图

	//深度优先搜索
	DFS<data_tp> DF_finder(school_map_1);
	cout << "校园地图关键点：" << endl;
	DF_finder.tarjan();

	//任意两点最短距离
	BestFinder<data_tp> finder(school_map);
	finder.Floyd();
	int from_, to_;
	cout << "from:";
	cin >> from_;
	cout << "to:";
	cin >> to_;
	finder.print_best_way(from_, to_);

	//所有路径
	cout << "寻找所有路径：" << endl;
	cout << "from:";
	cin >> from_;
	cout << "to:";
	cin >> to_;
	DF_finder.dfs(from_, to_);
	system("pause");
	return 0;
}