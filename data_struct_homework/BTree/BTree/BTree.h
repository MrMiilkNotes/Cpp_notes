#ifndef BTree_H_INCLUDED
#define BTree_H_INCLUDED
#include<vector>
#include<list>
#include<utility>
#include<iterator>
#include<algorithm>

template<typename _Tp_data_, std::size_t _Nm>
class BTree;

struct _Node_base_
{
	;
};

template<typename _Tp_data_, std::size_t _Nm>
class BTree_node : public _Node_base_
{
public:
	typedef _Tp_data_ _Tp_data;
	typedef typename BTree_node<_Tp_data, _Nm> self;
	//typedef BTree<_Tp_data, 2> _litter_node;//用于split 和 merge的数据存储
	typedef self data_bag;//对BTree开放的数据
	typedef std::vector<_Tp_data> data_list;
	typedef typename data_list::iterator data_iter;
	typedef typename data_list::const_iterator cdata_iter;
	typedef std::vector<self*> child_list;
	typedef typename child_list::iterator child_iter;
	typedef typename child_list::const_iterator cchild_iter;
public:
	static const int _top_nd_num = _Nm;
	int num_sub_nds;
	data_list nd_data;
	child_list sub_nds;
	bool final_node;
public:
	/*BTree_node()
		:num_sub_nds(0), nd_data(_Nm, _Tp_data()),
		sub_nds(_Nm, std::nullptr), final_node(false){}*/
	BTree_node(const _Tp_data& d_, const bool f_nd=true)
		:num_sub_nds(2), nd_data(1, d_),
		sub_nds(2, NULL), final_node(f_nd){}
	BTree_node(const data_list& d_l, const child_list& c_l, const bool f_nd=true)
		:num_sub_nds(c_l.size()), nd_data(d_l),
		sub_nds(c_l), final_node(f_nd){}//类似拷贝构造函数
	//析构函数

	bool compare(const _Tp_data& a, const _Tp_data& b) const;//用于比较，防止之后数据更改
	int check();//检查是否是合格的节点

	bool is_final_node() const;
	bool set_final_node(const bool is_);
	self* find_next(const _Tp_data& d_) const;//非叶子节点，找到插入位置
	data_bag* insert(const _Tp_data& d_);//插入数据，新数据，没有指针 之后进行检查是否是满足要求的节点
	data_bag* insert(data_bag* bag);//插入来自子节点的数据和指针 之后进行检查是否是满足要求的节点
	self* merge(const self& nd_);//合并点
	_Tp_data& exchg_min(const _Tp_data& d_);//交换最小值
	_Tp_data& exchg_max(const _Tp_data& d_);//
	data_bag* split();//分裂成三个节点

	//friend class BTree;
	friend std::ostream& operator<< (std::ostream& os, const self& node)
	{
		os << '[';
		std::ostream_iterator<_Tp_data_> out_iter(os, ",");
		copy(node.nd_data.cbegin(), node.nd_data.cend(), out_iter);
		/*for (int i = 0; i != node.num_sub_nds; ++i) {
			os << *((node.sub_nds)[i]);
		}*/
		os << ']';
		if (!node.final_node) {
			for (auto nd : node.sub_nds) {
				os << *(nd);
			}
		}
		
		return os;
	}
private:
	data_iter _get_data(const int& i);
	cdata_iter _get_data(const int& i) const;
	child_iter _get_ptr(const int& i);
	cchild_iter _get_ptr(const int& i) const;
};

template<typename _Tp_data_, std::size_t _Nm>
class BTree
{
public:
	typedef _Tp_data_ _Tp_data;
	typedef BTree_node< _Tp_data, _Nm> node_type;
	typedef typename BTree_node< _Tp_data, _Nm>::data_bag data_bag;
	typedef BTree<_Tp_data_, _Nm> self;
private:
	node_type* root_ptr;
public:
	BTree()
		:root_ptr(NULL){}
	//析构函数
	//拷贝构造，拷贝赋值
	bool Insert(const _Tp_data& d_);//如果存在则返回false
	bool search(const _Tp_data& d_);
	bool delete_(const _Tp_data& d_);
	//遍历 输出->在node中重载输出操作

	friend std::ostream& operator << (std::ostream& os, const self& tree)
	{
		if (tree.root_ptr != NULL) {
			os << *(tree.root_ptr);
		}
		return os;
	}
private:
	data_bag* insert(node_type* nd_ptr, const _Tp_data& d_);
};

#endif // !BTree_H_INCLUDED

template<typename _Tp_data_, std::size_t _Nm>
inline bool BTree_node<_Tp_data_, _Nm>::compare(const _Tp_data & a, const _Tp_data & b) const
{
	return b < a;
}

template<typename _Tp_data_, std::size_t _Nm>
inline bool BTree_node<_Tp_data_, _Nm>::is_final_node() const
{
	return final_node;
}

template<typename _Tp_data_, std::size_t _Nm>
inline bool BTree_node<_Tp_data_, _Nm>::set_final_node(const bool is_)
{
	this->final_node = is_;
	return is_final_node();
}

template<typename _Tp_data_, std::size_t _Nm>
inline typename BTree_node<_Tp_data_, _Nm>::self *
BTree_node<_Tp_data_, _Nm>::find_next(const _Tp_data & d_) const
{
	for (int i = 1; i != num_sub_nds - 1; ++i) {
		if (*_get_data(i) > d_) {
			return *(_get_ptr(i));
		}
	}
	return *(_get_ptr(num_sub_nds - 1));
}

