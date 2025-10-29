#include "Service.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

void CarteStore::incarcaDateDinFisier(const string& numeFisier) {
    std::ifstream fisier(numeFisier);

    if (!fisier.is_open()) {
        throw RepoException("Eroare la deschiderea fisierului pentru citire!");
    }

    string linie;
    while (getline(fisier, linie)) {
        std::stringstream ss(linie);
        string token;

        int id, an;
        string titlu, autor, gen;

        getline(ss, token, '|');
        id = stoi(token);

        getline(ss, titlu, '|');
        getline(ss, autor, '|');
        getline(ss, gen, '|');
        getline(ss, token, '|');
        an = stoi(token);

        adaugaCarte(id, titlu, autor, gen, an);
    }

    fisier.close();
}



void CarteStore::salveazaDateInFisier(const string& numeFisier) {
    std::ofstream fisier(numeFisier);

    if (!fisier.is_open()) {
        throw RepoException("Eroare la deschiderea fisierului pentru scriere!");
    }

    for (const auto& carte : repo.getAllCarti()) {
        fisier << carte.getId() << "|"
            << carte.getTitlu() << "|"
            << carte.getAutor() << "|"
            << carte.getGen() << "|"
            << carte.getAnulAparitiei() << endl;
    }

    fisier.close();
}




// Funcție helper pentru filtrarea cărților
vector<Carte> CarteStore::filter(function<bool(const Carte&)> fct) {
    vector<Carte> rezultat;
    for (const Carte& c : repo.getAllCarti()) {
        if (fct(c)) {
            rezultat.push_back(c);
        }
    }
    return rezultat;
}

// Funcție helper pentru sortarea generală a cărților
vector<Carte> CarteStore::generalSort(bool(*maiMicF)(const Carte&, const Carte&)) {
    vector<Carte> rezultat = repo.getAllCarti();
    sort(rezultat.begin(), rezultat.end(), maiMicF);
    return rezultat;
}

void CarteStore::adaugaCarte(int id, string titlu, string autor, string gen, int an) {
    Carte carte{ id, titlu, autor, gen, an };
    validator.valideaza(carte);
    repo.store(carte);

    // Adăugarea unei acțiuni de undo pentru adăugare
    undoActions.push_back(new UndoAdauga(repo, id));
}


void CarteStore::stergeCarte(int id) {
    Carte carte = repo.find(id);
    repo.remove(repo.find(id));

    // Adăugarea unei acțiuni de undo pentru ștergere
    undoActions.push_back(new UndoSterge(repo, carte));
}


void CarteStore::modificaCarte(int id, string titluNou, string autorNou, string genNou, int anNou) {
    Carte carteVeche = repo.find(id);
    Carte carteNoua{ id, titluNou, autorNou, genNou, anNou };
    validator.valideaza(carteNoua);
    repo.modify(carteNoua);

    // Adăugarea unei acțiuni de undo pentru modificare
    undoActions.push_back(new UndoModifica(repo, carteVeche));
}

CarteStore::~CarteStore() {
    for (auto& undoAction : undoActions) {
        delete undoAction;
    }
}


Carte CarteStore::cautaCarteDupaTitlu(const string& titlu) {
    for (const Carte& carte : repo.getAllCarti()) {
        if (carte.getTitlu() == titlu) {
            return carte;
        }
    }
    throw RepoException("Cartea cu titlul dat nu exista!");
}

void CarteStore::clearCos() {
    cosInchirieri.clear();
    notify();
}

void CarteStore::addToCos(const Carte& carte) {
    cosInchirieri.push_back(carte);
    notify();
}

const vector<Carte>& CarteStore::getCos() const {
    return cosInchirieri;
}

void CarteStore::undo() {
    if (!undoActions.empty()) {
        undoActions.back()->doUndo();
        undoActions.pop_back();
    }
}

Carte CarteStore::cautaCarte(int id) {
    return repo.find(id);
}

vector<Carte> CarteStore::filtreazaDupaTitlu(string Titlu) {
    return filter([Titlu](const Carte& c) { return c.getTitlu() == Titlu; });
}

vector<Carte> CarteStore::filtreazaDupaAn(int an) {
    return filter([an](const Carte& c) { return c.getAnulAparitiei() == an; });
}

vector<Carte> CarteStore::sorteazaDupaTitlu() {
    return generalSort([](const Carte& c1, const Carte& c2) { return c1.getTitlu() < c2.getTitlu(); });
}

vector<Carte> CarteStore::sorteazaDupaAutor() {
    return generalSort([](const Carte& c1, const Carte& c2) { return c1.getAutor() < c2.getAutor(); });
}

vector<Carte> CarteStore::sorteazaDupaAnSiGen() {
    return generalSort([](const Carte& c1, const Carte& c2) {
        if (c1.getAnulAparitiei() == c2.getAnulAparitiei()) {
            return c1.getGen() < c2.getGen();
        }
        return c1.getAnulAparitiei() < c2.getAnulAparitiei();
        });
}

// Funcții de testare pentru service


