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
#include "engine/Overworld/Characters/player_overworld.h"
#include "engine/Camera/camera_fx.h"
#include "engine/Overworld/room_entrance_node.h"
#include "engine/Overworld/textBox/text_box.h"
namespace godot {
    class Overworld : public Node2D {
        GDCLASS(Overworld, Node2D)
        
        protected:
            static void _bind_methods();

        public:
            PlayerOverworld* player;
            AudioStreamPlayer* music_player;
            CameraFx* camera;
        
        private:
            String world_name;
            NodePath player_path;
            NodePath camera_path;
            Ref<AudioStream> music;
            TypedArray<RoomEntranceNode> room_entrances;
            Ref<PackedScene> text_box;

            void set_property(Object* value);
            PlayerOverworld* get_player();
        
        public:
            Overworld();
            ~Overworld();

            void _ready() override;
        
            virtual void ready();

            void room_init(const Dictionary& data);
            void _on_saved();
            void start_music_fade_in();

            // 사용 함수
            TextBox* summontextbox();

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