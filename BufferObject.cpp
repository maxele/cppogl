class BufferObject
{
public:
	GLenum _type;
	BufferObject(GLenum type);
	~BufferObject();
	void loadData(int count, void **data, GLenum usage);
	void bind();
	void draw(GLsizei count);
private:
	GLuint id;
};
BufferObject::BufferObject(GLenum type) {
	_type = type;
	glGenBuffers(1, &id);
}
BufferObject::~BufferObject() {
	glDeleteBuffers(1, &id);
}
void BufferObject::loadData(int count, void **data, GLenum usage) {
	bind();
	glBufferData(_type, count, data, usage);
}
void BufferObject::bind() {
	glBindBuffer(_type, id);
}
