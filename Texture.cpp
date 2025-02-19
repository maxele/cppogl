class Texture
{
public:
	unsigned int _texture;
	int _width, _height;
	Texture() {
		_texture = 0;
		INFOLN("Created (X)");
	}
	Texture(FT_GlyphSlot glyph) {
		/* INFO("pitch: %d\n", glyph->pitch); */
		_width = glyph->bitmap.width;
		_height = glyph->bitmap.rows;
		glCreateTextures(GL_TEXTURE_2D,1, &_texture);
		bind();
		glTextureStorage2D(_texture, 1, GL_R8, glyph->bitmap.width, glyph->bitmap.rows);
		glTextureSubImage2D(_texture, 0, 0, 0, glyph->bitmap.width, glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
		INFO("Created (%d) (%d)\n", glyph->glyph_index, _texture);

		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	~Texture() {
		INFO("Free (%d)\n", _texture);
		if (_texture)
			glDeleteTextures(1, &_texture);
	}
	void bind() {
		if (_texture)
			glBindTexture(GL_TEXTURE_2D, _texture);
		else WARNINGLN("Texture is null");
	}
};
