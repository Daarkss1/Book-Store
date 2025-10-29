#pragma once

#include "Domain.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

class ValidationException {
    vector<string> errorMsg;
public:
    ValidationException(vector<string> errorMessages) : errorMsg{ errorMessages } {};

    string getErrorMessages() {
        string fullMsg = "";
        for (const string& e : errorMsg) {
            fullMsg += e + "\n";
        }
        return fullMsg;
    }
};

/*
Clasa pentru validarea cărților
O carte este validă dacă:
- are un ID strict pozitiv
- are titlul, autorul și genul nenule
- are anul de apariție strict pozitiv
*/
class CarteValidator {
public:
    void valideaza(const Carte& c) {
        vector<string> errors;
        if (c.getId() <= 0)
            errors.push_back("ID-ul trebuie să fie strict pozitiv.");
        if (c.getTitlu().empty())
            errors.push_back("Titlul trebuie să fie nenul.");
        if (c.getAutor().empty())
            errors.push_back("Autorul trebuie să fie nenul.");
        if (c.getGen().empty())
            errors.push_back("Genul trebuie să fie nenul.");
        if (c.getAnulAparitiei() <= 0)
            errors.push_back("Anul de apariție trebuie să fie strict pozitiv.");

        if (errors.size() > 0)
            throw ValidationException(errors);
    }
};
