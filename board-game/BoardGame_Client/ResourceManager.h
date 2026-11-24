#pragma once
#include "Sprite.h"

#define SPRITE_MAX 32
#define BITMAP_MAX 32

class CResourceManager
{
public:
	static CResourceManager& GetInstance()
	{
		static CResourceManager instance;
		return instance;
	}

private:
	CResourceManager();
	CResourceManager(const CResourceManager&) = delete;
	CResourceManager& operator=(const CResourceManager&) = delete;
	~CResourceManager();

public:
	CSprite* GetSprite(const wchar_t* imagePath, const wchar_t* sprPath);
	ID2D1Bitmap* GetBitmap(const wchar_t* imagePath);
	void Clear();

private:
	wchar_t m_keys[SPRITE_MAX][MAX_PATH];
	CSprite* m_sprites[SPRITE_MAX];
	int m_spriteCount;

	wchar_t m_bitmapKeys[BITMAP_MAX][MAX_PATH];
	ID2D1Bitmap* m_bitmaps[BITMAP_MAX];
	int m_bitmapCount;
};

