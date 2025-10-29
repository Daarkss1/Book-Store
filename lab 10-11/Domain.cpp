#include "Domain.h"
#include <assert.h>

int Carte::getId() const {
    return this->id;
}

string Carte::getTitlu() const {
    return this->titlu;
}

string Carte::getAutor() const {
    return this->autor;
}

string Carte::getGen() const {
    return this->gen;
}

int Carte::getAnulAparitiei() const {
    return this->anulAparitiei;
}

void Carte::setId(int idNou) {
    this->id = idNou;
}

void Carte::setTitlu(string titluNou) {
    this->titlu = titluNou;
}

void Carte::setAutor(string autorNou) {
    this->autor = autorNou;
}

void Carte::setGen(string genNou) {
    this->gen = genNou;
}

void Carte::setAnulAparitiei(int anulAparitieiNou) {
    this->anulAparitiei = anulAparitieiNou;
}

bool cmpAnulAparitiei(const Carte& c1, const Carte& c2) {
    return c1.getAnulAparitiei() < c2.getAnulAparitiei();
}

bool cmpAutorCarte(const Carte& c1, const Carte& c2) {
    if (c1.getAutor() == c2.getAutor())
        return c1.getTitlu() < c2.getTitlu();
    else
        return c1.getAutor() < c2.getAutor();
}

void testGetSet() {
    Carte carte1{ 1, "Marea Speranță", "Stephen King", "Ficțiune", 1978 };
    assert(carte1.getTitlu() == "Marea Speranță");
    assert(carte1.getAutor() == "Stephen King");
    assert(carte1.getGen() == "Ficțiune");
    assert(carte1.getAnulAparitiei() == 1978);

    Carte carte2{ 2, "Povestea Poveștilor", "J.R.R. Tolkien", "Fantezie", 1954 };
    assert(carte2.getTitlu() == "Povestea Poveștilor");
    assert(carte2.getAutor() == "J.R.R. Tolkien");
    assert(carte2.getGen() == "Fantezie");
    assert(carte2.getAnulAparitiei() == 1954);

    carte2.setTitlu("Hobbitul");
    carte2.setAutor("J.R.R. Tolkien");
    carte2.setGen("Fantezie");
    carte2.setAnulAparitiei(1937);

    assert(carte2.getTitlu() == "Hobbitul");
    assert(carte2.getAutor() == "J.R.R. Tolkien");
    assert(carte2.getGen() == "Fantezie");
    assert(carte2.getAnulAparitiei() == 1937);

}

void testeDomain() {
    testGetSet();
}
