#include<iostream>
#include<vector>
#include<utility>
#include"SparseMatrix.h"

using namespace std;
using Mat = SparseMatrix<int>;

void input_mat(istream& is, vector<Mat::node>& inpt_vec, const int& num)
{
	Mat::node nd;
	for (int i = 0; i != num; i++) {
		cin >> nd.first.first >> nd.first.second >> nd.second;
		inpt_vec.push_back(nd);
	}
}


bool can_prodect(const Mat::size_type size_1, const Mat::size_type size_2)
{
	return size_1.second == size_2.first;
}

bool can_add(const Mat::size_type size_1, const Mat::size_type size_2)
{
	return size_1 == size_2;
}
