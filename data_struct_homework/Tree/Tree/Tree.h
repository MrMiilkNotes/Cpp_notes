#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
#include<vector>
#include<utility>

template<typename T>
struct Node;
template<typename T>
class Tree;

enum link_type {None, pre_order, in_order, after_order};

template<typename T>
struct Node
{
	T data;
	Node<T> *l_child, *r_child;
	bool l_sign, r_sign;
	Node() = default;
	Node(const T& data_);
	friend Tree<T>;
};
template<typename T>
class Tree
{
public:
	typedef Node<T> node_type;
	typedef const Node<T>* node_container;
	Node<T> root_parent;
	bool _empty_;
	bool _linked_;
	link_type l_type;
	//深度？？
public:
	Tree() = default;
	Tree(const T& data_);
	bool empty() const;
	const Node<T>* root();//获取根节点
	const Node<T>* to_left_child(const Node<T>* parent_);
	const Node<T>* to_right_child(const Node<T>* parent_);
	const Node<T>* add_left_child(const Node<T>* parent_, const T& data_);
	const Node<T>* add_right_child(const Node<T>* parent_, const T& data_);
	//bool rm_left_child(const Node<T>* parent_);
	//bool rm_right_child(const Node<T>* parent_);
	template<typename Y, typename...Types>
	bool recur_preorder_traverse(const node_type* nd, Y fp(node_type*, Types...), Types... args);
	template<typename Y, typename...Types>
	bool in_order_traverse(Y fp(node_type*, Types...), Types... args);
	bool in_order_linked();//其他方式的线索化还没写
	bool pre_order_linked();
	template<typename Y, typename...Types>
	bool pre_order_traverse(Y fp(node_type*, Types...), Types...args);
	template<typename Y, typename...Types>
	bool after_order_traverse(Y fp(node_type*, Types...), Types...args);
	bool swap(const Node<T>* parent = NULL) const;
	bool clear();
private:
	bool __swap__(Node<T>* ptr) const;
	static bool __collect_ptr(Node<T>* ptr, std::vector<Node<T>*>& vec);
	bool __clear__();//清除以parent为根的树
	template<typename Y, typename...Types>//没有线索化前的遍历
	bool __in_order_traverse_1(Y fp(node_type*, Types...), Types... args);
	template<typename Y, typename...Types>//线索化后的遍历
	bool __in_order_traverse_2(Y fp(node_type*, Types...), Types... args);
	template<typename Y, typename...Types>
	bool __pre_order_traverse_1(Y fp(node_type*, Types...), Types...args);
	template<typename Y, typename...Types>
	bool __pre_order_traverse_2(Y fp(node_type*, Types...), Types...args);
	template<typename Y, typename...Types>
	bool __after_order_traverse_1(Y fp(node_type*, Types...), Types...args);
	//需求：遍历算法
};
template<typename T>
inline Node<T>::Node(const T & data_)
	:data(data_), l_child(NULL), r_child(NULL),
	l_sign(false), r_sign(false) {}
