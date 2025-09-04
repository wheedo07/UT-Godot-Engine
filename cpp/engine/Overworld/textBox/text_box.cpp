#include "text_box.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/viewport.hpp>
#include<godot_cpp/classes/sprite2d.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/classes/time.hpp>
#include<godot_cpp/core/math.hpp>

TextBox::TextBox() {
    soulpos = 0;
    skip_count = 0;
    soul_position = Vector2(0, 0);
    selecting = false;
    optionamt = 0;
    talking_character = DEFAULT;
}

TextBox::~TextBox() {}

void TextBox::_bind_methods() {
    ClassDB::bind_method(D_METHOD("abstract", "text", "options", "text_after_options"), &TextBox::abstract, DEFVAL(PackedStringArray()), DEFVAL(TypedArray<Dialogues>()));
    ClassDB::bind_method(D_METHOD("generic", "text", "options", "text_after_options"), &TextBox::generic, DEFVAL(PackedStringArray()), DEFVAL(TypedArray<Dialogues>()));
    ClassDB::bind_method(D_METHOD("character", "head_hide", "chr", "text", "options", "text_after_options"), &TextBox::character, DEFVAL(PackedStringArray()), DEFVAL(TypedArray<Dialogues>()));
    ClassDB::bind_method(D_METHOD("set_head_frame", "expr"), &TextBox::set_head_frame);
    ClassDB::bind_method(D_METHOD("set_key", "is"), &TextBox::set_key);
    ClassDB::bind_method(D_METHOD("on_text_click_played"), &TextBox::on_text_click_played);
    ClassDB::bind_method(D_METHOD("on_option_selected", "option"), &TextBox::on_option_selected);
    ClassDB::bind_method(D_METHOD("on_text_typing_finished"), &TextBox::on_text_typing_finished);
    ClassDB::bind_method(D_METHOD("on_option_typing_finished", "option_index", "options"), &TextBox::on_option_typing_finished);
    ClassDB::bind_method(D_METHOD("on_all_texts_finished", "options"), &TextBox::on_all_texts_finished);
    
    ClassDB::bind_method(D_METHOD("setup_options_typing", "options"), &TextBox::setup_options_typing);
    ClassDB::bind_method(D_METHOD("setup_soul_selection", "options"), &TextBox::setup_soul_selection);
    ClassDB::bind_method(D_METHOD("_setup_options_timer"), &TextBox::_setup_options_timer);
    ClassDB::bind_method(D_METHOD("finish_dialogue"), &TextBox::finish_dialogue);
    ClassDB::bind_method(D_METHOD("on_finish_dialogue"), &TextBox::on_finish_dialogue);
    ClassDB::bind_method(D_METHOD("_on_skip"), &TextBox::_on_skip);
    ClassDB::bind_method(D_METHOD("_on_confirm"), &TextBox::_on_confirm);
    
    ADD_SIGNAL(MethodInfo("selected_option", PropertyInfo(Variant::INT, "option")));
    ADD_SIGNAL(MethodInfo("typing_skip", PropertyInfo(Variant::INT, "count")));
    ADD_SIGNAL(MethodInfo("character_talk"));
    ADD_SIGNAL(MethodInfo("dialogue_finished"));
}

void TextBox::_ready() {
    if(isEditor) return;
    Text = Object::cast_to<TextBoxWriter>(get_node_internal("Control/TextContainer/Text"));
    head = Object::cast_to<AnimatedSprite2D>(get_node_internal("Control/Head"));
    soul = Object::cast_to<MenuSoul>(get_node_internal("Control/Soul"));
    text_container = Object::cast_to<MarginContainer>(get_node_internal("Control/TextContainer"));
    Text->set_text("");
    Text->connect("skip", Callable(this, "_on_skip"));
    
    Options.resize(4);
    for(int i = 0; i < 4; i++) {
        String path = "Control/TextContainer/Options/";
        switch (i) {
            case 0: path += "First"; break;
            case 1: path += "Second"; break;
            case 2: path += "Third"; break;
            case 3: path += "Fourth"; break;
        }
        TextBoxOptionWriter* option = Object::cast_to<TextBoxOptionWriter>(get_node_internal(path));
        option->set_text("");
        Options[i] = option;
    }
    
    selected_option = 0;
    if(global->get_player_position().y >= 240) {
        get_node_internal("Control")->call("set_position", Vector2(33, 10));
    }
    
    char_sound[Variant(DEFAULT)] = "Sounds/Generic";
    Array arr_value = get_character_name().values();
    Array arr_key = get_character_name().keys();
    for (int i = 0; i < arr_key.size(); i++) {
        char_sound[Variant(arr_key[i])] = "Sounds/" + String(arr_value[i]);
    }

    ResourceLoader* loader = ResourceLoader::get_singleton();
    // * 캐릭터 추가시 추가 * //
    Dictionary sans_font;
    sans_font["font"] = loader->load("res://Text/Fonts/character/sans.ttf");
    sans_font["size"] = 26;
    char_font[Variant(SANS)] = sans_font;
    
    Dictionary gaster_font;
    gaster_font["font"] = loader->load("res://Text/Fonts/character/gaster.otf");
    gaster_font["size"] = 23;
    char_font[Variant(GASTER_TEXT)] = gaster_font;
    // * //
}

