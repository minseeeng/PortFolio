#include "WicFactory.h"

CWicFactory::CWicFactory()
	: m_wicFactory(nullptr)
{
}

CWicFactory::~CWicFactory()
{
	CleanUp();
}

bool CWicFactory::Init()
{
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL,
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_wicFactory));
	if (FAILED(hr))
		return false;

	return true;
}

void CWicFactory::CleanUp()
{
	if (m_wicFactory) { m_wicFactory->Release(); m_wicFactory = nullptr; }
}

bool CWicFactory::Load(ID2D1HwndRenderTarget& renderTarget, const wchar_t* path, ID2D1Bitmap** bitmap)
{
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	IWICFormatConverter* pConverter = nullptr;

	HRESULT hr = m_wicFactory->CreateDecoderFromFilename(
		path,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder);

	if (SUCCEEDED(hr))
		hr = pDecoder->GetFrame(0, &pFrame);

	if (SUCCEEDED(hr))
		hr = m_wicFactory->CreateFormatConverter(&pConverter);

	if (SUCCEEDED(hr))
		hr = pConverter->Initialize(pFrame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.0f,
			WICBitmapPaletteTypeCustom);

	if (SUCCEEDED(hr))
		hr = renderTarget.CreateBitmapFromWicBitmap(pConverter, NULL, bitmap);

	if (pConverter) { pConverter->Release(); pConverter = nullptr; }
	if (pFrame) { pFrame->Release(); pFrame = nullptr; }
	if (pDecoder) { pDecoder->Release(); pDecoder = nullptr; }

	return SUCCEEDED(hr);
}
