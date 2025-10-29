#include "CarteStoreGUI.h"

// Constructorul clasei CarteStoreGUI
CarteStoreGUI::CarteStoreGUI(CarteStore& CarteSrv) : srv{ CarteSrv } {
    initializeGUIComponents(); // Inițializare componentelor grafice
    connectSignalsSlots(); // Conectarea semnalelor la sloturile corespunzătoare
    try {
        srv.incarcaDateDinFisier("books.txt"); // Load books from file
        reloadCarteList(srv.getToateCartile()); // Load initial list of books
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Error Loading Books", e.what());
    }
}

void CarteStoreGUI::closeEvent(QCloseEvent* event) {
    try {
        srv.salveazaDateInFisier("books.txt"); // Save books to file
        QMessageBox::information(this, "Save Successful", "Books have been saved to file.");
        event->accept(); // Accept the close event
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error Saving Books", e.what());
        event->ignore(); // Ignore the close event if there's an error
    }
}

// Metoda pentru inițializarea componentelor grafice
void CarteStoreGUI::initializeGUIComponents() {
    QHBoxLayout* lyMain = new QHBoxLayout;
    this->setLayout(lyMain);

    // Componentele din partea stângă a interfeței
    QWidget* left = new QWidget;
    QVBoxLayout* lyLeft = new QVBoxLayout;
    left->setLayout(lyLeft);

    // Formularul pentru introducerea datelor unei cărți
    QWidget* form = new QWidget;
    QFormLayout* lyForm = new QFormLayout;
    form->setLayout(lyForm);

    // Câmpuri de text pentru datele cărții
    editId = new QLineEdit;
    editTitlu = new QLineEdit;
    editAutor = new QLineEdit;
    editGen = new QLineEdit;
    editAnulAparitiei = new QLineEdit;

    // Adăugarea câmpurilor în formular
    lyForm->addRow(lblId, editId);
    lyForm->addRow(lblTitlu, editTitlu);
    lyForm->addRow(lblAutor, editAutor);
    lyForm->addRow(lblGen, editGen);
    lyForm->addRow(lblAnulAparitiei, editAnulAparitiei);

    // Buton pentru adăugarea unei cărți
    btnAddCarte = new QPushButton("Adauga carte");
    lyForm->addWidget(btnAddCarte);

    // Buton pentru ștergerea unei cărți
    btnRemoveCarte = new QPushButton("Sterge carte");
    lyForm->addWidget(btnRemoveCarte);

    // Buton pentru modificarea unei cărți
    btnModifyCarte = new QPushButton("Modifica carte");
    lyForm->addWidget(btnModifyCarte);

    lyLeft->addWidget(form);

    // Grupul pentru opțiunile de sortare
    QVBoxLayout* lyRadioBoxSort = new QVBoxLayout;
    groupBox1->setLayout(lyRadioBoxSort);
    lyRadioBoxSort->addWidget(radioSrtTitlu);
    lyRadioBoxSort->addWidget(radioSrtAutor);
    lyRadioBoxSort->addWidget(radioSrtAnGen);
    lyLeft->addWidget(groupBox1);

    // Grupul pentru opțiunile de filtrare
    QVBoxLayout* lyRadioBoxFilter = new QVBoxLayout;
    groupBox2->setLayout(lyRadioBoxFilter);
    QHBoxLayout* titleLayout = new QHBoxLayout;
    editFilTitluVal = new QLineEdit;
    titleLayout->addWidget(radioFilTitlu);
    titleLayout->addWidget(editFilTitluVal);
    lyRadioBoxFilter->addLayout(titleLayout);

    QHBoxLayout* yearLayout = new QHBoxLayout;
    editFilAnVal = new QLineEdit;
    yearLayout->addWidget(radioFilAn);
    yearLayout->addWidget(editFilAnVal);
    lyRadioBoxFilter->addLayout(yearLayout);

    btnFilterCartes = new QPushButton("Filtreaza cartile");
    lyRadioBoxFilter->addWidget(btnFilterCartes);
    lyLeft->addWidget(groupBox2);

    btnOpenNewWindow = new QPushButton("Cos");
    lyLeft->addWidget(btnOpenNewWindow);

    btnReloadData = new QPushButton("Reincarca datele");
    lyLeft->addWidget(btnReloadData);

    // Buton pentru undo
    btnUndo = new QPushButton("Undo");
    lyLeft->addWidget(btnUndo);

    // Butoane pentru deschiderea ferestrelor coșului
    btnOpenCosCRUD = new QPushButton{ "Open Cos CRUD" };
    btnOpenCosReadOnly = new QPushButton{ "Open Cos ReadOnly" };

    // Adăugăm butoanele la layout-ul existent
    lyLeft->addWidget(btnOpenCosCRUD);
    lyLeft->addWidget(btnOpenCosReadOnly);

    // Componentele din partea dreaptă a interfeței
    QWidget* right = new QWidget;
    QVBoxLayout* lyRight = new QVBoxLayout;
    right->setLayout(lyRight);

    // Model pentru tabel
    modelCarti = new CarteTableModel(srv.getToateCartile());

    // Tabel pentru afișarea cărților
    tableCartes = new QTableView;
    tableCartes->setModel(modelCarti);

    lyRight->addWidget(tableCartes);
    lyMain->addWidget(left);
    lyMain->addWidget(right);
}