template<typename _Tp_data_, std::size_t _Nm>
inline typename BTree_node<_Tp_data_, _Nm>::data_bag *
BTree_node<_Tp_data_, _Nm>::insert(const _Tp_data & d_)
{
	auto iter_d = nd_data.begin();
	for (; iter_d != nd_data.end(); ++iter_d) {
		if (compare(*(iter_d), d_)) { break; }
	}
	auto pad = iter_d - nd_data.begin();
	nd_data.insert(iter_d, d_);
	sub_nds.push_back(NULL);//最后节点的指针域都是NULL
	++num_sub_nds;
	if (num_sub_nds <= _top_nd_num) {
		return NULL;
	}
	//分裂
	pad = (_top_nd_num + 1) / 2;
	self* l_child = new self(
		data_list(nd_data.begin(), (nd_data.begin() + pad - 1)), 
		child_list(sub_nds.begin(), (sub_nds.begin() + pad))
	);
	self* r_child = new self(
		data_list((nd_data.begin() + pad), nd_data.end()),
		child_list((sub_nds.begin() + pad), sub_nds.end())
	);
	auto res = new self(
		data_list(1, *_get_data(pad - 1)),
		child_list{l_child, r_child}
	);
	res->final_node = false;
	delete this;
	return res;
}

template<typename _Tp_data_, std::size_t _Nm>
inline typename BTree_node<_Tp_data_, _Nm>::data_bag *
BTree_node<_Tp_data_, _Nm>::insert(data_bag * bag)
{
	this->final_node = false;
	auto iter_d = nd_data.begin();
	for (; iter_d != nd_data.end(); ++iter_d) {
		if (compare(*(iter_d), *(bag->_get_data(0)))) { break; }
	}
	auto pad = iter_d - nd_data.begin();
	nd_data.insert(iter_d, *(bag->_get_data(0)));
	//child_iter iter_c = sub_nds.begin() + pad;
	sub_nds.erase(sub_nds.begin() + pad);
	sub_nds.insert(sub_nds.begin() + pad, *(bag->_get_ptr(1)));
	sub_nds.insert(sub_nds.begin() + pad, *(bag->_get_ptr(0)));
	//sub_nds.pop_back();
	++num_sub_nds;

	//正常节点
	if (num_sub_nds <= _top_nd_num) {
		return NULL;
	}
	//分裂
	//this->final_node = false;
	pad = (_top_nd_num + 1) / 2;
	self* l_child = new self(
		data_list(nd_data.begin(), (nd_data.begin() + pad - 1)),
		child_list(sub_nds.begin(), (sub_nds.begin() + pad)),
		false
	);
	self* r_child = new self(
		data_list((nd_data.begin() + pad), nd_data.end()),
		child_list((sub_nds.begin() + pad), sub_nds.end()),
		false
	);
	auto res = new self(
		data_list(1, *_get_data(pad - 1)),
		child_list{ l_child, r_child }
	);
	res->final_node = false;
	return res;
}

template<typename _Tp_data_, std::size_t _Nm>
inline typename BTree_node<_Tp_data_, _Nm>::self *
BTree_node<_Tp_data_, _Nm>::merge(const self & nd_)
{
	return NULL;
}

template<typename _Tp_data_, std::size_t _Nm>
inline bool BTree<_Tp_data_, _Nm>::Insert(const _Tp_data & d_)
{
	if (this->root_ptr == NULL) { //根节点
		this->root_ptr = new node_type(d_);
		return true;
	}
	auto res = this->insert(root_ptr, d_);//根节点也有可能需要分裂-->litter nodeg构建node
	if (res != NULL) {
		root_ptr = res;
	}
	return true;
}

template<typename _Tp_data_, std::size_t _Nm>
inline typename BTree<_Tp_data_, _Nm>::data_bag* 
BTree<_Tp_data_, _Nm>::insert(node_type * nd_ptr, const _Tp_data & d_)
{
	data_bag* bag_ptr;
	if (nd_ptr->is_final_node()) { //尾节点，可以在当前节点插入
		bag_ptr = nd_ptr->insert(d_);
		return bag_ptr;
	}
	else {
		bag_ptr = this->insert(nd_ptr->find_next(d_), d_);
	}
	if (bag_ptr != NULL) { //有包，说明有新的节点，需要插入到当前节点
		return nd_ptr->insert(bag_ptr);//合并到当前节点
	}
	return NULL;
}

template<typename _Tp_data_, std::size_t _Nm>
inline  typename BTree_node<_Tp_data_, _Nm>::data_iter
BTree_node<_Tp_data_, _Nm>::_get_data(const int & i)
{
	auto iter = this->nd_data.begin();
	return (iter + i);
}

template<typename _Tp_data_, std::size_t _Nm>
inline typename BTree_node<_Tp_data_, _Nm>::cdata_iter 
BTree_node<_Tp_data_, _Nm>::_get_data(const int & i) const
{
	auto iter = this->nd_data.begin();
	return (iter + i);
}

template<typename _Tp_data_, std::size_t _Nm>
inline typename BTree_node<_Tp_data_, _Nm>::child_iter
BTree_node<_Tp_data_, _Nm>::_get_ptr(const int & i)
{
	auto iter = this->sub_nds.begin();
	return (iter + i);
}

template<typename _Tp_data_, std::size_t _Nm>
inline typename BTree_node<_Tp_data_, _Nm>::cchild_iter 
BTree_node<_Tp_data_, _Nm>::_get_ptr(const int & i) const
{
	auto iter = this->sub_nds.begin();
	return (iter + i);
}