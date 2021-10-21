//---------------------------------------------------------------------------

#ifndef ImportZEH
#define ImportZEH
//---------------------------------------------------------------------------
#include <classes.hpp>

extern "C" _pascal __declspec(dllexport) int neg( LPSTR path );
extern "C" _pascal __declspec(dllexport) int averange( LPSTR path );
extern "C" _pascal __declspec(dllexport) int import_example( LPSTR path );
//---------------------------------------------------------------------------
#endif
