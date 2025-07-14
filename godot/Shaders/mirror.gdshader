shader_type canvas_item;

uniform sampler2D SCREEN_TEXTURE:hint_screen_texture, filter_nearest;
uniform float offset = 0;
uniform float scale: hint_range(0.05, 1.0, 0.05) = 1.0;
uniform bool reflect_x = false;
uniform bool reflect_y = true;

varying float screen_baseY;

void vertex() {
    mat4 t = mat4(vec4(2.0, 0, 0, 0), vec4(0, 2.0, 0, 0), vec4(0, 0, 1.0, 0), vec4(-1.0, -1.0, 0, 1.0));
    screen_baseY = (inverse(t)*SCREEN_MATRIX*CANVAS_MATRIX * (MODEL_MATRIX)*vec4(-1.0/TEXTURE_PIXEL_SIZE.x/2.0,  -1.0/TEXTURE_PIXEL_SIZE.y/2.0, 0, 1.0)).y;
}

void fragment() {

    vec2 uv = SCREEN_UV;

    uv.y -= screen_baseY;
    uv.y *= scale;
    uv.y += screen_baseY;

    vec2 final_uv = uv;

    if (reflect_x) {
        final_uv.x = 1.0 - uv.x;
    }

    if (reflect_y) {
        final_uv.y = screen_baseY * 2.0 - (uv.y - offset);
    } else {
        final_uv.y = uv.y - offset;
    }

    COLOR = texture(SCREEN_TEXTURE, final_uv);
}