#ifndef DFS_H_INCLUDED
#define DFS_H_INCLUDED
#include<vector>
#include<algorithm>
#include<numeric>
#include<iterator>
extern int INF;
template<typename T>
struct _node_base;
template<typename T>
class Graph;

template<typename T>
class DFS
{
public:
	typedef Graph<T> map_type;
	typedef typename Graph<T>::mat_type mat_type;
	typedef typename Graph<T>::row_type row_type;
	typedef typename Graph<T>::node_idxs node_idxs;
	typedef typename Graph<T>::node_list node_list;
private:
	mat_type* map;
	std::size_t size_;
	//DFS数据域
	row_type wei_rec;
	node_idxs nodes;
	int to_;
	//
	std::vector<int> dfn, low;
	int tarjan_index;
	//share
	std::vector<bool> visited_rec;
	std::vector<int> node_rec;
public:
	DFS(const Graph<T>& m);
	~DFS();
	void dfs(const int from_=0, const int to_=0, const int type=0);
	void tarjan(const int idx = 0);
	void find_all_routes(const int from_ = 0, const int to_ = 0);
	void find_if_have(const node_idxs& nodes, const int from_ = 0, const int to_ = 0);
private:
	void _dfs_(const int idx, const int type = 1);
	void _tarjan_(const int idx);
	bool _find_all_routes_();
	bool _find_if_have_();
};

#endif // !DFS_H_INCLUDED

template<typename T>
inline DFS<T>::DFS(const Graph<T>& m)
	:size_(m.size()), dfn(m.size(), 0), low(m.size(), 0),
	tarjan_index(0)
{
	map = new mat_type(*(m._related_mat_));
}

template<typename T>
inline DFS<T>::~DFS()
{
	delete map;
}

template<typename T>
inline void DFS<T>::dfs(const int from_, const int to_, const int type)
{
	this->to_ = to_;
	visited_rec.assign(size_, false);
	node_rec.clear();
	//调用深度搜索
	node_rec.push_back(from_);
	visited_rec[from_] = true;
	this->_dfs_(0);
}

template<typename T>
inline void DFS<T>::_dfs_(const int idx, const int type)
{
	auto next_nodes = (*map)[idx];
	bool end_node = true;
	
	if (type==1 && this->_find_all_routes_()) {
		return;
	}

	for (auto i = 0; i != next_nodes.size(); ++i) {
		if (next_nodes[i] != 0 && !visited_rec[i]) {
			end_node = false;
			visited_rec[i] = true;
			node_rec.push_back(i);
			wei_rec.push_back(next_nodes[i]);
			this->_dfs_(i);
			wei_rec.pop_back();
			node_rec.pop_back();
			visited_rec[i] = false;
		}
	}
}

template<typename T>
inline void DFS<T>::_tarjan_(const int idx)
{
	dfn[idx] = low[idx] = tarjan_index++;
	bool main_node = false;
	bool root_visited = false;
	node_rec.push_back(idx);
	visited_rec[idx] = true;
	auto next_nodes = (*map)[idx];
	for (int i = 0; i != next_nodes.size(); ++i) {
		if (next_nodes[i] != 0 && !visited_rec[i]) {
			this->_tarjan_(i);
			if (!main_node) {
				main_node = (low[i] >= dfn[idx])&&((idx != 0)||root_visited);
			}
			low[idx] = low[idx] < low[i] ? low[idx] : low[i];
			root_visited = true;
		}
		else if (next_nodes[i] != 0 && std::find(node_rec.cbegin(), node_rec.cend(), i) != node_rec.cend()) {
			low[idx] = low[idx] < dfn[i] ? low[idx] : dfn[i];
		}
	}
	if (main_node) {
		std::cout << "关节点：" << idx << ' ' << std::endl;
	}
	if (dfn[idx] == low[idx]) {
		int tmp;
		do {
			tmp = node_rec.back();
			node_rec.pop_back();
		} while (tmp != idx);
	}
}

template<typename T>
inline bool DFS<T>::_find_all_routes_()
{
	if (this->to_ == node_rec.back()) {
		std::cout << "花费：" << accumulate(wei_rec.begin(), wei_rec.end(), 0) << std::endl;
		std::ostream_iterator<int> out_iter(std::cout, "->");
		copy(node_rec.cbegin(), node_rec.cend(), out_iter);
		/*for (auto nd : node_rec) {
			std::cout << nd << "->";
		}*/
		std::cout << "end" << std::endl;
		return true;
	}
	return false;
}

template<typename T>
inline bool DFS<T>::_find_if_have_()
{
	return true;
}

template<typename T>
inline void DFS<T>::tarjan(const int idx)
{
	tarjan_index = 0;
	visited_rec.assign(size_, false);
	node_rec.clear();
	
	this->_tarjan_(idx);
}

template<typename T>
inline void DFS<T>::find_all_routes(const int from_, const int to_)
{
	this->dfs(from_, to_, 1);
}

template<typename T>
inline void DFS<T>::find_if_have(const node_idxs& nodes, const int from_, const int to_)
{
	this->nodes.assign(nodes);

}
