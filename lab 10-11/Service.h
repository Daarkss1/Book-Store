#pragma once
#include "Repo.h"
#include "validators.h"
#include "Observable.h"
#include <vector>
#include <string>
#include <functional>
#include <memory>
using std::function;
using std::string;
using std::vector;

class ActiuneUndo {
public:

    virtual ~ActiuneUndo() {}
    virtual void doUndo() = 0;
};

class UndoAdauga : public ActiuneUndo {
private:
    CarteRepository& repo;
    int id;

public:
    UndoAdauga(CarteRepository& repo, int id) : repo(repo), id(id) {}

    void doUndo() override {
        repo.remove(repo.find(id));
    }
};

class UndoSterge : public ActiuneUndo {
private:
    CarteRepository& repo;
    Carte carteAdaugata;

public:
    UndoSterge(CarteRepository& repo, const Carte& carte) : repo(repo), carteAdaugata(carte) {}

    void doUndo() override {
        repo.store(carteAdaugata);
    }
};

class UndoModifica : public ActiuneUndo {
private:
    CarteRepository& repo;
    Carte carteModificata;

public:
    UndoModifica(CarteRepository& repo, const Carte& carte) : repo(repo), carteModificata(carte) {}

    void doUndo() override {
        repo.modify(carteModificata);
    }
};


class CarteStore : public Observable {
private:
    CarteRepository& repo;
    CarteValidator& validator;
    vector<ActiuneUndo*> undoActions;
    vector<Carte> cosInchirieri; // Vector pentru coșul de cumpărături

    vector<Carte> filter(function<bool(const Carte&)> fct);
    vector<Carte> generalSort(bool(*maiMicF)(const Carte&, const Carte&));

public:
    CarteStore(CarteRepository& repo, CarteValidator& validator) : repo{ repo }, validator(validator) {}
    CarteStore(const CarteStore& ot) = delete;

    void incarcaDateDinFisier(const string& numeFisier);
    void salveazaDateInFisier(const string& numeFisier);

    void adaugaCarte(int id, string titlu, string autor, string gen, int an);
    void stergeCarte(int id);
    void modificaCarte(int id, string titluNou, string autorNou, string genNou, int anNou);

    const vector<Carte> getToateCartile() {
        return this->repo.getAllCarti();
    }

    void undo();
    Carte cautaCarte(int id);
    Carte cautaCarteDupaTitlu(const string& titlu);
    vector<Carte> filtreazaDupaTitlu(string Titlu);
    vector<Carte> filtreazaDupaAn(int an);
    vector<Carte> sorteazaDupaTitlu();
    vector<Carte> sorteazaDupaAutor();
    vector<Carte> sorteazaDupaAnSiGen();

    void addToCos(const Carte& carte);
    void clearCos();
    const vector<Carte>& getCos() const;

    ~CarteStore();
};

void testeService();