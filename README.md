# High Resolution Calorimeter (HRC)

## Spis treści
1. [Opis projektu](#opis-projektu)
2. [Główne funkcjonalności](#główne-funkcjonalności)
3. [Struktura repozytorium](#struktura-repozytorium)
4. [Wymagania systemowe](#wymagania-systemowe)
5. [Instrukcja uruchomienia](#instrukcja-uruchomienia)
   - [Kompilacja i uruchomienie w trybie wsadowym](#kompilacja-i-uruchomienie-w-trybie-wsadowym)
   - [Uruchomienie z wizualizacją (tryb interaktywny)](#uruchomienie-z-wizualizacją-tryb-interaktywny)
   - [Analiza danych w ROOT](#analiza-danych-w-root)
6. [Analiza kodu i zasady działania](#analiza-kodu-i-zasady-działania)
   - [Geometria i konstrukcja detektora](#geometria-i-konstrukcja-detektora)
   - [Lista procesów fizycznych](#lista-procesów-fizycznych)
   - [Generator cząstek](#generator-cząstek)
   - [RunAction i SteppingAction](#runaction-i-steppingaction)
   - [Makra i wizualizacja](#makra-i-wizualizacja)
7. [Możliwe rozszerzenia](#możliwe-rozszerzenia)
8. [Uwagi końcowe](#uwagi-końcowe)

---

## Opis projektu
**High Resolution Calorimeter (HRC)** to projekt wykorzystujący środowisko [Geant4](https://geant4.web.cern.ch/) do symulacji działania kalorymetru wysokiej rozdzielczości typu "spagetti". W projekcie symulujemy wysokoenergetyczne fotony (np. 100 MeV), które w absorberze (wolfram) wytwarzają kaskadę elektromagnetyczną. W wyniku tej kaskady, w włóknach kwarcowych, generowane są fotony optyczne (Czerenkowa), a informacje o ich energii i czasie dotarcia do końca włókna zapisywane są w pliku ROOT do dalszej analizy.

---

## Główne funkcjonalności
1. **Symulacja detektora**: Definicja absorbera (wolfram), pre-absorbera (miedź), włókien kwarcowych z powłoką PMMA.
2. **Rejestracja fotonów optycznych**: Zbieranie informacji o energii i czasie w pliku `MyOutput.root`.
3. **Analiza w ROOT**: Automatyczne tworzenie histogramów energii i czasu fotonów optycznych (makra `analyzePhotons.C`, `analyze_ntuple.C` itp.).
4. **Tryb wizualizacji**: Możliwość podglądu trajektorii cząstek w trybie interaktywnym.
5. **Łatwa konfiguracja**: Pliki `.mac` (np. `run.mac`, `init_vis.mac`) pozwalają na szybkie dopasowanie parametrów symulacji.

---

## Struktura repozytorium
Poniżej uproszczony widok ważniejszych katalogów i plików:

```
.
├── build
│   ├── analyzePhotons.C
│   ├── init_vis.mac
│   ├── run.mac
│   ├── traj_vis.mac
│   ├── MyOutput.root        # Wynikowa baza danych (ntupla) z symulacji
│   ├── My                   # Plik wykonywalny po kompilacji
│   ├── PhotonHistograms.png # Przykładowy wykres z makra ROOT
│   └── ... (inne pliki i foldery CMake)
└── My
    ├── CMakeLists.txt
    ├── include
    │   ├── MyAnalysis.hh
    │   ├── MyDetectorConstruction.hh
    │   ├── MyPhysicsList.hh
    │   ├── MyPrimaryGeneratorAction.hh
    │   ├── MyRunAction.hh
    │   └── MySteppingAction.hh
    ├── My.cc                 # Główny plik źródłowy (funkcja main)
    └── src
        ├── MyDetectorConstruction.cc
        ├── MyPhysicsList.cc
        ├── MyPrimaryGeneratorAction.cc
        ├── MyRunAction.cc
        └── MySteppingAction.cc
```

- **build/**: Katalog, w którym odbywa się kompilacja oraz gdzie generowane są pliki wykonywalne i pliki wynikowe (np. `MyOutput.root`). Znajdują się tu także różne pliki związane z CMake.
- **My/**: Główna część projektu, zawierająca pliki `.cc` i `.hh`. W tym miejscu jest plik `CMakeLists.txt`, który definiuje, jak budowana jest aplikacja.

---

## Wymagania systemowe
- System Linux (np. Ubuntu, CentOS) lub inny zgodny z Geant4.
- Zainstalowane i skonfigurowane **Geant4** (z bibliotekami fizycznymi i interfejsami do wizualizacji).
- Zainstalowane **ROOT** (do analizy danych i tworzenia plików `.root`).
- **CMake** w wersji co najmniej 3.10.
- Kompilator C++ (np. `g++` lub `clang++`).

---

## Instrukcja uruchomienia

### Kompilacja i uruchomienie w trybie wsadowym
1. **Klony repozytorium** lub skopiuj projekt do lokalnego folderu.
2. Przejdź do katalogu `My` i utwórz folder build (lub skorzystaj z istniejącego `build` na wyższym poziomie – zależnie od preferencji):
   ```bash
   cd My
   mkdir build
   cd build
   cmake ..
   make -j4
   ```
   Po udanej kompilacji w folderze `build` pojawi się plik wykonywalny, np. `My`.
3. Uruchom symulację w trybie wsadowym, podając plik makr (np. `run.mac`):
   ```bash
   ./My ../run.mac
   ```
   lub, jeśli pliki `.mac` są w folderze `build`:
   ```bash
   ./My run.mac
   ```
4. Po zakończeniu programu w katalogu roboczym powinien pojawić się plik `MyOutput.root` z zapisanym ntuplem.

### Uruchomienie z wizualizacją (tryb interaktywny)
1. Uruchom program bez pliku makr:
   ```bash
   ./My
   ```
2. W oknie/terminalu Geant4 możesz teraz wpisać:
   ```plaintext
   /control/execute init_vis.mac
   /run/beamOn 100
   ```
   aby włączyć wizualizację geometrii i uruchomić symulację 100 zdarzeń. Zobaczysz trajektorie cząstek na ekranie.
3. Możesz również załadować inne makra, takie jak `traj_vis.mac`, aby dostosować ustawienia wyświetlania trajektorii.

### Analiza danych w ROOT
1. Upewnij się, że masz plik `MyOutput.root` (wynik symulacji).
2. Wejdź do katalogu, w którym znajduje się makro `analyzePhotons.C` (np. `build/`).
3. Uruchom ROOT:
   ```bash
   root
   ```
4. W konsoli ROOT wpisz:
   ```cpp
   .x analyzePhotons.C
   ```
   Makro otworzy plik `MyOutput.root`, odczyta TTree `Photons` i narysuje histogramy energii oraz czasu fotonów optycznych. Zapisze je np. jako `PhotonHistograms.png`.

---

## Analiza kodu i zasady działania

### Geometria i konstrukcja detektora
- **MyDetectorConstruction.cc**: Definiuje świat, absorber wolframowy (1×1×50 cm), pre-absorber (miedź) oraz włókna kwarcowe z powłoką PMMA (promień 0.625 mm, długość 50 cm). Tworzy siatkę 3×3 włókien.

### Lista procesów fizycznych
- **MyPhysicsList.cc**: Rejestruje główne modele EM, fizykę hadronową i optyczną (Czerenkow).

### Generator cząstek
- **MyPrimaryGeneratorAction.cc**: Ustawia pierwotny foton o energii 100 MeV, skierowany na detektor.

### RunAction i SteppingAction
- **MyRunAction.cc**: Tworzy menedżer analizy (ROOT), inicjuje ntuple `Photons` oraz zapisuje plik `.root`.
- **MySteppingAction.cc**: Na poziomie każdego kroku sprawdza, czy cząstka to foton optyczny, rejestruje jego energię i czas, a następnie zapisuje je do ntupla.

### Makra i wizualizacja
- **run.mac**: Przykładowy plik makr do wykonania symulacji w trybie wsadowym.
- **init_vis.mac** / **traj_vis.mac**: Pliki konfiguracyjne dla wizualizacji, określające typ okna (OpenGL) i sposoby rysowania trajektorii.
- **analyzePhotons.C** (oraz inne np. `analyze_ntuple.C`): Makra ROOT do odczytu pliku `MyOutput.root` i rysowania histogramów.

---

## Możliwe rozszerzenia
1. **Większa siatka włókien** – Rozszerzenie do kilkudziesięciu włókien, by lepiej uchwycić poprzeczny profil kaskady.
2. **Różne materiały** – Zmiana wolframu na inny materiał (np. ołów, miedź) lub włókien kwarcowych na scintillator.
3. **Analiza pozycji** – Zapis i analiza położenia fotonów optycznych w poszczególnych włóknach, by określić, gdzie dokładnie trafił foton pierwotny.
4. **Różne energie** – Test z fotonami 1 GeV lub elektronami w szerokim zakresie energetycznym.
5. **Tryb wielowątkowy** – Konfiguracja Geant4 do pracy z wieloma wątkami (
`/run/numberOfThreads`) i przyspieszenie symulacji dużej liczby zdarzeń.

---

## Uwagi końcowe
- Projekt został wykonany na potrzeby kursu z Geant4 i służy jako wstęp do zagadnień detekcji cząstek wysokich energii.
- W pliku *sprawozdanie* (LaTeX) zawarto szczegółowy opis implementacji, wyniki oraz wnioski.
- Wszelkie dodatkowe uwagi, błędy lub propozycje ulepszeń proszę zgłaszać w Issues (jeśli repo jest na GitHubie) bądź przez kontakt mailowy.

