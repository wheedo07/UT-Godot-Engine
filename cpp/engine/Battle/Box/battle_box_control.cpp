#include "battle_box.h"
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/geometry2d.hpp>
#include<godot_cpp/classes/method_tweener.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
using namespace godot;

// 사용 함수 //
void BattleBox::box_show() {
    get_node_internal("BoxContainer/Panel")->call("show");
    border->show();
    background->show();
}

void BattleBox::box_hide() {
    get_node_internal("BoxContainer/Panel")->call("hide");
    border->hide();
    background->hide();
}

void BattleBox::set_webs(int n, float separation, int margin) {
    clear_webs();
    if(n < 1) {
        ERR_PRINT("웹의 개수는 1개 이상이어야 합니다!");
        return;
    }
    
    float actual_separation = (separation == -1) ? 
        (current_size.y - 10 - margin * 2) / (n + 1) : separation;
    
    for(int i=1; i <= n; i++) {
        Node *web_instance = web_scene->instantiate();
        if(web_instance) {
            Line2D *web_line = Object::cast_to<Line2D>(web_instance);
            if (web_line) {
                webs->add_child(web_line);
                web_line->set_position(Vector2(0, actual_separation * i));
                webs_array.append(web_line);
            }
        }
    }
}

void BattleBox::clear_webs() {
    TypedArray<Node> children = webs->get_children();
    
    for (int i = 0; i < children.size(); i++) {
        Node *child = Object::cast_to<Node>(children[i]);
        if(child) {
            webs->remove_child(child);
            child->queue_free();
        }
    }
    
    webs_array.clear();
}

float BattleBox::get_web_y_pos(int id) {
    if (webs_array.is_empty()) {
        ERR_PRINT("웹이 비어 있습니다. set_webs()를 사용하여 보라색 영혼이 생길경우 추가해 주세요!");
        return 0;
    }
    
    id = UtilityFunctions::clamp(id, 0, webs_array.size() - 1);
    Node *web = Object::cast_to<Node>(webs_array[id]);
    
    if(web) {
        Vector2 vec = web->call("get_global_position");
        return vec.y;
    }
    
    return 0;
}

void BattleBox::reset_box(float duration) {
    if(tw.is_valid()) tw->kill();
    
    float rotation = rect_container->get_rotation();
    if (!Math::is_zero_approx(rotation)) {
        rotation = Math::fmod(rotation, float(Math_PI));
        
        tw = create_tween();
        tw->set_ease(EaseType);
        tw->set_trans(TransType);
        tw->tween_property(rect_container, "rotation", 0, duration);
        tw->play();
    }
    anchor_targets = def_anchors.duplicate();
    Ref<ArgsHolder> args = memnew(ArgsHolder);
    args->set_duration(duration);
    clear_webs();
    _tween_size(args);
    tw->connect("finished", Callable(this, "polygon_disable").bind(Vector2(0,0), duration));
}

Vector2 BattleBox::get_size() {
    return rect->get_size();
}

Vector2 BattleBox::get_box_position(RelativePosition relative_to) {
    Vector2 anchor_targets_0 = anchor_targets[0];
    Vector2 anchor_targets_1 = anchor_targets[1];
    Vector2 intended_size = anchor_targets_1 - anchor_targets_0;
    
    float rotation = rect_container->get_rotation();
    Vector2 center = anchor_targets_0 + intended_size / 2.0;
    
    Vector2 local_position;
    switch (relative_to) {
        case RELATIVE_TOP_LEFT:
            local_position = anchor_targets_0;
            break;
        case RELATIVE_TOP_RIGHT:
            local_position = anchor_targets_0 + intended_size.x * Vector2(1, 0);
            break;
        case RELATIVE_BOTTOM_LEFT:
            local_position = anchor_targets_0 + intended_size.y * Vector2(0, 1);
            break;
        case RELATIVE_BOTTOM_RIGHT:
            local_position = anchor_targets_1;
            break;
        case RELATIVE_CENTER:
            return center;
    }
    
    Vector2 offset = local_position - center;
    Vector2 rotated_offset = Vector2(
        offset.x * cos(rotation) - offset.y * sin(rotation),
        offset.x * sin(rotation) + offset.y * cos(rotation)
    );
    return center + rotated_offset;
}

