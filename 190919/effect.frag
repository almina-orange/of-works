uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;
uniform sampler2DRect texture;
uniform float ang;
uniform float bias;

const float PI = 3.1415926535;

// Generate random noise
float random (vec2 st) {
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453123);
}

// Generate random noise in 2D
vec2 random2(vec2 st){
    st = vec2(
        dot(st, vec2(127.1, 311.7)),
        dot(st, vec2(269.5, 183.3))
    );
    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);
}

// Generate gradation noise
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(
        mix(
            dot( random2(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0) ),
            dot( random2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0) ),
            u.x
        ),
        mix(
            dot( random2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0) ),
            dot( random2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0) ),
            u.x
        ),
        u.y
    );
}

float func(float x) {
    return x * (sin(ang) / cos(ang)) + bias;
}

void main(void) {
    vec2 uv = (gl_FragCoord.xy * 2.0 - resolution.xy) / min(resolution.x, resolution.y);
    vec2 st = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);
    vec2 pq = st;

    /**
     * splitted shift and strech
     */
    float shift = 100.0;
    if (func(st.x - 360.0) + shift < st.y - 360.0) { pq.y = st.y - shift; }
    else if (func(st.x - 360.0) - shift < st.y - 360.0) { pq.y = func(st.x - 360.0) + 360.0; }
    else { pq.y = st.y + shift; }

    vec3 color = texture2DRect(texture, pq).rgb;

    /**
     * splitted color
     */
    if (func(st.x - 360.0) + shift < st.y - 360.0) { color = color - 0.1; }
    else if (func(st.x - 360.0) - shift < st.y - 360.0) { color = color; }
    else { color = color + 0.1; }

    /**
     * Japanic effect
     */
    color += noise(uv * 3.0) * 0.1 + random(uv) * 0.1;

	gl_FragColor = vec4(color, 1.0);
}