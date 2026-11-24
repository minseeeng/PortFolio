#include "BoardGameApp.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	CBoardGameApp app;

    if ((app.Init(hInstance, L"장기", WINDOW_WIDTH, WINDOW_HEIGHT)) == false)
    {
        app.End();
        return 0;
    }

    app.Run();
    app.End();

	return 0;
}
