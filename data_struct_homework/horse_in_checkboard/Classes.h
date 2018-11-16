#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED
#include<vector>
#include<array>
using namespace std;
class Step;
class Checkboard;
class Horse;
static const int SIZE = 8;

class Step
{
private:
    int left, right;
public:
    Step(int l=0, int r=0)
        :left(l), right(r)
    {}
    Step(const Step& s)
    {
        left = s.left;
        right = s.right;
    }
    int le() const {return left;}
    int ri() const {return right;}
    friend Checkboard;
    friend Horse;
};
struct Step_posi
{
    Step s;
    int num_nei;
    Step_posi(const Step& s_, const int n)
        :s(s_), num_nei(n)
    {}
};
class Horse
{
private:
    int stepts_took;
    vector<vector<Step>> choices_table;//选择栈，每次会
    vector <Step> walk_table;
    Step posion_now;
protected:
    array<Step, 8> try_steps;
public:
    Horse(const Step& s, array<Step, 8>& a)
        :stepts_took(1), posion_now(s), try_steps(a)
    {}
    //choices_table
    void pop_back_(vector<Step>& s);//弹出末端元素
    bool push_back_(const vector<Step>& v);//添加末端元素
    //walk_table
    void to_last();//返回上一步，步数减1//查看末端元素(上一步)
    void to_next(const Step& s);//
    void show_walk() const;
    //stepts_took
    bool is_end() const;
    //查看现在在那一步
    Step posi_now() const;
    //try_stepts
    void get_try_steps(const Checkboard& ckb, array<Step, 8>& next_stepts) const;
    array<Step, 8> get_try_steps() const;
};
class Checkboard
{
private:
    int size_;
    int a[SIZE][SIZE];
public:
    Checkboard()
        :size_(SIZE)
    {
        for(int i = 0; i < size_; i++){
            for(int j = 0; j < size_; j++){
                a[i][j] = 0;
            }
        }
    }
    void take_step(const Step& s);//行走一格，1
    void back_step(const Step& s);//后退一格，0
    int num_nei(const Step&, const Horse&) const;
    void show_ckb() const;

    friend Horse;
};
inline void
Checkboard::take_step(const Step& s)
{
    a[s.left][s.right] = 1;
}
inline void
Checkboard::back_step(const Step& s)
{
    a[s.left][s.right] = 0;
}
inline void
Checkboard::show_ckb() const
{
    cout << '\n';
    for(int i=0; i < SIZE; i++){
        for(int j=0; j < SIZE; j++){
            cout << a[i][j];
        }
        cout << '\n';
    }
    cout << endl;
}
inline int
Checkboard::num_nei(const Step& s, const Horse& h) const
{//某个位置的下一个可行位置数目
    int j=0;
    array<Step, 8> next_steps=h.get_try_steps();
    //h.get_try_steps(*this, next_steps);
    for(unsigned int i=0; i < next_steps.size(); i++){
        if(next_steps[i].le() == 0) break;
        int x_ = s.le() + next_steps[i].left;
        int y_ = s.ri() + next_steps[i].right;
        if(x_ < SIZE && x_ >= 0 && y_ >= 0 && y_ <SIZE &&a[x_][y_] == 0){
            j++;
        }
    }
    return j;
}
//int Checkboard::size_ = 8;

inline void
Horse::pop_back_(vector<Step>& s)
{
    s = choices_table.back();
    choices_table.pop_back();
}
inline bool
Horse::push_back_(const vector<Step>& v)
{
    choices_table.push_back(v);
    return true;
}
inline void
Horse::to_last()
{
    stepts_took--;
    walk_table.pop_back();
    posion_now = walk_table.back();//获取上一步
}
inline void
Horse::to_next(const Step& s)
{
    posion_now = s;
    stepts_took++;
    walk_table.push_back(s);
    //cout << "taking step " << stepts_took << endl;
}
inline bool
Horse::is_end() const
{
    return stepts_took == SIZE * SIZE;
}
inline Step
Horse::posi_now() const
{
    Step s = posion_now;
    return s;
}
inline void
Horse::get_try_steps(const Checkboard& ckb, array<Step, 8>& next_stepts) const
{//寻找的是当前位置的所有下一个可行位置
    int j=0;
    for(unsigned int i=0; i < try_steps.size(); i++){
        int x_ = posion_now.left + try_steps[i].left;
        int y_ = posion_now.right + try_steps[i].right;
        if(x_ < SIZE && x_ >= 0 && y_ >= 0 && y_ <SIZE && ckb.a[x_][y_] == 0){
            next_stepts[j++] = try_steps[i];
        }
    }
}
inline array<Step, 8>
Horse::get_try_steps() const
{
    array<Step, 8> tmp = try_steps;
    return tmp;
}
inline void
Horse::show_walk() const
{
    int show_table[SIZE][SIZE] = {0};
    int i = 0;
    for(auto st: walk_table){
        show_table[st.left][st.right] = ++i;
    }
    for(int i = 0; i < SIZE; ++i){
        for(int j = 0; j < SIZE; ++j){
            cout << show_table[i][j] << "\t";
        }
        cout << '\n';
    }
    cout << endl;
}
#endif // CLASSES_H_INCLUDED
