# Letter Recognition App

## Spis treści
1. [Opis projektu](#opis-projektu)
2. [Główne funkcjonalności](#główne-funkcjonalności)
3. [Struktura repozytorium](#struktura-repozytorium)
4. [Wymagania systemowe](#wymagania-systemowe)
5. [Instrukcja uruchomienia](#instrukcja-uruchomienia)
   - [Instalacja zależności](#instalacja-zależności)
   - [Szkolenie modelu (`train.py`)](#szkolenie-modelu-trainpy)
   - [Uruchomienie aplikacji graficznej (`main.py`)](#uruchomienie-aplikacji-graficznej-mainpy)
6. [Analiza kodu i zasady działania](#analiza-kodu-i-zasady-działania)
   - [Plik `train.py` – trening i zapisywanie modelu](#plik-trainpy--trening-i-zapisywanie-modelu)
     - [Model `ConvNet`](#model-convnet)
     - [Funkcja `train_model`](#funkcja-train_model)
     - [Funkcja `evaluate_model`](#funkcja-evaluate_model)
     - [Funkcje zapisu/odczytu modelu (`save_checkpoint` i `load_checkpoint`)](#funkcje-zapisuodczytu-modelu-save_checkpoint-i-load_checkpoint)
     - [Funkcja `load_model_for_inference`](#funkcja-load_model_for_inference)
     - [Funkcja `get_data_loaders`](#funkcja-get_data_loaders)
     - [Funkcja `main()` w `train.py`](#funkcja-main-w-trainpy)
   - [Plik `main.py` – aplikacja okienkowa (GUI)](#plik-mainpy--aplikacja-okienkowa-gui)
     - [Klasa `ModernDrawingApp`](#klasa-moderndrawingapp)
     - [Struktura okna i elementów interfejsu](#struktura-okna-i-elementów-interfejsu)
     - [Proces rozpoznawania liter (`recognize`)](#proces-rozpoznawania-liter-recognize)
     - [Funkcja `main()` w `main.py`](#funkcja-main-w-mainpy)
7. [Możliwe rozszerzenia](#możliwe-rozszerzenia)

---

## Opis projektu
**Letter Recognition App** to aplikacja, która pozwala na rysowanie litery na wirtualnym płótnie (canvas) i rozpoznawanie tej litery przy pomocy wcześniej wytrenowanego modelu sieci neuronowej (CNN). Projekt opiera się na zbiorze danych [EMNIST](https://www.nist.gov/itl/products-and-services/emnist-dataset), który zawiera obrazy liter w formacie 28x28 pikseli.

Dzięki temu projektowi:
- Możesz przeprowadzić trening własnego modelu do rozpoznawania liter.
- Następnie uruchomić aplikację okienkową, w której narysujesz literę myszką, a sieć spróbuje zgadnąć, jaki to znak.

Projekt zawiera dwa główne pliki:
- **`train.py`**: Kod odpowiedzialny za trenowanie sieci neuronowej, walidację oraz zapisywanie modelu.
- **`main.py`**: Aplikacja graficzna (GUI) napisana w Tkinter, która wczytuje wytrenowany model i pozwala użytkownikowi rysować litery, a następnie je rozpoznawać.

---

## Główne funkcjonalności
1. **Trening modelu** – Skrypt `train.py` pobiera zbiór danych EMNIST (split `letters`) i przeprowadza proces uczenia głębokiej sieci neuronowej (CNN) z wykorzystaniem PyTorch.
2. **Zapisywanie i wczytywanie modelu** – Model zapisuje się do pliku `letter_model.pth`. Gdy plik istnieje, skrypt pozwala wczytywać istniejące wagi (kontynuacja nauki) lub korzystać z nich w trybie inference (w `main.py`).
3. **Aplikacja okienkowa** – `main.py` to prosty interfejs graficzny, gdzie:
   - Możesz rysować literę na czarnym tle za pomocą myszki.
   - Zmieniasz grubość pędzla (suwak).
   - Klikasz przycisk "Recognize" i otrzymujesz wynik w postaci trzech najbardziej prawdopodobnych liter wraz z ich prawdopodobieństwami.
4. **Możliwość czyszczenia rysunku** – Dzięki przyciskowi "Clear" można zresetować pole do rysowania.
5. **Obsługa spolszczonego zbioru liter** – W oryginalnym zbiorze EMNIST liter jest 26 (a tak naprawdę 27 klas). W kodzie zdefiniowano mapowanie `i -> chr(i + 96)` odpowiadające literom od `a` do `z`.

---

## Struktura repozytorium
W repozytorium znajdują się przykładowo następujące pliki i foldery:

- **`data/`** – Folder, gdzie automatycznie zostaną pobrane dane EMNIST przy pierwszym uruchomieniu treningu.
- **`letter_model.pth`** – Zapisany model PyTorch (czyli “wagi” sieci neuronowej). Pojawi się po zakończeniu treningu.
- **`main.py`** – Kod tworzący interfejs graficzny do rozpoznawania liter w czasie rzeczywistym.
- **`train.py`** – Kod do trenowania modelu i zapisywania/odczytu wag.
- **`README.md`** – Plik, który właśnie czytasz. Opisuje projekt.

---

## Wymagania systemowe
- Python 3.7+ (zalecane 3.9 lub wyższa).
- Biblioteki Python:
  - `torch` (PyTorch)
  - `torchvision`
  - `tkinter` (w większości systemów macOS/Windows jest wbudowane w instalację Pythona; w Linuxie może być wymagany dodatkowy pakiet `python3-tk`).
  - `Pillow`
  - `tensorboard` (opcjonalnie, jeśli chcesz monitorować trening).
  - Inne biblioteki wymienione w kodzie (np. `argparse`, `datetime`, `numpy` – w razie potrzeby).

Jeśli masz plik `requirements.txt`, możesz zainstalować wszystkie zależności poprzez:
```bash
pip install -r requirements.txt
```
lub zainstalować ręcznie:
```bash
pip install torch torchvision pillow tensorboard
```

**Uwaga**: Jeśli posiadasz kartę graficzną NVIDIA i sterowniki CUDA, PyTorch może korzystać z GPU w celu przyspieszenia treningu. W przeciwnym razie wszystko zadziała na CPU (będzie to jedynie dłuższe).

---
## Instrukcja uruchomienia

### Instalacja zależności
1. Upewnij się, że masz zainstalowanego Pythona w wersji co najmniej 3.7.
2. (Opcjonalnie) Aktywuj wirtualne środowisko (venv), aby zainstalować zależności lokalnie.
3. Zainstaluj wymagane biblioteki:

```bash
pip install torch torchvision pillow tensorboard
```
4. Upewnij się, że możesz importować wszystkie potrzebne pakiety w Pythonie (sprawdzisz, wpisując `python` i w interpretatorze `import torch`, itp.).


### Szkolenie modelu (`train.py`)
1. Jeśli **nie posiadasz** pliku `letter_model.pth` (czyli model nie jest jeszcze wytrenowany), uruchom w terminalu:

    ```bash
    python train.py
    ```
   - Skrypt automatycznie pobierze zbiór danych EMNIST (split `letters`) do folderu `data/`.
   - Rozpocznie się proces trenowania modelu sieci neuronowej – może to trochę potrwać (zależnie od CPU/GPU).
   - Po zakończeniu treningu powstanie plik `letter_model.pth`.

2. (Opcjonalnie) Możesz dostosować hiperparametry przez argumenty w wierszu poleceń, np.:
    ```bash
    python train.py --epochs 15 --batch_size 64 --lr 0.001
    ```

   - `--epochs`: liczba epok treningu.
   - `--batch_size`: rozmiar paczki danych.
   - `--lr`: współczynnik uczenia (learning rate).

3. Jeśli **masz** już plik `letter_model.pth`, to skrypt `train.py` automatycznie załaduje stare wagi i domyślnie sprawdzi, czy chcesz kontynuować trening. W kodzie sprawdzane jest, czy plik istnieje. Jeśli tak – ładuje się model, jeżeli nie – następuje trenowanie od zera.

### Uruchomienie aplikacji graficznej (`main.py`)

1. Po zakończeniu treningu (i posiadaniu pliku letter_model.pth) uruchom:
    ```bash
     python main.py
    ```
2. Otworzy się okienko aplikacji Tkinter:

    - Górny napis "Letter Recognition"
    - Pole rysowania (czarne tło 280x280 pikseli)
    - Przycisk "Clear" do czyszczenia płótna.
    - Przycisk "Recognize" do rozpoznania.
    - Suwak zmieniający grubość "pędzla".
    - Pole, w którym wyświetlą się wyniki rozpoznawania (top 3 przewidywania).

3. Sposób użycia:

   - Narysuj na płótnie jakąś literę (możesz klikać i przeciągać myszką, tak jakbyś rysował farbą).
   - Kliknij "Recognize".
   - W okienku "Most likely predictions:" pojawią się trzy najbardziej prawdopodobne litery wraz z wartościami `p = ...` (oznaczającymi pewność predykcji).

---
## Analiza kodu i zasady działania
Poniżej znajduje się bardziej szczegółowy opis najważniejszych elementów kodu.
### Plik `train.py` – trening i zapisywanie modelu
#### Model ConvNet
```python
class ConvNet(nn.Module):
    def __init__(self):
        super(ConvNet, self).__init__()
        self.conv1 = nn.Conv2d(1, 32, 3, 1)
        self.conv2 = nn.Conv2d(32, 64, 3, 1)
        self.batch_norm1 = nn.BatchNorm2d(32)
        self.batch_norm2 = nn.BatchNorm2d(64)
        self.dropout1 = nn.Dropout2d(0.25)
        self.dropout2 = nn.Dropout2d(0.5)
        self.fc1 = nn.Linear(9216, 256)
        self.fc2 = nn.Linear(256, 27)

    def forward(self, x):
        x = self.conv1(x)
        x = self.batch_norm1(x)
        x = F.relu(x)
        x = self.conv2(x)
        x = self.batch_norm2(x)
        x = F.relu(x)
        x = F.max_pool2d(x, 2)
        x = self.dropout1(x)
        x = torch.flatten(x, 1)
        x = self.fc1(x)
        x = F.relu(x)
        x = self.dropout2(x)
        x = self.fc2(x)
        return F.log_softmax(x, dim=1)
```
- Klasa `ConvNet` dziedziczy po `nn.Module`, co oznacza, że jest to sieć neuronowa w PyTorch.
- Składa się z:
  - Dwóch warstw konwolucyjnych (`conv1`, `conv2`) z filtrami 3x3.
  - Normalizacji BatchNorm (`batch_norm1`, `batch_norm2`) po każdej konwolucji w celu stabilizacji i przyspieszenia treningu.
  - Warstw Dropout (`dropout1`, `dropout2`), które pomagają uniknąć przeuczenia (overfittingu).
  - Warstw w pełni połączonych (ang. fully connected, `fc1` i `fc2`).
- Ostatnia warstwa ma 27 neuronów wyjściowych (w EMNIST Letters bywa 26 lub 27 klas w zależności od konfiguracji; tutaj przyjęto 27).
- W metodzie `forward` wykonujemy operacje aktywacji ReLU, pooling (max_pool2d), flattenowanie i na końcu `log_softmax`, który jest często używany w PyTorch do zadań klasyfikacji.

#### Funkcja `train_model`
```python
def train_model(model, train_loader, val_loader, device, epochs=10, lr=0.0005, log_dir="logs"):
    optimizer = torch.optim.AdamW(model.parameters(), lr=lr)
    scheduler = torch.optim.lr_scheduler.StepLR(optimizer, step_size=5, gamma=0.1)
    writer = SummaryWriter(log_dir=log_dir)
    model.to(device)

    for epoch in range(epochs):
        model.train()
        total_loss = 0.0
        for batch_idx, (data, target) in enumerate(train_loader):
            data, target = data.to(device), target.to(device)

            optimizer.zero_grad()
            output = model(data)
            loss = F.nll_loss(output, target)
            loss.backward()
            optimizer.step()
            total_loss += loss.item()

            if batch_idx % 100 == 0:
                print(f'Epoch: {epoch}, Batch: {batch_idx}, Loss: {loss.item():.6f}')

        scheduler.step()
        val_acc = evaluate_model(model, val_loader, device)
        avg_loss = total_loss / len(train_loader)
        print(f'Epoch: {epoch} | Avg Train Loss: {avg_loss:.4f} | Val Accuracy: {val_acc:.4f}')

        # Log metrics to TensorBoard
        writer.add_scalar('Loss/train', avg_loss, epoch)
        writer.add_scalar('Accuracy/val', val_acc, epoch)

    writer.close()
    return model
```

- Przyjmuje model, DataLoadery (treningowe i walidacyjne), liczbę epok, LR (learning rate) i ścieżkę do logów.
- Używa optymalizatora `AdamW` i scheduler’a uczenia `StepLR`, który zmniejsza LR co pewną liczbę epok.
- Dla każdej epoki pętla przechodzi przez `train_loader`:
  - Odczytuje batch, przenosi go na `device` (CPU lub GPU).
  - Oblicza wyjście sieci, liczy funkcję straty `F.nll_loss`.
  - Wykonuje kroki backprop (optim.zero_grad -> loss.backward -> optim.step).
- Po zakończeniu każdej epoki – sprawdza dokładność na zbiorze walidacyjnym (`val_loader` przez `evaluate_model`) i loguje wyniki do TensorBoard.

#### Funkcja `evaluate_model`
```python
def evaluate_model(model, loader, device):
    model.eval()
    correct = 0
    total = 0
    with torch.no_grad():
        for data, target in loader:
            data, target = data.to(device), target.to(device)
            output = model(data)
            pred = output.argmax(dim=1)
            correct += pred.eq(target).sum().item()
            total += target.size(0)
    return correct / total
```
- Wyłącza grad (`model.eval()` i `with torch.no_grad()`).
- Przelicza dla każdego batcha, ile przewidywań jest trafnych (poprzez `output.argmax(dim=1).eq(target).sum()`).
- Zwraca dokładność (liczbę poprawnych / liczbę wszystkich próbek).

#### Funkcje zapisu/odczytu modelu (`save_checkpoint` i `load_checkpoint`)
```python
def save_checkpoint(model, optimizer, epoch, path='letter_model.pth'):
    checkpoint = {
        'model_state_dict': model.state_dict(),
        'optimizer_state_dict': optimizer.state_dict(),
        'epoch': epoch
    }
    torch.save(checkpoint, path)
    print(f"Model checkpoint saved to {path}")
```
- Zapisuje do słownika `checkpoint`:
  - Stan modelu (`model_state_dict`)
  - Stan optymalizatora (`optimizer_state_dict`)
  - Numer epoki
- Następnie wywołuje `torch.save(checkpoint, path)`.

```python
def load_checkpoint(model, optimizer, path='letter_model.pth'):
    if os.path.exists(path):
        checkpoint = torch.load(path, map_location=torch.device('cpu'))
        model.load_state_dict(checkpoint['model_state_dict'])
        optimizer.load_state_dict(checkpoint['optimizer_state_dict'])
        start_epoch = checkpoint['epoch'] + 1
        print("Model loaded from file, starting from epoch:", start_epoch)
        return model, optimizer, start_epoch
    else:
        print(f"No checkpoint found at {path}. Starting training from scratch.")
        return model, optimizer, 0
```
- Wczytuje `checkpoint = torch.load(path, map_location=torch.device('cpu'))`.
- Ustawia wagi modelu i stany optymalizatora.
- Zwraca zaktualizowany model, optimizer i epokę, od której można dalej trenować.

#### Funkcja `load_model_for_inference`
```python
def load_model_for_inference(path='letter_model.pth'):
    model = ConvNet()
    if os.path.exists(path):
        checkpoint = torch.load(path, map_location=torch.device('cpu'))
        if 'model_state_dict' in checkpoint:
            model.load_state_dict(checkpoint['model_state_dict'])
            print("Model weights loaded for inference.")
        else:
            raise RuntimeError("The checkpoint does not contain 'model_state_dict'. Make sure you provided the correct file.")
    else:
        print("No pre-trained model found. Please train the model first.")
    model.eval()
    return model
```
- To uproszczona wersja wczytywania modelu tylko do inferencji (pomija stany optymalizatora).
- Wczytuje wagi i ustawia tryb `model.eval()`, co wyłącza dropout itp.

#### Funkcja `get_data_loaders`
```python
def get_data_loaders(batch_size=128, augment=True):
    transform_list = []
    if augment:
        transform_list += [
            transforms.RandomRotation(10),
            transforms.RandomAffine(degrees=0, translate=(0.1, 0.1))
        ]
    transform_list += [
        transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))
    ]

    transform = transforms.Compose(transform_list)

    train_dataset = EMNIST('./data', split='letters', train=True, download=True, transform=transform)

    dataset_size = len(train_dataset)
    val_size = int(0.1 * dataset_size)
    train_size = dataset_size - val_size
    train_data, val_data = random_split(train_dataset, [train_size, val_size])

    train_loader = DataLoader(train_data, batch_size=batch_size, shuffle=True, num_workers=4, pin_memory=True)
    val_loader = DataLoader(val_data, batch_size=batch_size, shuffle=False, num_workers=4, pin_memory=True)

    return train_loader, val_loader
```

- Definiuje listę transformacji obrazów, m.in. `RandomRotation(10)`, `RandomAffine`.
- Wczytuje zbiór EMNIST w trybie `train=True` (split 'letters').
- Dzieli go na zbiór treningowy i walidacyjny w proporcjach 90% / 10%.
- Tworzy `DataLoader` dla każdej części (z `batch_size` i shuffle).
- Zwraca `(train_loader, val_loader)`.

#### Funkcja `main()` w `train.py`
```python
def main():
    parser = argparse.ArgumentParser(description="Train or evaluate the ConvNet model.")
    parser.add_argument('--epochs', type=int, default=10, help="Number of epochs for training.")
    parser.add_argument('--batch_size', type=int, default=128, help="Batch size for DataLoader.")
    parser.add_argument('--lr', type=float, default=0.0005, help="Learning rate.")
    parser.add_argument('--log_dir', type=str, default=f"logs/{datetime.now().strftime('%Y%m%d_%H%M%S')}", help="Directory for TensorBoard logs.")
    args = parser.parse_args()

    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print("Using device:", device)

    need_training = not os.path.exists('letter_model.pth')

    model = ConvNet()
    optimizer = torch.optim.AdamW(model.parameters(), lr=args.lr)

    if need_training:
        train_loader, val_loader = get_data_loaders(batch_size=args.batch_size)
        trained_model = train_model(model, train_loader, val_loader, device, epochs=args.epochs, lr=args.lr, log_dir=args.log_dir)
        save_checkpoint(trained_model, optimizer, args.epochs - 1)
    else:
        model, optimizer, start_epoch = load_checkpoint(model, optimizer)
```

- Używa `argparse` do wczytania argumentów wiersza poleceń (epoki, batch_size, LR).
- Sprawdza, czy istnieje l`etter_model.pth`. Jeśli nie, trenuje nowy model. Jeśli tak, wczytuje poprzedni stan.
- Domyślnie logi zapisuje do `logs/` z bieżącą datą i czasem (np.` logs/20250125_122000`).

### Plik `main.py` – aplikacja okienkowa (GUI)
#### Klasa `ModernDrawingApp`
```python
class ModernDrawingApp:
    def __init__(self, root, model):
        self.canvas = None
        self.root = root
        self.root.title("Letter Recognition App")
        self.root.geometry("500x600")
        self.root.configure(bg="#2B2B2B")  # Dark modern background

        self.model = model
        self.canvas_width = 280
        self.canvas_height = 280
        self.brush_size = 20

        self.idx_to_letter = {i: chr(i + 96) for i in range(1, 27)}

        self.create_widgets()
    ...
```

- Przyjmuje `root` – główne okno Tkinter i `model` – sieć neuronową wczytaną do inferencji.
- W `__init__` definiuje parametry rysowania, wymiary płótna, domyślny rozmiar pędzla `brush_size=20`.
- Tworzy słownik `idx_to_letter` mapujący indeksy (1..26) na litery `'a'..'z'`.

#### Struktura okna i elementów interfejsu
- **Nagłówek** (`ttk.Label` z tekstem "Letter Recognition").
- **Canvas** (`tk.Canvas`) o wymiarach 280x280, czarne tło. Tutaj użytkownik rysuje myszką.
- **Przyciski**:
  - "Clear" – czyści płótno (`clear_canvas()`),
  - "Recognize" – uruchamia proces rozpoznania (`recognize()`).
- **Suwak** (`ttk.Scale`) – pozwala ustawić grubość pędzla (1..40).
- **Etykieta wynikowa** (`result_label`) – wyświetla opis lub wyniki klasyfikacji.

#### Proces rozpoznawania liter (`recognize`)
```python
def recognize(self):
    img = self.get_canvas_image()
    img = img.rotate(90, expand=True)

    transform = transforms.Compose([
        transforms.Resize((28, 28)),
        transforms.Grayscale(),
        transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))
    ])

    image = transform(img).unsqueeze(0)

    self.model.eval()
    with torch.no_grad():
        output = self.model(image)
        probs = torch.softmax(output, dim=1)
        top3_prob, top3_idx = torch.topk(probs, 3, dim=1)

        results_text = "Most likely predictions:\n"
        for i in range(3):
            class_idx = top3_idx[0, i].item()
            prob_value = top3_prob[0, i].item()
            letter = self.idx_to_letter.get(class_idx, '?').upper()
            results_text += f"{i + 1}) {letter} (p={prob_value:.2f})\n"

        self.result_label.config(text=results_text)
```

1. Pobiera zawartość canvasu jako obraz PIL w metodzie `get_canvas_image()`:
   - Tworzy nowy obraz PIL w trybie `RGB` z tłem czarnym (280x280).
   - Rysuje białe linie na podstawie kształtów z canvasu (ich współrzędnych).
   - Odwraca obraz w poziomie (`Image.Transpose.FLIP_LEFT_RIGHT`) i następnie obraca o 90° (`img = img.rotate(90, expand=True)`), by dostosować orientację do modelu EMNIST.
2. Stosuje transformacje PyTorch:
   - `Resize((28, 28))`
   - `Grayscale()`
   - `ToTensor()`
   - `Normalize((0.1307,), (0.3081,))`
3. Dokonuje predykcji przez sieć neuronową w trybie `eval()`.
4. Oblicza `torch.softmax` i pobiera trzy najwyższe przewidywania (`torch.topk`).
5. Mapuje indeks na literę i wyświetla wyniki w polu tekstowym aplikacji.

#### Funkcja `main()` w `main.py`
```python
def main():
    model = load_model_for_inference('letter_model.pth')
    root = tk.Tk()
    app = ModernDrawingApp(root, model)
    root.mainloop()
```
- Ładuje model przez load_model_for_inference('letter_model.pth').
- Tworzy główne okno (tk.Tk()).
- Inicjalizuje ModernDrawingApp(root, model).
- Uruchamia pętlę główną Tkinter (root.mainloop()).

---
## Możliwe rozszerzenia
- **Obsługa większej liczby znaków**: Można spróbować rozpoznawać cyfry, znaki specjalne lub całe słowa.
- **Inny zbiór danych**: Możesz wytrenować model na innym zestawie znaków lub obrazów.
- **Usprawniony interfejs**: Dodać np. opcje zapisywania obrazów, rysowania różnych kolorów, automatyczne czyszczenie po rozpoznaniu itp.
- **Więcej warstw CNN**: Rozbudować sieć, dodać kolejne warstwy i eksperymentować z hyperparametrami.
- **Wizualizacje uczenia**: Użycie TensorBoard do szczegółowej analizy krzywych strat i dokładności w czasie rzeczywistym.
- **Użycie GPU**: Jeśli posiadasz kompatybilną kartę i zainstalowane CUDA, możesz przyspieszyć trening (`train.py`).
