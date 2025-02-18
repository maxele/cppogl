int compileShader(int program, const char *filename, GLenum shaderType) {/*{{{*/
	/* INFO("compiling shader from file '%s'\n", filename); */
	string sourceStr = read_file(filename);
	const char *source = sourceStr.c_str();
	if (!source) return 1;

	int shader;
	int len = sourceStr.length();
	int status;

	shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &source, &len);
	glCompileShader(shader);
	len = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len) {
		char buf[len];
		glGetShaderInfoLog(shader, len, NULL, buf);
		INFO("ShaderInfoLog: %s", buf);
	}
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status) {
		char log_buffer[1028];
		ERROR("failed to compile shader from file '%s'\n", filename);
		return 0;
	}

	return shader;
}/*}}}*/

class ShaderProgram
{
public:
	ShaderProgram() {};
	ShaderProgram(string vertex, string fragment, bool verbose);
	~ShaderProgram();
	bool setUniform1f(string name, float var);
	bool setUniformMatrix4fv(string name, float var[4*4]);
	bool setUniform3fv(string name, float var[3]);
	void use();
	void recompile();
	int getUniformLocation(string name);
	int _program = 0;
	bool _verbose;
	string _vertex, _fragment;
};

ShaderProgram::ShaderProgram(string vertex, string fragment, bool verbose = false) {
	_verbose = verbose;
	_vertex = vertex;
	_fragment = fragment;
	recompile();
}

void ShaderProgram::recompile() {/*{{{*/
	INFO("compiling shaderProgram vert: '%s', frag: '%s'\n", _vertex.c_str(), _fragment.c_str());
	int program = 0;
	int fragment_shader = 0;
	int vertex_shader = 0;
	int status, len;

	// shaders program
	program = glCreateProgram();
	if (program == 0) {
		ERRORLN("couldn't create shader program");
		goto abort;
	}

	vertex_shader = compileShader(program, _vertex.c_str(), GL_VERTEX_SHADER);
	if (!vertex_shader) {
		ERRORLN("Failed to compile vertex shader");
		goto abort;
	}

	fragment_shader = compileShader(program, _fragment.c_str(), GL_FRAGMENT_SHADER);
	if (!fragment_shader) {
		ERRORLN("Failed to compile fragment shader");
		goto abort;
	}

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	int info_log_len;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_len);
	if (info_log_len) {
		char buf[info_log_len];
		glGetProgramInfoLog(program, info_log_len, NULL, buf);
		INFO("ShaderInfoLog: %s\n", buf);
	}
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		ERRORLN("failed to link program");
		if (program) glDeleteProgram(program);
		return;
	}
	_program = program;
	return;
abort:
	if (vertex_shader) glDeleteShader(vertex_shader);
	if (fragment_shader) glDeleteShader(fragment_shader);
}/*}}}*/

ShaderProgram::~ShaderProgram() {
	glDeleteShader(_program);
}

int ShaderProgram::getUniformLocation(string name) {
	return glGetUniformLocation(_program, name.c_str());
}

void ShaderProgram::use() {
	glUseProgram(_program);
}

bool ShaderProgram::setUniform3fv(string name, float var[3]) {
	int location = glGetUniformLocation(_program, name.c_str());
	if (_verbose && location == -1) {
		WARNING("Couldn't find '%s' in shader program\n", name.c_str());
		return false;
	}
	glUniform3fv(location, 1, var);
	return true;
}

bool ShaderProgram::setUniform1f(string name, float var) {
	int location = glGetUniformLocation(_program, name.c_str());
	if (_verbose && location == -1) {
		WARNING("Couldn't find '%s' in shader program\n", name.c_str());
		return false;
	}
	glUniform1f(location, var);
	return true;
}

bool ShaderProgram::setUniformMatrix4fv(string name, float var[4*4]) {
	int location = glGetUniformLocation(_program, name.c_str());
	if (_verbose && location == -1) {
		WARNING("Couldn't find '%s' in shader program\n", name.c_str());
		return false;
	}
	glUniformMatrix4fv(location, 1, GL_TRUE, var);
	return true;
}
