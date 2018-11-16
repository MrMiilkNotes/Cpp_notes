#include<iostream>
#include<vector>
#include<utility>
#include"SparseMatrix.h"
/*
先输入size，再输入非零元个数，然后输入矩阵
*/
/*第一组
0 0 10
1 2 9
2 0 -1

1 2 -1
2 0 1
2 2 -3
*/

/*第二组
0 0 10
1 1 9
2 0 -1

1 1 -1
2 0 1
2 1 -3

*/

/*第三组
0 0 4
0 1 -3
0 4 1
1 3 8
2 2 1
3 4 70
*/
/*
0 0 3
1 0 4
1 1 2
2 1 1
3 0 1
*/

using namespace std;
using Mat = SparseMatrix<int>;

bool can_prodect(const Mat::size_type size_1, const Mat::size_type size_2);
bool can_add(const Mat::size_type size_1, const Mat::size_type size_2);
void input_mat(istream& is, vector<Mat::node>& inpt_vec, const int& num);

int main()
{
	Mat::size_type size_M1, size_M2;
	int len_M1, len_M2;
	vector<Mat::node> inpt_vec_1, inpt_vec_2;
	cout << "size of Matrix 1: ";
	cin >> size_M1.first >> size_M1.second;
	cout << "num of elem: ";
	cin >> len_M1;
	cout << "input Mat: " << endl;
	input_mat(cin, inpt_vec_1, len_M1);

	cout << "size of Matrix 2: ";
	cin >> size_M2.first >> size_M2.second;
	cout << "num of elem: ";
	cin >> len_M2;
	cout << "input Mat: " << endl;
	input_mat(cin, inpt_vec_2, len_M2);

	//矩阵初始化
	Mat M1(size_M1.first, size_M2.second, inpt_vec_1);
	Mat M2(size_M2.first, size_M2.second, inpt_vec_2);

	cout << "M1: " << endl;
	cout << M1 << endl;
	cout << "M2: " << endl;
	cout << M2 << endl;

	if (can_add(size_M1, size_M2)) {
		cout << "M1 + M2 = " << endl;
		Mat M3 = M1 + M2;
		cout << M3 << endl;

		cout << "M1 - M2 = " << endl;
		Mat M4 = M1 - M2;
		cout << M4 << endl;
	}

	if (can_prodect(size_M1, size_M2)) {
		cout << "M1 * M2 = " << endl;
		Mat M5 = M1 * M2;
		cout << M5 << endl;
	}

	//system("pause");
	return 0;
}
