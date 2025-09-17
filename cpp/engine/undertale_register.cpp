#include "undertale_engine.h"
#include<gdextension_interface.h>
#include<godot_cpp/core/defs.hpp>
#include<godot_cpp/godot.hpp>

void undertale_engine_init(ModuleInitializationLevel p_level) {
    if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
    GDExtensionMainLoopCallbacks cbs{};
    cbs.startup_func = &on_startup;
    cbs.shutdown_func = &on_shutdown;
    cbs.frame_func = &on_frame;
    godot::internal::gdextension_interface_register_main_loop_callbacks(godot::internal::library, &cbs);

    // [resources] //
    // Dialogues
    GDREGISTER_CLASS(DialoguePause);
    GDREGISTER_CLASS(Dialogue);
    GDREGISTER_CLASS(Dialogues);

    // Encounters
    GDREGISTER_CLASS(Encounter);

    // Enemies
    GDREGISTER_CLASS(ActInfo);
    GDREGISTER_CLASS(EnemyState);

    // ShopItems
    GDREGISTER_CLASS(ShopItem);

    // Item
    GDREGISTER_CLASS(Item);
    // [end] //

    // [defaultNode] //
    GDREGISTER_CLASS(AudioPlayer);
    GDREGISTER_CLASS(Global);
    GDREGISTER_CLASS(Settings);
    GDREGISTER_CLASS(SceneContainer);
    GDREGISTER_CLASS(OverworldSceneChanger);
    // [end] //

    // [Camera] //
    GDREGISTER_CLASS(CameraFx);
    GDREGISTER_CLASS(CameraRemoteController);
    // [end] //

    // [Text] //
    GDREGISTER_CLASS(GenericTextTyper);
    GDREGISTER_CLASS(AdvancedTextTyper);
    // [end] //

    // [Overworld] //
    GDREGISTER_CLASS(Overworld);
    GDREGISTER_CLASS(BattleTransition);
    GDREGISTER_CLASS(RoomEntranceNode);
    GDREGISTER_CLASS(SaveMenu);
    GDREGISTER_CLASS(UI_Box);
    GDREGISTER_CLASS(UI);

    // Interactions
    GDREGISTER_CLASS(InteractionTrigger);
    GDREGISTER_CLASS(OverworldAreaTrigger);
    GDREGISTER_CLASS(SavePoint);
    GDREGISTER_CLASS(ItemInteraction);

    // textBox
    GDREGISTER_CLASS(TextBoxWriter);
    GDREGISTER_CLASS(TextBoxOptionWriter);
    GDREGISTER_CLASS(TextBox);

    // Characters
    GDREGISTER_CLASS(PlayerOverworld);
    GDREGISTER_CLASS(EnemyOverworld);

    // Shop
    GDREGISTER_CLASS(SHOP);
    GDREGISTER_CLASS(ShopCreate);
    // [end] //

    // [Menus] //
    GDREGISTER_CLASS(MenuSoul);
    GDREGISTER_CLASS(OptionSelectable);
    GDREGISTER_CLASS(OptionSelectableSolo);
    GDREGISTER_CLASS(SettingBoolButton);
    GDREGISTER_CLASS(SettingToggleButton);
    GDREGISTER_CLASS(AudiobusSettings);
    GDREGISTER_CLASS(ItemSlider);
    GDREGISTER_CLASS(SaveLoader);
    GDREGISTER_CLASS(Joystick);
    // [end] //

    // [Intro] //
    GDREGISTER_CLASS(Intro);
    GDREGISTER_CLASS(Options);
    GDREGISTER_CLASS(Typing);
    GDREGISTER_CLASS(NameSelection);
    // [end] //

    // [Battle] //
    GDREGISTER_CLASS(BattleButtons);
    GDREGISTER_CLASS(Slash);
    GDREGISTER_CLASS(BattleHUD);
    GDREGISTER_CLASS(Enemy);
    GDREGISTER_CLASS(BattleMain);

    // AttackMeter
    GDREGISTER_CLASS(AttackBar);
    GDREGISTER_CLASS(DamageIndicator);
    GDREGISTER_CLASS(AttackMeter);

    // Attacks
    GDREGISTER_CLASS(AttackBase);
    GDREGISTER_CLASS(AttackManager);

    // Box
    GDREGISTER_CLASS(Blitter);
    GDREGISTER_CLASS(ArgsHolder);
    GDREGISTER_CLASS(BattleBox);
    GDREGISTER_CLASS(BattleBoxBehaviour);
    GDREGISTER_CLASS(FightingBoxBehaviour);
    GDREGISTER_CLASS(ActingBoxBehaviour);
    GDREGISTER_CLASS(ItemingBoxBehaviour);
    GDREGISTER_CLASS(MercyingBoxBehaviour);
    GDREGISTER_CLASS(TargettingBoxBehaviour);
    GDREGISTER_CLASS(BlitteringBoxBehaviour);
    GDREGISTER_CLASS(BlitteringCasualBoxBehaviour);

    // Bullets
    GDREGISTER_CLASS(Bullet);
    GDREGISTER_CLASS(BattleObject);
    GDREGISTER_CLASS(BattlePlatform);
    GDREGISTER_CLASS(BulletArea);
    GDREGISTER_CLASS(DefaultBullet);
    GDREGISTER_CLASS(Bone);
    GDREGISTER_CLASS(BoneSpike);
    GDREGISTER_CLASS(Blaster);

    // Death
    GDREGISTER_CLASS(DeathSoul);
    GDREGISTER_CLASS(DeathScreen);

    // Soul
    GDREGISTER_CLASS(CyanDetection);
    GDREGISTER_CLASS(GreenShielding);
    GDREGISTER_CLASS(YellowBullet);
    GDREGISTER_CLASS(SoulBattle);

    // Misc
    GDREGISTER_CLASS(EnemySpeech);
    GDREGISTER_CLASS(DialogueControl);
    // [end] //
    
    register_init();
}

void undertale_engine_uninit(ModuleInitializationLevel p_level) {
    if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
}