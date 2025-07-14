#ifndef ShopCreate_H
#define ShopCreate_H

#include<godot_cpp/classes/control.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include "shop.h"
namespace godot {
    class ShopCreate : public Control {
        GDCLASS(ShopCreate, Control);

        protected:
            static void _bind_methods();

        private:
            SHOP* shop_ui;
            AudioStreamPlayer* music_player;
            RoomEntranceNode* room_exit;
            AdvancedTextTyper* keeper_dialogue;

            Ref<AudioStream> music;
            NodePath click_path;

            int room_id;
            String room_path;

            Array offerings;
            Array sellferings;
            Array keeper_dialogues;
            Ref<Dialogues> keeper_def_dialogue;
            bool can_be_sold_to;
            Ref<Dialogues> keeper_cannot_sell_dialogues;
            PackedStringArray dialogue_option;

        public:
            ShopCreate();
            ~ShopCreate();

            virtual void ready();

            void _ready() override;

            void set_music(const Ref<AudioStream>& p_music);
            Ref<AudioStream> get_music() const;

            void set_room_id(int p_room_id);
            int get_room_id() const;

            void set_room_path(const String& p_room_path);
            String get_room_path() const;

            void set_offerings(const Array& p_offerings);
            Array get_offerings() const;
            
            void set_sellferings(const Array& p_sellferings);
            Array get_sellferings() const;
            
            void set_keeper_dialogues(const Array& p_keeper_dialogues);
            Array get_keeper_dialogues() const;
            
            void set_keeper_def_dialogue(const Ref<Dialogues>& p_keeper_def_dialogue);
            Ref<Dialogues> get_keeper_def_dialogue() const;
            
            void set_can_be_sold_to(bool p_can_be_sold_to);
            bool get_can_be_sold_to() const;
            
            void set_keeper_cannot_sell_dialogues(const Ref<Dialogues>& p_keeper_cannot_sell_dialogues);
            Ref<Dialogues> get_keeper_cannot_sell_dialogues() const;

            void set_dialogue_options(const PackedStringArray& p_dialogue_options);
            PackedStringArray get_dialogue_options() const;

            void set_click_path(const NodePath& p_click_path);
            NodePath get_click_path() const;
    };
};

#endif