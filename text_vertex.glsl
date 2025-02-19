#version 330

layout(location=0) in vec3 pos;
layout(location=1) in vec2 uv;

uniform vec2 uResolution;
uniform vec2 uSize;
uniform vec2 uBearing;
uniform vec2 uPosition;

out vec2 oUv;

void main() {
	vec2 p = pos.xy * uResolution;
	p.x = p.x / uResolution.x * uResolution.y;
	p.x = p.x / uSize.y * uSize.x;
	p = p * vec2(uSize.y / uResolution.y);
	p += uPosition;
	p.y = p.y - uSize.y + uBearing.y;
	p = p / uResolution * 2 - vec2(1);
	gl_Position = vec4(p, 0, 1);
	oUv = uv;
}
