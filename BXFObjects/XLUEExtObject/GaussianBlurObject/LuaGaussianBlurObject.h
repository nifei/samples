#ifndef __LUAGAUSSIANBLUROBJECT_H__
#define __LUAGAUSSIANBLUROBJECT_H__

#include "./GaussianBlurObject.h"

class LuaGaussianBlurObject
	: public ExtObjLuaHostImplEx<LuaGaussianBlurObject, GaussianBlurObject>
{
public:
	LuaGaussianBlurObject(void);
	virtual ~LuaGaussianBlurObject(void);

private:
public:

	static const XLLRTGlobalAPI s_szLuaMemberFuncs[];
};

#endif // __LUAMIRROROBJECT_H__