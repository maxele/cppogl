class ElementBufferObject
{
public:
	ElementBufferObject();
	~ElementBufferObject();
	void loadIndices(vector<GLuint> indices);
	void bind();
	void draw(GLsizei count);
private:
	unsigned int _id;
};
ElementBufferObject::ElementBufferObject() {
	glGenBuffers(1, &_id);
}
ElementBufferObject::~ElementBufferObject() {
	glDeleteBuffers(1, &_id);
}
void ElementBufferObject::loadIndices(vector<GLuint> indices) {
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);
}
void ElementBufferObject::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}
void ElementBufferObject::draw(GLsizei count) {
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}
