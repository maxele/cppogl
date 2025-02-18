#define NR_KEY_CODES SDLK_ENDCALL+1

class SDLHelper
{
public:
	bool _quit = false;
	SDL_Window *_window;
	void *_gl_context;
	int _width = 640;
	int _height = 480;
	bool *_keys;
	status init();
	SDLHelper();
	~SDLHelper();
	void handleEvents();
	void clear();
	void updateWindowSize();
	void swapWindows();
};
SDLHelper::SDLHelper() {
	_keys = (bool*)malloc(NR_KEY_CODES * sizeof(bool));
}
status SDLHelper::init() {
	INFOLN("Initializing sdl");
	SDL_Init(SDL_INIT_VIDEO);
	_window = SDL_CreateWindow(
		"C SDL2 Demo",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		_width,
		_height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	SDL_ERROR(_window);

	_gl_context = SDL_GL_CreateContext(_window);
	SDL_ERROR(_gl_context);

	SDL_ERROR(!SDL_GL_MakeCurrent(_window, _gl_context));

	// set the opengl context version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	// turn on double buffering set the depth buffer to 24 bits
	// you may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glewInit();
	return 0;
}
SDLHelper::~SDLHelper() {
	SDL_GL_DeleteContext(_gl_context);
	SDL_DestroyWindow(_window);
	/* glfwQuit(); */
	SDL_Quit();
	INFOLN("Uninitialized sdl");
	free(_keys);
}
void SDLHelper::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				_quit = true;
				break;
			case SDL_KEYDOWN:
				_keys[event.key.keysym.sym] = 1;
				break;
			case SDL_KEYUP:
				_keys[event.key.keysym.sym] = 0;
				break;
		}
	}
}
void SDLHelper::clear() {
	glViewport(0, 0, _width, _height);
	glClearColor(.9, .7, .4, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}
void SDLHelper::swapWindows() {
	SDL_GL_SwapWindow(_window);
}
void SDLHelper::updateWindowSize() {
	SDL_GetWindowSize(_window, &_width, &_height);
}