float BattleBox::get_box_rotation() {
    return rect_container->get_rotation();
}

void BattleBox::change_size(const Vector2& new_size, bool relative, float duration) {
    if(isPolygonMode) {
        ERR_PRINT("다각형 모드에서는 사용할 수 없습니다. reset_box()또는 polygon_disable()를 후출하고 사용해주세요");
        return;
    }
    Vector2 anchor_targets_0 = anchor_targets[0];
    Vector2 anchor_targets_1 = anchor_targets[1];

    Vector2 intended_size = anchor_targets_1 - anchor_targets_0;
    Vector2 current_center = anchor_targets_0 + intended_size / 2.0;
    Vector2 final_size = relative ? new_size + intended_size : new_size;
    
    Vector2 min_size = rect_container->get_custom_minimum_size();
    if (final_size.x < min_size.x) final_size.x = min_size.x;
    if (final_size.y < min_size.y) final_size.y = min_size.y;
    
    anchor_targets[0] = current_center - final_size / 2.0;
    anchor_targets[1] = current_center + final_size / 2.0;
    
    Ref<ArgsHolder> args = memnew(ArgsHolder);
    args->set_duration(duration);
    _tween_size(args);
}

void BattleBox::change_position(const Vector2& new_position, bool relative, float duration) {
    if(isPolygonMode) {
        ERR_PRINT("다각형 모드에서는 사용할 수 없습니다. reset_box()또는 polygon_disable()를 후출하고 사용해주세요");
        return;
    }
    Vector2 anchor_targets_0 = anchor_targets[0];
    Vector2 anchor_targets_1 = anchor_targets[1];

    Vector2 intended_size = anchor_targets_1 - anchor_targets_0;
    Vector2 final_position = relative ? new_position + anchor_targets_0 : new_position;
    
    anchor_targets[0] = final_position;
    anchor_targets[1] = final_position + intended_size;
    
    Ref<ArgsHolder> args = memnew(ArgsHolder);
    args->set_duration(duration);
    _tween_size(args);
}

void BattleBox::rotate_by(float rot, bool relative, float duration) {
    Ref<ArgsHolder> args = memnew(ArgsHolder);
    args->set_duration(duration);
    args->args.append(rot);
    args->args.append(relative);
    
    call_deferred("_real_rotate_by", args);
}

void BattleBox::advanced_change_size(RelativePosition relative_to, Vector2 new_position, 
                                            Vector2 new_size, bool position_relative, 
                                            bool size_relative, float duration) {
    if(isPolygonMode) {
        ERR_PRINT("다각형 모드에서는 사용할 수 없습니다. reset_box()또는 polygon_disable()를 후출하고 사용해주세요");
        return;
    }
    Vector2 anchor_targets_0 = anchor_targets[0];
    Vector2 anchor_targets_1 = anchor_targets[1];
    Vector2 intended_size = anchor_targets_1 - anchor_targets_0;
    Vector2 final_size = size_relative ? new_size + intended_size : new_size;
    
    Vector2 min_size = rect_container->get_custom_minimum_size();
    if (final_size.x < min_size.x) final_size.x = min_size.x;
    if (final_size.y < min_size.y) final_size.y = min_size.y;
    
    anchor_targets_0 = _anchor_position(relative_to, new_position, intended_size, final_size, position_relative);
    anchor_targets[0] = anchor_targets_0;
    anchor_targets[1] = anchor_targets_0 + final_size;
    
    Ref<ArgsHolder> args = memnew(ArgsHolder);
    args->set_duration(duration);
    _tween_size(args);
}

