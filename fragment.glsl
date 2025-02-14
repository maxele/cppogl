#version 330

in vec3 ourPos;
in float ourTime;

layout (location=0) out vec4 color;

// All components are in the range [0…1], including hue.
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

 

// All components are in the range [0…1], including hue.
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float sum(vec3 v) {
	return v.x + v.y + v.z;
}

void main() {
	// color = vec4((vec3((pos.x+pos.y)/2, pos.y, pos.x)), 1.0);
	vec3 P = fract(ourPos*1.1 * 2);
	// if (ourPos.x > 0) P.x *= -1;
	// if (ourPos.y > 0) P.y *= -1;
	P = P - vec3(+.5, +.5, 0);
	float d = sqrt(sum(P*P));
	float v = abs(sin(d*5 + ourTime/500));
	vec3 c = vec3(v, sin(ourTime/5000), d);
	color = vec4(c, 1);
}
