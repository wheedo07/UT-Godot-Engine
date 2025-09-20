#ifndef SCENE_CONTAINER_H
#define SCENE_CONTAINER_H
#include "engine/Camera/camera_fx.h"
#include<godot_cpp/classes/reference_rect.hpp>
#include<godot_cpp/classes/sub_viewport_container.hpp>
#include<godot_cpp/classes/sub_viewport.hpp>
#include<godot_cpp/classes/texture_rect.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
namespace godot {
    class SceneContainer : public Control {
        GDCLASS(SceneContainer, Control);

        protected:
            static void _bind_methods();
        
        private:
            SubViewportContainer* settings_viewport_container;
            SubViewportContainer* main_viewport_container;
            Control* mobile;
            SubViewport* main_viewport;
            CameraFx* camera;
            ReferenceRect* border;
            TextureRect* screen_copy;
            Node* current_scene;
            bool _just_toggled_border;
            ResourceLoader* loader;

            const Vector2 SCREEN_SIZE = Vector2(640, 480);
            Ref<PackedScene> camera_scene;
        
        public:
            SceneContainer();
            ~SceneContainer();
       
            virtual void _ready() override;

            // 사용 함수
            CameraFx* get_camera();
            Node* get_current_scene() const;
            Error change_scene_to_file(const String& path);
            Error change_scene_to_packed(const Ref<PackedScene>& file);
            void unload_current_scene();
            void reload_current_scene();
            void reload_camera();
            void init_camera();

            int get_fullscreen_scale() const;
            void _on_fullscreen_toggle(bool to);
            void _refresh_window();
            void _on_settings_setting_changed(const String& setting_name, const Variant& to);
            void set_current_scene(Node* p_scene);
            SubViewport* get_main_viewport();
    };
} 

#endif