#version 330

layout(location=0) in vec3 pos;
layout(location=1) in vec2 uv;

uniform vec2 uResolution;
uniform vec2 uSize;
uniform vec2 uBearing;
uniform vec2 uPosition;

out vec2 oUv;

void main() {
	vec2 p = pos.xy;
	// p.x = p.x / uResolution.x * uResolution.y;
	p = p / uResolution * uSize;
	p += uPosition / uResolution;
	p = p *vec2(2) - vec2(.5, 0);
	gl_Position = vec4(p, 0, 1);
	oUv = uv;
}
