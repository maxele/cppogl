#include <iostream>
#include <string>

typedef int status;

using namespace std;
#define N 10

#include "./ShaderProgram.cpp"
#include "./EntityBufferObject.cpp"

int main() {
	/* INITIALIZE(); */

	ShaderProgram shader;
	shader.loadShader(-1, "./fragment.glsl");
	shader.loadShader(-2, "./vertex.glsl");
	shader.compileAndLink();

	float vertices[3*N*N] = {0};
	unsigned int indices[3*N] = {0};
	/* LOAD_DATA(vertices, indices); */
	EntityBufferObject ebo;
	ebo.loadIndices(indices);

	cout << "MAIN" << endl;
	return 0;
}
