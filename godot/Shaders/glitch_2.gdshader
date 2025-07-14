shader_type canvas_item;

uniform float time = 0;
uniform vec4 overlay_color : source_color = vec4(0.46, 0, 1, 0.65);

float rand(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void fragment() {
    vec2 uv = UV;

    float noise = rand(uv * 10.0 + vec2(TIME * 0.1, TIME * 0.1));

    float scratch = smoothstep(0.9, 1.0, rand(vec2(uv.y * 50.0, TIME * 0.2)));

    float combined = max(noise * 0.7, scratch * 0.5);

    COLOR = texture(TEXTURE, UV);
    COLOR.rgb = mix(COLOR.rgb, overlay_color.rgb, combined * overlay_color.a);
}