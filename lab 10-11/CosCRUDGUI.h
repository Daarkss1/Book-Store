#pragma once
#include "Service.h"
#include "Observer.h"
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class CosCRUDGUI : public QWidget, public Observer {
private:
    CarteStore& srv;
    QTableWidget* tableCos;
    QPushButton* btnClearCos;
    QPushButton* btnGenerateCos;

    void initializeGUIComponents() {
        QVBoxLayout* mainLayout = new QVBoxLayout{ this };
        tableCos = new QTableWidget{ 0, 5 };
        QStringList headers{ "Id", "Titlu", "Autor", "Gen", "AnulAparitiei" };
        tableCos->setHorizontalHeaderLabels(headers);
        mainLayout->addWidget(tableCos);

        btnClearCos = new QPushButton{ "Clear Cos" };
        btnGenerateCos = new QPushButton{ "Generate Cos" };
        mainLayout->addWidget(btnClearCos);
        mainLayout->addWidget(btnGenerateCos);
    }

    void connectSignalsSlots() {
        QObject::connect(btnClearCos, &QPushButton::clicked, [&]() {
            srv.clearCos();
            });

        QObject::connect(btnGenerateCos, &QPushButton::clicked, [&]() {
            srv.clearCos();
            auto allBooks = srv.getToateCartile();
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(allBooks.begin(), allBooks.end(), std::default_random_engine(seed));
            int numBooks = 5; // You can choose a different number or add a text input to let the user choose
            for (int i = 0; i < numBooks && i < allBooks.size(); i++) {
                srv.addToCos(allBooks[i]);
            }
            });
    }

    void reloadCosList() {
        tableCos->setRowCount(srv.getCos().size());
        int row = 0;
        for (const auto& carte : srv.getCos()) {
            tableCos->setItem(row, 0, new QTableWidgetItem(QString::number(carte.getId())));
            tableCos->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(carte.getTitlu())));
            tableCos->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(carte.getAutor())));
            tableCos->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(carte.getGen())));
            tableCos->setItem(row, 4, new QTableWidgetItem(QString::number(carte.getAnulAparitiei())));
            row++;
        }
    }

public:
    CosCRUDGUI(CarteStore& srv) : srv{ srv } {
        initializeGUIComponents();
        connectSignalsSlots();
        srv.addObserver(this);
        reloadCosList();
    }

    void update() override {
        reloadCosList();
    }

    ~CosCRUDGUI() {
        srv.removeObserver(this);
    }
};
