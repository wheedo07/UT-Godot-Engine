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
    optionamt = 0;
    soul_offset = Vector2(0, -5);
    last_skip_time = 0;
    last_confirm_time = 0;
    selecting = false;
    selected_option = false;
    text_typing_completed = false;
    options_typing_completed = false;
    selection_completed = false;
    post_selection_typing_completed = false;
}

TextBox::~TextBox() {}

void TextBox::_bind_methods() {
    ClassDB::bind_method(D_METHOD("generic", "text", "options", "text_after_options"), &TextBox::generic, DEFVAL(PackedStringArray()), DEFVAL(TypedArray<Dialogues>()));
    ClassDB::bind_method(D_METHOD("character", "head_hide", "chr", "text", "options", "text_after_options"), &TextBox::character, DEFVAL(PackedStringArray()), DEFVAL(TypedArray<Dialogues>()));
    ClassDB::bind_method(D_METHOD("set_key", "is"), &TextBox::set_key);

    ClassDB::bind_method(D_METHOD("set_soul_offset", "offset"), &TextBox::set_soul_offset);
    ClassDB::bind_method(D_METHOD("get_soul_offset"), &TextBox::get_soul_offset);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "soul_offset"), "set_soul_offset", "get_soul_offset");

    ClassDB::bind_method(D_METHOD("_set_head_frame", "expr"), &TextBox::_set_head_frame);
    ClassDB::bind_method(D_METHOD("_on_text_click_played"), &TextBox::_on_text_click_played);
    ClassDB::bind_method(D_METHOD("_on_option_selected", "option"), &TextBox::_on_option_selected);
    ClassDB::bind_method(D_METHOD("_on_text_typing_finished"), &TextBox::_on_text_typing_finished);
    ClassDB::bind_method(D_METHOD("_on_option_typing_finished", "option_index", "options"), &TextBox::_on_option_typing_finished);
    ClassDB::bind_method(D_METHOD("_on_all_texts_finished", "options"), &TextBox::_on_all_texts_finished);
    ClassDB::bind_method(D_METHOD("_setup_options_typing", "options"), &TextBox::_setup_options_typing);
    ClassDB::bind_method(D_METHOD("_setup_soul_selection", "options"), &TextBox::_setup_soul_selection);
    ClassDB::bind_method(D_METHOD("_setup_options_timer"), &TextBox::_setup_options_timer);
    ClassDB::bind_method(D_METHOD("_finish_dialogue"), &TextBox::_finish_dialogue);
    ClassDB::bind_method(D_METHOD("_on_skip"), &TextBox::_on_skip);
    ClassDB::bind_method(D_METHOD("_on_confirm"), &TextBox::_on_confirm);
    ClassDB::bind_method(D_METHOD("_reset_state"), &TextBox::_reset_state);
    
    ADD_SIGNAL(MethodInfo("selected_option", PropertyInfo(Variant::INT, "option")));
    ADD_SIGNAL(MethodInfo("typing_skip", PropertyInfo(Variant::INT, "count")));
    ADD_SIGNAL(MethodInfo("dialogue_finished"));
}

void TextBox::_ready() {
    Text = Object::cast_to<TextBoxWriter>(get_node_internal("Control/TextContainer/Text"));
    head = Object::cast_to<AnimatedSprite2D>(get_node_internal("Control/Head"));
    soul = Object::cast_to<MenuSoul>(get_node_internal("Control/Soul"));
    text_container = Object::cast_to<MarginContainer>(get_node_internal("Control/TextContainer"));
    default_settings["font"] = Text->get_theme_font("normal_font");
    default_settings["text_size"] = Text->get_theme_font_size("normal_font_size");
    default_settings["no_sound"] = Text->get_no_sound();
    default_settings["extra_delay"] = Text->get_extra_delay();
    default_settings["entire_text_bbcode"] = Text->get_entire_text_bbcode();
    Text->set_text("");
    
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
    _reset_state();
}

void TextBox::_input(const Ref<InputEvent>& event) {
    if(!selecting) return;
    
    if(event->is_action_pressed("ui_left") && soulpos > 0) {
        selected_option = true;
        stagehand->audio_player->play("choice");
        soulpos--;
        soul_position = Options[soulpos].call("get_global_position");
        soul->move_global(soul_position + soul_offset);
    }
    
    if(event->is_action_pressed("ui_right") && soulpos < optionamt-1) {
        selected_option = true;
        stagehand->audio_player->play("choice");
        soulpos++;
        soul_position = Options[soulpos].call("get_global_position");
        soul->move_global(soul_position + soul_offset);
    }
    
    if(event->is_action_pressed("ui_accept") && selected_option) {
        get_viewport()->set_input_as_handled();
        selecting = false;
        stagehand->audio_player->play("select");
        finish_options();
    }
}

