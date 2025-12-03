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
    GDREGISTER_CLASS(DialogueLocalization);
    GDREGISTER_CLASS(DialogueAsset);

    // Encounters
    GDREGISTER_CLASS(ButtonSet);
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
    GDREGISTER_RUNTIME_CLASS(Stagehand);
    GDREGISTER_RUNTIME_CLASS(AudioPlayer);
    GDREGISTER_RUNTIME_CLASS(Global);
    GDREGISTER_RUNTIME_CLASS(DebugMenu);
    GDREGISTER_RUNTIME_CLASS(SceneContainer);
    GDREGISTER_RUNTIME_CLASS(OverworldSceneChanger);
    // [end] //

    // [Camera] //
    GDREGISTER_RUNTIME_CLASS(CameraFx);
    GDREGISTER_RUNTIME_CLASS(CameraController);
    // [end] //

    // [Text] //
    GDREGISTER_CLASS(GenericTextTyper);
    GDREGISTER_CLASS(AdvancedTextTyper);
    GDREGISTER_RUNTIME_CLASS(CharacterSetting);
    // [end] //

    // [Overworld] //
    GDREGISTER_RUNTIME_CLASS(Overworld);
    GDREGISTER_RUNTIME_CLASS(BattleTransition);
    GDREGISTER_RUNTIME_CLASS(RoomEntranceNode);
    GDREGISTER_RUNTIME_CLASS(SaveMenu);
    GDREGISTER_RUNTIME_CLASS(UI_Box);
    GDREGISTER_RUNTIME_CLASS(UI);
    GDREGISTER_RUNTIME_CLASS(SoulOverworld);
    GDREGISTER_RUNTIME_CLASS(UIdimensionalBox);

    // Interactions
    GDREGISTER_RUNTIME_CLASS(InteractionTrigger);
    GDREGISTER_RUNTIME_CLASS(OverworldAreaTrigger);
    GDREGISTER_RUNTIME_CLASS(SavePoint);
    GDREGISTER_RUNTIME_CLASS(DimensionalBox);
    GDREGISTER_RUNTIME_CLASS(ItemInteraction);

    // textBox
    GDREGISTER_CLASS(TextBoxWriter);
    GDREGISTER_CLASS(TextBoxOptionWriter);
    GDREGISTER_RUNTIME_CLASS(TextBox);

    // Characters
    GDREGISTER_RUNTIME_CLASS(PlayerOverworld);
    GDREGISTER_RUNTIME_CLASS(EnemyOverworld);

    // Shop
    GDREGISTER_RUNTIME_CLASS(SHOP);
    GDREGISTER_RUNTIME_CLASS(ShopCreate);
    // [end] //

    // [Menus] //
    GDREGISTER_RUNTIME_CLASS(MenuSoul);
    GDREGISTER_RUNTIME_CLASS(OptionSelectable);
    GDREGISTER_RUNTIME_CLASS(OptionSelectableSolo);
    GDREGISTER_RUNTIME_CLASS(SettingBoolButton);
    GDREGISTER_RUNTIME_CLASS(SettingToggleButton);
    GDREGISTER_RUNTIME_CLASS(AudiobusSettings);
    GDREGISTER_CLASS(ItemSlider);
    GDREGISTER_RUNTIME_CLASS(SaveLoader);
    GDREGISTER_RUNTIME_CLASS(Joystick);
    // [end] //

    // [Intro] //
    GDREGISTER_RUNTIME_CLASS(Intro);
    GDREGISTER_RUNTIME_CLASS(Options);
    GDREGISTER_RUNTIME_CLASS(Typing);
    GDREGISTER_RUNTIME_CLASS(NameSelection);
    // [end] //

    // [Battle] //
    GDREGISTER_RUNTIME_CLASS(BattleButtons);
    GDREGISTER_RUNTIME_CLASS(Slash);
    GDREGISTER_RUNTIME_CLASS(BattleHUD);
    GDREGISTER_RUNTIME_CLASS(Enemy);
    GDREGISTER_RUNTIME_CLASS(BattleMain);
    GDREGISTER_RUNTIME_CLASS(DustTransition);

    // AttackMeter
    GDREGISTER_RUNTIME_CLASS(AttackBar);
    GDREGISTER_RUNTIME_CLASS(DamageIndicator);
    GDREGISTER_RUNTIME_CLASS(AttackMeter);

    // Attacks
    GDREGISTER_RUNTIME_CLASS(AttackBase);
    GDREGISTER_RUNTIME_CLASS(AttackManager);

    // Box
    GDREGISTER_CLASS(Blitter);
    GDREGISTER_RUNTIME_CLASS(ArgsHolder);
    GDREGISTER_RUNTIME_CLASS(BattleBox);
    GDREGISTER_RUNTIME_CLASS(BattleBoxBehaviour);
    GDREGISTER_RUNTIME_CLASS(FightingBoxBehaviour);
    GDREGISTER_RUNTIME_CLASS(ActingBoxBehaviour);
    GDREGISTER_RUNTIME_CLASS(ItemingBoxBehaviour);
    GDREGISTER_RUNTIME_CLASS(MercyingBoxBehaviour);
    GDREGISTER_RUNTIME_CLASS(TargettingBoxBehaviour);
    GDREGISTER_RUNTIME_CLASS(BlitteringBoxBehaviour);
    GDREGISTER_RUNTIME_CLASS(BlitteringCasualBoxBehaviour);

    // Bullets
    GDREGISTER_RUNTIME_CLASS(Bullet);
    GDREGISTER_RUNTIME_CLASS(BattleObject);
    GDREGISTER_RUNTIME_CLASS(BattlePlatform);
    GDREGISTER_RUNTIME_CLASS(BulletArea);
    GDREGISTER_RUNTIME_CLASS(DefaultBullet);
    GDREGISTER_RUNTIME_CLASS(Bone);
    GDREGISTER_RUNTIME_CLASS(BoneSpike);
    GDREGISTER_RUNTIME_CLASS(Blaster);

    // Death
    GDREGISTER_RUNTIME_CLASS(DeathSoul);
    GDREGISTER_RUNTIME_CLASS(DeathScreen);

    // Soul
    GDREGISTER_RUNTIME_CLASS(CyanDetection);
    GDREGISTER_RUNTIME_CLASS(GreenShielding);
    GDREGISTER_RUNTIME_CLASS(YellowBullet);
    GDREGISTER_RUNTIME_CLASS(SoulBattle);

    // Misc
    GDREGISTER_CLASS(EnemySpeech);
    GDREGISTER_RUNTIME_CLASS(DialogueControl);
    // [end] //
    
    register_init();
}

void undertale_engine_uninit(ModuleInitializationLevel p_level) {
    if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
}