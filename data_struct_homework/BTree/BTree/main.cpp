#include<iostream>
#include"BTree.h"


using namespace std;

typedef int _Tp_data;

int main()
{
	BTree<_Tp_data, 3> BT;
	BT.Insert(2);
	cout << BT << endl;
	BT.Insert(3);
	cout << BT << endl;
	BT.Insert(4);
	cout << BT << endl;
	BT.Insert(5);
	cout << BT << endl;
	BT.Insert(6);
	cout << BT << endl;
	BT.Insert(7);
	cout << BT << endl;
	BT.Insert(8);
	cout << BT << endl;
	system("pause");
	return 0;
}