void TextBox::_reset_state() {
    soulpos = 0;
    skip_count = 0;
    selected_option = 0;
    selecting = false;
    selected_option = false;
    text_typing_completed = false;
    options_typing_completed = false;
    selection_completed = false;
    post_selection_typing_completed = false;
    
    Text->set_text("");
    Text->add_theme_font_override("normal_font", default_settings["font"]);
    Text->add_theme_font_size_override("normal_font_size", default_settings["text_size"]);
    Text->set_no_sound(default_settings["no_sound"]);
    Text->set_extra_delay(default_settings["extra_delay"]);
    Text->set_entire_text_bbcode(default_settings["entire_text_bbcode"]);
    text_container->set_position(Vector2(0, 5));
    text_container->set_size(Vector2(578, 143));

    head->hide();
    soul->hide();

    Callable calls = Callable(this, "_set_head_frame");
    if(Text->is_connected("expression_textbox_set", calls)) {
        Text->disconnect("expression_textbox_set", calls);
    }

    Array selected_option = get_signal_connection_list("selected_option");
    for(int i=0; i < selected_option.size(); i++) {
        Dictionary calls = selected_option[i];
        disconnect("selected_option", calls["callable"]);
    }

    Array dialogue_finished = get_signal_connection_list("dialogue_finished");
    for(int i=0; i < dialogue_finished.size(); i++) {
        Dictionary calls = dialogue_finished[i];
        disconnect("dialogue_finished", calls["callable"]);
    }

    Array typing_skip = get_signal_connection_list("typing_skip");
    for(int i=0; i < typing_skip.size(); i++) {
        Dictionary calls = typing_skip[i];
        disconnect("typing_skip", calls["callable"]);
    }
    
    for(int i = 0; i < 4; i++) {
        Options[i].call("hide");
    }
    set_key(true);
    hide();
}

TextBox* TextBox::_create() {
    _reset_state();
    Control* control = Object::cast_to<Control>(get_node_internal("Control"));
    if(global->get_player_position().y >= 240) {
        control->set_position(Vector2(33, 10));
    }else control->set_position(Vector2(33, 322));
    show();
    return this;
}

void TextBox::finish_options() {
    soul->hide();
    
    for(int i=0; i < 4; i++) {
        Options[i].call("set_text", "");
        Options[i].call("hide");
    }
    
    Text->set_text("");
    if(!text_after_option.is_empty() && text_after_option.size() > soulpos && text_after_option[soulpos].get_type() != Variant::NIL) {
        Text->type_text_advanced(text_after_option[soulpos]);
        Text->connect("finished_all_texts_textbox", Callable(this, "emit_signal").bind("selected_option", soulpos), CONNECT_ONE_SHOT);
        Text->connect("finished_all_texts_textbox", Callable(this, "_finish_dialogue"), CONNECT_ONE_SHOT);
    }else {
        emit_signal("selected_option", soulpos);
        _finish_dialogue();
    }
}

void TextBox::abstract(const Ref<Dialogues>& text, const PackedStringArray& options, const TypedArray<Dialogues>& text_after_options) {
    if(!is_visible()) {
        _reset_state();
        ERR_PRINT("TextBox가 활성화되어 있지 않습니다. stagehand.summontextbox() 함수를 사용하여 활성화 해주세요.");
        return;
    }
    global->_set_player_in_menu(true);
    global->_set_player_text_box(true);
    text_after_option = text_after_options;
    if(options.size() >= 4) {
        WARN_PRINT("option 갯수가 4개를 초과했습니다. 4개까지만 표시됩니다.");
    }
    if(text_after_option.size() >= 4) {
        WARN_PRINT("text_after_options 갯수가 4개를 초과했습니다. 4개까지만 표시됩니다.");
    }
    
    Text->call_deferred("type_text_advanced", text);
    text_typing_completed = false;
    Text->connect("finished_all_texts_textbox", Callable(this, "_on_text_typing_finished"), CONNECT_ONE_SHOT);
    Text->connect("finished_all_texts_textbox", Callable(this, "_on_all_texts_finished").bind(options), CONNECT_ONE_SHOT);
}

