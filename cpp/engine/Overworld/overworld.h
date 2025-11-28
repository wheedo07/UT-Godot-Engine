#ifndef OVERWORLD_H
#define OVERWORLD_H

#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/audio_stream.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/property_tweener.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/variant/dictionary.hpp>
#include<godot_cpp/variant/typed_array.hpp>
#include<godot_cpp/core/binder_common.hpp>
#include<godot_cpp/core/gdvirtual.gen.inc>
#include "engine/Overworld/Characters/player_overworld.h"
#include "engine/Camera/camera_fx.h"
#include "engine/Overworld/room_entrance_node.h"
#include "engine/Camera/camera_controller.h"
namespace godot {
    class Bullet;
    class TextBox;

    class Overworld : public Node2D {
        GDCLASS(Overworld, Node2D)
        
        protected:
            static void _bind_methods();

        public:
            PlayerOverworld* player;
            AudioStreamPlayer* music_player;
            CameraController* camera;
        
        private:
            String world_name;
            NodePath player_path;
            NodePath camera_path;
            Ref<AudioStream> music;
            TypedArray<RoomEntranceNode> room_entrances;

            void set_property(Object* value);
            PlayerOverworld* get_player();
            AudioStreamPlayer* get_music_player();
            CameraController* get_camera();
        
        public:
            Overworld();
            ~Overworld();

            void _ready() override;
        
            virtual void ready();
            virtual void start_cellphone(int id);
            virtual PackedStringArray player_died();
            GDVIRTUAL0(ready);
            GDVIRTUAL1(start_cellphone, int);
            GDVIRTUAL0R(PackedStringArray, player_died);

            void _room_init(const Dictionary& data);
            void _on_saved();

            // 사용 함수
            Bullet* add_bullet(Ref<PackedScene> bullet_scene);
            Bullet* quick_bullet(Ref<PackedScene> bullet_scene, Vector2 pos, float rot_deg=0, Vector2 scale=Vector2(0.5, 0.5));
            void toggle_encounter();
            void start_music_fade_in();

            void set_world_name(const String& p_name);
            String get_world_name() const;

            void set_player_path(const NodePath& p_path);
            NodePath get_player_path() const;

            void set_music(const Ref<AudioStream>& p_music);
            Ref<AudioStream> get_music() const;
            
            void set_room_entrances(const TypedArray<RoomEntranceNode>& p_entrances);
            TypedArray<RoomEntranceNode> get_room_entrances() const;

            void set_camera(const NodePath& p_path);
            NodePath get_camera_path() const;
    };
}

#endif