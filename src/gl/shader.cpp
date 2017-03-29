// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "shader.hpp"

#include <iostream>

ShaderUnit::ShaderUnit(const std::string& file, ShaderType type) {
	FILE* fp = fopen(file.c_str(), "rb");
	if (!fp)
		throw ShaderUnitException("File not found!");

	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	char* str = (char*)malloc(size + 1);
	if (!str) {
		fclose(fp);
		throw ShaderUnitException("Malloc failed");
	}
	str[size] = '\0';

	fread(str, size, 1, fp);
	fclose(fp);

	_unit = glCreateShader((GLenum)type);
	glShaderSource(_unit, 1, &str, NULL);
	glCompileShader(_unit);
	free(str);

	GLint status;
	glGetShaderiv(_unit, GL_COMPILE_STATUS, &status);
	/*if (status == GL_FALSE)*/ {
		GLint len;
		glGetShaderiv(_unit, GL_INFO_LOG_LENGTH, &len);

		std::vector<GLchar> errorLog(len);
		glGetShaderInfoLog(_unit, len, &len, errorLog.data());

		const char* strtype = toString(type);

		char buf[0x1000];
		snprintf(buf, sizeof(buf), "Compile %s in %s(%s) shader:\n%s\n", status == GL_FALSE ? "failure" : "successful", file.c_str(), strtype, errorLog.data());
		if (status == GL_FALSE)
			throw ShaderUnitException(std::string(buf));
		else
			printf("%s", buf);
	}
}

ShaderUnit::~ShaderUnit() {
	glDeleteShader(_unit);
}

UniformBuffer::UniformBuffer(size_t size, GLenum dataMode) {
	glGenBuffers(1, &_bufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, _bufferID);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, dataMode);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::~UniformBuffer() {
	glDeleteBuffers(1, &_bufferID);
}

ShaderProgram::ShaderProgram() {
	_program = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
	for (std::map<std::string, std::shared_ptr<UniformBuffer>>::iterator it = _uniformBuffer.begin(); it != _uniformBuffer.end(); ++it)
		it->second.reset();

	while (_units.size()) {
		glDetachShader(_program, _units.back()->getUnit());
		_units.pop_back();
	}

	glDeleteProgram(_program);
}

ShaderProgram& ShaderProgram::attach(std::shared_ptr<ShaderUnit> unit) {
	glAttachShader(_program, unit->getUnit());
	_units.push_back(unit);
	return *this;
}

void ShaderProgram::finalize() {
	glLinkProgram(_program);
	GLint status;
	glGetProgramiv(_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint len;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &len);

		std::vector<GLchar> errorLog(len);
		glGetProgramInfoLog(_program, len, &len, errorLog.data());

		char buf[0x1000];
		snprintf(buf, sizeof(buf), "Linking failed (%u):\n%s", _program, errorLog.data());
		throw ShaderUnitException(std::string(buf));
	}
}

ShaderProgram& ShaderProgram::addUniform(const std::string& name) {
	GLint loc = glGetUniformLocation(_program, name.c_str());
	if (loc == -1) {
		// throw ShaderProgramException(std::string("Uniform not found: %s") + name);
		std::cerr << "Uniform not found: " << name << std::endl;
	}

	_uniform[name] = loc;
	return *this;
}

ShaderProgram& ShaderProgram::addUniformBuffer(const std::string& name, std::shared_ptr<UniformBuffer> buffer, GLuint bindingID) {
	_uniformBuffer[name] = buffer;

	GLint uniformBlockID = glGetUniformBlockIndex(_program, name.c_str());
	glUniformBlockBinding(_program, uniformBlockID, bindingID);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingID, buffer->getID());
	return *this;
}

ShaderProgram& ShaderProgram::bind() {
	glUseProgram(_program);
	return *this;
}

ShaderProgram& ShaderProgram::compute(const glm::ivec3& groups) {
	glDispatchCompute(groups.x, groups.y, groups.z);
	return *this;
}
