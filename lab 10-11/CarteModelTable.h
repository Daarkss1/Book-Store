#pragma once

#include <QAbstractTableModel>
#include "Service.h"

class CarteTableModel : public QAbstractTableModel {
    Q_OBJECT

private:
    vector<Carte> carti;

public:
    CarteTableModel(const vector<Carte>& carti) : carti{ carti } {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return static_cast<int>(carti.size());
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        return 5;
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole) {
            const auto& carte = carti[index.row()];
            switch (index.column()) {
            case 0: return QString::number(carte.getId());
            case 1: return QString::fromStdString(carte.getTitlu());
            case 2: return QString::fromStdString(carte.getAutor());
            case 3: return QString::fromStdString(carte.getGen());
            case 4: return QString::number(carte.getAnulAparitiei());
            }
        }
        return {};
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole) {
            if (orientation == Qt::Horizontal) {
                switch (section) {
                case 0: return "Id";
                case 1: return "Titlu";
                case 2: return "Autor";
                case 3: return "Gen";
                case 4: return "AnulAparitiei";
                }
            }
        }
        return {};
    }

    void setCarti(const vector<Carte>& carti) {
        this->carti = carti;
        emit layoutChanged();
    }

    const Carte& getCarte(int row) const {
        return carti.at(row);
    }
};
