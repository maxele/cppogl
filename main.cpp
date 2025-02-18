#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include <ft2build.h>
#include FT_FREETYPE_H

typedef int status;

using namespace std;

#include "./util.cpp"

#include "./ShaderProgram.cpp"
#include "./BufferObject.cpp"
#include "./SDLHelper.cpp"
#include "./Texture.cpp"
#include "./TTFHelper.cpp"

int main() {
	SDLHelper sdl;
	if (sdl.init()) {
		return 1;
	}

	TTFHelper ttf;
	if (!ttf._face) {
		return 1;
	}
	ttf.setPixelSize(0, sdl._height*.9);
	Texture character = ttf.renderC('M');
	if (!character._texture) {
		return 1;
	}

	ShaderProgram shader("./vertex.glsl", "./fragment.glsl");
	if (!shader._program) {
		ERRORLN("Failed to create ShaderProgram");
		return 1;
	}
	shader.use();
	shader.setUniform1f("uRatio", 1.);
	shader.setUniform1f("uTime", 0.);

	vector<float> vertices = {
		+0.9, +0.9, 0, 1, 0,
		-0.9, +0.9, 0, 0, 0,
		-0.9, -0.9, 0, 0, 1,
		+0.9, -0.9, 0, 1, 1,
	};
	vector<GLuint> indices = {
		0, 1, 2,
		0, 3, 2,
	};

	BufferObject vbo(GL_ARRAY_BUFFER), ebo(GL_ELEMENT_ARRAY_BUFFER);
	vbo.loadData(vertices.size()*sizeof(float), (void**)&vertices[0], GL_STATIC_DRAW);
	ebo.loadData(indices.size()*sizeof(GLuint), (void**)&indices[0], GL_STATIC_DRAW);

	while (!sdl._quit) {
		shader.setUniform1f("uTime", SDL_GetTicks());
		if (sdl._keys[SDLK_r]) {
			shader.recompile();
			sdl._keys[SDLK_r] = false;
		} else if (sdl._keys[SDLK_q]) {
			sdl._quit = true;
		}
		sdl.updateWindowSize();
		sdl.handleEvents();
		sdl.clear();

		// draw rectangle {{{
		shader.use();
		shader.setUniform1f("uRatio", (float)sdl._width / sdl._height
		      * (float)character._height / character._width);
		vbo.bind();
		ebo.bind();
		character.bind();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		// }}}

		sdl.swapWindows();
	}

	return 0;
}