void TextBox::_input(const Ref<InputEvent>& event) {
    if(!selecting || isEditor) return;
    
    if(event->is_action_pressed("ui_left") && soulpos > 0) {
        selected_option = true;
        get_node_internal("Control/Soul/choice")->call("play");
        soulpos--;
        soul_position = Options[soulpos].call("get_global_position");
        soul->move_global(soul_position);
    }
    
    if(event->is_action_pressed("ui_right") && soulpos < optionamt-1) {
        selected_option = true;
        get_node_internal("Control/Soul/choice")->call("play");
        soulpos++;
        soul_position = Options[soulpos].call("get_global_position");
        soul->move_global(soul_position);
    }
    
    if(event->is_action_pressed("ui_accept") && selected_option) {
        get_viewport()->set_input_as_handled();
        selecting = false;
        get_node_internal("Control/Soul/select")->call("play");
        finish_options();
    }
}

void TextBox::finish_options() {
    soul->hide();
    
    for (int i = 0; i < 4; i++) {
        Options[i].call("set_text", "");
        Options[i].call("hide");
    }
    
    Text->set_text("");
    if(!text_after_option.is_empty() && text_after_option.size() > soulpos && text_after_option[soulpos].get_type() != Variant::NIL) {
        Text->type_text_advanced(text_after_option[soulpos]);
        Text->connect("finished_all_texts_textbox", Callable(this, "emit_signal").bind("selected_option", soulpos), CONNECT_ONE_SHOT);
        Text->connect("finished_all_texts_textbox", Callable(this, "finish_dialogue"), CONNECT_ONE_SHOT);
    }else {
        emit_signal("selected_option", soulpos);
        finish_dialogue();
    }
}

void TextBox::abstract(const Ref<Dialogues>& text, const PackedStringArray& options, const TypedArray<Dialogues>& text_after_options) {
    global->set_player_in_menu(true);
    global->set_player_text_box(true);
    text_after_option = text_after_options;
    if(options.size() >= 4) {
        WARN_PRINT("option 갯수가 4개를 초과했습니다. 4개까지만 표시됩니다.");
    }
    if(text_after_option.size() >= 4) {
        WARN_PRINT("text_after_options 갯수가 4개를 초과했습니다. 4개까지만 표시됩니다.");
    }
    
    Text->call_deferred("type_text_advanced", text);
    text_typing_completed = false;
    Text->connect("finished_all_texts_textbox", Callable(this, "on_text_typing_finished"), CONNECT_ONE_SHOT);
    Text->connect("finished_all_texts_textbox", Callable(this, "on_all_texts_finished").bind(options), CONNECT_ONE_SHOT);
}

void TextBox::generic(const Ref<Dialogues>& text, const PackedStringArray& options, const TypedArray<Dialogues>& text_after_options) {
    for (int i = 0; i < 5; i++) {
        if (i == 0) {
            AudioStreamPlayer* default_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal(String(char_sound[Variant(DEFAULT)])));
            Text->set_click(default_sound);
        } else if (i <= 4) {
            AudioStreamPlayer* default_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal(String(char_sound[Variant(DEFAULT)])));
            TextBoxOptionWriter* option = Object::cast_to<TextBoxOptionWriter>(Options[i-1]);
            option->set_click(default_sound);
        }
    }
    
    abstract(text, options, text_after_options);
}

