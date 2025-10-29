#include "Repo.h"
#include <assert.h>
#include <exception>
#include <algorithm> // pentru std::find_if
#include <stdexcept> // pentru std::out_of_range
#include <string>    // pentru std::to_string
#include <vector>    // pentru std::vector

const Carte& CarteRepository::find(int id) {
    auto it = std::find_if(allCarti.begin(), allCarti.end(),
        [id](const Carte& c) { return c.getId() == id; });
    if (it != allCarti.end()) {
        return *it;
    }
    throw RepoException("Cartea cu id-ul " + std::to_string(id) + " nu există în listă");
}

void CarteRepository::store(const Carte& c) {
    if (exists(c)) {
        throw RepoException("Cartea cu id-ul " + std::to_string(c.getId()) + " deja există în listă");
    }
    allCarti.add(c); // Folosim metoda add din VectorDinamic pentru a adăuga elementul
}

bool CarteRepository::exists(const Carte& c) {
    return std::find_if(allCarti.begin(), allCarti.end(),
        [&c](const Carte& elem) { return elem.getId() == c.getId(); }) != allCarti.end();
}

const vector<Carte> CarteRepository::getAllCarti() {
    vector<Carte> carti;
    // Utilizăm construcția de inițializare a vectorului pentru a face o copie a elementelor
    for (const auto& carte : allCarti) {
        carti.push_back(carte);
    }
    return carti;
}

void CarteRepository::remove(const Carte& c) {
    if (!exists(c)) {
        throw RepoException("Cartea cu id-ul " + std::to_string(c.getId()) + " nu există în listă");
    }
    allCarti.removeElement(c); // Folosim metoda removeElement din VectorDinamic pentru a șterge elementul
}


void CarteRepository::modify(const Carte& carteModificata) {
    for (auto& carte : allCarti) {
        if (carte.getId() == carteModificata.getId()) {
            carte = carteModificata;
            return;
        }
    }
    throw RepoException("Cartea cu id-ul " + std::to_string(carteModificata.getId()) + " nu există în listă");
}




void testAddRepo() {
    CarteRepository testRepo;
    Carte carte1{ 1, "Marea Speranță", "Stephen King", "Ficțiune", 1978 };
    testRepo.store(carte1);
    assert(testRepo.getAllCarti().size() == 1);

    Carte carte2{ 2, "Hobbitul", "J.R.R. Tolkien", "Fantezie", 1937 };
    Carte carte3{ 1, "Marea Speranță", "Stephen King", "Ficțiune", 1978 }; // Duplicate
    try {
        testRepo.store(carte3);
        assert(false);
    }
    catch (RepoException) {
        assert(true);
    }
}


void testFindRepo() {
    CarteRepository testRepo;

    Carte carte1{ 1, "Marea Speranță", "Stephen King", "Ficțiune", 1978 };
    Carte carte2{ 2, "Hobbitul", "J.R.R. Tolkien", "Fantezie", 1937 };
    testRepo.store(carte1);
    testRepo.store(carte2);

    assert(testRepo.exists(carte1));
    assert(!testRepo.exists(Carte{ 3, "Nu există", "Autor", "Gen", 2000 }));

    auto foundCarte = testRepo.find(1);
    assert(foundCarte.getAutor() == "Stephen King");
    assert(foundCarte.getAnulAparitiei() == 1978);
    assert(foundCarte.getGen() == "Ficțiune");

    try {
        testRepo.find(3);
        assert(false);
    }
    catch (RepoException& ve) {
        assert(ve.getErrorMessage() == "Cartea cu id-ul 3 nu există în listă");
    }
}

void testRemoveRepo() {
    CarteRepository testRepo;

    Carte carte1{ 1, "Marea Speranță", "Stephen King", "Ficțiune", 1978 };
    Carte carte2{ 2, "Hobbitul", "J.R.R. Tolkien", "Fantezie", 1937 };
    testRepo.store(carte1);
    testRepo.store(carte2);

    assert(testRepo.getAllCarti().size() == 2);

    // Încercăm să ștergem carte1
    testRepo.remove(carte1);
    assert(testRepo.getAllCarti().size() == 1); // Ar trebui să rămână doar carte2

    // Verificăm că doar carte2 este în repository
    auto allCarti = testRepo.getAllCarti();
    assert(allCarti[0] == carte2);

    // Încercăm să ștergem o carte inexistentă
    Carte carteInexistenta{ 3, "Nu există", "Autor", "Gen", 2000 };
    try {
        testRepo.remove(carteInexistenta);
        assert(false); // Ar trebui să arunce o excepție, deci testul ar trebui să eșueze
    }
    catch (RepoException& ex) {
        // Ne asigurăm că a aruncat excepția corectă
        assert(ex.getErrorMessage() == "Cartea cu id-ul 3 nu există în listă");
    }
}

void testModificaCarte() {
    CarteRepository testRepo;

    Carte carte1{ 1, "Titlu 1", "Autor 1", "Gen 1", 2000 };
    Carte carte2{ 2, "Titlu 2", "Autor 2", "Gen 2", 2005 };

    // Adăugăm cărți în repository
    testRepo.store(carte1);
    testRepo.store(carte2);

    // Modificăm prima carte
    Carte carteModificata{ 1, "Titlu modificat", "Autor modificat", "Gen modificat", 2020 };
    testRepo.modify(carteModificata);

    // Verificăm dacă modificarea s-a realizat corect
    assert(testRepo.find(1).getTitlu() == "Titlu modificat");
    assert(testRepo.find(1).getAutor() == "Autor modificat");
    assert(testRepo.find(1).getGen() == "Gen modificat");
    assert(testRepo.find(1).getAnulAparitiei() == 2020);

    // Verificăm că nu s-a modificat a doua carte
    assert(testRepo.find(2).getTitlu() == "Titlu 2");
    assert(testRepo.find(2).getAutor() == "Autor 2");
    assert(testRepo.find(2).getGen() == "Gen 2");
    assert(testRepo.find(2).getAnulAparitiei() == 2005);
}


void testeRepo() {
    testAddRepo();
    testFindRepo();
    testRemoveRepo();
    testModificaCarte();
}
