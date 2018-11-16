#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include "walk_array.h"

using namespace std;
/*输入为 size row，col，下面的输入默认为9 8
0 0 1 0 0 0 1 0
0 0 1 0 0 0 1 0
0 0 0 0 1 1 0 1
0 1 1 1 0 0 1 0
0 0 0 1 0 0 0 0
0 1 0 0 0 1 0 1
0 1 1 1 1 0 0 1
1 1 0 0 0 1 0 1
1 1 0 0 0 0 0 0
*/

int main()
{
    typedef pair<int, int> posi;
    typedef pair<posi, int> position;//每一步类型
    //PosiVec<position> walk_table, choices;
    //PosiVec<PosiVec<position>> choices_table;
    vector<position> walk_table, choices;
    vector<vector<position>> choices_table;
    map<int, posi> dir_table = {
        {1, posi(0, 1)},
        {2, posi(1, 0)},
        {3, posi(0, -1)},
        {4, posi(-1, 0)},
    };
    int ckb_size1, ckb_size2;
    //输入
    cout << "table size \n>>>";
    cin >> ckb_size1;
    cout << endl;
    cin >> ckb_size2;
    cout << endl;
    int checktable[ckb_size1][ckb_size2];
    int tmp, idx=0;
    while(cin >> tmp){
        checktable[idx/ckb_size2][(idx++)%ckb_size2] = tmp;
        //cout << ' ' << idx/ckb_size << ' ' << (idx++)%ckb_size;
        if(idx == ckb_size1 * ckb_size2){
            break;
        }
    }
    //search_direct(checktable, ckb_size);
    //cout << checktable[1][1];
    posi posi_now = {0, 0}, posi_start = {0, 0};
    posi posi_end = {ckb_size1 - 1, ckb_size2 - 1};
    bool flag = true;
    while(flag){
        if(checktable[posi_now.first][posi_now.second] == 0){//新的位置，需要搜索新策略
            vector<position> walk_vec;
            for(int i = 1; i != 5; ++i){
                int x = posi_now.first + dir_table[i].first;
                int y = posi_now.second + dir_table[i].second;
                if(x >= 0 && x < ckb_size1 && y >= 0 && y < ckb_size2 && checktable[x][y] != 1){
                    //可以行走的下一步
                    position tmp_posi(posi(x, y), i);
                    walk_vec.push_back(tmp_posi);
                }
            }
            choices_table.push_back(walk_vec);//得到策略，丢尽策略栈
            checktable[posi_now.first][posi_now.second] = 1;
        }
        //不是新的位置 弹出策咯来行走
        if(choices_table.size() == 0){
            cout << "no solution" << endl;
            break;
        }
        choices = choices_table.back();
        choices_table.pop_back();

        if(choices.size() != 0){//策咯非空
            position tmp_posi_1 = choices.back();
            choices.pop_back();//拿出策略
            choices_table.push_back(choices);
            walk_table.push_back(tmp_posi_1);
            posi_now = tmp_posi_1.first;//位置更新
            if(posi_now.first == posi_end.first && posi_now.second == posi_end.second){
                // walk_table.push_back(position(posi_end, 0));
                for(auto iter = walk_table.begin(); iter != (walk_table.end() + 1); ++iter){
                    if(iter == walk_table.begin()){
                        cout << "[(" << posi_start.first << ',' << posi_start.second << ")" << (*iter).second << "]" << endl;
                    }
                    else if(iter == (walk_table.end())){
                        cout << "[(" << (*(iter - 1)).first.first << ',' << (*(iter - 1)).first.second << ")" << 0 << "]" << endl;
                    }
                    else{
                        cout << "[(" << (*(iter - 1)).first.first << ',' << (*(iter - 1)).first.second << ")" << (*(iter)).second << "]" << endl;
                    }
                }
                break;
            }
        }
        else
        {//策略为空
            position tmp_posi_2 = walk_table.back();
            walk_table.pop_back();//后退一步
            checktable[posi_now.first][posi_now.second] = 0;
            posi_now = walk_table.back().first;
        }
    }
    return 0;
}
