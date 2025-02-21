#ifndef __LIB_HPP__
#define __LIB_HPP__

#include <vector>
#include <iostream>
#include <map>
#include <fstream>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

using namespace std;


/* Util {{{*/
std::string read_file(std::string_view path);
/*}}}*/

class Window
{/*{{{*/
public:
	SDL_Window *window;
	SDL_GLContext gl_context;
	int size[2] = {640, 480};
	bool quit = false;

	Window(string title);
	~Window();
	void swapWindows();
	void updateSize();
	void throwIfError(bool error);
	void handleEvents();
};
/*}}}*/

class FreeType
{/*{{{*/
public:
	struct Char {
		int texture;
		float size[2], bearing[2], advance[2];
	};
	map<long int, Char*> chars;
	FreeType(string path);
	~FreeType();
};
/*}}}*/

class Shader
{/*{{{*/
public:
	int program;

	Shader(string vertex, string fragment);
	~Shader();

	static int compile(string filename, GLenum type);
};
/*}}}*/

class Object
{/*{{{*/
public:
	class Attribute {
	public:
		GLint size;
		GLsizei stride;
		GLint skip;
	};
	Shader *shader;
	GLuint vbo, ebo;
	vector<Attribute> attributes;
	int count;

	Object(Shader &s, vector<Attribute> a, vector<float> v);
	Object(Shader &s, vector<Attribute> a, vector<float> v, vector<unsigned int> i);
	~Object();

	void draw();
};
/*}}}*/

#endif // __LIB_HPP__
