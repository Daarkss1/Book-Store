#pragma once
#include "Domain.h"
#include "VectorDinamicTemplate.h"
#include <vector>
#include <algorithm> // Pentru std::find_if
using std::vector;

/*
Clasa de exceptii specifice Repo
*/
class RepoException {
private:
    string errorMsg;
public:
    RepoException(string errorMsg) :errorMsg{ errorMsg } {};
    string getErrorMessage() {
        return this->errorMsg;
    }
};

class CarteRepository {
private:
    VectorDinamic<Carte> allCarti;
public:
    CarteRepository() = default;
    //constructor de copiere
    //punem delete fiindca nu vrem sa se faca nicio copie la Repository
    //(in aplicatie avem 1 singur Repo)
    CarteRepository(const CarteRepository& ot) = delete;
    /*
    Adauga o carte in lista
    @param c: cartea care se adauga (Carte)
    @return -
    post: cartea va fi adaugata in lista

    @throws: RepoException daca o carte cu acelasi titlu si acelasi autor
             exista deja
    */
    void store(const Carte& c);

    /*
    Returneaza o lista cu toate cartile
    @return: lista cu cartile (vector of Carte objects)
    */
    const vector<Carte> getAllCarti();

    /*
    Cauta o carte cu titlul si autorul dati

    @param titlu: titlul dupa care se cauta
    @param autor: autorul dupa care se cauta
    @returns: entitatea Carte cu titlul si autorul dati (daca aceasta exista)
    @throws: RepoException daca nu exista carte cu titlul si autorul dati
    */
    const Carte& find(int id);

    /*
    Verifica daca o carte data exista in lista

    @param c: cartea care se cauta in lista
    @return: true daca cartea exista, false altfel
    */
    bool exists(const Carte& c);

    /*
    Sterge o carte din lista

    @param c: cartea care se doreste a fi stearsa
    @throws: RepoException daca cartea nu exista in lista
    */
    void remove(const Carte& c);

    void modify(const Carte& c);
};

void testeRepo();