void TextBox::character(bool head_hide, Character chr, const Ref<Dialogues>& dialogues, const PackedStringArray& options, const TypedArray<Dialogues>& dialogues_after_options) {
    if (head_hide) {
        head->hide();
    } else {
        head->show();
        text_container->set_size(Vector2(470, 143));
        text_container->set_position(Vector2(108, 5));
        head->set_animation(String(get_character_name()[Variant(chr)]));
        Text->connect("expression_set", Callable(this, "set_head_frame"));
    }

    Dictionary font_data = char_font.has(Variant(chr)) ? Dictionary(char_font[Variant(chr)]) : Dictionary();
    if (!font_data.is_empty() && Text) {
        Text->add_theme_font_size_override("normal_font_size", font_data["size"]);
        Text->add_theme_font_override("normal_font", font_data["font"]);
    }
    
    for (int i = 0; i < 5; i++) {
        if (i == 0) {
            AudioStreamPlayer* char_sound_node = Object::cast_to<AudioStreamPlayer>(get_node_internal(String(char_sound[Variant(chr)])));
            Text->set_click(char_sound_node);
        } else if (i <= 4) {
            AudioStreamPlayer* char_sound_node = Object::cast_to<AudioStreamPlayer>(get_node_internal(String(char_sound[Variant(chr)])));
            TextBoxOptionWriter* option = Object::cast_to<TextBoxOptionWriter>(Options[i-1]);
            option->set_click(char_sound_node);
        }
    }
    
    talking_character = chr;
    
    abstract(dialogues, options, dialogues_after_options);
}

void TextBox::set_head_frame(const Array& expr) {
    head->set_frame(expr[0]);
}

void TextBox::on_text_click_played() {
    Text->kill_tweens(true);
    Text->call_deferred("emit_signal", "confirm");
}

void TextBox::set_key(bool is) {
    Text->set_process_unhandled_input(is);
    set_process_input(is);
}

void TextBox::setup_options_typing(const PackedStringArray& options) {
    options_typing_completed = false;
    int complete_count = 0;
    
    for (int i = 0; i < Math::min<float>(options.size(), 4); i++) {
        if (Options[i]) {
            Options[i].call("show");
        }
    }
    
    for (int i = 0; i < Math::min<float>(options.size(), 4); i++) {
        Ref<Dialogues> option_dialogue = memnew(Dialogues); 
        Array option_text;
        option_text.append(options[i]);
        option_dialogue->from(option_text);

        TextBoxOptionWriter* option = Object::cast_to<TextBoxOptionWriter>(Options[i]);
        option->connect("finished_typing_options", Callable(this, "on_option_typing_finished").bind(i, options), CONNECT_ONE_SHOT);
        Ref<SceneTreeTimer> timer = get_tree()->create_timer(i * 0.1);
        timer->connect("timeout", Callable(option, "type_text_advanced").bind(option_dialogue), CONNECT_ONE_SHOT);
    }
}

void TextBox::setup_soul_selection(const PackedStringArray& options) {
    soul->show();
    optionamt = options.size();
    soulpos = 0;
    
    get_node_internal("Control/Soul/choice")->call("play");
    
    Vector2 option_pos = Options[0].call("get_global_position");
    soul->set_global_position(option_pos);
    selecting = true;

    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.35);
    timer->connect("timeout", Callable(this, "_setup_options_timer"), CONNECT_ONE_SHOT);
}

void TextBox::_setup_options_timer() {
    selected_option = true;
}

void TextBox::finish_dialogue() {
    call_deferred("on_finish_dialogue");
    queue_free();
}

void TextBox::on_finish_dialogue() {
    Text->set_text("");

    global->set_player_in_menu(false);
    global->set_player_text_box(false);
    
    emit_signal("dialogue_finished");
}

void TextBox::on_option_selected(int option) {
    soulpos = option;
    selection_completed = true;
}

void TextBox::on_text_typing_finished() {
    text_typing_completed = true;
}

void TextBox::on_option_typing_finished(int option_index, const PackedStringArray& options) {
    if (option_index == options.size() - 1) {
        options_typing_completed = true;
        call_deferred("setup_soul_selection", options);
    }
}

void TextBox::on_all_texts_finished(const PackedStringArray& options) {
    post_selection_typing_completed = true;
    if (!options.is_empty()) {
        setup_options_typing(options);
        return;
    }
    finish_dialogue();
}

void TextBox::_on_skip() {
    double current_time = Time::get_singleton()->get_ticks_msec() / 1000.0;
    
    if(current_time - last_confirm_time < 0.5) {
        skip_count++;
        emit_signal("typing_skip", skip_count);
    }
    
    last_skip_time = current_time;
    
    Text->connect("confirm", Callable(this, "_on_confirm"), CONNECT_ONE_SHOT);
}

void TextBox::_on_confirm() {
    double current_time = Time::get_singleton()->get_ticks_msec() / 1000.0;
    last_confirm_time = current_time;
    
    if(current_time - last_skip_time < 0.5) {
        skip_count++;
        emit_signal("typing_skip", skip_count);
    }
}