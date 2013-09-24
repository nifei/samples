#ifndef __GAUSSIANBLUROBJECT_H__
#define __GAUSSIANBLUROBJECT_H__


class GaussianBlurObject
	: public ExtLayoutObjMethodsImpl
{
public:
public:

	GaussianBlurObject(XLUE_LAYOUTOBJ_HANDLE hObj);
	virtual ~GaussianBlurObject(void);

public:

	// ExtLayoutObjMethodsImpl virtual funtions

	// 该对象是剪裁敏感的，所以需要配合LayerObject+clipsens使用
	virtual void OnPaint(XL_BITMAP_HANDLE hBitmapDest, const RECT* lpDestClipRect, const RECT* lpSrcClipRect, unsigned char alpha);

	void SetSigma(double sigma) {m_sigma = sigma;}
	double GetSigma()const {return m_sigma;}

	void SetRadius(int radius) {m_radius = radius;}
	int GetRadius()const {return m_radius;}

private:
	void Simple(XL_BITMAP_HANDLE hBitmap)const;
	void OneDimentionRender(XL_BITMAP_HANDLE hBitmap)const;
	inline unsigned int getR(const unsigned long & buffer)const
	{
		return buffer<<8>>24;
	}
	inline unsigned int getG(const unsigned long & buffer)const
	{
		return buffer<<16>>24;
	}
	inline unsigned int getB(const unsigned long & buffer)const
	{
		return buffer<<24>>24;
	}
	inline unsigned int getA(const unsigned long & buffer)const
	{
		return buffer>>24;
	}
private:
	double m_sigma;
	int m_radius;
	enum Type {
		OneDimention = 1, 
		TwoDimention = 2
	};
	Type m_type;
};

#endif // __GAUSSIANBLUROBJECT_H__