#ifndef INC_SFX_H
#define INC_SFX_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <complex>
#include <valarray>
#include <vector>

// Functions and TypeDefs for Plugins to use
// Using complex objects because it makes the maths/functions easier to read
typedef std::complex <double>  Complex;
// Using valarray for the ability to use "splice" and "apply" to alter the 
// whole array without using loops
typedef std::valarray<Complex> ComplexArray;

// Declare BUILD_SFX to make the Plugin DLL
#ifdef BUILD_SFX
#define SFXAPI __declspec(dllexport)
#else
#define SFXAPI __declspec(dllimport)
#endif

// Define EXTERN_C for easier reading
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C 
#endif

// If C++ then use the class
#ifdef __cplusplus

class ISfx
{
public:
	virtual void		Release() = 0;
	virtual const char* GetName() = 0;
	virtual void		DoSFX(ComplexArray& arr) = 0;
};

// Define the interface to be used by the factory function
typedef ISfx* SFXHANDLE;

#else

// Define an empty struct in order to ensure type safety
typedef struct tagSFXHANDLE {} *SFXHANDLE;

#endif

// Factory function that creates instances of the Sfx object.
EXTERN_C SFXAPI SFXHANDLE __stdcall GetSfx(void);

#endif