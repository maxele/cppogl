/*
struct Character {
	Texture _texture;
	unsigned int _size[2];
	int _bearing[2];
	long int _advance[2];
};

void *map;
void *face;
void *ft;

void cli_init() {
	INFOLN("Initializing freetype2");

	// initialize the library
	if ( FTInitFreeType( &ft ))
	{
		ERRORLN("FreeType failed to initialize\n");
		return;
	}

	// create a new font face
	int error = FTNewFace( ft,
			 "/usr/share/fonts/TTF/VictorMonoNerdFont-Medium.ttf",
			 0,
			 &face );

	// fancy error from internet
	if ( error == FTErrUnknownFileFormat )
	{
		ERRORLN("the font file could be opened and read, but it appears that its font format is unsupported");
		return;
	}
	else if ( error )
	{
		ERRORLN("the font file could not be opened or read, or it is broken");
		return;
	}
	if (setPixelSize(0, 48)) {
		return;
	}

	shader = ShaderProgram("./textvertex.glsl", "./textfragment.glsl");
	if (!shader.program) {
		ERRORLN("Failed to load text-shaders");
		return;
	}
	// free the all the stuff
	FTDoneFace(face);
	FTDoneFreeType(library);}

~Character() {
	INFO("Free (%d)\n", _texture._texture);
	if (_texture._texture)
		glDeleteTextures(1, &_texture._texture);
}
return;
*/

class TTFHelper
{
public:
	class Character
	{
	public:
		Texture _texture;
		unsigned int _size[2];
		int _bearing[2];
		signed long int _advance[2];
		~Character() {
			INFO("Free (%d)\n", _texture._texture);
			if (_texture._texture)
				glDeleteTextures(1, &_texture._texture);
		}
	};

	FT_Library _library;
	FT_Face _face = 0;
	Character *_characters[128];
	ShaderProgram _shader;
	TTFHelper();
	int setPixelSize(int w, int h);
	bool loadChars();
	bool loadShaders();
	int getLocation(string name) {
		return _shader.getUniformLocation(name);
	}
	void setupShader(int x, int y, char c) {
		_characters[c]->_texture.bind();
		glUniform2f(getLocation("uSize"), _characters[c]->_size[0], _characters[c]->_size[1]);
		glUniform2f(getLocation("uBearing"), _characters[c]->_bearing[0], _characters[c]->_bearing[1]);
		glUniform2f(getLocation("uPosition"), (float)x, (float)y);
	}
};

TTFHelper::TTFHelper() {
	INFOLN("Initializing freetype2");
	if (
		FT_Init_FreeType( &_library )
	) {
		ERRORLN("FreeType failed to initialize\n");
		return;
	}
	int error = FT_New_Face( _library,
			 "/usr/share/fonts/TTF/VictorMonoNerdFont-Medium.ttf",
			 0,
			 &_face );
	if ( error == FT_Err_Unknown_File_Format )
	{
		ERRORLN("the font file could be opened and read, but it appears that its font format is unsupported");
		return;
	}
	else if ( error )
	{
		ERRORLN("the font file could not be opened or read, or it is broken");
		return;
	}
	if (setPixelSize(0, 48)) {
		return;
	}

	_shader = ShaderProgram("./text_vertex.glsl", "./text_fragment.glsl");
	if (!_shader._program) {
		ERRORLN("Failed to load text-shaders");
		return;
	}
	return;
	FT_Done_Face(_face);
	FT_Done_FreeType(_library);
}

bool TTFHelper::loadChars() {
	if (!_face) return true;
	if (!_shader._program) return true;
	for (int c = 0; c < 128; c++) {
		int error = FT_Load_Char(_face, c, FT_LOAD_RENDER);
		if (error) {
			INFO("Failed to load char '%c' (%d)\n", c);
			return true;
		}
		_characters[c] = new Character({
			Texture(_face->glyph),
			{_face->glyph->bitmap.width, _face->glyph->bitmap.rows},
			{_face->glyph->bitmap_left, _face->glyph->bitmap_top},
			{_face->glyph->advance.x >> 6, _face->glyph->advance.y >> 6},
		});
	}
	return false;
}

int TTFHelper::setPixelSize(int w, int h) {
	int error = FT_Set_Pixel_Sizes(
		_face,    /* handle to face object         */
		w,     /* width */
		h );   /* height */
	if (error) ERRORLN("Failed to set font size");
	return error;
}


int font_size = 50;
void cli_puts(TTFHelper &ttf, string s, int *w, int *h) {
	for (char c : s) {
		if (c == '\n') {
			*h = *h + font_size;
			*w = 10;
			continue;
		}
		ttf.setupShader(*w + ttf._characters[c]->_bearing[0], *h + ttf._characters[c]->_bearing[1], c);
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