void testAdaugaCarte() {
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testService{ testRepo, testValidator };

    testService.adaugaCarte(1, "Marea Speranță", "Stephen King", "Ficțiune", 1978);
    assert(testService.getToateCartile().size() == 1);
    assert(testService.getToateCartile()[0].getTitlu() == "Marea Speranță");
}

void testStergeCarte() {
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testService{ testRepo, testValidator };

    testService.adaugaCarte(1, "Marea Speranță", "Stephen King", "Ficțiune", 1978);
    testService.stergeCarte(1);
    assert(testService.getToateCartile().size() == 0);
}

void testModificaCarteS() {
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testService{ testRepo, testValidator };

    // Adăugăm o carte
    testService.adaugaCarte(1, "Marea Speranță", "Stephen King", "Ficțiune", 1978);

    // Modificăm cartea adăugată
    testService.modificaCarte(1, "Alt titlu", "Alt autor", "Alt gen", 2000);

    // Verificăm modificările
    auto toateCartile = testService.getToateCartile();
    assert(toateCartile.size() == 1);
    assert(toateCartile[0].getTitlu() == "Alt titlu");
    assert(toateCartile[0].getAutor() == "Alt autor");
    assert(toateCartile[0].getGen() == "Alt gen");
    assert(toateCartile[0].getAnulAparitiei() == 2000);
}

void testCautaCarte() {
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testService{ testRepo, testValidator };

    // Adăugăm o carte
    testService.adaugaCarte(1, "Marea Speranță", "Stephen King", "Ficțiune", 1978);

    // Căutăm cartea adăugată
    Carte carteGasita = testService.cautaCarte(1);

    // Verificăm că am găsit corect cartea
    assert(carteGasita.getTitlu() == "Marea Speranță");
    assert(carteGasita.getAutor() == "Stephen King");
    assert(carteGasita.getGen() == "Ficțiune");
    assert(carteGasita.getAnulAparitiei() == 1978);
}

void testFiltreazaDupaTitlu() {
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testService{ testRepo, testValidator };

    // Adăugăm mai multe cărți
    testService.adaugaCarte(1, "Marea Speranță", "Stephen King", "Ficțiune", 1978);
    testService.adaugaCarte(2, "Micul Print", "Antoine de Saint-Exupéry", "Ficțiune", 1943);
    testService.adaugaCarte(3, "Harry Potter și Piatra Filozofală", "J.K. Rowling", "Ficțiune", 1997);

    // Filtrează după titlu
    auto rezultatFiltrare = testService.filtreazaDupaTitlu("Micul Print");

    // Verificăm că a fost găsită corect cartea filtrată
    assert(rezultatFiltrare.size() == 1);
    assert(rezultatFiltrare[0].getTitlu() == "Micul Print");
}

void testFiltreazaDupaAn() {
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testService{ testRepo, testValidator };

    // Adăugăm mai multe cărți
    testService.adaugaCarte(1, "Marea Speranță", "Stephen King", "Ficțiune", 1978);
    testService.adaugaCarte(2, "Micul Print", "Antoine de Saint-Exupéry", "Ficțiune", 1943);
    testService.adaugaCarte(3, "Harry Potter și Piatra Filozofală", "J.K. Rowling", "Ficțiune", 1997);

    // Filtrează după an
    auto rezultatFiltrare = testService.filtreazaDupaAn(1997);

    // Verificăm că a fost găsită corect cartea filtrată
    assert(rezultatFiltrare.size() == 1);
    assert(rezultatFiltrare[0].getAnulAparitiei() == 1997);
}

void testSorteazaDupaTitlu() {
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testService{ testRepo, testValidator };

    // Adăugăm mai multe cărți
    testService.adaugaCarte(1, "Marea Speranță", "Stephen King", "Ficțiune", 1978);
    testService.adaugaCarte(2, "Micul Print", "Antoine de Saint-Exupéry", "Ficțiune", 1943);
    testService.adaugaCarte(3, "Harry Potter și Piatra Filozofală", "J.K. Rowling", "Ficțiune", 1997);

    // Sortează după titlu
    auto rezultatSortare = testService.sorteazaDupaTitlu();

    // Verificăm că a fost sortată corect lista de cărți
    assert(rezultatSortare.size() == 3);
    assert(rezultatSortare[0].getTitlu() == "Harry Potter și Piatra Filozofală");
    assert(rezultatSortare[1].getTitlu() == "Marea Speranță");
    assert(rezultatSortare[2].getTitlu() == "Micul Print");
}

void testSorteazaDupaAutor() {
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testService{ testRepo, testValidator };

    // Adăugăm mai multe cărți
    testService.adaugaCarte(1, "Marea Speranță", "Stephen King", "Ficțiune", 1978);
    testService.adaugaCarte(2, "Micul Print", "Antoine de Saint-Exupéry", "Ficțiune", 1943);
    testService.adaugaCarte(3, "Harry Potter și Piatra Filozofală", "J.K. Rowling", "Ficțiune", 1997);

    // Sortează după autor
    auto rezultatSortare = testService.sorteazaDupaAutor();

    // Verificăm că a fost sortată corect lista de cărți
    assert(rezultatSortare.size() == 3);
    assert(rezultatSortare[0].getAutor() == "Antoine de Saint-Exupéry");
    assert(rezultatSortare[1].getAutor() == "J.K. Rowling");
    assert(rezultatSortare[2].getAutor() == "Stephen King");
}

