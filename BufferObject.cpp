class BufferObject
{
public:
	GLenum _type;
	GLuint _id;
	BufferObject(GLenum type);
	~BufferObject();
	void loadData(int count, void **data, GLenum usage);
	void bind();
	void draw(GLsizei count);
};
BufferObject::BufferObject(GLenum type) {
	_type = type;
	glGenBuffers(1, &_id);
}
BufferObject::~BufferObject() {
	glDeleteBuffers(1, &_id);
}
void BufferObject::loadData(int count, void **data, GLenum usage) {
	bind();
	glBufferData(_type, count, data, usage);
}
void BufferObject::bind() {
	glBindBuffer(_type, _id);
}
