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

	ShaderProgram * Functions::CreateDefaultShader()
	{
		ShaderProgram* defaultShader = new ShaderProgram();
		defaultShader->name = "Default Shader";
		//Success flag
		GLint programSuccess = GL_TRUE;

		//Generate program
		defaultShader->programID = glCreateProgram();

		//Create vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//Get vertex source
		const GLchar* vertexShaderSource[] =
		{
			"#version 330 core\n"
			"layout(location = 0) in vec3 position;\n"
			"layout(location = 1) in vec3 normal;\n"
			"layout(location = 2) in vec2 texCoord;\n"
			"layout(location = 3) in vec4 color;\n"
			"out float ourTime;\n"
			"out vec4 ourColor;\n"
			"out vec3 ourNormal;\n"
			"out vec2 TexCoord;\n"
			"uniform float _time;\n"
			"uniform vec4 _color;\n"
			"uniform mat4 model;\n"
			"uniform mat4 viewproj;\n"
			"uniform mat4 view;\n"
			"void main()\n"
			"{\n"
			"gl_Position = viewproj *  model * vec4(position.x,position.y,position.z, 1.0f);\n"
			"ourColor = _color;\n"
			"TexCoord = texCoord;\n"
			"ourTime = _time;\n"
			"ourNormal = mat3(model) * normal;"
			"}\n"
		};

		//Set vertex source
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

		//Compile vertex source
		glCompileShader(vertexShader);

		//Check vertex shader for errors
		GLint vShaderCompiled = GL_FALSE;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
		if (vShaderCompiled != GL_TRUE)
		{
			//ShaderLog(vertexShader);
			return nullptr;
		}

		//Attach vertex shader to program
		glAttachShader(defaultShader->programID, vertexShader);

		//Create fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//Get fragment source
		const GLchar* fragmentShaderSource[] =
		{
			"#version 330 core\n"
			"in vec4 ourColor;\n"
			"in float ourTime;\n"
			"in vec2 TexCoord;\n"
			"in vec3 ourNormal;\n"
			"out vec4 color;\n"
			"uniform sampler2D _texture;\n"
			"void main()\n"
			"{\n"
			"vec3 lightDir = vec3(1);\n"
			"float angle = dot(lightDir, ourNormal);\n"
			"color = texture(_texture, TexCoord) * ourColor * vec4(angle,angle,angle,1);\n"
			"}\n"
		};

		//Set fragment source
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		//Compile fragment source
		glCompileShader(fragmentShader);

		//Check fragment shader for errors
		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{

			//ShaderLog(fragmentShader);
			return nullptr;
		}

		//Attach fragment shader to program
		glAttachShader(defaultShader->programID, fragmentShader);

		//Link program
		glLinkProgram(defaultShader->programID);

		//Check for errors
		glGetProgramiv(defaultShader->programID, GL_LINK_STATUS, &programSuccess);
		if (programSuccess != GL_TRUE)
		{
			//ProgramLog(default_shader.mProgramID);
			return nullptr;
		}

		Shader* newFragment = new Shader();
		newFragment->shaderID = fragmentShader;
		newFragment->shaderText = *fragmentShaderSource;
		newFragment->shaderType = ShaderType::fragment;
		newFragment->shaderPath = "";

		defaultShader->AddFragment(newFragment);

		Shader* newVertex = new Shader();
		newVertex->shaderID = vertexShader;
		newVertex->shaderText = *vertexShaderSource;
		newVertex->shaderType = ShaderType::vertex;
		newVertex->shaderPath = "";

		defaultShader->AddVertex(newVertex);
		
//SHOULD DO THIS IN THE ENGINE :(

		/*
		UUID obj_uuid = IID_NULL;
		UUID null_uuid; UuidCreateNil(&null_uuid);
		RPC_STATUS stat;

		//TODO probably dont need
		if (UuidCompare(&obj_uuid, &null_uuid, &stat) == 0) {
			Resource* res = App->resources->CreateNewResource(Resource::shader_program);
			res->file = "default.material";
			res->exported_file = "default.material";
			res->type = Resource::shader_program;
			obj_uuid = res->uuid;

		}
		*/

		programs.push_back(defaultShader);
		shaders.push_back(newFragment);
		shaders.push_back(newVertex);
		
		return defaultShader;

	}

	Shader* Functions::CompileShader(std::string path, ShaderType type)
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

		return newShader;

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

		/*GLenum error = glGetError();

		if (error != GL_NO_ERROR) {

			printf("Error binding shader! %s\n", gluErrorString(error));			
			return false;
		}*/
		return true;
	}

	void ShaderProgram::Unbind()
	{
		glUseProgram(NULL);
	}

	UniformVar ShaderProgram::GetVariableInfo(uint index)
	{
		UniformVar ret;
		int name_len = -1, num = -1;
		GLenum type = GL_ZERO;
		char name[100];
		glGetActiveUniform(programID, GLuint(index), sizeof(ret.name) - 1,
			&name_len, &num, &ret.type, ret.name);

		ret.name[name_len] = 0;
		
		return ret;
	}

	GLint ShaderProgram::GetVariablesSize() const
	{
		GLint total = 0;
		glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &total);
		return total;
	}
	
}