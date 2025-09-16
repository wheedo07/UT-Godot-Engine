#ifndef GLOBAL_H
#define GLOBAL_H

#include "engine/defaultNode/scene_container.h"
#include "engine/resources/Items/item.h"
#include<godot_cpp/classes/os.hpp>
#include<godot_cpp/classes/canvas_layer.hpp>
#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/timer.hpp>
#include<godot_cpp/classes/input.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/display_server.hpp>
namespace godot {
    class Global : public CanvasLayer {
        GDCLASS(Global, CanvasLayer)

        protected:
            static void _bind_methods();
			void _notification(int what);
		
		public:
	        AudioStreamPlayer* heal_sound;
			Dictionary game_over;
			Object* battle_encounter;
	        bool start;

        private:
			String savepath;
			String savepaths;
			String settingpaths;
	        AudioStreamPlayer* Music;
            RichTextLabel* Info;
            Timer* KrTimer;
	        SceneContainer* scene_container;
	        bool first, isEditor;
	        bool fullscreen;
	        bool debugmode;
	        bool boxesinmenu;
	        int unlockedboxes;
	        Dictionary equipment;
	        Array cells;
	        Array items;
	        Array boxitems;
	        Dictionary settings;

	        bool battle_start;
	        bool player_in_menu;
	        bool player_set_menu;
	        bool player_can_move;
	        bool player_move;
	        bool player_text_box;
	        bool battle_text_box;
	        bool paused;

	        String player_name;
	        int player_gold;
	        int player_lv;
	        int player_exp;
	        int player_hp;
	        int player_max_hp;
	        int player_defense;
	        int player_attack;
	        int player_kr;
	        int player_kills;

            int temp_atk;
	        int temp_def;
            Vector2 player_position;
            Dictionary overworld_temp_data;
            bool just_died;
            Dictionary overworld_data;
            Dictionary g_flags;
            Dictionary flags;
            Dictionary flags_tmp;
            double playtime;
            double cache_playtime;
            TypedArray<Item> item_list;

			double krtime;
			Dictionary backup;

			OS* os;
            DisplayServer* display;

			double quit_time;
			Ref<Tween> tw_quit;
			Dictionary input_event;
			bool is_Mobile;

			void loop_Music();
	        void _on_kr_tick();
        
        public:
            Global();
            ~Global();
                
	        void _ready() override;
	        void _process(double delta) override;
			void _input(const Ref<InputEvent>& event) override;
			void _unhandled_input(const Ref<InputEvent>& event) override;
         
			void disable_input(String key);
			void enable_input(String key);
			bool has_input_disabled(String key);
			void alert(String text, String title = "Alert!");
	        void heal(int amt);
	        void resetgame();
	        void true_resetgame();
			void save(String room_name="");
			bool isMobile();
	        void save_settings();
			void save_flag(const String& key, const Variant& value);
			void set_flag(String key, Variant value);
			Variant get_flag(String key, Variant defaultValue=false);
			Variant get_g_flags(String key, Variant defaultValue=false);
			void set_g_flags(String key, Variant value);
	        bool check_level_up();
	        void toggle_fullscreen();

	        void save_game(bool is_sys);
	        void load_game();
	        PackedStringArray item_use_text(int item_id);
	        PackedStringArray equip_item(int item_id);
	        void refresh_audio_busses();
	        void toggle_collision_shape_visibility();

	        void set_item_list(TypedArray<Item> value);
	        TypedArray<Item> get_item_list() const;

    		void set_first(bool value);
    		bool get_first() const;

    		void set_fullscreen(bool value);
    		bool get_fullscreen() const;

    		void set_debugmode(bool value);
    		bool get_debugmode() const;

    		void set_battle_text_box(bool value);
    		bool get_battle_text_box() const;

    		void set_battle_start(bool value);
    		bool get_battle_start() const;

    		void set_boxesinmenu(bool value);
    		bool get_boxesinmenu() const;

    		void set_unlockedboxes(int value);
    		int get_unlockedboxes() const;

    		void set_equipment(const Dictionary& value);
    		Dictionary get_equipment() const;

    		void set_cells(const Array& value);
    		Array get_cells() const;

    		void set_items(const Array& value);
    		Array get_items() const;

    		void set_boxitems(const Array& value);
    		Array get_boxitems() const;

    		void set_settings(const Dictionary& value);
    		Dictionary get_settings() const;

    		void set_player_in_menu(bool value);
    		bool get_player_in_menu() const;

    		void set_player_set_menu(bool value);
    		bool get_player_set_menu() const;

    		void set_player_can_move(bool value);
    		bool get_player_can_move() const;

    		void set_player_move(bool value);
    		bool get_player_move() const;

    		void set_player_text_box(bool value);
    		bool get_player_text_box() const;

    		void set_paused(bool value);
    		bool get_paused() const;

    		void set_player_name(const String& value);
    		String get_player_name() const;

    		void set_player_gold(int value);
    		int get_player_gold() const;

    		void set_player_lv(int value);
    		int get_player_lv() const;

    		void set_player_exp(int value);
    		int get_player_exp() const;

    		void set_player_hp(int value);
    		int get_player_hp() const;

    		void set_player_max_hp(int value);
    		int get_player_max_hp() const;

    		void set_player_defense(int value);
    		int get_player_defense() const;

    		void set_player_attack(int value);
    		int get_player_attack() const;

    		void set_player_kr(int value);
    		int get_player_kr() const;

    		void set_player_kills(int value);
    		int get_player_kills() const;

			void set_player_position(Vector2 value);
			Vector2 get_player_position();

    		void set_scene_container(SceneContainer* value);
    		SceneContainer* get_scene_container() const;

    		AudioStreamPlayer* get_Music();

			void set_just_died(bool value);
			bool get_just_died();

			void set_overworld_data(Dictionary value);
			Dictionary get_overworld_data();

			void set_cache_playtime(double value);
			double get_cache_playtime();

			void set_temp_atk(int value);
			int get_temp_atk();

			void set_temp_def(int value);
			int get_temp_def();

			void set_flags(Dictionary value);
			Dictionary get_flags();

			void set_savepath(String value);
			String get_savepath();
    };
}

#endif