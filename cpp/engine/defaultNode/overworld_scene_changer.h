#ifndef OVERWORLD_SCENE_CHANGER_H
#define OVERWORLD_SCENE_CHANGER_H

#include "engine/Camera/camera_fx.h"
#include "engine/Overworld/overworld.h"
#include "engine/resources/Encounters/encounter.h"
#include "engine/Overworld/battle_transistion.h"
#include<godot_cpp/classes/canvas_layer.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/variant/dictionary.hpp>
#include<godot_cpp/variant/vector2.hpp>
#include<godot_cpp/variant/vector2i.hpp>
#include<godot_cpp/variant/string.hpp>
#include<godot_cpp/variant/callable.hpp>
namespace godot {
    class OverworldSceneChanger : public CanvasLayer {
        GDCLASS(OverworldSceneChanger, CanvasLayer)

        protected:
            static void _bind_methods();

        private:
            String default_scene;
            String battle_scene_path;
            Overworld* overworld_scene = nullptr;
            ResourceLoader* loader;

            float blind_time;

            Dictionary data;
            Dictionary data_default;

            bool waiting_for_transition = false;

            void on_battle_transition_finished();
            void on_scene_setup_finished(bool transition);

            void _load_and_set_scene(const String& path);
            void _set_player_data(Node* current_scene);
            void _scene_setup_thing(bool transition);

        public:
            OverworldSceneChanger();
            ~OverworldSceneChanger();

            void _ready() override;

            void enter_room_default();
            void enter_room_path(const String& room_path, const Dictionary& extra_data = Dictionary());

            void load_cached_overworld_scene(bool transition = true);
            void load_battle(const Ref<Encounter>& battle_resource, bool transition = true, const Vector2& to_position = Vector2(48, 452));
            void load_general_scene(const String& scene_path);
            void _load_battle_scene(const String& scene_path, const Ref<Encounter>& encounter);

            void set_default_scene(const String& p_scene);
            String get_default_scene() const;

            void set_battle_scene_path(const String& p_path);
            String get_battle_scene_path() const;
    };
}


#endif