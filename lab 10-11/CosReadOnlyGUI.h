#pragma once
#include "Service.h"
#include "Observer.h"
#include <QWidget>
#include <QPainter>
#include <cstdlib>

class CosReadOnlyGUI : public QWidget, public Observer {
private:
    CarteStore& srv;

public:
    CosReadOnlyGUI(CarteStore& srv) : srv{ srv } {
        srv.addObserver(this);
        setMinimumSize(400, 300);
    }

    void update() override {
        repaint();
    }

    void paintEvent(QPaintEvent* ev) override {
        QPainter p{ this };
        auto cos = srv.getCos();
        int width = this->width();
        int height = this->height();

        for (size_t i = 0; i < cos.size(); ++i) {
            int x = rand() % width;
            int y = rand() % height;
            p.drawEllipse(x, y, 20, 20);
        }
    }

    ~CosReadOnlyGUI() {
        srv.removeObserver(this);
    }
};
