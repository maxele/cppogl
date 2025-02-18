class TTFHelper
{
public:
	class Character
	{
	public:
		Texture _texture;
		unsigned int _size[2];
		int _bearing[2];
		long int _advance[2];
	};

	FT_Library _library;
	FT_Face _face = 0;
	Character _characters[128];
	ShaderProgram _shader;
	TTFHelper();
	int setPixelSize(int w, int h);
	bool loadChars();
	bool loadShaders();
	int getLocation(string name) {
		return _shader.getUniformLocation(name);
	}
	void setupShader(int x, int y, char c) {
		_characters[c]._texture.bind();
		glUniform2f(getLocation("uSize"), _characters[c]._size[0], _characters[c]._size[1]);
		glUniform2f(getLocation("uBearing"), _characters[c]._bearing[0], _characters[c]._bearing[1]);
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
		_characters[c] = {
			Texture(_face->glyph),
			{_face->glyph->bitmap.width, _face->glyph->bitmap.rows},
			{_face->glyph->bitmap_left, _face->glyph->bitmap_top},
			{_face->glyph->advance.x, _face->glyph->advance.y},
		};
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

