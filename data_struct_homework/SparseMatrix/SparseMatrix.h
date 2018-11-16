#ifndef SPARSEMATRIX_H_INCLUDED
#define SPARSEMATRIX_H_INCLUDED

#include<vector>
#include<utility>

template<typename T>
class SparseMatrix
{
public://������Ҫ�ı�������
	typedef std::pair<int, int> posi;
	typedef std::pair<posi, T> node;
	typedef std::pair<int, int> size_type;
private:
	std::vector<node> data;
	int num_rows, num_col, num_nonzero;
	std::vector<int> nonezero_posi;

	SparseMatrix<T>& __add__(const SparseMatrix<T>& m) const;//�Ӽ��ĺ���
	static void insert(std::vector<node>& d_vec, const posi& posi_, T& val);//���ھ���˷�����¼һ��һ�е�vector
	static void del_zero_node(std::vector<node>& d_vec);//ɾ��vector����ĵ�
public:
	SparseMatrix(const int& n_r, const int& n_c, const std::vector<node>& d)
		:data(d), num_rows(n_r), num_col(n_c), num_nonzero(d.size())
	{//��ʼ��������ÿ�з���Ԫ�ؿ�ʼλ��
		int rec = 0;
		int row = -1;//��0��ʼ������Ԫ����±�
		//std::cout << "nonzero:";
		for (auto nd: data) {
			++row;
			if(nd.first.first > rec) {//������Ϊ��
				int num_null = nd.first.first - rec;
				for (int i = 0; i != num_null; ++i) {
					nonezero_posi.push_back(row);
					//std::cout << row << ' ';
				}
				rec = nd.first.first;
			}
			if (nd.first.first == rec) {
				nonezero_posi.push_back(row);
				//std::cout << row << ' ';
				++rec;
			}
		}
		++row;
		while (rec != num_rows) {
			++rec;
			nonezero_posi.push_back(row);
			//std::cout << row << ' ';
		}
		nonezero_posi.push_back(row);//���һ�н��м�¼���Ա����һ�в�ѯ
		//std::cout << row << ' ';
		//std::cout << '\n';
	}
	SparseMatrix() = default;
	SparseMatrix<T>& operator+(const SparseMatrix<T>& m) const;
	SparseMatrix<T>& operator-(const SparseMatrix<T>& m) const;
	SparseMatrix<T>& operator*(const SparseMatrix<T>& m) const;

	template<typename Y>
	friend std::ostream& operator<< (std::ostream& os, SparseMatrix<Y>& m);

};

template<typename T>
inline SparseMatrix<T>&
SparseMatrix<T>::__add__(const SparseMatrix<T>& m) const
{
	std::vector<node> added_vec;
	auto m1_iter = data.begin();
	auto m2_iter = m.data.begin();
	for (; m1_iter != data.end() || m2_iter != m.data.end();) {
		if (m1_iter == data.end()) {
			added_vec.push_back(*(m2_iter));
			++m2_iter;
			continue;
		}
		if (m2_iter == m.data.end()) {
			added_vec.push_back(*(m1_iter));
			++m1_iter;
			continue;
		}

		if ((*m1_iter).first.first == (*m2_iter).first.first) {
			if ((*m1_iter).first.second == (*m2_iter).first.second) {
				node n((*m1_iter));
				n.second += (*m2_iter).second;
				if (n.second) { added_vec.push_back(n); }
				++m2_iter;
				++m1_iter;
			}
			else if ((*m1_iter).first.second > (*m2_iter).first.second) {
				added_vec.push_back(*(m2_iter));
				++m2_iter;
			}
			else{
				added_vec.push_back(*(m1_iter));
				++m1_iter;
			}
		}
		else if ((*m1_iter).first.first > (*m2_iter).first.first) {
			added_vec.push_back(*(m2_iter));
			++m2_iter;
		}
		else{
			added_vec.push_back(*(m1_iter));
			++m1_iter;
		}
	}
	return *(new SparseMatrix<T>((*this).num_col, m.num_rows, added_vec));
}

template<typename T>
inline void
SparseMatrix<T>::insert(std::vector<node>& d_vec, const posi & posi_, T & val)
{
	auto iter = d_vec.begin();
	for (; iter != d_vec.end(); ++iter) {
		if ((*iter).first == posi_) {
			(*iter).second += val;
			return;
		}
	}
	d_vec.push_back(node(posi_, val));
}

template<typename T>
inline void SparseMatrix<T>::del_zero_node(std::vector<node>& d_vec)
{
	std::vector<node> tmp_vec;
	for (auto nd : d_vec) {
		if (nd.second != 0) {
			tmp_vec.push_back(nd);
		}
	}
	d_vec.swap(tmp_vec);
}

template<typename T>
inline SparseMatrix<T>&
SparseMatrix<T>::operator+(const SparseMatrix<T>& m) const
{
	return (*this).__add__(m);
}

template<typename T>
inline SparseMatrix<T>&
SparseMatrix<T>::operator-(const SparseMatrix<T>& m) const
{
	SparseMatrix<T> m_(m);
	//std::cout << m_ << std::endl;
	auto iter = m_.data.begin();
	for (; iter != m_.data.end(); ++iter) {
		(*iter).second = -(*iter).second;
	}
	return (*this).__add__(m_);
}

template<typename T>
inline SparseMatrix<T>& SparseMatrix<T>::operator*(const SparseMatrix<T>& m) const
{
	std::vector<node> res_vec;
	posi posi_;
	int m1_y, m2_x;//������˵������
	auto iter_m1 = (*this).data.begin();
	for (; iter_m1 != (*this).data.end(); ++iter_m1) {
		posi_.first = (*iter_m1).first.first;//��ƥ��
		m1_y = (*iter_m1).first.second;
		//�ڵڶ���������Ѱ��λ��ƥ���
		for (int i = m.nonezero_posi[m1_y]; i != m.nonezero_posi[m1_y + 1]; ++i) {
			posi_.second = m.data[i].first.second;
			T val = (*iter_m1).second * m.data[i].second;
			insert(res_vec, posi_, val);
		}
	}
	del_zero_node(res_vec);
	return *(new SparseMatrix<T>((*this).num_rows, m.num_col, res_vec));
}

template<typename Y>
std::ostream& operator<< (std::ostream& os, SparseMatrix<Y>& m)
{
	for (auto a : m.data) {
		os << "[" << a.first.first << "," << a.first.second << "," << a.second << "]" << '\n';
	}
	return os;
}

#endif // !SPARSEMATRIX_H_INCLUDED
