#version 330

in vec2 oUv;

uniform vec3 uColor;
uniform sampler2D iTexture;

out vec4 color;

void main() {
	vec4 t = vec4(vec3(1.0), texture(iTexture, oUv).r);
	// vec4 c = vec4(0.3, .4, .9, 1.0);
	color = vec4(uColor, t.a);
}
