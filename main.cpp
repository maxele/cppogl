#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef int status;

using namespace std;
#define N 10

#define SDL_ERROR(v) if (!v) { printf("ERROR: %s\n", SDL_GetError()); return 1; }
#define INFO(fmt, ...) printf(("Info [%s]: " + string(fmt)).c_str(), __PRETTY_FUNCTION__, __VA_ARGS__)
#define INFOLN(fmt) INFO("%s\n", fmt)
#define ERROR(fmt, ...) printf(("Error [%s]: " + string(fmt)).c_str(), __PRETTY_FUNCTION__, __VA_ARGS__)
#define ERRORLN(fmt) ERROR("%s\n", fmt)
#define WARNING(fmt, ...) printf(("Warning [%s]: " + string(fmt)).c_str(), __PRETTY_FUNCTION__, __VA_ARGS__)
#define WARNINGLN(fmt) WARNING("%s\n", fmt)

auto read_file(std::string_view path) -> std::string {
    constexpr auto read_size = std::size_t(4096);
    auto stream = std::ifstream(path.data());
    stream.exceptions(std::ios_base::badbit);

    if (not stream) {
        throw std::ios_base::failure("file does not exist");
    }
    
    auto out = std::string();
    auto buf = std::string(read_size, '\0');
    while (stream.read(& buf[0], read_size)) {
        out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
}

#include "./ShaderProgram.cpp"
#include "./ElementBufferObject.cpp"
#include "./VertexBufferObject.cpp"
#include "./BufferObject.cpp"
#include "./SDLHelper.cpp"

int main() {
	SDLHelper sdl;
	if (sdl.init()) {
		return 1;
	}

	ShaderProgram shader("./vertex.glsl", "./fragment.glsl");
	if (!shader._program) {
		ERRORLN("Failed to create ShaderProgram");
		return 1;
	}
	float color[3] = {0, 1, 0};
	shader.use();
	shader.setUniform3fv("uColor", color);

	vector<float> vertices = {
		+0.5, +0.5, 0,
		-0.5, +0.5, 0,
		-0.5, -0.5, 0,
		+0.5, -0.5, 0,
	};
	vector<GLuint> indices = {
		0, 1, 2,
		0, 3, 2,
	};

	BufferObject vbo(GL_ARRAY_BUFFER), ebo(GL_ELEMENT_ARRAY_BUFFER);
	vbo.loadData(vertices.size()*sizeof(float), (void**)&vertices[0], GL_STATIC_DRAW);
	ebo.loadData(indices.size()*sizeof(GLuint), (void**)&indices[0], GL_STATIC_DRAW);

	while (!sdl._quit) {
		sdl.updateWindowSize();
		sdl.handleEvents();
		sdl.clear();
		glEnableVertexAttribArray(0);
		vbo.bind();
		ebo.bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
		shader.use();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
		sdl.swapWindows();
	}

	return 0;
}
