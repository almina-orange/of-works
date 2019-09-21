uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;
uniform int hex1, hex2, hex3, hex4, hex5;

const float PI = 3.14159265358979;

vec3 hex2color(int hex) {
    float r = float(hex / 256 / 256);
    float g = float(hex / 256 - int(r * 256.0));
    float b = float(hex - int(r * 256.0 * 256.0) - int(g * 256.0));
    return vec3(r / 255.0, g / 255.0, b / 255.0);
}

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

void main(void) {
    vec2 uv = (gl_FragCoord.xy * 2.0 - resolution.xy) / min(resolution.x, resolution.y);
	
    vec3 pal[5];
    pal[0] = hex2color(hex1);
    pal[1] = hex2color(hex2);
    pal[2] = hex2color(hex3);
    pal[3] = hex2color(hex4);
    pal[4] = hex2color(hex5);
    
    float num = 2.0;
    vec2 st = uv * 8.0;
    st.x *= -1.0;
    
    vec3 color = pal[4];
    for (int i = 0; i < 4; i++) {
        st.y += pow(st.x, (sin(length(st) * 0.7 * PI - time) * 0.5 + 0.5) / float(i + 1));
        st.x += pow(st.y, (sin(length(st) * 0.7 * PI - time) * 0.5 + 0.5) / float(i + 1));
        float tmp = sin(length(st) + noise(st)) + 0.5;
        
        color = mix(color, pal[i], tmp);
    }
    
    gl_FragColor = vec4(color, 1.0);
}