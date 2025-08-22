#include "mainNode.h"
#include<godot_cpp/classes/scene_tree.hpp>
using namespace godot;
struct sleepFunction {
    function<void()> fun;
    double cool;
    double time;
    int id;
};
struct loopFunction {
    function<bool(double delta, TimeAccumPtr acc)> fun;
    int id;
    double time[3] = {0,0,0};
};

vector<sleepFunction> sleepFuns;
void MainNode::sleep(function<void()> fun, double cool, int id) {
    sleepFuns.push_back({fun, cool, 0, id});
}

vector<loopFunction> loopFuns;
void MainNode::loop(Array init, function<bool(double delta, TimeAccumPtr acc)> fun, int id) {
    loopFuns.push_back({ fun, id });
    loopFunction& lo = loopFuns.back();
    for(int i=0; i < init.size() && i < 3; i++)
        lo.time[i] = init[i];
}

int loop_count = 0;
static int next_sequence_id = 0;
void MainNode::sequence(vector<pair<function<void()>, LoopTime>> funs) {
    if(loop_count == 0) next_sequence_id = 0;
    loop_count++;

    int base_id = next_sequence_id;
    next_sequence_id += funs.size() + 1;
    int* index = new int(0);

    LoopTime looptime = funs[*index].second;
    if(holds_alternative<double>(looptime)) {
        sleep(funs[*index].first, std::get<double>(looptime), base_id + *index);
    }else executeTrue(std::get<function<int()>>(looptime), funs[*index].first, base_id + *index);

    loop(Array(),[this, funs, index, base_id](double delta, TimeAccumPtr acc) {
        if(*index < funs.size()) {
            bool is = false;
            for(auto& fun : sleepFuns) {
                if(fun.id == base_id + *index) is = true;
                if(is) break;
            }

            if(!is) for(auto fun : loopFuns) {
                if(fun.id == base_id + *index) is = true;
                if(is) break;
            }

            if(!is) {
                (*index)++;
                if(*index < funs.size()) {
                    LoopTime looptime = funs[*index].second;
                    if(holds_alternative<double>(looptime)) {
                        sleep(funs[*index].first, std::get<double>(looptime), base_id + *index);
                    }else executeTrue(std::get<function<int()>>(looptime), funs[*index].first, base_id + *index);
                }
            }
            return false;
        }else {
            loop_count--;
            delete index;
            return true;
        }
    });
}

void MainNode::executeTrue(function<bool()> isFun, function<void()> fun, int id) {
    loop(Array(),[fun, isFun](double delta, TimeAccumPtr acc) {
        if (isFun()) {
            fun();
            return true;
        }else return false;
    }, id);
}

void MainNode::time_loop(Array init, function<void(double delta, TimeAccumPtr acc)> fun, double duration) {
    double* total_time = new double(0);
    loop(init,[fun, duration, total_time](double delta, TimeAccumPtr acc) {
        *total_time += delta;
        
        if(*total_time >= duration) {
            return true;
        }else {
            fun(delta, acc);
            return false;
        }
    });
}

void MainNode::clear_system(bool is) {
    isClear = is;
    if(is) {
        Ref<SceneTreeTimer> timer = get_tree()->create_timer(clearReload_time);
        timer->connect("timeout", Callable(this, "clear_system").bind(false), CONNECT_ONE_SHOT);
    }
}

void MainNode::system(double delta) {
    if(isClear) {
        sleepFuns.clear();
        loopFuns.clear();
        loop_count = 0;
        next_sequence_id = 0;
    }else {
        int i1 = 0;
        for(auto& fun : sleepFuns) {
            if(fun.cool <= fun.time) {
                fun.fun();
                sleepFuns.erase(sleepFuns.begin() + i1);
            }else {
                fun.time+=delta;
                i1++;
            }
        }
        
        vector<int> to_remove;
        int i2 = 0;
        for (auto& fun : loopFuns) {
            if(fun.fun(delta, &fun.time)) 
                to_remove.push_back(i2);
            i2++;
        }

        for(int i = to_remove.size() - 1; i >= 0; --i) {
            loopFuns.erase(loopFuns.begin() + to_remove[i]);
        }
    }
}