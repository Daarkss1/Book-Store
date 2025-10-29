#pragma once
#include "UI.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
using namespace std;
void ConsoleUI::menu() {
    cout << "Comenzi disponibile:" << endl;
    cout << "1. Afisare carti" << endl;
    cout << "2. Adauga carte" << endl;
    cout << "3. Sterge carte" << endl;
    cout << "4. Modifica carte" << endl;
    cout << "5. Gaseste carte" << endl;
    cout << "6. Filtrare" << endl;
    cout << "7. Sorate" << endl;
    cout << "8. Genereaza cos" << endl;
    cout << "9. Adauga in cos" << endl;
    cout << "10. Goleste cos" << endl;
    cout << "11. Exporta cos" << endl;
    cout << "12. Undo" << endl;
    cout << "0. Exit" << endl;
}

void ConsoleUI::uiPrintAll(const vector<Carte>& allCarti) {
    if (allCarti.empty()) {
        cout << "Nu exista carti." << endl;
    }
    else {
        for (const auto& carte : allCarti) {
            cout << "ID: " << carte.getId() << " | Titlu: " << carte.getTitlu() << " | Autor: " << carte.getAutor() << " | Gen: " << carte.getGen() << " | An: " << carte.getAnulAparitiei() << endl;
        }
    }
}

void ConsoleUI::uiAdd() {
    string titlu, autor, gen;
    int id, an;

    cout << "Introduceti id-ul cartii:";
    cin >> id;

    cout << "Introduceti titlul cartii:";
    getline(cin >> ws, titlu);

    cout << "Introduceti autorul cartii:";
    getline(cin >> ws, autor);

    cout << "Introduceti genul cartii:";
    getline(cin >> ws, gen);

    cout << "Introduceti anul aparitiei:";
    cin >> an;

    try {
        // Adaugarea cărții în magazin
        srv.adaugaCarte(id, titlu, autor, gen, an);

        // Adăugarea unei acțiuni de undo pentru adăugarea cărții
        ActiuneUndo* actiuneUndo = new ActiuneUndoAdauga(srv, srv.cautaCarte(id));
        adaugaActiuneUndo(actiuneUndo);

        cout << "Cartea a fost adaugata cu succes!" << endl;
    }
    catch (RepoException& re) {
        cout << re.getErrorMessage();
    }
    catch (ValidationException& ve) {
        cout << "Cartea nu este valida!" << endl;
        cout << ve.getErrorMessages();
    }
}

void ConsoleUI::uiDelete() {
    int id;
    cout << "Introduceti id-ul cartii pe care doriti sa o stergeti:";
    cin >> id;

    try {
        // Salvarea cărții care urmează să fie ștearsă pentru a o putea restaura în caz de undo
        Carte carteStearsa = srv.cautaCarte(id);

        // Ștergerea cărții din magazin
        srv.stergeCarte(id);

        // Adăugarea unei acțiuni de undo pentru ștergerea cărții
        ActiuneUndo* actiuneUndo = new ActiuneUndoSterge(srv, carteStearsa);
        adaugaActiuneUndo(actiuneUndo);

        cout << "Cartea a fost stearsa cu succes!" << endl;
    }
    catch (RepoException& re) {
        cout << re.getErrorMessage();
    }
}

void ConsoleUI::uiModify() {
    int id;
    cout << "Introduceti id-ul cartii pe care doriti sa o modificati:";
    cin >> id;

    string titlu, autor, gen;
    int an;
    cout << "Introduceti noul titlu al cartii:";
    getline(cin >> ws, titlu);

    cout << "Introduceti noul autor al cartii:";
    getline(cin >> ws, autor);

    cout << "Introduceti noul gen al cartii:";
    getline(cin >> ws, gen);

    cout << "Introduceti noul an al aparitiei cartii:";
    cin >> an;

    try {
        // Salvarea cărții care urmează să fie modificată pentru a o putea restaura în caz de undo
        Carte carteModificata = srv.cautaCarte(id);

        // Modificarea cărții din magazin
        srv.modificaCarte(id, titlu, autor, gen, an);

        // Adăugarea unei acțiuni de undo pentru modificarea cărții
        ActiuneUndo* actiuneUndo = new ActiuneUndoModifica(srv, carteModificata);
        adaugaActiuneUndo(actiuneUndo);

        cout << "Cartea a fost modificata cu succes!" << endl;
    }
    catch (RepoException& re) {
        cout << re.getErrorMessage();
    }
    catch (ValidationException& ve) {
        cout << "Datele introduse nu sunt valide!" << endl;
        cout << ve.getErrorMessages();
    }
}

