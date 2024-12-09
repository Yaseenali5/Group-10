#include <QApplication>
#include "ComplianceWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ComplianceWindow window;
    window.show();
    return app.exec();
}
