shader_type canvas_item;

uniform sampler2D base_texture : hint_screen_texture;
uniform vec2 resolution = vec2(0, 0);

float peak(float x, float xpos, float scale) {
    return clamp((1.0 - x) * scale * log(1.0 / abs(x - xpos)), 0.0, 1.0);
}

float random(vec2 uv, float t) {
    float d = 437.0 + mod(t, 10.0);
    vec2 p = uv;
    return fract(sin(cos(1.0 - sin(p.x) * 1.0 - cos(p.y)) * d) * d);
}

vec2 TilingMirror(vec2 uv, vec2 tiling) {
    uv = (uv - 0.5) * tiling + 0.5;
    uv = abs(mod(uv - 1.0, 2.0) - 1.0);
    return uv;
}

vec2 GetAspectRatio(vec2 uv, vec2 res) {
    float aspectRatio = res.x / res.y;
    return (res.x > res.y)
    ? vec2(uv.x * aspectRatio, uv.y)
    : vec2(uv.x, uv.y / aspectRatio);
}

// Shake
uniform bool shake_enable = false;
uniform float shake_speed = 0;
uniform float shake_magnitude = 0;
uniform float shake_hspeed = 0;
uniform float shake_vspeed = 0;

vec2 apply_shake(vec2 uv) {
    float t = 100.0 + TIME * shake_speed;
    vec2 offset;
    offset.x = shake_hspeed * cos(t * 135.0 + sin(t)) * shake_magnitude;
    offset.y = shake_vspeed * sin(t * 90.0 + cos(t)) * shake_magnitude;
    return uv + offset;
}

// Lens distortion
uniform bool lens_enable = false;
uniform float lens_amount = 0;

vec2 apply_lens_distortion(vec2 uv) {
    vec2 center = vec2(0.5);
    vec2 dir = uv - center;
    float polar = atan(dir.y, dir.x);
    float radius = length(dir);
    radius *= (1.0 + pow(radius, 2.0) * lens_amount);
    return center + radius * vec2(cos(polar), sin(polar));
}

// Pixelize
uniform bool pixelize_enable = false;
uniform float pixelize_amount = 0;
uniform float pixelize_max_size = 0;
uniform float pixelize_steps = 0;

vec2 apply_pixelize(vec2 uv) {
    float amount = pixelize_amount;
    if (pixelize_steps > 0.0) amount = ceil(amount * pixelize_steps) / pixelize_steps;
    vec2 gridSize = vec2(pixelize_max_size) * amount;
    if (amount > 0.0)
        uv = floor(uv * resolution / gridSize + 0.5) * gridSize / resolution;
    return uv;
}

// Swirl
uniform bool swirl_enable = false;
uniform float swirl_angle = 0;
uniform float swirl_radius = 0;
uniform float swirl_rounded = 0;
uniform vec2 swirl_center = vec2(0, 0);

vec2 apply_swirl(vec2 uv) {
    uv -= swirl_center;
    float angle = radians(swirl_angle) * smoothstep(0.0, swirl_radius,
        swirl_radius - length((swirl_rounded > 0.5) ? GetAspectRatio(uv, resolution) : uv));
    float c = cos(angle);
    float s = sin(angle);
    mat2 rot = mat2(vec2(c, -s), vec2(s, c));
    uv = rot * uv;
    uv += swirl_center;
    return uv;
}

// Sine wave
uniform bool sine_enable = false;
uniform vec2 sine_frequency = vec2(0, 0);
uniform vec2 sine_amplitude = vec2(0, 0);
uniform float sine_speed = 0;
uniform vec2 sine_offset = vec2(0, 0);

vec2 apply_sinewave(vec2 uv) {
    vec2 uv2 = (uv * resolution + sine_offset) / resolution;
    float spd = TIME * sine_speed;
    vec2 offset;
    offset.x = cos(uv2.y * sine_frequency.x - spd) * sine_amplitude.x;
    offset.y = sin(uv2.x * sine_frequency.y - spd) * sine_amplitude.y;
    return uv + offset;
}

// Interference
uniform bool inter_enable = false;
uniform float inter_speed = 0;
uniform float inter_block_size = 0;
uniform float inter_interval = 0;
uniform float inter_intensity = 0;
uniform float inter_amp1 = 0;
uniform float inter_amp2 = 0;

vec2 apply_interference(vec2 uv) {
    float t = TIME * inter_speed;
    float interference = random(vec2(0.0, ceil(uv.y * (resolution.y * (1.0 - inter_block_size)))), t);
    float vscan = interference;
    interference *= (interference > inter_interval) ? inter_amp1 : inter_amp2;
    float ds = 0.05 * inter_intensity * interference * peak(uv.y, vscan, vscan);
    uv.x -= ds;
    return uv;
}

void fragment() {
    vec2 uv = SCREEN_UV;

    if (shake_enable) uv = apply_shake(uv);
    if (lens_enable) uv = apply_lens_distortion(uv);
    if (pixelize_enable) uv = apply_pixelize(uv);
    if (swirl_enable) uv = apply_swirl(uv);
    if (sine_enable) uv = apply_sinewave(uv);
    if (inter_enable) uv = apply_interference(uv);

    COLOR = texture(base_texture, uv);
}