void ConsoleUI::uiFind() {
    int id;
    cout << "Introduceti id-ul cartii pe care doriti sa o gasiti:";
    cin >> id;

    try {
        Carte carteGasita = srv.cautaCarte(id);
        cout << "Cartea a fost gasita:" << endl;
        cout << "ID: " << carteGasita.getId() << endl;
        cout << "Titlu: " << carteGasita.getTitlu() << endl;
        cout << "Autor: " << carteGasita.getAutor() << endl;
        cout << "Gen: " << carteGasita.getGen() << endl;
        cout << "An aparitie: " << carteGasita.getAnulAparitiei() << endl;
    }
    catch (RepoException& re) {
        cout << re.getErrorMessage();
    }
}

void ConsoleUI::uiFilter() {
    int criteriu;
    cout << "Alegeti criteriul de filtrare:" << endl;
    cout << "1. Filtrare dupa titlu" << endl;
    cout << "2. Filtrare dupa anul aparitiei" << endl;
    cin >> criteriu;

    string valoare;
    cout << "Introduceti valoarea pentru filtrare:";
    getline(cin >> ws, valoare);

    try {
        vector<Carte> rezultatFiltrare;
        switch (criteriu) {
        case 1:
            rezultatFiltrare = srv.filtreazaDupaTitlu(valoare);
            break;
        case 2:
            rezultatFiltrare = srv.filtreazaDupaAn(stoi(valoare));
            break;
        default:
            cout << "Criteriu de filtrare invalid!";
            return;
        }

        cout << "Rezultatul filtrarii:" << endl;
        for (const auto& carte : rezultatFiltrare) {
            cout << "ID: " << carte.getId() << " | Titlu: " << carte.getTitlu() << " | Autor: " << carte.getAutor() << " | Gen: " << carte.getGen() << " | An aparitie: " << carte.getAnulAparitiei() << endl;
        }
    }
    catch (RepoException& re) {
        cout << re.getErrorMessage();
    }
}

void ConsoleUI::uiSort() {
    int criteriu;
    cout << "Alegeti criteriul de sortare:" << endl;
    cout << "1. Sortare dupa titlu" << endl;
    cout << "2. Sortare dupa autor" << endl;
    cout << "3. Sortare dupa an si gen" << endl;
    cin >> criteriu;

    try {
        vector<Carte> rezultatSortare;
        switch (criteriu) {
        case 1:
            rezultatSortare = srv.sorteazaDupaTitlu();
            break;
        case 2:
            rezultatSortare = srv.sorteazaDupaAutor();
            break;
        case 3:
            rezultatSortare = srv.sorteazaDupaAnSiGen();
            break;
        default:
            cout << "Criteriu de sortare invalid!";
            return;
        }

        cout << "Rezultatul sortarii:" << endl;
        for (const auto& carte : rezultatSortare) {
            cout << "ID: " << carte.getId() << " | Titlu: " << carte.getTitlu() << " | Autor: " << carte.getAutor() << " | Gen: " << carte.getGen() << " | An aparitie: " << carte.getAnulAparitiei() << endl;
        }
    }
    catch (RepoException& re) {
        cout << re.getErrorMessage();
    }
}

void ConsoleUI::afiseazaNumarCartiInCos() {
    cout << "Numar total de carti in cos: " << cosInchirieri.size() << endl;
}

void ConsoleUI::genereazaCos() {
    int numarTotalCarti;
    cout << "Introduceti numarul total de carti pe care doriti sa le adaugati in cos: ";
    cin >> numarTotalCarti;

    try {
        cosInchirieri.clear(); // Golește coșul existent
        vector<Carte> toateCartile = srv.getToateCartile();
        srand(time(NULL)); // Inițializează generatorul de numere aleatoare

        for (int i = 0; i < numarTotalCarti; ++i) {
            int indexAleatoriu = rand() % toateCartile.size();
            cosInchirieri.push_back(toateCartile[indexAleatoriu]);
        }

        cout << "Cosul a fost generat cu succes!" << endl;
        afiseazaNumarCartiInCos();
    }
    catch (RepoException& re) {
        cout << re.getErrorMessage();
    }
}

void ConsoleUI::adaugaInCos() {
    string titlu;
    cout << "Introduceti titlul cartii pe care doriti sa o adaugati in cos: ";
    getline(cin >> ws, titlu);

    try {
        Carte carte = srv.cautaCarteDupaTitlu(titlu);
        cosInchirieri.push_back(carte);
        cout << "Cartea a fost adaugata in cos!" << endl;
        afiseazaNumarCartiInCos();
    }
    catch (RepoException& re) {
        cout << re.getErrorMessage();
    }
}

void ConsoleUI::golesteCos() {
    cosInchirieri.clear();
    cout << "Cosul a fost golit!" << endl;
    afiseazaNumarCartiInCos();
}


