#include <iostream>
#include <algorithm>

using namespace std;

class Triple
{
private:
    double d[3];
public:
    Triple(const double a1, const double a2, const double a3)
        {
            d[0] = a1; d[1] = a2; d[2] = a3;
        }
    double get(const int idx);
    bool put(const int idx, const double val);
    bool isAscending();
    bool isDecending();
    double max_val();
    double min_val();
};

inline double
Triple::get(const int idx){
    if(1 > idx || 3 < idx) return NULL;
    return d[idx-1];
}

inline bool
Triple::put(const int idx, const double val){
    if(1 > idx || 3 < idx) return false;
    d[idx - 1] = val;
    return true;
}

inline bool
Triple::isAscending(){
    return d[2] >= d[1] && d[1] >= d[0];
}

inline bool
Triple::isDecending(){
    return d[0] >= d[1] && d[1] >= d[2];
}

inline double
Triple::max_val(){
    return *max_element(d, d+3);
}
inline double
Triple::min_val(){
    return *min_element(d, d+3);
}

int main()
{
    Triple a(1., 2., 3.);
    cout << "Test" << endl;
    //cout << "a:" <<a << endl;
    cout << "put:" <<a.put(1, 0.) << endl;
    cout << "get:" <<a.get(1) <<a.get(2) <<a.get(3) << endl;
    cout << "isAscending:" <<a.isAscending() << endl;
    cout << "isDecending:" <<a.isDecending() << endl;
    cout << "max:" <<a.max_val() << endl;
    cout << "min:" <<a.min_val() << endl;
    return 0;
}
