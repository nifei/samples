#ifdef XLUEEXTOBJECT_EXPORTS
#define XLUEEXTOBJECT_API __declspec(dllexport)
#else
#define XLUEEXTOBJECT_API __declspec(dllimport)
#endif

#include <XLUE.h>