#ifndef __MainNode_H__
#define __MainNode_H__
#include "engine/defaultNode/scene_container.h"
#include "engine/resources/Dialogues/dialogues.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include<functional>
#include<variant>

#define clearReload_time 1.3f
using LoopTime = std::variant<double, std::function<bool()>>;
using TimeAccumPtr = double (*)[3];
namespace godot {
    using UFus = UtilityFunctions;
    class MainNode : public SceneContainer {
        GDCLASS(MainNode, SceneContainer);

        protected:
            static void _bind_methods();

        private:
            struct sleepFunction {
                std::function<void()> fun;
                double cool;
                double time;
                int id;
            };
            struct loopFunction {
                std::function<bool(double delta, TimeAccumPtr acc)> fun;
                int id;
                double time[3] = {0,0,0};
            };
            
        private:
            ResourceLoader *loader;
           
            // system functions
            std::vector<sleepFunction> sleepFuns;
            std::vector<loopFunction> loopFuns;

            // sequence
            int loop_count;
            int next_sequence_id;

            // system clear
            bool isClear;

            bool has_pending_id(int id);
        
        public:
            MainNode();
            ~MainNode();

            void _ready() override;
            void _process(double delta) override;

            void clear_system(bool is=true);
            void system(double delta);
            void sleep(std::function<void()> fun, double cool, int id=-1);
            void loop(Array init, std::function<bool(double delta, TimeAccumPtr acc)> fun, int id=-1);
            void time_loop(Array init, std::function<void(double delta, TimeAccumPtr acc)> fun, double duration);
            void executeTrue(std::function<bool()> isFun, std::function<void()> fun, int id=-1);
            void sequence(std::vector<std::pair<std::function<void()>, LoopTime>> funs);

            Ref<Dialogues> dia();
    };
}

#endif