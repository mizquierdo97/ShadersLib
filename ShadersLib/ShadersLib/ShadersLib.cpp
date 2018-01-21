#include "stdafx.h"  
#include "ShadersLib.h"  


#pragma comment (lib, "Glew/libx86/glew32.lib") 

namespace ShaderLib {

	ShaderProgram* Functions::CreateShader(char* name)
	{

		ShaderProgram* newProgram = new ShaderProgram();
	
		newProgram->name = name;

		programs.push_back(newProgram);
		return newProgram;
	}

	uint Functions::CompileShader(std::string path, ShaderType type)
	{
		uint id = 0;
		switch (type) {
		case fragment:
			id = glCreateShader(GL_FRAGMENT_SHADER);
			break;

		case vertex:
			id = glCreateShader(GL_VERTEX_SHADER);
			break;

		case geometry:
			id = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		}


		FILE * pFile;
		long lSize;
		char * buffer;
		size_t result;
		std::string string_buffer;
		fopen_s(&pFile,path.c_str(), "rb");
		if (pFile == NULL) { fputs("File error", stderr); exit(1); }

		// obtain file size:
		fseek(pFile, 0, SEEK_END);
		lSize = ftell(pFile);
		rewind(pFile);

		// allocate memory to contain the whole file:
		buffer = new char[lSize + 1];// (char*)malloc(sizeof(char)*lSize);
		memset(buffer, 0, lSize + 1);
		if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

		// copy the file into the buffer:
		result = fread(buffer, 1, lSize, pFile);

		fclose(pFile);

		//Set fragment source
		glShaderSource(id, 1, (const GLchar* const *)&buffer, NULL);

		//Compile fragment source
		glCompileShader(id);

		//Check fragment shader for errors
		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(id, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			return 0;
		}
		
		Shader* newShader = new Shader();
		newShader->shaderType = type;
		newShader->shaderID = id;
		newShader->shaderPath = path;
		newShader->shaderText = buffer;

		AddShaderList(newShader);

		return id;

	}

	void Functions::AddShaderList(Shader* newShader)
	{
		shaders.push_back(newShader);
	}
	

	void ShaderProgram::AddFragment(Shader* newFragment)
	{
		fragment = newFragment;
	}

	void ShaderProgram::AddVertex(Shader* newVertex)
	{
		vertex = newVertex;
	}

	void ShaderProgram::AddGeometry(Shader* newGeometry)
	{
		geometry = newGeometry;
	}

	bool ShaderProgram::LoadProgram()
	{
		
		GLint programSuccess = GL_TRUE;

		programID = glCreateProgram();

		if (vertex!= nullptr)
			glAttachShader(programID, vertex->shaderID);

		if (fragment != nullptr)
			glAttachShader(programID, fragment->shaderID);

		if (geometry != nullptr)
			glAttachShader(programID, geometry->shaderID);

		glLinkProgram(programID);
		glGetProgramiv(programID, GL_LINK_STATUS, &programSuccess);

		if (programSuccess != GL_TRUE)
		{
			//printProgramLog(mProgramID);
			return false;
		}
		
		
		return true;

	}
	bool ShaderProgram::Bind()
	{
		glUseProgram(programID);

		GLenum error = glGetError();

		if (error != GL_NO_ERROR) {

			printf("Error binding shader! %s\n", gluErrorString(error));			
			return false;
		}
		return true;
	}

	void ShaderProgram::Unbind()
	{
		glUseProgram(NULL);
	}
	
}