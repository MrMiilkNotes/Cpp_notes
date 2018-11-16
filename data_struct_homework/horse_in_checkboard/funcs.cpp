#include <iostream>
#include <algorithm>
#include "Classes.h"

using namespace std;

bool sort_by_posi(const Step_posi& s1, const Step_posi& s2)
{
    return s1.num_nei >= s2.num_nei;
}

vector<Step_posi> next_steps(const Checkboard& ckb, const Horse& h)
{
    //获取可行的stepts以及相应的可行位置数量
    array<Step, 8> try_next;
    vector<Step_posi> res_;
    Step step_tmp;
    Step posi_now = h.posi_now();
    h.get_try_steps(ckb, try_next);
    int j = 0;
    for(unsigned int i=0; i < try_next.size(); i++){
        step_tmp = try_next[i];
        if(step_tmp.le() != 0){
            Step rec(posi_now.le()+step_tmp.le(), posi_now.ri()+step_tmp.ri());
            int num_nei = ckb.num_nei(rec, h);
            //Step_posi tmp(rec, num_nei);
            res_.push_back(Step_posi(rec, num_nei));
        }
    }
    //选出最优解
    sort(res_.begin(), res_.end(), sort_by_posi);
    return res_;
}

void A2B(const vector<Step_posi>& vs, vector<Step>& v)
{
    v.clear();
    for(unsigned int i=0; i < vs.size(); i++){
        v.push_back(vs[i].s);
    }
}
