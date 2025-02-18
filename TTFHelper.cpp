class TTFHelper
{
public:
	FT_Library _library;
	FT_Face _face = 0;
	TTFHelper() {
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

	}
	~TTFHelper() {
		FT_Done_Face(_face);
		FT_Done_FreeType(_library);
	}
	Texture renderC(unsigned int charcode) {
		int glyph_index = FT_Get_Char_Index( _face, charcode );
		int error = FT_Load_Char(_face, charcode, FT_LOAD_RENDER);
		if (error) {
			return Texture(); // return empty texture
		}
		Texture t(_face->glyph);
		return t;
	}
	int setPixelSize(int w, int h) {
		int error = FT_Set_Pixel_Sizes(
			_face,    /* handle to face object         */
			w,     /* width */
			h );   /* height */
		if (error) ERRORLN("Failed to set font size");
		return error;
	}
};
