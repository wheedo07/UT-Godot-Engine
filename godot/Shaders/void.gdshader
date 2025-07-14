shader_type canvas_item;

uniform sampler2D SCREEN_TEXTURE : hint_screen_texture, filter_linear;
uniform float distortion_amount : hint_range(0.0, 1.0) = 0.002;
uniform float wave_speed : hint_range(0.0, 5.0) = 2.0;
uniform float wave_frequency : hint_range(0.0, 20.0) = 5.0;
uniform vec4 void_color : source_color = vec4(0.0, 0.0, 0.0, 1.0);
uniform float glitch_intensity : hint_range(0.0, 1.0) = 0.1;

// 랜덤 함수 구현
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void fragment() {
    // 기본 UV 좌표
    vec2 uv = SCREEN_UV;

    // 시간에 따른 왜곡
    float time = TIME * wave_speed;

    // 사인파를 이용한 왜곡 효과
    vec2 distortion = vec2(
        sin(uv.y * wave_frequency + time) * distortion_amount,
        cos(uv.x * wave_frequency + time) * distortion_amount
    );

    // 글리치 효과
    float glitch = step(1.0 - glitch_intensity, sin(TIME * 50.0));
    distortion *= mix(1.0, random(vec2(TIME * 0.1, TIME * 0.1)), glitch);

    // 최종 색상 계산
    vec4 screen_color = texture(SCREEN_TEXTURE, uv + distortion);

    // void 색상과 블렌딩
    COLOR = mix(screen_color, void_color, length(distortion) * 2.0);
}