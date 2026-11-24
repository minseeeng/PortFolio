#include "ResourceManager.h"
#include "SpriteLoader.h"


CResourceManager::CResourceManager()
	: m_spriteCount(0),m_bitmapCount(0)
{
	for (int i = 0; i < SPRITE_MAX; i++)
		m_sprites[i] = nullptr;

	for (int i = 0; i < BITMAP_MAX; i++)
		m_bitmaps[i] = nullptr;
}

CResourceManager::~CResourceManager()
{
}


CSprite* CResourceManager::GetSprite(const wchar_t* imagePath, const wchar_t* sprPath)
{
	for (int i = 0; i < m_spriteCount; i++)
	{
		if (wcscmp(m_keys[i], imagePath) == 0)   
			return m_sprites[i];               
	}

	if (m_spriteCount >= SPRITE_MAX)
		return nullptr;

	CSprite* sprite = CSpriteLoader::Load(imagePath, sprPath);
	if (sprite == nullptr) 
		return nullptr;

	wcscpy_s(m_keys[m_spriteCount], MAX_PATH, imagePath);
	m_sprites[m_spriteCount] = sprite;
	m_spriteCount++;

	return sprite;
}

ID2D1Bitmap* CResourceManager::GetBitmap(const wchar_t* imagePath)
{
	if (m_bitmapCount >= BITMAP_MAX)
		return nullptr; 

	for (int i = 0; i < m_bitmapCount; i++)
	{
		if (wcscmp(m_bitmapKeys[i], imagePath) == 0)
			return m_bitmaps[i];
	}

	ID2D1Bitmap* bitmap = nullptr;
	if (GraphicsCore::GetInstance().LoadBitmapFromFile(imagePath, &bitmap) == false)
		return nullptr;                                 

	wcscpy_s(m_bitmapKeys[m_bitmapCount], MAX_PATH, imagePath);
	m_bitmaps[m_bitmapCount] = bitmap;
	m_bitmapCount++;                                   

	return bitmap;
}

void CResourceManager::Clear()
{
	for (int i = 0; i < m_spriteCount; i++)
	{
		// 이 sprite 를 지워
		if (m_sprites[i]) { delete m_sprites[i]; m_sprites[i] = nullptr; }
	}
	
	m_spriteCount = 0;

	for (int i = 0; i < m_bitmapCount; i++)
	{
		m_bitmaps[i]->Release();
		m_bitmaps[i] = nullptr;
	}

	m_bitmapCount = 0;
}



