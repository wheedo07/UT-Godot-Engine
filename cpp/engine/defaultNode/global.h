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
#include<godot_cpp/classes/marshalls.hpp>
#include<godot_cpp/classes/thread.hpp>
namespace godot {
    class Global : public CanvasLayer {
        GDCLASS(Global, CanvasLayer)

        protected:
            static void _bind_methods();
			void _notification(int what);
		
		public:
			Dictionary game_over;
			Object* battle_encounter;
	        bool start, isSetting, isDebugTurn;

        private:
	        Dictionary variables;

	        String saveDir;
			String savepath;
			String settingpath;
	        AudioStreamPlayer* speedup_sound;
	        AudioStreamPlayer* Music;
            RichTextLabel* Info;
            Timer* KrTimer;
	        SceneContainer* scene_container;
	        bool first;
	        bool fullscreen;
	        bool debugmode, collision_visible;
	        Dictionary equipment;
	        PackedStringArray cells;
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

			String scan_directory;

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
			Marshalls* marshalls;

			double quit_time, speed_time, paused_time;
			Ref<Tween> tw_label;
			Dictionary input_event;
			bool is_Mobile;

			Ref<Thread> collision_thread;
			Array collision_nodes_cache;

    		void _update_collision_visibility();
			void _loop_Music();
	        void _on_kr_tick();

			// 내부용 함수
			String xor_encrypt(String data, String key);
			String xor_decrypt(String data, String key);
			void init_paths();
			void init_scene_nodes();
        
        public:
            Global();
            ~Global();
                
	        void _ready() override;
	        void _process(double delta) override;
			void _input(const Ref<InputEvent>& event) override;
			void _unhandled_input(const Ref<InputEvent>& event) override;
        
			// 사용 함수
			void disable_input(String key);
			void enable_input(String key);
			bool has_input_disabled(String key);
			void alert(String text, String title = "Alert!");
	        void heal(int amt);
	        void resetgame();
	        void true_resetgame();
			void save(String room_name="");
			void save_file(int slot, Dictionary save_data);
			Dictionary load_file(int slot);
			bool exists_file(int slot);
			bool isMobile();
	        void save_settings();
			void save_flag(String key, Variant value);
			void set_flag(String key, Variant value);
			Variant get_flag(String key, Variant defaultValue=false);
			Variant get_g_flags(String key, Variant defaultValue=false);
			void set_g_flags(String key, Variant value);
	        bool check_level_up();
	        void toggle_fullscreen();
			void change_setting(String setting_name, Variant to);
    		AudioStreamPlayer* get_Music();
			Vector2 get_player_position();
    		SceneContainer* get_scene_container() const;
    		bool get_first() const;
    		bool get_fullscreen() const;

			// ClassDB에 등록 안할 함수
			void set_player_position(Vector2 value);
    		void set_scene_container(SceneContainer* value);
    		void set_first(bool value);
    		void set_fullscreen(bool value);

			// 내부용 함수
	        void _save_game_data(String custom_path = "", Dictionary save_data = Dictionary());
			void _save_flags_data();
	        void load_game();
	        PackedStringArray item_use_text(int item_id);
	        PackedStringArray equip_item(int item_id);
	        void refresh_audio_busses();
	        void toggle_collision_shape_visibility();
    		void set_debugmode(bool value);
    		bool get_debugmode() const;
    		void _set_battle_text_box(bool value);
    		bool get_battle_text_box() const;
    		void _set_battle_start(bool value);
    		bool get_battle_start() const;
    		void _set_player_text_box(bool value);
    		bool get_player_text_box() const;
			void set_just_died(bool value);
			bool get_just_died();
			double get_cache_playtime();
			void set_temp_atk(int value);
			int get_temp_atk();
			void set_temp_def(int value);
			int get_temp_def();
    		void _set_player_in_menu(bool value);
    		bool get_player_in_menu() const;

		public:
			// 사용자가 설정할 getter/setter 함수
	        void set_item_list(TypedArray<Item> value);
	        TypedArray<Item> get_item_list() const;

			void set_saveDir(String value);
			String get_saveDir();

    		void set_equipment(const Dictionary& value);
    		Dictionary get_equipment() const;

    		void set_cells(const PackedStringArray& value);
    		PackedStringArray get_cells() const;

    		void set_items(const Array& value);
    		Array get_items() const;

    		void set_boxitems(const Array& value);
    		Array get_boxitems() const;

    		void set_settings(const Dictionary& value);
    		Dictionary get_settings() const;

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

			void set_flags(Dictionary value);
			Dictionary get_flags();

			void set_scan_directory(String value);
			String get_scan_directory() const;

			// 숨겨진 getter/setter 함수
			void set_variables(Dictionary value);
			Dictionary get_variables();

			void set_overworld_data(Dictionary value);
			Dictionary get_overworld_data();

    		void set_player_can_move(bool value);
    		bool get_player_can_move() const;

    		void set_player_set_menu(bool value);
    		bool get_player_set_menu() const;

    		void set_player_move(bool value);
    		bool get_player_move() const;

			void set_playtime(double value);
			double get_playtime() const;
    };
}

#endif