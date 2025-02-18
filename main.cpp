#include <iostream>
#include <map>
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

void setUniforms(TTFHelper ttf, int w, int h) {
	glUniform3f(ttf.getLocation("uColor"), 0.1, .3, .4);
	glUniform2f(ttf.getLocation("uResolution"), (float)w, (float)h);
}

int main() {
	SDLHelper sdl;
	if (sdl.init()) {
		return 1;
	}

	TTFHelper ttf;
	ttf.setPixelSize(0, 50);
	if (ttf.loadChars()) {
		return 1;
	}

	vector<float> vertices = {
		+1.0, +1.0, 0, 1, 0,
		-0.0, +1.0, 0, 0, 0,
		-0.0, -0.0, 0, 0, 1,
		+1.0, -0.0, 0, 1, 1,
	};
	vector<GLuint> indices = {
		0, 1, 2,
		0, 3, 2,
	};

	BufferObject vbo(GL_ARRAY_BUFFER), ebo(GL_ELEMENT_ARRAY_BUFFER);
	vbo.loadData(vertices.size()*sizeof(float), (void**)&vertices[0], GL_STATIC_DRAW);
	ebo.loadData(indices.size()*sizeof(GLuint), (void**)&indices[0], GL_STATIC_DRAW);

	while (!sdl._quit) {
		if (sdl._keys[SDLK_r]) {
			ttf._shader.recompile();
			sdl._keys[SDLK_r] = false;
		} else if (sdl._keys[SDLK_q]) {
			sdl._quit = true;
		}
		sdl.updateWindowSize();
		sdl.handleEvents();
		sdl.clear();


		// draw rectangle {{{
		ttf._shader.use();
		setUniforms(ttf, sdl._width, sdl._height);

		vbo.bind();
		ebo.bind();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));

		string s = "Fuck my life";
		int w = 0;
		for (char c : s) {
			w += ttf._characters[c]._bearing[0];
			ttf.setupShader(w, 0, c);
			w += ttf._characters[c]._size[0];
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		// }}}

		sdl.swapWindows();
	}

	return 0;
}
