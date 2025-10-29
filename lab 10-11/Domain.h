#pragma once
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

class Carte {
private:
    int id;
    string titlu;
    string autor;
    string gen;
    int anulAparitiei;
public:
    int contor = 0;

    Carte() = default;

    Carte(int id, string titlu, string autor, string gen, int anulAparitiei)
        : id{ id }, titlu{ titlu }, autor{ autor }, gen{ gen }, anulAparitiei{ anulAparitiei } {}
    Carte(const Carte& ot)
        : id{ ot.id }, titlu{ ot.titlu }, autor{ ot.autor }, gen{ ot.gen }, anulAparitiei{ ot.anulAparitiei } {

    }

    bool operator==(const Carte& other) const {
        return id == other.id && titlu == other.titlu && autor == other.autor && gen == other.gen && anulAparitiei == other.anulAparitiei;
    }

    int getId() const;
    string getTitlu() const;
    string getAutor() const;
    string getGen() const;
    int getAnulAparitiei() const;

    void setId(int idNou);
    void setTitlu(string titluNou);
    void setAutor(string autorNou);
    void setGen(string genNou);
    void setAnulAparitiei(int anulAparitieiNou);

    ~Carte() {
    }
};

/*
Compara doua carti dupa anul aparitiei

@return: true daca anul aparitiei cartii c1 e mai mic decat
         anul aparitiei cartii c2, false altfel
*/
bool cmpAnulAparitiei(const Carte& c1, const Carte& c2);

/*
Compara doua carti dupa autor si titlul cartii
@return: true daca autorul cartii c1 e inainte (alfabetic)
         autorului cartii c2, false altfel
         daca avem acelasi autor, atunci comparam titlurile cartilor
*/
bool cmpAutorCarte(const Carte& c1, const Carte& c2);

void testeDomain();
