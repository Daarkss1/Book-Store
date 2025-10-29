#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_lab1011.h"

class lab1011 : public QMainWindow
{
    Q_OBJECT

public:
    lab1011(QWidget *parent = nullptr);
    ~lab1011();

private:
    Ui::lab1011Class ui;
};