// Metoda pentru reîncărcarea listei de cărți
void CarteStoreGUI::reloadCarteList(const vector<Carte>& Cartes) {
    modelCarti->setCarti(Cartes);
}

// Metoda pentru conectarea semnalelor la sloturi
void CarteStoreGUI::connectSignalsSlots() {
    QObject::connect(btnOpenNewWindow, &QPushButton::clicked, this, &CarteStoreGUI::openCos);
    QObject::connect(btnAddCarte, &QPushButton::clicked, this, &CarteStoreGUI::guiAddCarte);
    QObject::connect(btnRemoveCarte, &QPushButton::clicked, this, &CarteStoreGUI::guiRemoveCarte);
    QObject::connect(btnModifyCarte, &QPushButton::clicked, this, &CarteStoreGUI::guiModifyCarte);

    QObject::connect(radioSrtTitlu, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            reloadCarteList(srv.sorteazaDupaTitlu());
        }
        });

    QObject::connect(radioSrtAutor, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            reloadCarteList(srv.sorteazaDupaAutor());
        }
        });

    QObject::connect(radioSrtAnGen, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            reloadCarteList(srv.sorteazaDupaAnSiGen());
        }
        });

    QObject::connect(btnFilterCartes, &QPushButton::clicked, this, [=]() {
        if (radioFilTitlu->isChecked()) {
            string filtruTitlu = editFilTitluVal->text().toStdString();
            reloadCarteList(srv.filtreazaDupaTitlu(filtruTitlu));
        }
        else if (radioFilAn->isChecked()) {
            int filtruAn = editFilAnVal->text().toInt();
            reloadCarteList(srv.filtreazaDupaAn(filtruAn));
        }
        });

    QObject::connect(btnUndo, &QPushButton::clicked, this, &CarteStoreGUI::guiUndo);

    QObject::connect(btnOpenCosCRUD, &QPushButton::clicked, [&]() {
        auto cosCrudGui = new CosCRUDGUI{ srv };
        cosCrudGui->show();
        });

    QObject::connect(btnOpenCosReadOnly, &QPushButton::clicked, [&]() {
        auto cosReadOnlyGui = new CosReadOnlyGUI{ srv };
        cosReadOnlyGui->show();
        });
}

// Metoda GUI pentru adăugarea unei cărți noi
void CarteStoreGUI::guiAddCarte() {
    try {
        // Preluarea și validarea datelor de la utilizator
        int id = editId->text().toInt();
        string titlu = editTitlu->text().toStdString();
        string autor = editAutor->text().toStdString();
        string gen = editGen->text().toStdString();
        int anulAparitiei = editAnulAparitiei->text().toInt();

        // Apelul serviciului pentru adăugarea cărții
        srv.adaugaCarte(id, titlu, autor, gen, anulAparitiei);
        reloadCarteList(srv.getToateCartile());

        // Notificarea utilizatorului
        QMessageBox::information(this, "Info", "Cartea a fost adaugata cu succes.");
    }
    catch (RepoException& re) {
        QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
    }
    catch (ValidationException& ve) {
        QMessageBox::warning(this, "Warning", QString::fromStdString(ve.getErrorMessages()));
    }
}