void BattleBox::advanced_set_size(RelativePosition relative_to, Vector2 new_position, 
                                        Vector2 new_size, bool position_relative, 
                                        bool size_relative) {
    if(isPolygonMode) {
        ERR_PRINT("다각형 모드에서는 사용할 수 없습니다. reset_box()또는 polygon_disable()를 후출하고 사용해주세요");
        return;
    }
    Vector2 anchor_targets_0 = anchor_targets[0];
    Vector2 anchor_targets_1 = anchor_targets[1];
    Vector2 intended_size = anchor_targets_1 - anchor_targets_0;
    Vector2 final_size = size_relative ? new_size + intended_size : new_size;
    
    Vector2 min_size = rect_container->get_custom_minimum_size();
    if(final_size.x < min_size.x) final_size.x = min_size.x;
    if(final_size.y < min_size.y) final_size.y = min_size.y;
    
    anchor_targets_0 = _anchor_position(relative_to, new_position, intended_size, final_size, position_relative);
    anchor_targets[0] = anchor_targets_0;
    anchor_targets[1] = anchor_targets_0 + final_size;

    _box_set_size();
}

void BattleBox::set_box_rotation(float rot, bool relative) {
    float final_rotation = relative ? rect_container->get_rotation() + rot : rot;
    rect_container->set_rotation(final_rotation);
}

PackedVector2Array BattleBox::get_polygon_points() {
    return target_shape.duplicate();
}

Vector2 BattleBox::get_vertex_position(int vertex_index) {
    if (vertex_index < 0 || vertex_index >= target_shape.size()) {
        return Variant();
    }
    return target_shape[vertex_index];
}

void BattleBox::polygon_enable() {
    isPolygonMode = true;
    
    Vector2 margin_offset = Vector2(rect_container->get_theme_constant("margin_left"), rect_container->get_theme_constant("margin_top"));
    PackedVector2Array rect_poly;
    
    float perimeter = (current_size.x + current_size.y) * 2.0f;
    float segment_length = perimeter / float(polygon_point_count);
    for(int i=0; i < polygon_point_count; i++) {
        float distance = segment_length * float(i);
        Vector2 point;
        
        if(distance < current_size.x) {
            point = margin_offset + Vector2(distance, 0);
        }else if (distance < current_size.x + current_size.y) {
            point = margin_offset + Vector2(current_size.x, distance - current_size.x);
        }else if (distance < current_size.x * 2.0f + current_size.y) {
            point = margin_offset + Vector2(current_size.x - (distance - current_size.x - current_size.y), current_size.y);
        }else {
            point = margin_offset + Vector2(0, current_size.y - (distance - current_size.x * 2.0f - current_size.y));
        }
        rect_poly.push_back(point);
    }
    polygon->set_polygon(rect_poly);
    target_shape = rect_poly.duplicate();
    static_shape = rect_poly.duplicate();

    polygon->set_disabled(false);
    for (int i = 0; i < 4; i++) {
        collisions[i].set("disabled", true);
    }
}

void BattleBox::polygon_disable(Vector2 box_size, float duration) {
    if(!isPolygonMode) return;
    stop_all_point_tweens();

    if(!box_size.is_zero_approx()) {
        Vector2 anchor_targets_0 = anchor_targets[0];
        Vector2 anchor_targets_1 = anchor_targets[1];
        Vector2 intended_size = anchor_targets_1 - anchor_targets_0;
        Vector2 current_center = anchor_targets_0 + intended_size / 2.0;

        Vector2 min_size = rect_container->get_custom_minimum_size();
        if(box_size.x < min_size.x) box_size.x = min_size.x;
        if(box_size.y < min_size.y) box_size.y = min_size.y;

        anchor_targets[0] = current_center - box_size / 2.0;
        anchor_targets[1] = current_center + box_size / 2.0;
        _box_set_size();
    }

    get_tree()->connect("physics_frame", Callable(this, "_polygon_disable_real").bind(false, duration), CONNECT_ONE_SHOT);
}

bool BattleBox::polygon_is_enabled() {
    return isPolygonMode;
}