template<typename T>
inline Tree<T>::Tree(const T & data_)
	: _empty_(false), _linked_(false), l_type(None)
{
	Node<T> *node = new Node<T>(data_);
	root_parent.l_child = node;
	root_parent.r_child = NULL;
	root_parent.l_sign = false;
	root_parent.r_sign = false;
}
template<typename T>
inline bool
Tree<T>::empty() const
{
	return _empty_;
}
template<typename T>
inline const Node<T>*
Tree<T>::root()
{
	return const_cast<const Node<T>*>(root_parent.l_child);
}
template<typename T>
inline const Node<T>*
Tree<T>::to_left_child(const Node<T>* parent_)
{
	return const_cast<const Node<T>*>(parent_->l_child);
}
template<typename T>
inline const Node<T>*
Tree<T>::to_right_child(const Node<T>* parent_)
{
	return const_cast<const Node<T>*>(parent_->r_child);
}
template<typename T>
inline const Node<T>*
Tree<T>::add_left_child(const Node<T>* parent_, const T & data_)
{
	Node<T> *tmp = const_cast<Node<T> *>(parent_);
	Node<T> *node = new Node<T>(data_);
	tmp->l_child = node;
	return const_cast<const Node<T> *>(node);
}
template<typename T>
inline const Node<T>*
Tree<T>::add_right_child(const Node<T>* parent_, const T & data_) {
	Node<T> *tmp = const_cast<Node<T> *>(parent_);
	Node<T> *node = new Node<T>(data_);
	tmp->r_child = node;
	return const_cast<const Node<T> *>(node);
}
template<typename T>
inline bool Tree<T>::in_order_linked()
{//线索化后不能执行之前的遍历
	if (this->_linked_) { return false; }
	this->_linked_ = true;
	this->l_type = in_order;
	std::vector<node_type*> visit_vec;
	node_type *now_ptr = root_parent.l_child, *pre_ptr = &root_parent;
	while (now_ptr || visit_vec.size() != 0)
	{
		if (now_ptr) {
			visit_vec.push_back(now_ptr);
			now_ptr = now_ptr->l_child;
		}
		else {
			now_ptr = visit_vec.back();
			visit_vec.pop_back();
			if (!pre_ptr->r_child) {
				pre_ptr->r_child = now_ptr;
				pre_ptr->r_sign = true;
			}
			if (!now_ptr->l_child) {
				now_ptr->l_child = pre_ptr;
				now_ptr->l_sign = true;
			}
			pre_ptr = now_ptr;
			now_ptr = now_ptr->r_child;
		}
	}
	return true;
}
template<typename T>
inline bool Tree<T>::pre_order_linked()
{
	if (this->_linked_) { return false; }
	this->_linked_ = true;
	this->l_type = pre_order;
	std::vector<Node<T>*> visit_vec;
	Node<T>* ptr = root_parent.l_child, *pre_ptr = &(root_parent);
	while (ptr || visit_vec.size() != 0) {
		if (ptr) {
			//visit
			if (pre_ptr->r_child == NULL) {
				pre_ptr->r_sign = true;
				pre_ptr->r_child = ptr;
			}
			if (ptr->l_child == NULL) {
				ptr->l_sign = true;
				ptr->l_child = pre_ptr;
			}
			pre_ptr = ptr;
			visit_vec.push_back(ptr);
			if (ptr->l_sign) { ptr = NULL; }
			else{ ptr = ptr->l_child; }
		}
		else
		{
			ptr = visit_vec.back();
			visit_vec.pop_back();
			if (!ptr->r_sign) {
				ptr = ptr->r_child;
			}
			else{
				ptr = NULL;
			}
		}
	}
	return true;
}
template<typename T>
inline bool Tree<T>::swap(const Node<T>* parent) const
{
	if (!parent) {
		return this->__swap__(const_cast<Node<T> *>(root_parent.l_child));
	}
	return this->__swap__(const_cast<Node<T>* >(parent));
}
template<typename T>
inline bool Tree<T>::clear()
{
	/*if (!_linked_) {
		this->_empty_ = true;
		this->_linked_ = false;
		this->l_type = None;
		return this->__clear__();
	}*/
	this->__clear__();
	this->_empty_ = true;
	this->_linked_ = false;
	this->l_type = None;
	return true;
}
template<typename T>
inline bool Tree<T>::__swap__(Node<T>* ptr) const
{
	if (ptr == NULL) { return true; }
	Node<T> *tmp = ptr->r_child;
	bool sign_tmp = ptr->r_sign;
	ptr->r_child = ptr->l_child;
	ptr->r_sign = ptr->l_sign;
	ptr->l_child = tmp;
	ptr->l_sign = sign_tmp;
	return true;
}
template<typename T>
inline bool Tree<T>::__collect_ptr(Node<T>* ptr, std::vector<Node<T>*>& vec)
{
	vec.push_back(ptr);
	return true;
}
template<typename T>
inline bool Tree<T>::__clear__()
{
	std::vector<Node<T> *> rec_vec;
	if (!_linked_ || l_type == in_order) {
		this->in_order_traverse<bool, std::vector<Node<T> *>&>(this->__collect_ptr, rec_vec);
	}
	else if( _linked_ && l_type == pre_order) {
		this->pre_order_traverse<bool, std::vector<Node<T> *>&>(this->__collect_ptr, rec_vec);
	}
	else{
		std::cout << "还没解决这种清除方式" << std::endl;
		return false;
	}
	for (auto ptr : rec_vec) {
		delete ptr;
	}
	root_parent.l_child = NULL;
	return true;
}
template<typename T>
template<typename Y, typename ...Types>
inline bool Tree<T>::recur_preorder_traverse(const node_type * nd, Y fp(node_type *, Types...), Types ...args)
{
	if (nd == NULL) { return  true; }
	(*fp)(const_cast<node_type*>(nd), args...);
	return this->recur_preorder_traverse<Y, Types...>(const_cast<const node_type*>(nd->l_child), fp, args...) &&
		this->recur_preorder_traverse<Y, Types...>(const_cast<const node_type*>(nd->r_child), fp, args...);
}
template<typename T>
template<typename Y, typename ...Types>
inline bool Tree<T>::in_order_traverse(Y fp(node_type *, Types...), Types ...args) {
	if (_linked_) {
		return this->__in_order_traverse_2<Y, Types...>(fp, args...);
	}
	return this->__in_order_traverse_1<Y, Types...>(fp, args...);
}
template<typename T>
template<typename Y, typename ...Types>
inline bool Tree<T>::pre_order_traverse(Y fp(node_type *, Types...), Types ...args) {
	if (_linked_) {
		return this->__pre_order_traverse_2<Y, Types...>(fp, args...);
	}
	return this->__pre_order_traverse_1<Y, Types...>(fp, args...);
}
template<typename T>
template<typename Y, typename ...Types>
inline bool Tree<T>::after_order_traverse(Y fp(node_type *, Types...), Types ...args)
{
	if (this->_linked_) {
		return false;
	}
	return this->__after_order_traverse_1<Y, Types...>(fp, args...);
}
template<typename T>
template<typename Y, typename ...Types>
inline bool Tree<T>::__pre_order_traverse_1(Y fp(node_type *, Types...), Types ...args)
{
	std::vector<node_type*> visit_vec;
	node_type *ptr = root_parent.l_child;
	while (ptr || visit_vec.size() != 0) {
		if (ptr) {
			(*fp)(ptr, args...);
			visit_vec.push_back(ptr);
			ptr = ptr->l_child;
		}
		else {
			ptr = visit_vec.back();
			visit_vec.pop_back();
			ptr = ptr->r_child;
		}
	}
	return true;
}
template<typename T>
template<typename Y, typename ...Types>
inline bool Tree<T>::__pre_order_traverse_2(Y fp(node_type *, Types...), Types ...args)
{
	if (l_type != pre_order) { return false; }//非先序，不能这样写
	std::vector<node_type*> visit_vec;
	node_type* ptr = root_parent.l_child;
	while (ptr) {
		(*fp)(ptr, args...);//visit
		while (!ptr->l_sign) {//指示左孩子而非上一个
			ptr = ptr->l_child;
			(*fp)(ptr, args...);
		}
		ptr = ptr->r_child;
	}
	return true;
}
template<typename T>
template<typename Y, typename ...Types>
inline bool Tree<T>::__after_order_traverse_1(Y fp(node_type *, Types...), Types ...args)
{
	typedef std::pair<bool, node_type*> node_pair;
	std::vector<node_pair> visit_vec;
	node_type* ptr = root_parent.l_child;
	visit_vec.push_back(node_pair(true, NULL));
	while (ptr || visit_vec.size() != 0) {
		if (ptr) {
			visit_vec.push_back(node_pair(true, ptr));
			if (ptr->r_child) { visit_vec.push_back(node_pair(false, ptr->r_child)); }
			ptr = ptr->l_child;
		}
		else {
			node_pair tmp = visit_vec.back();
			ptr = tmp.second;
			visit_vec.pop_back();
			while (tmp.first)
			{
				if (!tmp.second) { break; }
				(*fp)(tmp.second, args...);
				if (visit_vec.size() == 0) { break; }
				tmp = visit_vec.back();
				visit_vec.pop_back();
				ptr = tmp.second;
			}
		}
	}
	return true;
}
template<typename T>
template < typename Y, typename ...Types>
inline bool
Tree<T>::__in_order_traverse_1(Y fp(node_type*, Types...), Types ...args)
{//返回类型得修改
	std::vector<node_type*> visit_vec;
	node_type* ptr = root_parent.l_child;
	while (ptr != NULL || visit_vec.size() != 0) {
		if (ptr != 0) {
			visit_vec.push_back(ptr);
			ptr = ptr->l_child;
		}
		else {
			ptr = visit_vec.back();
			visit_vec.pop_back();
			(*fp)(ptr, args...);
			ptr = ptr->r_child;
		}
	}
	return true;
}
template<typename T>
template<typename Y, typename ...Types>
inline bool
Tree<T>::__in_order_traverse_2(Y fp(node_type *, Types...), Types ...args)
{
	std::vector<node_type*> visit_vec;
	node_type *ptr = root_parent.r_child;
	while (ptr) {
		while (!ptr->l_sign) {
			ptr = ptr->l_child;
		}
		(*fp)(ptr, args...);
		while (ptr->r_sign) {
			ptr = ptr->r_child;
			(*fp)(ptr, args...);
		}
		ptr = ptr->r_child;
	}
	return false;
}
#endif