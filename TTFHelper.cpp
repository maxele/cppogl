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
			  "/usr/share/fonts/TTF/Monoid Bold Nerd Font Complete Mono.ttf",
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
		if (setPixelSize(32, 0)) {
			return;
		}

	}
	Texture renderC(unsigned int charcode) {
		int glyph_index = FT_Get_Char_Index( _face, charcode );
		int load_flags = 0;
		int error = FT_Load_Glyph(
			_face,          /* handle to face object */
			glyph_index,   /* glyph index           */
			load_flags );  /* load flags, see below */
		if (error) {
			ERRORLN("failed to load glyph");
		}
		error = FT_Render_Glyph( _face->glyph,   /* glyph slot  */
			  FT_RENDER_MODE_NORMAL ); /* render mode */
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