void CarteStoreGUI::guiRemoveCarte() {
    try {
        if (!editTitlu->text().isEmpty() || !editAutor->text().isEmpty() || !editGen->text().isEmpty() || !editAnulAparitiei->text().isEmpty()) {
            QMessageBox::warning(this, "Eroare", "Doar câmpul ID trebuie completat pentru ștergere.");
            editId->clear();
            editTitlu->clear();
            editAutor->clear();
            editGen->clear();
            editAnulAparitiei->clear();
            return;
        }
        int id = editId->text().toInt();

        srv.stergeCarte(id);
        reloadCarteList(srv.getToateCartile());

        QMessageBox::information(this, "Info", "Cartea a fost stearsa cu succes.");
    }
    catch (RepoException& re) {
        QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
    }
}

void CarteStoreGUI::guiModifyCarte() {
    try {
        int id = editId->text().toInt();
        string titlu = editTitlu->text().toStdString();
        string autor = editAutor->text().toStdString();
        string gen = editGen->text().toStdString();
        int anulAparitiei = editAnulAparitiei->text().toInt();

        srv.modificaCarte(id, titlu, autor, gen, anulAparitiei);
        reloadCarteList(srv.getToateCartile());
        QMessageBox::information(this, "Info", "Cartea a fost modificata cu succes.");
    }
    catch (RepoException& re) {
        QMessageBox::warning(this, "Warning", QString::fromStdString(re.getErrorMessage()));
        editId->clear();
        editTitlu->clear();
        editAutor->clear();
        editGen->clear();
        editAnulAparitiei->clear();
    }
    catch (ValidationException& ve) {
        QMessageBox::warning(this, "Warning", QString::fromStdString(ve.getErrorMessages()));
    }
}

void CarteStoreGUI::openCos() {
    QMainWindow* newWindow = new QMainWindow;
    newWindow->setWindowTitle("Cos de Cumparaturi");
    newWindow->resize(600, 300);

    // Central widget setup with a horizontal layout
    QWidget* centralWidget = new QWidget;
    QHBoxLayout* mainLayout = new QHBoxLayout;
    centralWidget->setLayout(mainLayout);
    newWindow->setCentralWidget(centralWidget);

    // Left side: Table for displaying books in the cos
    QTableView* table = new QTableView;
    auto cosModel = new CarteTableModel(srv.getCos());
    table->setModel(cosModel);
    mainLayout->addWidget(table);

    // Right side: Buttons and QLineEdit for number input
    QVBoxLayout* buttonLayout = new QVBoxLayout;
    QPushButton* btnGenerateCos = new QPushButton("Genereaza Cos");
    QLineEdit* editNumberOfBooks = new QLineEdit;  // User inputs the number of books to add
    editNumberOfBooks->setPlaceholderText("Numar de carti");

    QPushButton* btnAddToCos = new QPushButton("Adauga in Cos");
    QLineEdit* editBookId = new QLineEdit;  // User inputs the book ID
    editBookId->setPlaceholderText("Introduceti ID-ul cartii");

    QPushButton* btnEmptyCos = new QPushButton("Goleste Cos");
    QPushButton* btnExportCos = new QPushButton("Exporta Cos");
    QRadioButton* radioCsv = new QRadioButton("CSV");
    QRadioButton* radioHtml = new QRadioButton("HTML");
    radioCsv->setChecked(true); // Default to CSV
    QLineEdit* editFileName = new QLineEdit;
    editFileName->setPlaceholderText("Enter file name");

    buttonLayout->addWidget(btnGenerateCos);
    buttonLayout->addWidget(editNumberOfBooks);  // Add QLineEdit under the button
    buttonLayout->addWidget(btnAddToCos);
    buttonLayout->addWidget(editBookId);  // Add QLineEdit under the button
    buttonLayout->addWidget(btnEmptyCos);
    buttonLayout->addWidget(radioCsv);
    buttonLayout->addWidget(radioHtml);
    buttonLayout->addWidget(editFileName);
    buttonLayout->addWidget(btnExportCos);
    mainLayout->addLayout(buttonLayout);

    QObject::connect(btnGenerateCos, &QPushButton::clicked, [this, editNumberOfBooks, cosModel]() {
        bool ok;
        int num = editNumberOfBooks->text().toInt(&ok);

        if (!ok) {
            QMessageBox::warning(this, "Input Error", "Please enter a valid number.");
            return;
        }

        if (num < 0) {
            QMessageBox::warning(this, "Input Error", "Please enter a non-negative number.");
            return;
        }

        auto allBooks = srv.getToateCartile();
        if (num > allBooks.size()) {
            QMessageBox::warning(this, "Input Error", "The number entered exceeds the total number of books available.");
            return;
        }

        srv.clearCos();
        std::shuffle(allBooks.begin(), allBooks.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
        for (int i = 0; i < num && i < allBooks.size(); i++) {
            srv.addToCos(allBooks[i]);
        }
        cosModel->setCarti(srv.getCos());
        });

    QObject::connect(btnAddToCos, &QPushButton::clicked, [this, editBookId, cosModel]() {
        bool ok;
        int bookId = editBookId->text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Input Error", "Please enter a valid numeric ID.");
            return;
        }

        addBookToCos(bookId);
        cosModel->setCarti(srv.getCos());
        });

    QObject::connect(btnEmptyCos, &QPushButton::clicked, [this, cosModel]() {
        clearCos();
        cosModel->setCarti(srv.getCos());
        });

    QObject::connect(btnExportCos, &QPushButton::clicked, [this, radioCsv, editFileName]() {
        QString fileType = radioCsv->isChecked() ? "csv" : "html";
        exportCos(editFileName->text(), fileType);
        });

    newWindow->show();
}

