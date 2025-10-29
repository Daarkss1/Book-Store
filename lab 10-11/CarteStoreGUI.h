#pragma once

#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <QtWidgets/QApplication>
#include <QLabel>
#include <QPushButton>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QTableView>
#include <QMessageBox>
#include <QHeaderView>
#include <QGroupBox>
#include <QRadioButton>
#include <QMainWindow>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include "Service.h"
#include "CosCRUDGUI.h"
#include "CosReadOnlyGUI.h"
#include "CarteModelTable.h"

using std::vector;
using std::string;

class CarteStoreGUI : public QWidget {
protected:
    void closeEvent(QCloseEvent* event) override;

private:
    CarteStore& srv;  // Referință la serviciul care gestionează logica de afaceri

    // Modelul pentru tabelul de cărți
    CarteTableModel* modelCarti;

    // Etichete pentru detalii carte
    QLabel* lblId = new QLabel{ "Id-ul cartii:" }; // Eticheta pentru ID-ul cărții
    QLabel* lblTitlu = new QLabel{ "Titlul cartii:" }; // Eticheta pentru titlul cărții
    QLabel* lblAutor = new QLabel{ "Autorul cartii:" }; // Eticheta pentru autorul cărții
    QLabel* lblGen = new QLabel{ "Genul cartii:" }; // Eticheta pentru genul cărții
    QLabel* lblAnulAparitiei = new QLabel{ "Anul aparitiei cartii:" }; // Eticheta pentru anul apariției cărții

    // Campuri de text pentru introducere detalii carte
    QLineEdit* editId; // Câmp pentru introducerea ID-ului
    QLineEdit* editTitlu; // Câmp pentru introducerea titlului
    QLineEdit* editAutor; // Câmp pentru introducerea autorului
    QLineEdit* editGen; // Câmp pentru introducerea genului
    QLineEdit* editAnulAparitiei; // Câmp pentru introducerea anului apariției

    // Buton pentru adaugarea unei carti
    QPushButton* btnAddCarte; // Buton pentru adăugarea unei noi cărți
    QPushButton* btnRemoveCarte;
    QPushButton* btnModifyCarte;

    QGroupBox* groupBox1 = new QGroupBox(tr("Tip sortare")); // Grup pentru opțiunile de sortare

    QRadioButton* radioSrtTitlu = new QRadioButton(QString::fromStdString("Titlu")); // Radio button pentru sortare după titlu
    QRadioButton* radioSrtAutor = new QRadioButton(QString::fromStdString("Autor")); // Radio button pentru sortare după autor
    QRadioButton* radioSrtAnGen = new QRadioButton(QString::fromStdString("An+Gen")); // Radio button pentru sortare după an și gen

    // Grup pentru filtrare
    QGroupBox* groupBox2 = new QGroupBox(tr("Tip filtrare")); // Grup pentru opțiunile de filtrare

    // Radio buttons și QLineEdit-uri pentru filtrare
    QRadioButton* radioFilTitlu = new QRadioButton(QString::fromStdString("Titlu")); // Radio button pentru filtrare după titlu
    QLineEdit* editFilTitluVal; // Câmp pentru valoarea filtrului de titlu
    QRadioButton* radioFilAn = new QRadioButton(QString::fromStdString("Anul aparitiei")); // Radio button pentru filtrare după anul apariției
    QLineEdit* editFilAnVal; // Câmp pentru valoarea filtrului de an

    // Buton pentru filtrare
    QPushButton* btnFilterCartes; // Buton pentru aplicarea filtrului

    // Buton pentru coș
    QPushButton* btnOpenNewWindow;

    // Buton pentru reincarcarea datelor
    QPushButton* btnReloadData; // Buton pentru reîncărcarea datelor din tabel

    QPushButton* btnUndo;

    // Tabel pentru afisarea cartilor
    QTableView* tableCartes; // Tabel pentru afișarea cărților

    // Butoane pentru deschiderea ferestrelor coșului
    QPushButton* btnOpenCosCRUD;
    QPushButton* btnOpenCosReadOnly;

    void initializeGUIComponents(); // Inițializarea componentelor GUI
    void connectSignalsSlots(); // Conectarea semnalelor cu sloturile
    void reloadCarteList(const vector<Carte>& Cartes); // Funcție pentru reîncărcarea listei de cărți
    void generateRandomBooks(int num);
    void addBookToCos(int bookId);
    void clearCos();
    void exportCos(const QString& fileName, const QString& fileType);
    void guiUndo();
public:
    CarteStoreGUI(CarteStore& CarteSrv); // Constructor
    void guiAddCarte(); // Metoda GUI pentru adăugarea unei cărți
    void guiRemoveCarte();
    void guiModifyCarte();
public slots:
    void openCos();
};
