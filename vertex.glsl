#version 330

layout(location=0) in vec3 pos;

out vec3 ourColor;

uniform vec3 uColor;

void main() {
	gl_Position = vec4(pos, 1.0);
	ourColor = uColor;
}
