shader_type canvas_item;

uniform bool enabled = true;
uniform bool pixel_perfect = true;
uniform sampler2D SCREEN_TEX : hint_screen_texture, filter_nearest;

uniform vec2 strength_px = vec2(3.0, 2.5);           // 축별 최대 흔들림(픽셀)
uniform float frequency = 12.0;                      // 흔들림 속도(Hz 느낌)
uniform float strength_scale = 0;

/* (다른 느낌 원하면 변경) */
uniform vec2 phase = vec2(0.0, 1.7);

void fragment() {
    if (!enabled || strength_scale <= 0.0) {
        COLOR = texture(SCREEN_TEX, SCREEN_UV);
    }else {
    	float t = TIME * frequency;

		float sx = sin(t * 1.21 + phase.x) + 0.5 * sin(t * 0.77 + phase.y);
		float sy = cos(t * 1.11 + phase.y) + 0.5 * sin(t * 0.69 + phase.x);

		vec2 offs_px = vec2(sx, sy) * strength_px * strength_scale;

		if (pixel_perfect) {
			offs_px = round(offs_px);
		}

		vec2 uv = SCREEN_UV + offs_px * SCREEN_PIXEL_SIZE;
		COLOR = texture(SCREEN_TEX, uv);
	}
}