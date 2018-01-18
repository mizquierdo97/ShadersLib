#include "stdafx.h"  
#include "ShadersLib.h"  


#pragma comment (lib, "Glew/libx86/glew32.lib") 

namespace ShaderLib {

	Shader Functions::CreateShader(char* name)
	{
		Shader temp_shader;
		temp_shader.name = name;
		return temp_shader;
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
		
		return id;

	}
	
	double Functions::Add(double a, double b)
	{
		return a + b;
	}

	double Functions::Multiply(double a, double b)
	{
		return a * b;
	}

	double Functions::AddMultiply(double a, double b)
	{
		return a + (a * b);
	}

	void Shader::AddFragment(int ID)
	{
		fragmentID = ID;
	}

	void Shader::AddVertex(int ID)
	{
		vertexID = ID;
	}

	void Shader::AddGeometry(int ID)
	{
		geometryID = ID;
	}

	bool Shader::LoadProgram()
	{
		
		GLint programSuccess = GL_TRUE;

		programID = glCreateProgram();

		if (vertexID)
			glAttachShader(programID, vertexID);

		if (fragmentID)
			glAttachShader(programID, fragmentID);

		if (geometryID)
			glAttachShader(programID, geometryID);

		glLinkProgram(programID);
		glGetProgramiv(programID, GL_LINK_STATUS, &programSuccess);

		if (programSuccess != GL_TRUE)
		{
			//printProgramLog(mProgramID);
			return false;
		}
		
		return true;

	}
	
}