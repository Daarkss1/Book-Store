#pragma once
#include "Service.h"
#include <stack>

// Forward declaration pentru a evita dependențele ciclice
class CarteStore;

// Clasa de interfață pentru UI
class ConsoleUI {
private:
    CarteStore& srv;
    vector<Carte> cosInchirieri;
    vector<ActiuneUndo*> undoActions;

public:
    ConsoleUI(CarteStore& srv) : srv{ srv } {};
    ConsoleUI(const CarteStore& ot) = delete;

    void menu();
    void uiAdd();
    void uiModify();
    void uiDelete();
    void uiPrintAll(const vector<Carte>& allCarte);
    void uiFind();
    void uiFilter();
    void uiSort();
    void addOnLoad();
    void run();
    void afiseazaNumarCartiInCos();
    void genereazaCos();
    void adaugaInCos();
    void golesteCos();
    void exportaCos();
    void undo();
    void exportaCarti();

    // Funcții pentru gestionarea acțiunilor de undo
    void adaugaActiuneUndo(ActiuneUndo* actiuneUndo);
    void stergeActiuneUndo();
};

// Clasa pentru adăugarea cărții în magazin (acțiune de undo)
class ActiuneUndoAdauga : public ActiuneUndo {
private:
    CarteStore& srv;
    Carte carteAdaugata;
public:
    ActiuneUndoAdauga(CarteStore& srv, const Carte& carteAdaugata) : srv{ srv }, carteAdaugata{ carteAdaugata } {}
    void doUndo() override {
        srv.stergeCarte(carteAdaugata.getId());
    }
};

// Clasa pentru ștergerea cărții din magazin (acțiune de undo)
class ActiuneUndoSterge : public ActiuneUndo {
private:
    CarteStore& srv;
    Carte carteStearsa;
public:
    ActiuneUndoSterge(CarteStore& srv, const Carte& carteStearsa) : srv{ srv }, carteStearsa{ carteStearsa } {}
    void doUndo() override {
        srv.adaugaCarte(carteStearsa.getId(), carteStearsa.getTitlu(), carteStearsa.getAutor(), carteStearsa.getGen(), carteStearsa.getAnulAparitiei());
    }
};

// Clasa pentru modificarea unei cărți în magazin (acțiune de undo)
class ActiuneUndoModifica : public ActiuneUndo {
private:
    CarteStore& srv;
    Carte carteModificata;
public:
    ActiuneUndoModifica(CarteStore& srv, const Carte& carteModificata) : srv{ srv }, carteModificata{ carteModificata } {}
    void doUndo() override {
        srv.modificaCarte(carteModificata.getId(), carteModificata.getTitlu(), carteModificata.getAutor(), carteModificata.getGen(), carteModificata.getAnulAparitiei());
    }
};