void ConsoleUI::exportaCos() {
    string numeFisier;
    cout << "Introduceti numele fisierului pentru export (fara extensie): ";
    cin >> numeFisier;

    string extensie;
    cout << "Alegeti formatul de export (csv / html): ";
    cin >> extensie;

    string numeFisierCuExtensie = numeFisier + "." + extensie;

    ofstream fisier;
    fisier.open(numeFisierCuExtensie);

    if (!fisier.is_open()) {
        cout << "Eroare la deschiderea fisierului pentru scriere!";
        return;
    }

    if (extensie == "csv") {
        fisier << "ID,Titlu,Autor,Gen,AnulAparitiei" << endl;
        for (const auto& carte : cosInchirieri) {
            fisier << carte.getId() << "," << carte.getTitlu() << "," << carte.getAutor() << "," << carte.getGen() << "," << carte.getAnulAparitiei() << endl;
        }
    }
    else if (extensie == "html") {
        fisier << "<html><head><title>Cos Inchirieri</title></head><body><table border=\"1\"><tr><th>ID</th><th>Titlu</th><th>Autor</th><th>Gen</th><th>AnulAparitiei</th></tr>";
        for (const auto& carte : cosInchirieri) {
            fisier << "<tr><td>" << carte.getId() << "</td><td>" << carte.getTitlu() << "</td><td>" << carte.getAutor() << "</td><td>" << carte.getGen() << "</td><td>" << carte.getAnulAparitiei() << "</td></tr>";
        }
        fisier << "</table></body></html>";
    }
    else {
        cout << "Format de export invalid!";
        fisier.close();
        return;
    }

    fisier.close();
    cout << "Exportul a fost realizat cu succes in fisierul " << numeFisierCuExtensie << "!" << endl;
}

void ConsoleUI::undo() {
    if (undoActions.empty()) {
        cout << "Nu exista actiuni de undo disponibile." << endl;
        return;
    }

    // Obținem ultima acțiune de undo din vector
    ActiuneUndo* lastUndoAction = undoActions.back();

    // Apelăm funcția de undo a acțiunii
    lastUndoAction->doUndo();

    // Ștergem ultima acțiune de undo din vector
    undoActions.pop_back();

    // Eliberăm memoria alocată pentru obiectul de tip UndoAction
    delete lastUndoAction;
}

void ConsoleUI::adaugaActiuneUndo(ActiuneUndo* actiuneUndo) {
    undoActions.push_back(actiuneUndo);
}

void ConsoleUI::stergeActiuneUndo() {
    if (!undoActions.empty()) {
        ActiuneUndo* lastUndoAction = undoActions.back();
        undoActions.pop_back();
        delete lastUndoAction;
    }
}



void ConsoleUI::addOnLoad() {
    srv.adaugaCarte(1, "Marea Speranta", "Stephen King", "Fictiune", 1978);
    srv.adaugaCarte(2, "Micul Print", "Antoine de Saint-Exupery", "Fictiune", 1943);
    srv.adaugaCarte(3, "Harry Potter si Piatra Filozofala", "J.K. Rowling", "Fictiune", 1997);
    srv.adaugaCarte(4, "Dune", "Frank Herbert", "Fictiune", 1965);
    srv.adaugaCarte(5, "1984", "George Orwell", "Fictiune", 1949);
    srv.adaugaCarte(6, "Crima si pedeapsa", "Fyodor Dostoevsky", "Fictiune", 1866);
    srv.adaugaCarte(7, "To Kill a Mockingbird", "Harper Lee", "Fictiune", 1960);
    srv.adaugaCarte(8, "The Catcher in the Rye", "J.D. Salinger", "Fictiune", 1951);
    srv.adaugaCarte(9, "Don Quijote", "Miguel de Cervantes", "Fictiune", 1605);
    srv.adaugaCarte(10, "Razboi si pace", "Leo Tolstoy", "Fictiune", 1869);
}

void ConsoleUI::run() {
    int running = 1;
    int cmd;
    //addOnLoad();
    try {
        srv.incarcaDateDinFisier("Load.txt");
    }
    catch (RepoException& re) {
        cout << re.getErrorMessage() << endl;
    }

    menu();
    while (running) {
        cout << "Comanda este:";
        cin >> cmd;
        switch (cmd)
        {
        case 1:
            uiPrintAll(srv.getToateCartile());
            break;
        case 2:
            uiAdd();
            break;
        case 3:
            uiDelete();
            break;
        case 4:
            uiModify();
            break;
        case 5:
            uiFind();
            break;
        case 6:
            uiFilter();
            break;
        case 7:
            uiSort();
            break;
        case 8:
            genereazaCos();
            break;
        case 9:
            adaugaInCos();
            break;
        case 10:
            golesteCos();
            break;
        case 11:
            exportaCos();
            break;
        case 12:
            undo();
            break;
        case 0:
            running = 0;
            break;
        default:
            break;
        }

    }

    try {
        srv.salveazaDateInFisier("Load.txt");
    }
    catch (RepoException& re) {
        cout << re.getErrorMessage() << endl;
    }
}