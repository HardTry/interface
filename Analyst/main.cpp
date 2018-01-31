#include "mainwindow.h"
#include <QApplication>


#include "utility.h"
#include "ctpif.h"

int main(int argc, char *argv[])
{
  GmdParam* param = nullptr;
  int ret = 0;
  if (argc == 1)
    ret = (nullptr == (param = initConfig("analyst.json")));
  else if (argc == 2)
    ret = (nullptr == (param = initConfig(argv[1])));
  else
    ret = 1;

  if (ret)
    std::cout << "Usage: Analyst [config filepath]\n";
  else {
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(analyst);
    MainWindow w;
    w.show();

    ret = a.exec();
  }

  releaseConfig(param);
  return ret;
}
