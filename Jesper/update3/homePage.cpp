#include "homePage.hpp"

homePage::homePage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create buttons
    QPushButton *button1 = new QPushButton("Pollutant Overview", this);
    QPushButton *button2 = new QPushButton("Persistent Organic Pollutants (POPs)", this);
    QPushButton *button3 = new QPushButton("Environmental Litter Indicators", this);
    QPushButton *button4 = new QPushButton("Fluorinated Compounds", this);
    QPushButton *button5 = new QPushButton("Compliance Dashboard", this);

    QFont buttonFont;
    buttonFont.setPointSize(14); // Set desired text size

    QPushButton *buttons[] = {button1, button2, button3, button4, button5};
    for (QPushButton *button : buttons) {
        button->setFont(buttonFont);
        button->setStyleSheet("text-align: left;");
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(button);
        //button->setStyleSheet("padding-left: 10px;");
    }
    
    // Emit signal with corresponding ID when a button is clicked
    connect(button1, &QPushButton::clicked, this, [this]() { emit buttonClicked("Page1"); });
    connect(button2, &QPushButton::clicked, this, [this]() { emit buttonClicked("Page2"); });
    connect(button3, &QPushButton::clicked, this, [this]() { emit buttonClicked("Page3"); });
    connect(button4, &QPushButton::clicked, this, [this]() { emit buttonClicked("Page4"); });
    connect(button5, &QPushButton::clicked, this, [this]() { emit buttonClicked("Page5"); });
}
