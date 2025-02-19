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

void setUniforms(TTFHelper &ttf, int w, int h) {
	glUniform3f(ttf.getLocation("uColor"), 0.1, .3, .4);
	glUniform2f(ttf.getLocation("uResolution"), (float)w, (float)h);
}

int font_size = 50;
void cli_puts(TTFHelper &ttf, string s, int *w, int *h) {
	for (char c : s) {
		if (c == '\n') {
			*h = *h + font_size;
			*w = 10;
			continue;
		}
		ttf.setupShader(*w, *h, c);
		*w += ttf._characters[c]->_advance[0];
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void cli_handle_events(TTFHelper &ttf, string &inputBuffer, string &logBuffer, bool *quit) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			char c = event.key.keysym.sym;
			if (c == 13) { // newline
				if (inputBuffer == "quit") {
					*quit = true;
				} else if (inputBuffer == "clear") {
					logBuffer.clear();
					inputBuffer.clear();
					continue;
				} else if (inputBuffer.rfind("resize ", 0) == 0) {
					inputBuffer.erase(inputBuffer.begin(), inputBuffer.begin()+7);
					ttf.setPixelSize(0, stoi(inputBuffer.c_str()));
					font_size = stoi(inputBuffer.c_str())*1.2;
					if (ttf.loadChars()) {
						inputBuffer = "Error: Failed to resize chars";
					} else {
						inputBuffer = "Info: Resized chars to " + inputBuffer;
					}
				}
				inputBuffer.append(1, '\n');
				inputBuffer.append(logBuffer);
				logBuffer = inputBuffer;
				inputBuffer = "";
			} else if (c == 8) { // backspace
				if (!inputBuffer.empty())
					inputBuffer.pop_back();
			} else if (c > 0 && c < 128) {
				inputBuffer.append(1, c);
			}
		}
	}
}

int main() {
	SDLHelper sdl;
	if (sdl.init()) {
		return 1;
	}

	TTFHelper ttf;
	ttf.setPixelSize(0, 50);
	font_size = 50*1.2;
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

	string inputBuffer = "";
	string logBuffer = "";
	while (!sdl._quit) {
		/* sdl.handleEvents(); */
		/* if (sdl._keys['r']) { */
		/* 	ttf._shader.recompile(); */
		/* 	sdl._keys[SDLK_r] = false; */
		/* } else if (sdl._keys[SDLK_q]) { */
		/* 	sdl._quit = true; */
		/* } */

		cli_handle_events(ttf, inputBuffer, logBuffer, &sdl._quit);

		sdl.updateWindowSize();
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

		int w = font_size/4, h = font_size/4;
		cli_puts(ttf, ">", &w, &h);
		w += font_size/10;
		cli_puts(ttf, inputBuffer, &w, &h);
		w = 10;
		h += font_size;
		cli_puts(ttf, logBuffer, &w, &h);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		// }}}

		sdl.swapWindows();
	}

	return 0;
}
