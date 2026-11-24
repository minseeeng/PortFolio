#pragma once
#include "Sprite.h"

class CSpriteLoader
{
public:
	static CSprite* Load(const wchar_t* imagePath, const wchar_t* sprPath);

private:
	static bool LoadSpriteFile(PCWSTR path, wchar_t* outImageFile, int maxLen, std::vector<RECT>& outRects);
};

