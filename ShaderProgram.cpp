class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();
	status loadShader(int type, string sourceFile);
	status compileAndLink();
	void activate();
private:
	int id = 0;
};

ShaderProgram::ShaderProgram() {
}
ShaderProgram::~ShaderProgram() {
}
status ShaderProgram::loadShader(int type, string sourceFile) {
	return 0;
}
status ShaderProgram::compileAndLink() {
	return 0;
}
void ShaderProgram::activate() {
}
