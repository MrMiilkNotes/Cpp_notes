#ifndef GRAPH_H_INCLUDED
#define GGRAPH_H_INCLUDED
#include<vector>

template<typename T>
struct _node_base;
template<typename T>
class Graph;
template<typename T>
class BestFinder;
template<typename T>
class DFS;

template<typename T>
struct _node_base
{
	T data;

	_node_base(const T& d)
		:data(d) {};
};

template<typename T>
class Graph
{
public:
	typedef std::vector<int> node_idxs;
	typedef std::vector<int> row_type;
	typedef std::vector<row_type> mat_type;
	typedef std::vector<_node_base<T>> node_list;
	typedef _node_base<T> node_type;
	typedef Graph<T> self;
private:
	std::size_t _size_;
	mat_type* _related_mat_;
	node_list* _nodes_map_; //_nodes_map_[idx] = node
public:
	//Graph()=default;
	//Graph(const int s);
	Graph(const mat_type& m, const node_list& nl);
	Graph(const self& g);
	//·Çconst °æ±¾
	~Graph();

	std::size_t size() const;

	friend BestFinder<T>;
	friend DFS<T>;
};

template<typename T>
Graph<T>::Graph(const mat_type & m, const node_list & nl)
	:_size_(nl.size())
{
	this->_related_mat_ = new mat_type(m);
	this->_nodes_map_ = new node_list(nl);
}

template<typename T>
inline Graph<T>::Graph(const self & g)
	:_size_(g._size_)
{
	this->_related_mat_ = new mat_type(*(g._related_mat_));
	this->_nodes_map_ = new node_list(*(g._nodes_map_));
	//return this;
}

template<typename T>
Graph<T>::~Graph()
{
	delete _related_mat_;
	delete _nodes_map_;
}
template<typename T>
inline std::size_t Graph<T>::size() const
{
	return this->_size_;
}
#endif // !GRAPH_H_INCLUDED