void testSorteazaDupaAnSiGen() {
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testService{ testRepo, testValidator };

    // Adăugăm mai multe cărți
    testService.adaugaCarte(1, "Marea Speranță", "Stephen King", "Ficțiune", 1978);
    testService.adaugaCarte(2, "Micul Print", "Antoine de Saint-Exupéry", "Ficțiune", 1943);
    testService.adaugaCarte(3, "Harry Potter și Piatra Filozofală", "J.K. Rowling", "Ficțiune", 1997);

    // Sortează după an și gen
    auto rezultatSortare = testService.sorteazaDupaAnSiGen();

    // Verificăm că a fost sortată corect lista de cărți
    assert(rezultatSortare.size() == 3);
    assert(rezultatSortare[0].getAnulAparitiei() == 1943);
    assert(rezultatSortare[1].getAnulAparitiei() == 1978);
    assert(rezultatSortare[2].getAnulAparitiei() == 1997);
}

void testCautaCarteDupaTitlu() {
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testService{ testRepo, testValidator };

    // Adăugăm mai multe cărți
    testService.adaugaCarte(1, "Marea Speranță", "Stephen King", "Ficțiune", 1978);
    testService.adaugaCarte(2, "Micul Print", "Antoine de Saint-Exupéry", "Ficțiune", 1943);
    testService.adaugaCarte(3, "Harry Potter și Piatra Filozofală", "J.K. Rowling", "Ficțiune", 1997);

    // Testăm căutarea unei cărți existente
    Carte carteGasita = testService.cautaCarteDupaTitlu("Micul Print");
    assert(carteGasita.getTitlu() == "Micul Print");
    assert(carteGasita.getAutor() == "Antoine de Saint-Exupéry");
    assert(carteGasita.getGen() == "Ficțiune");
    assert(carteGasita.getAnulAparitiei() == 1943);

    // Testăm căutarea unei cărți inexistente
    try {
        testService.cautaCarteDupaTitlu("Titlu inexistent");
        assert(false); // Așteptăm ca acest punct să nu fie atins, deoarece ar trebui să arunce o excepție
    }
    catch (const RepoException& re) {
        // Dacă ajungem aici, testul este trecut cu succes
    }
}

void testUndoAdauga() {
    // Inițializare repo și validator
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testController{ testRepo, testValidator };

    // Adăugare carte
    testController.adaugaCarte(1, "Marea Speranță", "Stephen King", "Ficțiune", 1978);

    // Salvare număr inițial de cărți
    int initialSize = testController.getToateCartile().size();

    // Apel undo pentru adăugare
    testController.undo();

    // Verificare dacă acțiunea de undo a eliminat cartea adăugată
    assert(testController.getToateCartile().size() == initialSize - 1);
}

void testUndoSterge() {
    // Inițializare repo și validator
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testController{ testRepo, testValidator };

    // Adăugare carte
    testController.adaugaCarte(1, "Marea Speranță", "Stephen King", "Ficțiune", 1978);

    // Salvare număr inițial de cărți
    int initialSize = testController.getToateCartile().size();

    // Ștergere carte
    testController.stergeCarte(1);

    // Apel undo pentru ștergere
    testController.undo();

    // Verificare dacă acțiunea de undo a readăugat cartea ștearsă
    assert(testController.getToateCartile().size() == initialSize);
}

void testUndoModifica() {
    // Inițializare repo și validator
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testController{ testRepo, testValidator };

    // Adăugare carte
    testController.adaugaCarte(1, "Marea Speranță", "Stephen King", "Ficțiune", 1978);

    // Modificare carte
    testController.modificaCarte(1, "Alt titlu", "Alt autor", "Alt gen", 2000);

    // Apel undo pentru modificare
    testController.undo();

    // Verificare dacă acțiunea de undo a readus cartea la starea inițială
    Carte carte = testController.cautaCarte(1);
    assert(carte.getTitlu() == "Marea Speranță");
    assert(carte.getAutor() == "Stephen King");
    assert(carte.getGen() == "Ficțiune");
    assert(carte.getAnulAparitiei() == 1978);
}

void testUndoEmpty() {
    // Inițializare repo și validator
    CarteRepository testRepo;
    CarteValidator testValidator;
    CarteStore testController{ testRepo, testValidator };

    // Apel undo când lista de acțiuni undo este goală
    testController.undo();

    // Dacă apelul de undo nu aruncă excepții, testul este trecut
    // Deoarece nu s-a făcut nicio acțiune undo
}


void testeService() {
    testAdaugaCarte();
    testStergeCarte();
    testModificaCarteS();
    testCautaCarte();
    testFiltreazaDupaTitlu();
    testFiltreazaDupaAn();
    testSorteazaDupaTitlu();
    testSorteazaDupaAutor();
    testSorteazaDupaAnSiGen();
    testCautaCarteDupaTitlu();
    testUndoAdauga();
    testUndoSterge();
    testUndoModifica();
    testUndoEmpty();
}

