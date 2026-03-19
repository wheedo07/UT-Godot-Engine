#include "mainNode.h"
#include<memory>
#include<godot_cpp/classes/scene_tree.hpp>
using namespace godot;
using namespace std;

void MainNode::sleep(function<void()> fun, double cool, int id) {
    sleepFuns.push_back({fun, cool, 0, id});
}

void MainNode::loop(Array init, function<bool(double delta, TimeAccumPtr acc)> fun, int id) {
    loopFuns.push_back({ fun, id });
    loopFunction& lo = loopFuns.back();
    for(int i=0; i < init.size() && i < 3; i++) {
        lo.time[i] = static_cast<double>(init[i]);
    }
}

void MainNode::sequence(vector<pair<function<void()>, LoopTime>> funs) {
    if(funs.empty()) return;

    if(loop_count == 0) next_sequence_id = 0;
    loop_count++;

    const int base_id = next_sequence_id;
    next_sequence_id += funs.size() + 1;
    auto index = std::make_shared<size_t>(0);

    auto schedule_current = [this, funs, index, base_id]() {
        if(*index >= funs.size()) return;

        const LoopTime &looptime = funs[*index].second;
        int current_id = base_id + (*index);

        if(holds_alternative<double>(looptime)) {
            sleep(funs[*index].first, std::get<double>(looptime), current_id);
        }else {
            executeTrue(std::get<function<bool()>>(looptime), funs[*index].first, current_id);
        }
    };

    schedule_current();
    loop(Array(), [this, funs, index, base_id, schedule_current](double delta, TimeAccumPtr acc) mutable {
        if(*index >= funs.size()) {
            if(loop_count > 0) loop_count--;
            return true;
        }

        int current_id = base_id + (*index);
        if(!has_pending_id(current_id)) {
            (*index)++;

            if(*index >= funs.size()) {
                if(loop_count > 0) loop_count--;
                return true;
            }
            schedule_current();
        }
        return false;
    });
}

bool MainNode::has_pending_id(int id) {
    for(auto &fun : sleepFuns) {
        if(fun.id == id) return true;
    }

    for(auto &fun : loopFuns) {
        if(fun.id == id) return true;
    }
    return false;
}

void MainNode::executeTrue(function<bool()> isFun, function<void()> fun, int id) {
    loop(Array(), [fun, isFun](double delta, TimeAccumPtr acc) {
        if(isFun()) {
            fun();
            return true;
        }else return false;
    }, id);
}

void MainNode::time_loop(Array init, function<void(double delta, TimeAccumPtr acc)> fun, double duration) {
    auto total_time = std::make_shared<double>(0);

    loop(init, [fun, duration, total_time](double delta, TimeAccumPtr acc) {
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
        return;
    }

    size_t i = 0;
    while(i < sleepFuns.size()) {
        auto& fun = sleepFuns[i];

        if(fun.cool <= fun.time) {
            fun.fun();
            sleepFuns.erase(sleepFuns.begin() + i);
        }else {
            fun.time += delta;
            i++;
        }
    }
    
    vector<size_t> to_remove;
    for(size_t i=0; i < loopFuns.size(); i++) {
        if(loopFuns[i].fun(delta, &loopFuns[i].time)) {
            to_remove.push_back(i);
        }
    }
    
    for(auto it = to_remove.rbegin(); it != to_remove.rend(); it++) {
        loopFuns.erase(loopFuns.begin() + *it);
    }
}