#define BOX_PROTRUSION_SPREAD 8.0f
void BattleBox::create_protrusion(Vector2 direction, float offset, Vector2 size, float duration) {
    if(!isPolygonMode) {
        ERR_PRINT("다각형 모드가 활성화되어 있지 않습니다. polygon_enable()을 먼저 호출해주세요.");
        return;
    }
    direction = direction.normalized();
    Vector2 margin_offset = Vector2(rect_container->get_theme_constant("margin_left"), rect_container->get_theme_constant("margin_top"));
    
    PackedInt64Array vertex_indices;
    PackedVector2Array target_points;
    if(direction.y > 0) {
        float start_x = margin_offset.x + offset;
        float end_x = margin_offset.x + offset + size.x;
        float y_line = margin_offset.y + current_size.y;
        
        for(int i=0; i < target_shape.size(); i++) {
            Vector2 point = target_shape[i];
            if(Math::abs(point.y - y_line) < 5.0f && point.x >= start_x && point.x <= end_x) {
                vertex_indices.append(i);
                Vector2 target_pos = Vector2(point.x, y_line + size.y);
                target_points.append(to_global(target_pos));
            }
        }
    }else if(direction.y < 0) {
        float start_x = margin_offset.x + offset;
        float end_x = margin_offset.x + offset + size.x;
        float y_line = margin_offset.y;
        
        for(int i=0; i < target_shape.size(); i++) {
            Vector2 point = target_shape[i];
            if(Math::abs(point.y - y_line) < 5.0f && point.x >= start_x && point.x <= end_x) {
                vertex_indices.append(i);
                Vector2 target_pos = Vector2(point.x, y_line - size.y);
                target_points.append(to_global(target_pos));
            }
        }
    }else if(direction.x > 0) {
        float start_y = margin_offset.y + offset;
        float end_y = margin_offset.y + offset + size.y;
        float x_line = margin_offset.x + current_size.x;
        
        float max_x = -Math_INF;
        for(int i=0; i < target_shape.size(); i++) {
            if(target_shape[i].x > max_x) max_x = target_shape[i].x;
        }
        
        for(int i=0; i < target_shape.size(); i++) {
            Vector2 point = target_shape[i];
            if(Math::abs(point.x - max_x) < 5.0f && point.y >= start_y && point.y <= end_y) {
                vertex_indices.append(i);
                Vector2 target_pos = Vector2(point.x + size.x, point.y);
                target_points.append(to_global(target_pos));
            }
        }
    }else if(direction.x < 0) {
        float start_y = margin_offset.y + offset;
        float end_y = margin_offset.y + offset + size.y;
        float min_x = Math_INF;
        for(int i=0; i < target_shape.size(); i++) {
            if(target_shape[i].x < min_x) min_x = target_shape[i].x;
        }
        
        for(int i=0; i < target_shape.size(); i++) {
            Vector2 point = target_shape[i];
            if(Math::abs(point.x - min_x) < 5.0f && point.y >= start_y && point.y <= end_y) {
                vertex_indices.append(i);
                Vector2 target_pos = Vector2(point.x - size.x, point.y);
                target_points.append(to_global(target_pos));
            }
        }
    }

    if(target_points.size() >= 2) {
        Vector2 first_point = target_points[0];
        Vector2 last_point = target_points[target_points.size() - 1];
        
        if(direction.x > 0) {
            first_point.y -= BOX_PROTRUSION_SPREAD;
            last_point.y += BOX_PROTRUSION_SPREAD;
        }else if(direction.x < 0) {
            first_point.y += BOX_PROTRUSION_SPREAD;
            last_point.y -= BOX_PROTRUSION_SPREAD;
        }else if(direction.y < 0) {
            first_point.x -= BOX_PROTRUSION_SPREAD;
            last_point.x += BOX_PROTRUSION_SPREAD;
        }else if(direction.y > 0) {
            first_point.x += BOX_PROTRUSION_SPREAD;
            last_point.x -= BOX_PROTRUSION_SPREAD;
        }
        
        target_points.set(0, first_point);
        target_points.set(target_points.size() - 1, last_point);
    }
    
    if(vertex_indices.size() > 0) {
        move_multiple_points(vertex_indices, target_points, duration);
    }else ERR_PRINT("지정된 방향과 크기에 해당하는 돌출부를 만들 수 없습니다. 점이 존재하지 않습니다.");
}

int BattleBox::move_closest_point(Vector2 target_point, float duration) {
    if(!isPolygonMode || target_shape.size() < 3) return -1;
    
    Vector2 local_target = to_local(target_point);
    int closest_vertex = find_closest_vertex(target_shape, local_target);
    
    if(closest_vertex >= 0 && closest_vertex < target_shape.size()) {
        move_point_by_index(closest_vertex, target_point, duration);
    }
    return closest_vertex;
}

