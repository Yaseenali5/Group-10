#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class homePage : public QWidget {
    Q_OBJECT

public:
    explicit homePage(QWidget *parent = nullptr);

signals:
    void buttonClicked(const QString& page); // Signal to pass button ID
};


