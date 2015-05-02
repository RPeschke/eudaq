#ifdef WIN32
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !_DEBUG
#endif // WIN32

#include <QApplication>
#include "euProd.hh"

int main(int argc, char ** argv) {
  QApplication app(argc, argv);
  QMainWindow *window = new ProducerGUI();
  window->show();
  return app.exec();
}