void BattleBox::move_point_by_index(int vertex_index, Vector2 target_point, float duration) {
    if (!isPolygonMode || vertex_index < 0 || vertex_index >= target_shape.size()) return;
    
    Vector2 local_target = to_local(target_point);
    Vector2 old_position = target_shape[vertex_index];

    target_shape.set(vertex_index, local_target);
    if(!is_polygon_valid(target_shape)) {
        target_shape.set(vertex_index, old_position);
        ERR_PRINT("유효하지 않은 다각형 모양입니다. 점을 이동할 수 없습니다.");
        return;
    }
    target_shape.set(vertex_index, old_position);
    
    if(active_tweens.has(vertex_index)) {
        Ref<Tween> existing_tween = active_tweens[vertex_index];
        if (existing_tween.is_valid()) {
            existing_tween->kill();
        }
        active_tweens.erase(vertex_index);
        
        for(int i=0; i < tweening_vertices.size(); i++) {
            if (tweening_vertices[i] == vertex_index) {
                tweening_vertices.remove_at(i);
                break;
            }
        }
    }
    
    Ref<Tween> new_tween = create_tween()->set_ease(EaseType)->set_trans(TransType);
    new_tween->tween_method(Callable(this, "_on_point_tween_step").bind(vertex_index), old_position, local_target, duration);
    new_tween->connect("finished", Callable(this, "_on_point_tween_finished").bind(vertex_index), CONNECT_ONE_SHOT);

    active_tweens[vertex_index] = new_tween;
    tweening_vertices.append(vertex_index);
    
    new_tween->play();
}

int BattleBox::move_point_by_offset(Vector2 from_point, Vector2 offset, float duration) {
    if (!isPolygonMode || target_shape.size() < 3) return -1;
    
    Vector2 local_from = to_local(from_point);
    int closest_vertex = find_closest_vertex(target_shape, local_from);
    
    if(closest_vertex >= 0 && closest_vertex < target_shape.size()) {
        Vector2 current_pos = target_shape[closest_vertex];
        Vector2 target_pos = to_global(current_pos + offset);
        move_point_by_index(closest_vertex, target_pos, duration);
    }
    return closest_vertex;
}

void BattleBox::move_multiple_points(PackedInt64Array vertex_indices, PackedVector2Array target_points, float duration) {
    if (vertex_indices.size() != target_points.size()) {
        ERR_PRINT("vertex_indices와 target_points의 크기가 다릅니다.");
        return;
    }
    
    for(int i=0; i < vertex_indices.size(); i++) {
        move_point_by_index(vertex_indices[i], target_points[i], duration);
    }
}

void BattleBox::stop_all_point_tweens() {
    Array keys = active_tweens.keys();
    for (int i = 0; i < keys.size(); i++) {
        Ref<Tween> tween = active_tweens[keys[i]];
        if (tween.is_valid()) {
            tween->kill();
        }
    }
    active_tweens.clear();
    tweening_vertices.clear();
}
// * * //


// 내부 함수 //
void BattleBox::_real_rotate_by(Ref<ArgsHolder> args) {
    float target_rotation = args->args[0];
    bool is_relative = args->args[1];
    
    tw = create_tween();
    tw->set_ease(EaseType);
    tw->set_trans(TransType);
    
    if (is_relative) {
        float current_rotation = rect_container->get_rotation();
        tw->tween_property(rect_container, "rotation", current_rotation + target_rotation, args->get_duration());
    } else {
        tw->tween_property(rect_container, "rotation", target_rotation, args->get_duration());
    }
    
    tw->play();
}

