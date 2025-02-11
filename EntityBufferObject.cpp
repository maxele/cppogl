class EntityBufferObject
{
public:
	EntityBufferObject();
	~EntityBufferObject();
	status loadIndices(float indices[]);
	void activate();
private:
	int id;
};
EntityBufferObject::EntityBufferObject() {
}
EntityBufferObject::~EntityBufferObject() {
}
status EntityBufferObject::loadIndices(float indices[]) {
	return 0;
}
void EntityBufferObject::activate() {
}
