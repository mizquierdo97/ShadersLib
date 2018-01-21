// MathLibrary.h - Contains declaration of Function class  
#pragma once  

#ifdef SHADERS_EXPORTS  
#define SHADERS_API __declspec(dllexport)   
#else  
#define SHADERS_API __declspec(dllimport)   
#endif  


#include <string>
#include <list>
#include "Glew\include\glew.h"

#pragma comment (lib, "Glew/libx86/glew32.lib") 
typedef unsigned int uint;

namespace ShaderLib {

	enum ShaderType {
		nullShader,
		fragment,
		geometry,
		vertex,
	};


	class Shader {
	public:
		Shader() {

		}
		~Shader() {

		}
	public:
		uint shaderID = 0;

		std::string shaderPath = "";

		std::string shaderText = "";

		ShaderType shaderType = ShaderType::nullShader;

	};


	class ShaderProgram {
	public:
		ShaderProgram() {

		}
		~ShaderProgram() {

		}

		SHADERS_API void AddFragment(Shader* fragment);
		SHADERS_API void AddVertex(Shader* vertex);
		SHADERS_API void AddGeometry(Shader* geometry);

		SHADERS_API bool LoadProgram();

		SHADERS_API bool Bind();

		SHADERS_API void Unbind();



	public:
		char* name = nullptr;

		uint programID = 0;

		Shader* fragment = nullptr;
		Shader* vertex = nullptr;
		Shader* geometry = nullptr;

	};

	// This class is exported from the MathLibrary.dll  
	

	class Functions
	{
	public:

		
		 SHADERS_API ShaderProgram* CreateShader(char* name);

		SHADERS_API uint CompileShader(std::string path, ShaderType type);
		// Returns a + b  
		 SHADERS_API void AddShaderList(Shader* newShader);
		

	private:
		 std::list<Shader*> shaders;
		 std::list<ShaderProgram*> programs;
	};


	


}

