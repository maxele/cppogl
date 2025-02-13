#version 330

layout(location=0) in vec3 pos;
layout(location=1) in vec2 uv;

out vec3 ourPos;
out float ourTime;

uniform float uRatio;
uniform float uTime;

void main() {
	gl_Position = vec4(pos * vec3(1/uRatio, 1, 1), 1.0);
	ourPos = pos;
	ourTime = uTime;
}