void BattleBox::_tween_size(Ref<ArgsHolder> args) {
    tw = create_tween()->set_parallel(true);
    tw->set_ease(EaseType);
    tw->set_trans(TransType);
    
    float margin_left = rect_container->get_theme_constant("margin_left");
    float margin_top = rect_container->get_theme_constant("margin_top");
    float margin_right = rect_container->get_theme_constant("margin_right");
    float margin_bottom = rect_container->get_theme_constant("margin_bottom");
   
    Vector2 current_corner0 = Vector2(margin_left, margin_top);
    Vector2 current_corner1 = Vector2(640 - margin_right, 480 - margin_bottom);

    Vector2 anchor_targets_0 = anchor_targets[0];
    Vector2 anchor_targets_1 = anchor_targets[1];
    
    tw->tween_property(rect_container, "theme_override_constants/margin_left", 
                      margin_left + (anchor_targets_0.x - current_corner0.x), args->get_duration());
                      
    tw->tween_property(rect_container, "theme_override_constants/margin_top", 
                      margin_top + (anchor_targets_0.y - current_corner0.y), args->get_duration());
                      
    tw->tween_property(rect_container, "theme_override_constants/margin_right", 
                      margin_right + (current_corner1.x - anchor_targets_1.x), args->get_duration());
                      
    tw->tween_property(rect_container, "theme_override_constants/margin_bottom", 
                      margin_bottom + (current_corner1.y - anchor_targets_1.y), args->get_duration());
    
    tw->play();
    tw->connect("finished", Callable(this, "emit_signal").bind("tween_finished"));
}

void BattleBox::_box_set_size() {
    float margin_left = rect_container->get_theme_constant("margin_left");
    float margin_top = rect_container->get_theme_constant("margin_top");
    float margin_right = rect_container->get_theme_constant("margin_right");
    float margin_bottom = rect_container->get_theme_constant("margin_bottom");
   
    Vector2 current_corner0 = Vector2(margin_left, margin_top);
    Vector2 current_corner1 = Vector2(640 - margin_right, 480 - margin_bottom);

    Vector2 anchor_targets_0 = anchor_targets[0];
    Vector2 anchor_targets_1 = anchor_targets[1];

    rect_container->add_theme_constant_override("margin_left", 
                      margin_left + (anchor_targets_0.x - current_corner0.x));
    
    rect_container->add_theme_constant_override("margin_top", 
                      margin_top + (anchor_targets_0.y - current_corner0.y));
                      
    rect_container->add_theme_constant_override("margin_right", 
                      margin_right + (current_corner1.x - anchor_targets_1.x));
                      
    rect_container->add_theme_constant_override("margin_bottom", 
                      margin_bottom + (current_corner1.y - anchor_targets_1.y));
}

Vector2 BattleBox::_anchor_position(RelativePosition relative_to, Vector2 new_position, Vector2 intended_size, Vector2 final_size, bool is_relative) {
    Vector2 anchor_targets_0 = anchor_targets[0];
    Vector2 anchor_targets_1 = anchor_targets[1];
    Vector2 final_position;
    
    switch (relative_to) {
        case RELATIVE_TOP_LEFT:
            final_position = is_relative ? new_position + anchor_targets_0 : new_position;
            return final_position;
            
        case RELATIVE_TOP_RIGHT:
            final_position = is_relative ? 
                new_position + anchor_targets_0 + intended_size.x * Vector2(1, 0) : 
                new_position;
            return final_position - final_size.x * Vector2(1, 0);
            
        case RELATIVE_BOTTOM_LEFT:
            final_position = is_relative ? 
                new_position + anchor_targets_0 + intended_size.y * Vector2(0, 1) : 
                new_position;
            return final_position - final_size.y * Vector2(0, 1);
            
        case RELATIVE_BOTTOM_RIGHT:
            final_position = is_relative ? new_position + anchor_targets_1 : new_position;
            return final_position - final_size;
            
        case RELATIVE_CENTER:
            final_position = is_relative ? 
                new_position + anchor_targets_0 + intended_size / 2.0 : 
                new_position;
            return final_position - final_size / 2.0;
            
        default:
            return anchor_targets_0;
    }
}

