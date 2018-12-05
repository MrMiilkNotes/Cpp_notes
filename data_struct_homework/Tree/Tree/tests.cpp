#include<iostream>
#include<vector>
#include"Tree.h"

using namespace std;
typedef int data_type;
typedef Tree<data_type> MyTree;
typedef MyTree::node_type node;
void print_res(const vector<data_type>& vec)
{
	for (auto data : vec) {
		cout << data << " ";
	}
	cout << endl;
}
void create_tree(MyTree& tree_1)
{
	const node *n_1 = tree_1.root();
	tree_1.add_right_child(n_1, 2);
	n_1 = tree_1.add_left_child(n_1, 3);
	tree_1.add_right_child(n_1, 4);
}
void func_1()
{
	MyTree tree_1(0);
	const node *n_1 = tree_1.root();
	//n_1 = tree_1.to_left_child(n_1);
	//const node *n_2 = tree_1.to_left_child(n_1);
	//cout << n_2 << endl;
	cout << n_1 << endl;
}
void func_2()
{
	MyTree tree_1(0);
	const node *n_1 = tree_1.root();
	tree_1.add_right_child(n_1, 2);
	n_1 = tree_1.add_left_child(n_1, 3);
	tree_1.add_right_child(n_1, 4);
}
bool func_3(node* node_, int i) {
	cout << node_->data << ' ';
	return true;
}
bool func_4(node* node_, vector<data_type>& data_vec)
{
	data_vec.push_back(node_->data);
	return true;
}
void func_5()
{
	MyTree tree_1(0);
	const node *n_1 = tree_1.root();
	tree_1.add_right_child(n_1, 2);
	n_1 = tree_1.add_left_child(n_1, 3);
	tree_1.add_right_child(n_1, 4);
	vector<data_type> rec_vec;
	tree_1.in_order_traverse<bool, vector<data_type>&>(func_4, rec_vec);
	cout << rec_vec.size() << ' ';
}
void func_6()
{
	MyTree tree_1(0);
	create_tree(tree_1);
	vector<data_type> rec_vec;
	cout << "递归前序遍历：";
	tree_1.recur_preorder_traverse<bool, vector<data_type>&>(tree_1.root(), func_4, rec_vec);
	print_res(rec_vec);
	rec_vec.clear();
	cout << "非递归中序遍历：";
	tree_1.in_order_traverse<bool, vector<data_type>&>(func_4, rec_vec);
	print_res(rec_vec);
	rec_vec.clear();
	cout << "非递归前序遍历：";
	tree_1.pre_order_traverse<bool, vector<data_type>&>(func_4, rec_vec);
	print_res(rec_vec);
	rec_vec.clear();
	cout << "非递归后序遍历：";
	//tree_1.swap();
	tree_1.after_order_traverse<bool, vector<data_type>&>(func_4, rec_vec);
	print_res(rec_vec);
	rec_vec.clear();
	/*tree_1.clear();
	cout << "clear后，非递归前序遍历：";
	tree_1.pre_order_traverse<bool, vector<data_type>&>(func_4, rec_vec);
	print_res(rec_vec);
	rec_vec.clear();*/
	cout << "中序线索化，遍历：";
	tree_1.in_order_linked();
	tree_1.in_order_traverse<bool, vector<data_type>&>(func_4, rec_vec);
	print_res(rec_vec);
	rec_vec.clear();
	tree_1.clear();
	MyTree tree_2(0);
	create_tree(tree_2);
	cout << "先序线索化，遍历：";
	tree_2.pre_order_linked();
	tree_2.pre_order_traverse<bool, vector<data_type>&>(func_4, rec_vec);
	print_res(rec_vec);
	rec_vec.clear();
}