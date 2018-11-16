#include <iostream>
#include "Classes.h"

using namespace std;

vector<Step_posi> next_steps(const Checkboard& ckb, const Horse& h);
void A2B(const vector<Step_posi>& vs, vector<Step>& v);
int main()
{
    //初始状态
    //cout << "Hello world!" << endl;
    Step start(0, 0), s1(-2, 1), s2(-1, 2), s3(1, 2), s4(2, 1), s5(2, -1), s6(1, -2), s7(-1, -2), s8(-2, -1);
    Step step_tmp;
    array<Step, 8> try_steps = {s1, s2, s3, s4, s5, s6, s7, s8};
    Horse horse(start, try_steps);
    Checkboard ckb;
    vector<Step_posi> tmp_steps;
    vector<Step> next_steps__;
    vector <Step> walk_vec;
    ckb.take_step(horse.posi_now());
    tmp_steps = next_steps(ckb, horse);
    A2B(tmp_steps, next_steps__);
    horse.push_back_(next_steps__);
    //
    int count_ = 0;
    while(true){
        horse.pop_back_(next_steps__);
        if(next_steps__.empty()){
            //回退
            ckb.back_step(horse.posi_now());
            horse.to_last();
            //ckb.show_ckb();
        }
        else{
            step_tmp = next_steps__.back();//退出一步行走
            next_steps__.pop_back();
            horse.push_back_(next_steps__);
            ckb.take_step(step_tmp);
            horse.to_next(step_tmp);
            if(horse.is_end()){
                count_ += 1;
                horse.show_walk();
                cout << '\n' << endl;
                if(count_ == 2){
                //ckb.show_ckb();
                    //horse.show_walk();
                    cout << "end" <<endl;
                //horse.show_walk(walk_vec);
                    break;}
            }
            //ckb.show_ckb();
            tmp_steps = next_steps(ckb, horse);
            A2B(tmp_steps, next_steps__);
            horse.push_back_(next_steps__);
        }
    }
    return 0;
}