void BattleBox::_polygon_disable_real(bool is, float duration) {
    if(!is) {
        get_tree()->connect("physics_frame", Callable(this, "_polygon_disable_real").bind(true, duration), CONNECT_ONE_SHOT);
    }
    Vector2 margin_offset = Vector2(rect_container->get_theme_constant("margin_left"), rect_container->get_theme_constant("margin_top"));
    PackedVector2Array rect_poly;
    PackedInt64Array vertex_indices;
    float perimeter = (current_size.x + current_size.y) * 2.0f;
    float segment_length = perimeter / float(static_shape.size());
    for(int i=0; i < static_shape.size(); i++) {
        float distance = segment_length * float(i);
        Vector2 point;

        if(distance < current_size.x) {
            point = margin_offset + Vector2(distance, 0);
        }else if(distance < current_size.x + current_size.y) {
            point = margin_offset + Vector2(current_size.x, distance - current_size.x);
        }else if(distance < current_size.x * 2.0f + current_size.y) {
            point = margin_offset + Vector2(current_size.x - (distance - current_size.x - current_size.y), current_size.y);
        }else {
            point = margin_offset + Vector2(0, current_size.y - (distance - current_size.x * 2.0f - current_size.y));
        }
        rect_poly.push_back(point);
        vertex_indices.push_back(i);
    }

    for(int i=0; i < rect_poly.size(); i++) {
        Vector2 poly = rect_poly[i];
        int vertex_index = vertex_indices[i];
        Vector2 old_position = static_shape[vertex_index];

        Ref<Tween> new_tween = create_tween()->set_ease(EaseType)->set_trans(TransType);
        new_tween->tween_method(Callable(this, "_on_point_tween_step").bind(vertex_index), old_position, poly, duration);
        new_tween->connect("finished", Callable(this, "_on_point_tween_finished").bind(vertex_index), CONNECT_ONE_SHOT);
        active_tweens[vertex_index] = new_tween;
    }
    Ref<Tween> final_tween = active_tweens[vertex_indices[vertex_indices.size() - 1]];
    final_tween->connect("finished", Callable(this, "_polygon_reset_finished"), CONNECT_ONE_SHOT);
}

void BattleBox::_polygon_reset_finished() {
    isPolygonMode = false;
    polygon->set_disabled(true);
    for(int i=0; i < 4; i++) collisions[i].set("disabled", false);
    
    // 모든 활성 트윈 중지
    stop_all_point_tweens();
    
    static_shape.clear();
    target_shape.clear();
}

int BattleBox::find_closest_edge_to_point(PackedVector2Array& poly, Vector2 point) {
    int best_edge = 0;
    float best_distance = Math_INF;
    for(int i=0; i < poly.size(); i++) {
        Vector2 a = poly[i];
        Vector2 b = poly[(i + 1) % poly.size()];
        Vector2 closest = Geometry2D::get_singleton()->get_closest_point_to_segment(point, a, b);
        float distance = (point - closest).length_squared();
        
        if (distance < best_distance) {
            best_distance = distance;
            best_edge = i;
        }
    }
    
    return best_edge;
}

int BattleBox::find_closest_vertex(const PackedVector2Array& poly, const Vector2& point) {
    if(poly.size() == 0) return -1;
    
    int closest_index = 0;
    float closest_distance = Math_INF;
    
    for(int i= 0; i < poly.size(); i++) {
        float distance = (poly[i] - point).length_squared();
        if(distance < closest_distance) {
            closest_distance = distance;
            closest_index = i;
        }
    }
    
    return closest_index;
}

bool BattleBox::is_polygon_valid(const PackedVector2Array& poly) {
    if (poly.size() < 3) return false;
    
    for(int i = 0; i < poly.size(); i++) {
        Vector2 a1 = poly[i];
        Vector2 a2 = poly[(i + 1) % poly.size()];
        
        for (int j = i + 2; j < poly.size(); j++) {
            if(j == poly.size() - 1 && i == 0) continue;
            
            Vector2 b1 = poly[j];
            Vector2 b2 = poly[(j + 1) % poly.size()];
            Variant intersection = Geometry2D::get_singleton()->segment_intersects_segment(a1, a2, b1, b2);
            
            if(intersection.get_type() == Variant::VECTOR2) return false;
        }
    }
    return true;
}

void BattleBox::_on_point_tween_step(Vector2 new_position, int vertex_index) {
    if(vertex_index >= 0 && vertex_index < target_shape.size()) {
        target_shape.set(vertex_index, new_position);
    }
}

void BattleBox::_on_point_tween_finished(int vertex_index) {
    active_tweens.erase(vertex_index);
    
    for(int i=0; i < tweening_vertices.size(); i++) {
        if(tweening_vertices[i] == vertex_index) {
            tweening_vertices.remove_at(i);
            break;
        }
    }
}
// * * //