#include "lab1011.h"
#include <QtWidgets/QApplication>
#include "CarteStoreGUI.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    CarteRepository repo;
    CarteValidator val;
    CarteStore srv{ repo, val };

    /* Load data from file
    try {
        srv.incarcaDateDinFisier("books.txt");
    }
    catch (const RepoException& e) {
        std::cerr << "Failed to load data: " << std::endl;
        return 1; // Exit if loading fails
    }*/

    CarteStoreGUI gui{ srv };
    gui.show();

    return a.exec();
}
