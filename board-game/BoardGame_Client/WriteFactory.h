#pragma once
#include <dwrite.h>

#pragma comment(lib,"dwrite.lib")
class CWriteFactory
{
public:
	CWriteFactory();
	~CWriteFactory();

	bool Init();
	void CleanUp();

	IDWriteTextFormat* GetTextFormat() { return m_textFormat; }

private:
	IDWriteFactory* m_writeFactory;
	IDWriteTextFormat* m_textFormat;
};
