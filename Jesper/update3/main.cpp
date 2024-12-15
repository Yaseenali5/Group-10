#include <QtWidgets>
#include <iostream>
#include "rawwindow.hpp"
#include "POPwindow.hpp"
#include "ComplianceWindow.hpp"
#include <QFont>

int main(int argc, char* argv[])
{
  QString StyleSheet = R"(
    QMainWindow {
        background-color: #434a5e; /* Slightly blue dark color */
        color: #e0e0e0;           /* Light grey text */
    }

    QMenuBar {
        background-color: #4b5165; 
        color: #e0e0e0;           /* Tetx */
        border: none;
    }

    QMenuBar::item {
        background-color: transparent;
        padding: 5px 10px;
    }

    QMenuBar::item:selected {
        background-color: #57607a; /* Hover */
    }

    QMenuBar::item:pressed {
        background-color: #68758d;
    }

    QToolBar {
        background-color: #4b5165; 
        border: none;
    }

    QToolBar QToolButton {
        background-color: #52596e;
        border: 1px solid #68758d; 
        border-radius: 4px;
        padding: 5px;
        color: #e0e0e0;           /* Text */
    }

    QToolBar QToolButton:hover {
        background-color: #57607a; 
    }

    QToolBar QToolButton:pressed {
        background-color: #68758d;
    }

    QMenu {
        background-color: #4b5165; /* Same as menu bar and toolbar */
        color: #e0e0e0;           /* Text */
        border: 1px solid #68758d;
    }

    QMenu::item:selected {
        background-color: #57607a; /* On hover */
    }

    QPushButton {
        background-color: #52596e; /* Same as toolbar buttons */
        color: #e0e0e0;           /* Text */
        border: 1px solid #68758d; /* Border */
        border-radius: 4px;
        padding: 8px 16px;
    }

    QPushButton:hover {
        background-color: #57607a;
    }

    QPushButton:pressed {
        background-color: #68758d;
    }

    QStatusBar {
        background-color: #4b5165; 
        color: #e0e0e0;           /* Light grey text */
        border-top: 1px solid #68758d; /* border */
    }

    QStatusBar::item {
        border: none;
        background: transparent;
    }

    QToolBar::separator {
        background-color: #68758d; 
        width: 2px;               /* Thickness */
        height: 4px;             /* Height */
        margin: 5px;              /* Spacing */
    }

    QLabel {
        color: #e0e0e0;
    }
)";



  
  int n;
  std::cout << "Enter number (0 = raw data page, 1 = POP page): ";
  std::cin>>n;

  if (n == 0)
  {
    QApplication app(argc, argv);
    app.setStyleSheet(StyleSheet);

    // Change app font
    QFont font;
    font.setFamily("Segoe UI");  // Preferred font
    font.setPointSize(10);       // Adjust size
    font.setWeight(QFont::Bold); // Slightly thicker (QFont::Medium is weight 500)

    app.setFont(font);
    
    QTranslator trn;
    if (trn.load(QLocale::system(), "adam", "_", ":/i18n")) {
      app.installTranslator(&trn);
    }
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
  else if (n==2)
  {
    QApplication app(argc, argv);
    ComplianceWindow window;
    window.show();
    return app.exec();    
  }

}

