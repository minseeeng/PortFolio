#include "SpriteLoader.h"

CSprite* CSpriteLoader::Load(const wchar_t* imagePath, const wchar_t* sprPath)
{
    ID2D1Bitmap* bitmap = nullptr;
    if (GraphicsCore::GetInstance().LoadBitmapFromFile(imagePath, &bitmap) == false)
        return nullptr;

    wchar_t buffer[MAX_PATH];
    std::vector<RECT> rects;
    if (LoadSpriteFile(sprPath, buffer, MAX_PATH, rects) == false)
    {
        bitmap->Release();      
        return nullptr;
    }

    return new CSprite(bitmap, rects);
}

bool CSpriteLoader::LoadSpriteFile(PCWSTR path, wchar_t* outImageFile, int maxLen, std::vector<RECT>& outRects)
{
    FILE* fp_read = nullptr;
    _wfopen_s(&fp_read, path, L"rb");
    if (fp_read == NULL)
        return false;

    int nameLen = 0;
    fread(&nameLen, sizeof(int), 1, fp_read);
    if (nameLen < 0 || nameLen >= maxLen)
    {
        fclose(fp_read);
        return false;
    }
    fread(outImageFile, sizeof(wchar_t), nameLen, fp_read);
    outImageFile[nameLen] = L'\0';

    int size = 0;
    fread(&size, sizeof(int), 1, fp_read);

    outRects.resize(size);
    fread(outRects.data(), sizeof(RECT), size, fp_read);

    fclose(fp_read);
    return true;
}
