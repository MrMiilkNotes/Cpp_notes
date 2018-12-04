#ifndef BEAT_ROUTE_H_INCLUDED
#define BEST_ROUTE_H_INCLUDED
extern int INF;

template<typename T>
struct _node_base;
template<typename T>
class Graph;

template<typename T>
class BestFinder
{
public:
	typedef Graph<T> map_type;
	typedef typename Graph<T>::mat_type mat_type;
	typedef typename Graph<T>::row_type row_type_;
private:
	map_type* map;
	mat_type* route_map;
public:
	BestFinder(const Graph<T>& m)
	{
		map = new map_type(m);
		route_map = new mat_type();
	}

	void Floyd();

	void print_best_way(const int from, const int to) const;


};

#endif // !BEAT_ROUTE_H_INCLUDED

template<typename T>
inline void BestFinder<T>::Floyd()
{
	//³õÊ¼»¯route_map
	auto size_ = map->size();
	for (auto i = 0; i != size_; ++i) {
		(*route_map).push_back(row_type_(size_, i));
	}
	//¸¥ÂåÒÁµÂ
	for (auto k = 0; k != size_; ++k) {
		for (auto j = 0; j != size_; ++j) {
			for (auto i = 0; i != size_; ++i) {
				if ((*(map->_related_mat_))[j][k]!=INF &&
					(*(map->_related_mat_))[k][i]!=INF &&
					(*(map->_related_mat_))[j][k] + (*(map->_related_mat_))[k][i]
					< (*(map->_related_mat_))[j][i]) {
					(*(map->_related_mat_))[j][i] 
						= (*(map->_related_mat_))[j][k] + (*(map->_related_mat_))[k][i];
					(*route_map)[j][i] = k;
				}
			}
		}
	}
}

template<typename T>
inline void BestFinder<T>::print_best_way(const int from, const int to) const
{
	std::cout << "cost: " << (*(map->_related_mat_))[from][to] << '\n';
	std::cout << "route:";
	int k = to;
	do {
		std::cout <<k << "<-";
		k = (*route_map)[from][k];
	} while (from != k);
	std::cout << from << std::endl;
}
