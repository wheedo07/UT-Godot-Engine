#ifndef Undertale_Engine_H
#define Undertale_Engine_H
#include<godot_cpp/core/class_db.hpp>
using namespace godot;

// [resources] //
// Dialogues
#include "resources/Dialogues/pause.h"
#include "resources/Dialogues/dialogue.h"
#include "resources/Dialogues/dialogues.h"

// Encounters
#include "resources/Encounters/encounter.h"

// Enemies
#include "resources/Enemies/Act.h"
#include "resources/Enemies/enemy_state.h"

// Item
#include "resources/Items/item.h"

// ShopItems
#include "resources/ShopItems/shop_item.h"
// [end] //

// [defaultNode] //
#include "defaultNode/audio_player.h"
#include "defaultNode/global.h"
#include "defaultNode/settings.h"
#include "defaultNode/scene_container.h"
#include "defaultNode/overworld_scene_changer.h"
// [end] //

// [Camera] //
#include "Camera/camera_fx.h"
#include "Camera/camera_remote_controller.h"
// [end //

// [Overworld] //
#include "Overworld/overworld.h"
#include "Overworld/room_entrance_node.h"
#include "Overworld/battle_transistion.h"
#include "Overworld/save_menu.h"
#include "Overworld/ui_box.h"
#include "Overworld/ui.h"

// Interactions
#include "Overworld/Interactions/interaction_trigger.h"
#include "Overworld/Interactions/overworld_area_trigger.h"
#include "Overworld/Interactions/save_point.h"
#include "Overworld/Interactions/item_interaction.h"

// textBox
#include "Overworld/textBox/text_box_options.h"
#include "Overworld/textBox/text_box_writer.h"
#include "Overworld/textBox/text_box.h"

// Characters
#include "Overworld/Characters/player_overworld.h"
#include "Overworld/Characters/enemy_overworld.h"

// Shop
#include "Overworld/shop/shop.h"
#include "Overworld/shop/shop_create.h"
// [end] //

// [Text] //
#include "Text/text_typer.h"
#include "Text/advanced_text_typer.h"
// [end] //

// [Menus] // 
#include "Menus/menu_soul.h"
#include "Menus/option_selectable.h"
#include "Menus/option_selectable_solo.h"
#include "Menus/audiobus_settings.h"
#include "Menus/bool_setting.h"
#include "Menus/toggle_setting.h"
#include "Menus/item_slider.h"
#include "Menus/save_loader.h"
#include "Menus/joystick.h"
// [end] //

// [Intro] //
#include "Intro/intro.h"
#include "Intro/options.h"
#include "Intro/typing.h"
#include "Intro/name_selection.h"
// [end] //

// [Battle] //
#include "Battle/battle_buttons.h"
#include "Battle/slash.h"
#include "Battle/hud.h"
#include "Battle/enemy.h"
#include "Battle/battle_system.h"

// AttackMeter
#include "Battle/AttackMeter/bar.h"
#include "Battle/AttackMeter/damage_indicator.h"
#include "Battle/AttackMeter/meter.h"

// Attacks
#include "Battle/Attacks/attacks.h"
#include "Battle/Attacks/attacks_manager.h"

// Box
#include "Battle/Box/argsHolder.h"
#include "Battle/Box/battle_box.h"
#include "Battle/Box/blitter.h"
#include "Battle/Box/box_behaviour.h"
#include "Battle/Box/fighting.h"
#include "Battle/Box/acting.h"
#include "Battle/Box/iteming.h"
#include "Battle/Box/mercying.h"
#include "Battle/Box/targetting.h"
#include "Battle/Box/blittering.h"
#include "Battle/Box/blittering_casual.h"

// Bullets
#include "Battle/Bullets/bullet.h"
#include "Battle/Bullets/battle_object.h"
#include "Battle/Bullets/battle_platform.h"
#include "Battle/Bullets/bullet_area.h"
#include "Battle/Bullets/defaultbullet.h"
#include "Battle/Bullets/custom/bone.h"
#include "Battle/Bullets/custom/bone_spike.h"
#include "Battle/Bullets/custom/blaster.h"

// Death
#include "Battle/Death/death_soul.h"
#include "Battle/Death/death_screen.h"

// Soul
#include "Battle/Soul/cyan_detection.h"
#include "Battle/Soul/green_soul.h"
#include "Battle/Soul/yellow_soul_bullet.h"
#include "Battle/Soul/soul.h"

// Misc
#include "Battle/Misc/enemy_speech.h"
#include "Battle/Misc/dialogue_control.h"
// [end] //

// Custom
#include "src/register_types.h"

void undertale_engine_init(ModuleInitializationLevel p_level);
void undertale_engine_uninit(ModuleInitializationLevel p_level);

#endif