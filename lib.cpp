#include "lib.hpp"

/* Util {{{*/
auto read_file(std::string_view path) -> std::string
{
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
}/*}}}*/

/*Window{{{*/
Window::Window(string title) {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		throw runtime_error("Couldn't initialize sdl");
	}
	window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		size[0],
		size[1],
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	throwIfError(!window);

	gl_context = SDL_GL_CreateContext(window);
	throwIfError(!gl_context);
	throwIfError(SDL_GL_MakeCurrent(window, gl_context));
	// set the opengl context version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	// turn on double buffering set the depth buffer to 24 bits
	// you may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable some options for rendering
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Initialize glew
	glewInit();
}

Window::~Window() {
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Window::swapWindows() {
	SDL_GL_SwapWindow(window);
}

void Window::updateSize() {
	SDL_GetWindowSize(window, &size[0], &size[1]);
}
void Window::throwIfError(bool error) {
	if (error) {
		throw runtime_error(SDL_GetError());
	}
}
void Window::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
		}
	}
}
/*}}}*/

/* FreeType {{{*/
FreeType::FreeType(string path) {
	chars['a'] = (Char*)malloc(sizeof(Char));
	chars['a']->texture = 1;
}
FreeType::~FreeType() {
	for (pair<long int, Char*> c: chars) {
		free(c.second);
	}
}
/*}}}*/

/* Shader {{{*/
Shader::Shader(string vertex, string fragment) {
	program = glCreateProgram();
	if (!program)
		throw runtime_error("Couldn't create new shader program");

	int vert = compile(vertex, GL_VERTEX_SHADER);
	if (!vert) {
		glDeleteProgram(program);
		throw runtime_error("Couldn't compile vertex shader");
	}

	int frag = compile(fragment, GL_FRAGMENT_SHADER);
	if (!frag) {
		glDeleteProgram(program);
		glDeleteShader(vert);
		throw runtime_error("Couldn't compile fragment shader");
	}

	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	glDetachShader(program, vert);
	glDetachShader(program, frag);
	glDeleteShader(vert);
	glDeleteShader(frag);

	int info_log_len;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_len);
	if (info_log_len) {
		char buf[info_log_len];
		glGetProgramInfoLog(program, info_log_len, NULL, buf);
		cout << "ShaderInfoLog: " << buf << endl;
	}
	int status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		if (program) glDeleteProgram(program);
		throw runtime_error("Failed to link shader program");
	}
}
Shader::~Shader() {
	glDeleteProgram(program);
}

int Shader::compile(string filename, GLenum type) {
	string sourceStr = read_file(filename);
	const char *source = sourceStr.c_str();
	if (!source) return 0;

	int shader;
	int len = sourceStr.length();
	int status;

	shader = glCreateShader(type);

	glShaderSource(shader, 1, &source, &len);
	glCompileShader(shader);
	len = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len) {
		char buf[len];
		glGetShaderInfoLog(shader, len, NULL, buf);
		cout << "ShaderInfoLog: " << buf << endl;
	}
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status) {
		throw runtime_error("failed to compile shader from file" + filename);
	}

	return shader;
}
/*}}}*/

/* Object {{{*/
Object::Object(Shader &s, vector<Attribute> a, vector<float> v)
{
	shader = &s;

	/* copy(a.begin(), a.end(), back_inserter(attributes)); */
	/* for (int i = 0; i < a.size(); i++) { */
	/* 	attributes[i] = a[i]; */
	/* } */

	/* cout << "1. " << attributes.size() << endl; */
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, v.size()*sizeof(float), (void**)&v[0], GL_STATIC_DRAW);
	count = v.size();
	ebo = 0;
}
Object::Object(Shader &s, vector<Attribute> a, vector<float> v, vector<unsigned int> i) {
	Object(s, a, v);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.size()*sizeof(unsigned int), (void**)&i[0], GL_STATIC_DRAW);
	count = i.size();
	copy(a.begin(), a.end(), back_inserter(attributes));

	/* cout << "2. " << this->attributes.size() << " : " << count << endl; */
}
Object::~Object() {
	glDeleteBuffers(1, &vbo);
	if (ebo) glDeleteBuffers(1, &ebo);
};

void Object::draw() {
	for (int i = 0; i < attributes.size(); i++) {
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attributes[i].size,
			GL_FLOAT, GL_FALSE, attributes[i].stride*sizeof(float),
			(void*)(attributes[i].skip*sizeof(float)));
	}

	if (!ebo) glDrawArrays(GL_TRIANGLES, 0, count);
	else glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

	for (int i = 0; i < attributes.size(); i++) {
		glDisableVertexAttribArray(i);
	}
}
/*}}}*/
