# 📚 Book Management System (OOP Project – C++ & Qt)

This project is an **Object-Oriented Programming (OOP)** application written in **C++**, featuring a **Qt-based GUI** and a **console interface** for managing a collection of books.  
It demonstrates strong OOP principles, modular architecture, and custom data management through templates and dynamic structures.

---

## 🚀 Features

### Core Functionality
- Add, modify, delete, and search for books  
- Filter books by title or publication year  
- Sort books by:
  - Title  
  - Author  
  - Year and Genre  

### Advanced Functionality
- **Undo** support for add, delete, and modify actions  
- **Custom Dynamic Vector Template** implementation (`VectorDinamicTemplate.h`)  
- **Persistent data storage** using plain text files (`books.txt`)  
- **Custom exception and validation system**  
- **Book basket (Coș de cărți)** system:
  - Random generation of books
  - Export to `.csv` or `.html`  
  - Real-time visual display (Qt)
  - Observable/Observer pattern integration  

### GUI (Qt)
- Developed using **Qt Widgets**
- Features:
  - Dynamic book table with sorting and filtering
  - Forms for adding, editing, and deleting books
  - Basket (cos) window with CRUD and Read-Only views
  - File export and Undo support through buttons

### Console Interface
- Menu-based text interface for performing all core operations  
- Includes book management, basket management, export, and undo  

---

## 🏗️ Architecture

The project follows a **layered MVC-like architecture**:

```
📂 Domain
 ┣ Domain.h / .cpp – Book class definition and comparators
 ┣ validators.h – Validation logic
📂 Repository
 ┣ Repo.h / .cpp – Custom repository using `VectorDinamicTemplate`
📂 Service
 ┣ Service.h / .cpp – Business logic layer (sorting, filtering, undo)
📂 UI
 ┣ UI.h / .cpp – Console interface
 ┣ CarteStoreGUI.h / .cpp – Qt GUI with widgets and event handling
📂 Utils
 ┣ Observable.h / Observer.h – Observer design pattern
 ┣ VectorDinamicTemplate.h – Custom dynamic vector template
📂 Data
 ┣ books.txt – Sample dataset (loaded on startup)
```

---

## 🧩 Design Patterns Used
- **Observer Pattern** – Real-time GUI updates for the basket  
- **Command Pattern (Undo Actions)** – Reversible user actions  
- **Layered Architecture** – Clear separation between UI, Service, Repo, and Domain  
- **Custom Iterator** – For iterating over the custom vector structure  

---

## 🧪 Testing

The project includes extensive testing functions:
- `testeDomain()` – Domain layer testing  
- `testeRepo()` – Repository correctness  
- `testeService()` – Service operations (CRUD, sorting, filtering, undo)

Each test validates core and edge-case behavior for all system components.

---

## 💾 Persistence

Books are automatically **loaded** and **saved** from `books.txt` using plain text serialization.  
Each line follows the format:

```
ID|Title|Author|Genre|Year
```

Example:
```
3|Harry Potter si Piatra Filozofala|J.K. Rowling|Fictiune|1997
```

---

## 🧰 Technologies

| Component | Technology |
|------------|-------------|
| Language | C++17 |
| Framework | Qt 6 |
| IDE | Visual Studio / Qt Creator |
| Paradigm | Object-Oriented Programming |
| Design Patterns | Observer, Command, MVC |

---

## 👨‍💻 Author

**Alex [Your Last Name]**  
📧 *[your.email@example.com]*  
🎓 Babeș-Bolyai University, Faculty of Mathematics and Computer Science  
📅 2025  

---

## 🏁 How to Run

### ▶️ Using Qt Creator:
1. Open the `.pro` or `.sln` project in **Qt Creator** or **Visual Studio**  
2. Build the project  
3. Run the application — the GUI will start automatically  

### 💬 Console Mode:
If you prefer the console interface:
```bash
ConsoleUI ui(service);
ui.run();
```

---

## ⚙️ Possible Extensions
- Database integration (SQLite or MySQL)  
- Enhanced GUI with search bar and pagination  
- Statistics dashboard (e.g., books per genre)  
- User authentication system  

---

### 🏷️ License
This project is distributed for educational purposes only.  
Free to use and modify under MIT-like academic terms.
