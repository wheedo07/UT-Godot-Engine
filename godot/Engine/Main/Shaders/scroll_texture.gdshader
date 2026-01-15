shader_type canvas_item;

uniform vec2 speed = vec2(1.0, 0.0);
uniform float time = 0.0;
uniform vec2 offset = vec2(0.0, 0.0);

void fragment() {
    vec2 uv_with_offset = mod(UV + time * speed + offset, 1.0);
    COLOR = texture(TEXTURE, uv_with_offset);
}