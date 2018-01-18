// MathLibrary.h - Contains declaration of Function class  
#pragma once  

#ifdef SHADERS_EXPORTS  
#define SHADERS_API __declspec(dllexport)   
#else  
#define SHADERS_API __declspec(dllimport)   
#endif  

namespace ShadersLibrary
{
	// This class is exported from the MathLibrary.dll  
	class Functions
	{
	public:
		// Returns a + b  
		static SHADERS_API double Add(double a, double b);

		// Returns a * b  
		static SHADERS_API double Multiply(double a, double b);

		// Returns a + (a * b)  
		static SHADERS_API double AddMultiply(double a, double b);
	};
}

