# Polution Viewer

**Polution Viewer** to aplikacja desktopowa stworzona w Qt 6.8.2, służąca do wizualizacji danych dotyczących zanieczyszczenia powietrza. Projekt został opracowany jako część projektu uniwersyteckiego.

## Technologie

- **Qt 6.8.2** – interfejs użytkownika (QML) i logika aplikacji (C++)
- **CMake** – system budowania projektu
- **QML** – warstwa prezentacji

## Budowanie projektu

### Wymagania wstępne

- Qt 6.8.2
- CMake 3.16 lub nowszy
- Kompilator C++17 (np. GCC, Clang lub MSVC)

### Instrukcje budowania

```bash
git clone https://github.com/Kalibrm/polution-viewer.git
cd polution-viewer
mkdir build
cd build
cmake ..
cmake --build .
```

---
This project uses the Qt framework, which is licensed under the GNU General Public License v3. You can find the full Qt license [here](https://www.qt.io/terms-conditions/#gpl).
