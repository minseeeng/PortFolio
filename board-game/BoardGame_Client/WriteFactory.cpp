#include "WriteFactory.h"

CWriteFactory::CWriteFactory()
	: m_writeFactory(nullptr), m_textFormat(nullptr)
{
}

CWriteFactory::~CWriteFactory()
{
	CleanUp();
}

bool CWriteFactory::Init()
{
	HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_writeFactory));
	if (FAILED(hr))
		return false;

	hr = m_writeFactory->CreateTextFormat(
		L"Malgun Gothic", NULL,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		20.0f, L"ko-kr", &m_textFormat);
	if (FAILED(hr))
		return false;

	m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	return true;
}

void CWriteFactory::CleanUp()
{
	if (m_textFormat) { m_textFormat->Release(); m_textFormat = nullptr; }
	if (m_writeFactory) { m_writeFactory->Release(); m_writeFactory = nullptr; }
}
