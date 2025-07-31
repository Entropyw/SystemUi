#include "MainWindow.h"
#include <QApplication>
#pragma comment(lib, "user32.lib")
#ifdef __cplusplus
extern "C"{
#include "EventDealer.h"
#endif
#ifdef __cplusplus
}
#endif
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    extern Contest Main;
    startContest(&Main);
    MainWindow w;
    w.resize(400, 400);
    w.show();
    return a.exec();
}