#include <QtWidgets>
#include <iostream>
#include "rawwindow.hpp"
#include "POPwindow.hpp"
#include "PollutantOverviewWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    int n;
    std::cout << "Enter number (0 = raw data page, 1 = pollutant overview page, 2 = POP page): ";
    std::cin >> n;

    if (n == 0) {
        RawWindow window;
        window.show();
        return app.exec();
    } else if (n == 1) {
        PollutantOverviewWindow window;
        window.show();
        return app.exec();
    } else if (n == 2) {
        POPWindow window;
        window.show();
        return app.exec();
    } else {
        std::cerr << "Invalid input!" << std::endl;
        return 1; // Return an error code
    }
}
