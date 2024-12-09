#include <QtWidgets>
#include <iostream>
#include "rawwindow.hpp"
#include "POPwindow.hpp"

int main(int argc, char* argv[])
{

  int n;
  std::cout << "Enter number (0 = raw data page, 1 = POP page): ";
  std::cin>>n;

  if (n == 0)
  {
    QApplication app(argc, argv);
    RawWindow window;
    window.show();
    return app.exec();

  }
  else if (n==1)
  {
    QApplication app(argc, argv);
    POPWindow window;
    window.show();
    return app.exec();    
  }

}
