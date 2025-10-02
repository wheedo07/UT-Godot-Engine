#include "ui_dimensional_box.h"
#include "env.h"

#define INV_SIZE 8
#define BOX_SIZE 10
UIdimensionalBox::UIdimensionalBox() {
    current_side = INVENTORY_SIDE;
    soulposition = Vector2(0, 0);
    seperation = Vector2(0, 28);
}

UIdimensionalBox::~UIdimensionalBox() {}

void UIdimensionalBox::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_seperation", "value"), &UIdimensionalBox::set_seperation);
    ClassDB::bind_method(D_METHOD("get_seperation"), &UIdimensionalBox::get_seperation);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "seperation"), "set_seperation", "get_seperation");
}

void UIdimensionalBox::_ready() {
    if(isEditor) return;
    main = Object::cast_to<UI_Box>(get_node_internal("Control/Main"));
    soul = Object::cast_to<MenuSoul>(get_node_internal("Control/Soul"));
    inventory_items = Object::cast_to<RichTextLabel>(get_node_internal("Control/Main/INVENTORY_Item"));
    box_items = Object::cast_to<RichTextLabel>(get_node_internal("Control/Main/BOX_Item"));

    global->call_deferred("set_player_in_menu", true);
    main->grow();
    _set_items();
    soul_move(Vector2(0, 0));
}

void UIdimensionalBox::_unhandled_input(const Ref<InputEvent>& event) {
    if(isEditor) return;

    if(event->is_action_pressed("ui_up")) {
        soul_move(Vector2(0, -1));
    }
    
    if(event->is_action_pressed("ui_down")) {
        soul_move(Vector2(0, 1));
    }

    if(event->is_action_pressed("ui_right")) {
        if(current_side == INVENTORY_SIDE) {
            current_side = BOX_SIDE;
            if(soulposition.y > BOX_SIZE-1) soulposition.y = BOX_SIZE-1;
            _set_items();
            soul_move(Vector2(0, 0));
        }
    }
    
    if(event->is_action_pressed("ui_left")) {
        if(current_side == BOX_SIDE) {
            current_side = INVENTORY_SIDE;
            if(soulposition.y > INV_SIZE-1) soulposition.y = INV_SIZE-1;
            _set_items();
            soul_move(Vector2(0, 0));
        }
    }

    if(event->is_action_pressed("ui_accept")) {
        Array items = global->get_items();
        Array boxitems = global->get_boxitems();
        if(current_side == INVENTORY_SIDE) {
            if(soulposition.y < items.size()) {
                if(boxitems.size() < BOX_SIZE) {
                    boxitems.append(items[soulposition.y]);
                    items.remove_at(soulposition.y);
                    global->set_items(items);
                    global->set_boxitems(boxitems);
                    if(soulposition.y > items.size()-1 && items.size() != 0) soulposition.y = items.size()-1;
                    _set_items();
                    soul_move(Vector2(0, 0));
                }
            }
        } else {
            if(soulposition.y < boxitems.size()) {
                if(items.size() < INV_SIZE) {
                    items.append(boxitems[soulposition.y]);
                    boxitems.remove_at(soulposition.y);
                    global->set_items(items);
                    global->set_boxitems(boxitems);
                    if(soulposition.y > boxitems.size()-1 && boxitems.size() != 0) soulposition.y = boxitems.size()-1;
                    _set_items();
                    soul_move(Vector2(0, 0));
                }
            }
        }
    }

    if(event->is_action_pressed("ui_cancel")) {
        _close_box();
    }
}

void UIdimensionalBox::_set_items() {
    Array items = global->get_items();
    Array boxitems = global->get_boxitems();
    String invtext = "";
    String boxtext = "";

    for(int i=0; i < INV_SIZE; i++) {
        if(i < items.size()) {
            Ref<Item> item = global->get_item_list()[items[i]];
            invtext += vformat("%s\n", item->get_item_name());
        } else {
            invtext += vformat("%s\n", "[color=red]--------------------------[/color]");
        }
    }

    for(int i=0; i < BOX_SIZE; i++) {
        if(i < boxitems.size()) {
            Ref<Item> item = global->get_item_list()[boxitems[i]];
            boxtext += vformat("%s\n", item->get_item_name());
        } else {
            boxtext += vformat("%s\n", "[color=red]--------------------------[/color]");
        }
    }

    inventory_items->set_text(invtext);
    box_items->set_text(boxtext);
}

void UIdimensionalBox::_close_box() {
    global->set_player_in_menu(false);
    soul->hide();
    main->shrink();
    main->get_tw()->connect("finished", Callable(this, "queue_free"), CONNECT_ONE_SHOT);
}

bool UIdimensionalBox::soul_move(Vector2 action) {
    Vector2 new_position = soulposition + action;
    
    if(current_side == INVENTORY_SIDE) {
        if(new_position.y < 0 || new_position.y > INV_SIZE-1) return false;
    } else {
        if(new_position.y < 0 || new_position.y > BOX_SIZE-1) return false;
    }
    
    soulposition = new_position;
    
    Vector2 target;
    if(current_side == INVENTORY_SIDE) {
        target = inventory_items->get_global_position() + soulposition * seperation + Vector2(-12, 15);
    } else {
        target = box_items->get_global_position() + soulposition * seperation + Vector2(-12, 15);
    }
    soul->move_global(target);
    
    return true;
}

void UIdimensionalBox::set_seperation(Vector2 value) {
    seperation = value;
}

Vector2 UIdimensionalBox::get_seperation() const {
    return seperation;
}