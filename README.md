# High Resolution Calorimeter (HRC)

## Spis treści
1. [Opis projektu](#opis-projektu)
2. [Główne funkcjonalności](#główne-funkcjonalności)
3. [Struktura repozytorium](#struktura-repozytorium)
4. [Wymagania systemowe](#wymagania-systemowe)
5. [Instrukcja uruchomienia](#instrukcja-uruchomienia)
   - [Kompilacja i instalacja](#kompilacja-i-instalacja)
   - [Uruchomienie symulacji (tryb wsadowy)](#uruchomienie-symulacji-tryb-wsadowy)
   - [Uruchomienie wizualizacji (tryb interaktywny)](#uruchomienie-wizualizacji-tryb-interaktywny)
   - [Uruchomienie analizy w ROOT](#uruchomienie-analizy-w-root)
6. [Analiza kodu i zasady działania](#analiza-kodu-i-zasady-działania)
   - [Geometria i konstrukcja detektora (MyDetectorConstruction)](#geometria-i-konstrukcja-detektora-mydetectorconstruction)
   - [Lista procesów fizycznych (MyPhysicsList)](#lista-procesów-fizycznych-myphysicslist)
   - [Generator cząstek (MyPrimaryGeneratorAction)](#generator-cząstek-myprimarygeneratoraction)
   - [Akcje Run i Stepping (MyRunAction, MySteppingAction)](#akcje-run-i-stepping-myrunaction-mysteppingaction)
   - [Makro ROOT do analizy (analyzePhotonsC)](#makro-root-do-analizy-analyzephotonsc)
7. [Możliwe rozszerzenia](#możliwe-rozszerzenia)
8. [Kod źródłowy na GitHubie](#kod-źródłowy-na-githubie)

---

## Opis projektu
**High Resolution Calorimeter (HRC)** to projekt symulujący działanie kalorymetru wysokiej rozdzielczości typu "spagetti" w środowisku **Geant4**. Celem jest zbadanie procesu detekcji wysokoenergetycznych fotonów, które przechodząc przez warstwę absorbera (w naszym przypadku wolfram), wytwarzają kaskadę cząstek wtórnych. W symulacji szczególnie interesują nas fotony optyczne powstające na skutek promieniowania Czerenkowa w włóknach kwarcowych. Dane o energii i czasie dotarcia tych fotonów są zapisywane do pliku **ROOT** i następnie analizowane w makrze **ROOT**.

Dzięki tej symulacji możemy wyznaczyć:
- Sumaryczną energię wyemitowaną w postaci fotonów optycznych (proporcjonalną do energii pierwotnego fotonu).
- Pozycję trafienia fotonu (z analizy poprzecznego profilu kaskady elektromagnetycznej).

---

## Główne funkcjonalności
1. **Pełna geometria kalorymetru HRC** – definiujemy świat symulacji, absorber z wolframu, pre-absorber z miedzi oraz włókna kwarcowe (z otoczką PMMA).
2. **Implementacja procesu Cerenkova** – umożliwia generację fotonów optycznych we włóknach kwarcowych.
3. **Zapis danych do ntupla (ROOT)** – informacje o numerze zdarzenia, energii i czasie fotonu optycznego docierającego do końca włókna.
4. **Makro ROOT** – automatycznie tworzy histogramy energii i czasu fotonów, dopasowując skalę do zakresu danych.
5. **Możliwość wizualizacji** – tryb interaktywny w Geant4 pozwala obserwować trajektorie cząstek w detektorze.

---

## Struktura repozytorium
Poniżej przykładowa struktura katalogów i plików (może się nieco różnić w Twoim repozytorium):

```
.
├── CMakeLists.txt               # Plik konfiguracyjny CMake
├── My.cc                        # Główny plik źródłowy (funkcja main) uruchamiający symulację
├── include/
│   ├── MyAnalysis.hh
│   ├── MyDetectorConstruction.hh
│   ├── MyPhysicsList.hh
│   ├── MyPrimaryGeneratorAction.hh
│   ├── MyRunAction.hh
│   ├── MySteppingAction.hh
│   └── (inne .hh)
├── src/
│   ├── MyAnalysis.cc
│   ├── MyDetectorConstruction.cc
│   ├── MyPhysicsList.cc
│   ├── MyPrimaryGeneratorAction.cc
│   ├── MyRunAction.cc
│   ├── MySteppingAction.cc
│   └── (inne .cc)
├── run.mac                      # Przykładowy plik makr do uruchomienia w trybie wsadowym
├── init_vis.mac                 # Plik makr do wizualizacji
├── analyzePhotons.C             # Makro ROOT do analizy ntupla
├── PhotonHistograms.png         # Wynikowy plik graficzny z histogramami
└── README.md                    # Niniejszy plik z opisem projektu
```

---

## Wymagania systemowe
- **Geant4** (wraz z odpowiednimi bibliotekami fizycznymi, np. Qt, X11 lub OpenGL do wizualizacji).
- **ROOT** (do analiz i pracy z plikami `.root`).
- **CMake** (do konfiguracji i kompilacji projektu).
- Kompilator C++ (np. **gcc** lub **clang**).
- System operacyjny: Linux, macOS lub Windows (z MinGW / MSYS2).

---

## Instrukcja uruchomienia

### Kompilacja i instalacja
1. Upewnij się, że posiadasz zainstalowane Geant4 oraz ROOT i odpowiednio skonfigurowane zmienne środowiskowe (np. `source /ścieżka/do/geant4.sh`).
2. Sklonuj repozytorium lub pobierz paczkę z kodem.
3. W głównym katalogu projektu utwórz folder build:
   ```bash
   mkdir build
   cd build
   ```
4. Uruchom CMake, wskazując na katalog projektu (gdzie jest `CMakeLists.txt`):
   ```bash
   cmake ..
   ```
5. Skorzystaj z komendy:
   ```bash
   make -j4
   ```
   - Opcjonalnie zastąp `-j4` liczbą rdzeni procesora, jeśli chcesz przyspieszyć kompilację.

Po poprawnym skompilowaniu w folderze `build` pojawi się plik wykonywalny (np. `My` lub `HRC` w zależności od konfiguracji CMake).

### Uruchomienie symulacji (tryb wsadowy)
1. Przejdź do katalogu `build`:
   ```bash
   cd build
   ```
2. Uruchom symulację:
   ```bash
   ./My run.mac
   ```
   - Plik `run.mac` zawiera m.in.:
     - Liczbę zdarzeń (`/run/beamOn 100`),
     - Poziomy verbosów (`/control/verbose 1`),
     - Ewentualne komendy inicjujące.
   - Po zakończeniu symulacji w katalogu pojawi się plik `MyOutput.root`.

### Uruchomienie wizualizacji (tryb interaktywny)
1. Również w katalogu `build` wpisz:
   ```bash
   ./My
   ```
   - Jeżeli **nie** podasz pliku makr, Geant4 uruchomi się w trybie interaktywnym.
   - Możesz wtedy załadować plik `init_vis.mac`:
     ```plaintext
     /control/execute init_vis.mac
     ```
     aby ustawić parametry wizualizacji (np. typ okna OpenGL, ustawienie kamery, włączenie rysowania trajektorii).

2. W konsoli Geant4 możesz teraz wydawać dodatkowe komendy, a następnie:
   ```plaintext
   /run/beamOn 100
   ```
   aby uruchomić 100 zdarzeń i obserwować przebieg ich trajektorii na ekranie.

### Uruchomienie analizy w ROOT
1. Po zakończeniu symulacji plik wynikowy (`MyOutput.root`) będzie zawierał ntuple z danymi fotonów optycznych.
2. Uruchom ROOT:
   ```bash
   root
   ```
3. W konsoli ROOT wczytaj makro:
   ```cpp
   .x analyzePhotons.C
   ```
   - Makro otworzy `MyOutput.root`, odczyta drzewo `Photons` i wygeneruje histogramy energii oraz czasu fotonów optycznych.
   - Po zakończeniu tworzenia histogramów plik graficzny `PhotonHistograms.png` zostanie zapisany w katalogu bieżącym.

---

## Analiza kodu i zasady działania

### Geometria i konstrukcja detektora (MyDetectorConstruction)
- Określa rozmiary świata symulacji (World) oraz absorbera (blok wolframu o wymiarach 1×1×50 cm).
- Dodaje włókna kwarcowe (rdzeń z kwarcu, otoczka z PMMA) rozstawione w siatce 3×3.
- Definiuje właściwości optyczne (refrakcyjność, długość absorpcji, itp.).

### Lista procesów fizycznych (MyPhysicsList)
- Zawiera rejestrację podstawowych modeli EM, fizyki hadronowej oraz procesów optycznych (Czerenkow, absorpcja Rayleigha, rozpraszanie itp.).

### Generator cząstek (MyPrimaryGeneratorAction)
- Wytwarza pierwotny foton o energii 100 MeV skierowany na detektor.
- Możesz zmienić typ cząstki (np. elektron), energię, pozycję i kierunek wylotu.

### Akcje Run i Stepping (MyRunAction, MySteppingAction)
- **MyRunAction**:
  - Inicjuje analizę (tworzy menedżer analiz, ntuple).
  - Na końcu zapisuje dane do `MyOutput.root`.
- **MySteppingAction**:
  - Na każdym kroku sprawdza, czy cząstka to foton optyczny.
  - Rejestruje energię i czas, gdy foton dotrze do końca włókna.
  - Dodaje nowy wiersz do ntupla (`FillNtuple`).

### Makro ROOT do analizy (analyzePhotons.C)
- Otwiera plik `MyOutput.root`, pobiera drzewo (TTree) o nazwie `Photons`.
- Odczytuje wartości energii i czasu, tworzy histogramy z automatycznym dopasowaniem zakresu.
- Zapisuje wykresy do pliku PNG (np. `PhotonHistograms.png`).

---

## Możliwe rozszerzenia
1. **Pełna siatka włókien** – Zwiększenie liczby włókien (np. do kilkudziesięciu) umożliwi szczegółową analizę rozkładu poprzecznego kaskady elektromagnetycznej.
2. **Inne materiały** – Wymiana wolframu na inny materiał (ołowiane szkło, uran) lub zmiana włókien kwarcowych na scintillator (np. polistyren).
3. **Analiza poprzecznego profilu** – Dodanie kodu zliczającego fotony w każdym włóknie z osobna, aby zrekonstruować pozycję trafienia cząstki.
4. **Większe energie** – Sprawdzenie zachowania kalorymetru przy wyższych energiach (np. fotony 1 GeV).
5. **Wielowątkowość (MT)** – Aktywacja trybu wielowątkowego w Geant4 w celu przyspieszenia symulacji dużej liczby zdarzeń.

---

## Kod źródłowy na GitHubie
Pełny kod źródłowy projektu, wraz z plikami konfiguracyjnymi, makrami oraz dokumentacją w LaTeX, jest dostępny w repozytorium GitHub:

```
[Wstaw tutaj link do swojego repozytorium, np. https://github.com/uzytkownik/HRC-calorimeter]
```

Jeśli masz dodatkowe pytania lub sugestie, zapraszam do kontaktu lub do otworzenia *Issue* w repozytorium.

**Powodzenia w dalszych pracach!**

