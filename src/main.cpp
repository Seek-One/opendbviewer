// Enable memory leak detection
#if defined(_MSC_VER) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
// Must be done after pre-build lirary
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _DEBUG

#include <QApplication>
#include <QPushButton>

#include "GUI/QWindowMain.h"

#include "GUIController/QWindowMainController.h"

int main(int argc, char *argv[])
{
#if defined(_MSC_VER) && defined(_DEBUG)
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    QApplication a( argc, argv );

    QWindowMain window;

    QWindowMainController controller;
    controller.init(&window);

    window.show();

    return a.exec();
}
