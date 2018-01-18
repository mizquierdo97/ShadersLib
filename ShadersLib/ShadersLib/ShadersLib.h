// MathLibrary.h - Contains declaration of Function class  
#pragma once  

#ifdef SHADERS_EXPORTS  
#define SHADERS_API __declspec(dllexport)   
#else  
#define SHADERS_API __declspec(dllimport)   
#endif  


#include <string>
#include "Glew\include\glew.h"

#pragma comment (lib, "Glew/libx86/glew32.lib") 
typedef unsigned int uint;

namespace ShaderLib {
	class Shader {
	public:
		Shader() {

		}
		~Shader() {

		}

		SHADERS_API void AddFragment(int ID);
		SHADERS_API void AddVertex(int ID);
		SHADERS_API void AddGeometry(int ID);

		SHADERS_API bool LoadProgram();

	public:
		char* name = nullptr;

		int programID = 0;

		int fragmentID = 0;
		int vertexID = 0;
		int geometryID = 0;

		std::string fragment_path;
		std::string vertex_path;
		std::string geometry_path;


		std::string fragment_text = "";
		std::string vertext_text = "";
		std::string geometry_text = "";
	};


	// This class is exported from the MathLibrary.dll  
	
	enum ShaderType {
		fragment,
		geometry,
		vertex,
	};

	class Functions
	{
	public:

		static SHADERS_API Shader CreateShader(char* name);

		static SHADERS_API uint CompileShader(std::string path, ShaderType type);
		// Returns a + b  
		static SHADERS_API double Add(double a, double b);

		// Returns a * b  
		static SHADERS_API double Multiply(double a, double b);

		// Returns a + (a * b)  
		static SHADERS_API double AddMultiply(double a, double b);
	};



}