void CarteStoreGUI::generateRandomBooks(int num) {
    auto allBooks = srv.getToateCartile();

    // Seeding with the current time ensures different results on different runs
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine engine(seed);

    std::shuffle(allBooks.begin(), allBooks.end(), engine);  // Randomize the order of books using shuffle

    // Clear existing entries in cosInchirieri to start fresh
    srv.clearCos();

    for (int i = 0; i < num && i < allBooks.size(); i++) {
        srv.addToCos(allBooks[i]);
    }
}

void CarteStoreGUI::addBookToCos(int bookId) {
    auto allBooks = srv.getToateCartile();
    auto it = std::find_if(allBooks.begin(), allBooks.end(), [bookId](const Carte& carte) {
        return carte.getId() == bookId;
        });

    if (it != allBooks.end()) {
        srv.addToCos(*it);
        QMessageBox::information(this, "Success", "Book added successfully to the basket.");
    }
    else {
        QMessageBox::warning(this, "Error", "No book found with the provided ID.");
    }
}

void CarteStoreGUI::clearCos() {
    srv.clearCos();
    QMessageBox::information(this, "Cos Cleared", "All books have been removed from the basket.");
}

void CarteStoreGUI::exportCos(const QString& fileName, const QString& fileType) {
    if (fileName.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a valid filename.");
        return;
    }

    QString filePath = QStandardPaths::locate(QStandardPaths::DesktopLocation, "", QStandardPaths::LocateDirectory) + fileName;
    if (fileType == "csv") {
        filePath += ".csv";
    }
    else {
        filePath += ".html";
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to create the file.");
        return;
    }

    QTextStream out(&file);
    auto cosInchirieri = srv.getCos();
    if (fileType == "csv") {
        for (const auto& carte : cosInchirieri) {
            out << QString("%1,%2,%3,%4,%5\n")
                .arg(carte.getId())
                .arg(carte.getTitlu().c_str())
                .arg(carte.getAutor().c_str())
                .arg(carte.getGen().c_str())
                .arg(carte.getAnulAparitiei());
        }
    }
    else { // HTML
        out << "<html><head><title>Cos Export</title></head><body>";
        out << "<table border='1'><tr><th>ID</th><th>Titlu</th><th>Autor</th><th>Gen</th><th>Anul Aparitiei</th></tr>";
        for (const auto& carte : cosInchirieri) {
            out << QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td></tr>")
                .arg(carte.getId())
                .arg(carte.getTitlu().c_str())
                .arg(carte.getAutor().c_str())
                .arg(carte.getGen().c_str())
                .arg(carte.getAnulAparitiei());
        }
        out << "</table></body></html>";
    }

    file.close();
    QMessageBox::information(this, "Export Successful", "The cos has been exported successfully.");
}

void CarteStoreGUI::guiUndo() {
    try {
        srv.undo();  // Call undo on the service layer
        reloadCarteList(srv.getToateCartile());  // Reload the updated list of books
        QMessageBox::information(this, "Undo", "Last action has been undone.");
    }
    catch (std::exception& e) {
        QMessageBox::warning(this, "Undo Failed", e.what());
    }
}