void TextBox::generic(const Ref<Dialogues>& text, const PackedStringArray& options, const TypedArray<Dialogues>& text_after_options) {
    CharacterSetting* setting = stagehand->get_character("DEFAULT");
    if(!setting) {
        ERR_PRINT("DEFAULT가 없습니다 대화를 진행할수 없습니다");
        return;
    }

    Text->add_theme_font_size_override("normal_font_size", setting->get_text_size());
    if(!setting->get_font().is_null()) Text->add_theme_font_override("normal_font", setting->get_font());
    Text->set_no_sound(setting->get_no_sound());
    Text->set_extra_delay(setting->get_extra_delay());

    for(int i=0; i < 5; i++) {
        if(i == 0) {
            Text->set_click(setting);
        }else if(i <= 4) {
            TextBoxOptionWriter* option = Object::cast_to<TextBoxOptionWriter>(Options[i-1]);
            option->set_click(setting);
        }
    }
    
    abstract(text, options, text_after_options);
}

void TextBox::character(bool head_hide, String chr, const Ref<Dialogues>& dialogues, const PackedStringArray& options, const TypedArray<Dialogues>& dialogues_after_options) {
    CharacterSetting* setting = stagehand->get_character(chr);
    if(!setting) {
        ERR_PRINT(vformat(String::utf8("%s 라는 캐릭터가 없습니다 대화를 진행할수 없습니다"), chr));
        return;
    }

    if(head_hide) {
        head->hide();
    }else {
        head->show();
        text_container->set_size(Vector2(470, 143));
        text_container->set_position(Vector2(108, 5));
        head->set_animation(chr);
        Text->connect("expression_textbox_set", Callable(this, "_set_head_frame"));
    }

    Text->add_theme_font_size_override("normal_font_size", setting->get_text_size());
    if(!setting->get_font().is_null()) Text->add_theme_font_override("normal_font", setting->get_font());
    Text->set_no_sound(setting->get_no_sound());
    Text->set_extra_delay(setting->get_extra_delay());
    Text->set_entire_text_bbcode(setting->get_entire_text_bbcode());
    
    for(int i = 0; i < 5; i++) {
        if(i == 0) {
            Text->set_click(setting);
        }else if (i <= 4) {
            TextBoxOptionWriter* option = Object::cast_to<TextBoxOptionWriter>(Options[i-1]);
            option->set_click(setting);
        }
    }

    abstract(dialogues, options, dialogues_after_options);
}

void TextBox::_set_head_frame(int expr) {
    head->set_frame(expr);
}

void TextBox::_on_text_click_played() {
    Text->kill_tweens(true);
    Text->call_deferred("emit_signal", "confirm");
}

void TextBox::set_key(bool is) {
    Text->set_process_unhandled_input(is);
    set_process_input(is);
}

void TextBox::_setup_options_typing(const PackedStringArray& options) {
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
        option->connect("finished_typing_options", Callable(this, "_on_option_typing_finished").bind(i, options), CONNECT_ONE_SHOT);
        Ref<SceneTreeTimer> timer = get_tree()->create_timer(i * 0.1);
        timer->connect("timeout", Callable(option, "type_text_advanced").bind(option_dialogue), CONNECT_ONE_SHOT);
    }
}

void TextBox::_setup_soul_selection(const PackedStringArray& options) {
    soul->show();
    optionamt = options.size();
    soulpos = 0;
    
    stagehand->audio_player->play("choice");
    
    Vector2 option_pos = Options[0].call("get_global_position");
    soul->set_global_position(option_pos + soul_offset);
    selecting = true;

    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.35);
    timer->connect("timeout", Callable(this, "_setup_options_timer"), CONNECT_ONE_SHOT);
}

void TextBox::_setup_options_timer() {
    selected_option = true;
}

void TextBox::_finish_dialogue() {
    global->_set_player_in_menu(false);
    global->_set_player_text_box(false);
    hide();
    emit_signal("dialogue_finished");
}

void TextBox::_on_option_selected(int option) {
    soulpos = option;
    selection_completed = true;
}

void TextBox::_on_text_typing_finished() {
    text_typing_completed = true;
}

void TextBox::_on_option_typing_finished(int option_index, const PackedStringArray& options) {
    if (option_index == options.size() - 1) {
        options_typing_completed = true;
        call_deferred("_setup_soul_selection", options);
    }
}

void TextBox::_on_all_texts_finished(const PackedStringArray& options) {
    post_selection_typing_completed = true;
    if (!options.is_empty()) {
        _setup_options_typing(options);
        return;
    }
    _finish_dialogue();
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

void TextBox::set_soul_offset(const Vector2& offset) {
    soul_offset = offset;
}

Vector2 TextBox::get_soul_offset() const {
    return soul_offset;
}