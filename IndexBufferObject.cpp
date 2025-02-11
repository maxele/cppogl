class VertexBufferObject
{
public:
	VertexBufferObject();
	~VertexBufferObject();
	status loadVertices(float vertices[]);
	void activate();
private:
	int id;
};
VertexBufferObject::VertexBufferObject() {
}
VertexBufferObject::~VertexBufferObject() {
}
status VertexBufferObject::loadVertices(float vertices[]) {
	return 0;
}
void VertexBufferObject::activate() {
}
