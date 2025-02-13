class VertexBufferObject
{
public:
	VertexBufferObject();
	~VertexBufferObject();
	void loadVertices(vector<float> vertices);
	void bind();
	void draw(GLsizei count);
private:
	GLuint id;
};
VertexBufferObject::VertexBufferObject() {
	glGenBuffers(1, &id);
}
VertexBufferObject::~VertexBufferObject() {
	glDeleteBuffers(1, &id);
}
void VertexBufferObject::loadVertices(vector<float> vertices) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
}
void VertexBufferObject::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}
void VertexBufferObject::draw(GLsizei count) {
	glDrawArrays(GL_TRIANGLES, 0, count);
}
