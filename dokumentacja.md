## Rozwiązania techniczne

Projekt został napisany w języku C++ z wykorzystaniem API graficznego OpenGL. Użyte zostały następujące biblioteki:
* GLFW
* GLEW
* assimp
* stb_image
* imgui 1.60

## Instruckja obsługi

Scena może być kontrolowana przez użytkownika przy użyciu wyświetlanego GUI, myszki i klawiatury. GUI pozwala na 
kontrolę następujących parametrów:
* typ cieniowania (_shading_), do wyboru spośród:
    * Phonga (_deferred rendering_)
    * Phonga (_forward rendering_)
    * Gouraud
    
* aktywna kamera, do wyboru spośród:
    * kamera kontrolowana przez użytkownika (możliwość przesuwania i obracania) pozwalająca na oglądanie całości sceny
    * kamera umieszczona pod poruszającym się śmigłowcem, z możliwością zmiany kierunku patrzenia przez użytkownika
    * kamera umieszczona na dachu jednego z budynków, podążająca za śmigłowcem
    
* gęstość mgły
* długość dnia (w klatkach)
* kolory nieba (dziennego i nocnego)

Przesuwanie kamery odbywa się poprzez klawisze _W_, _A_, _S_, _D_. Żeby obracać kamerę (zmieniać kierunek patrzenia),
należy kliknąć środkowy przycisk myszy. Aby ponownie zablokować kamerę, należy użyć prawego przycisku myszy. Możliwe 
jest również przybliżenie (lub oddalenie) obrazu, korzystając z klawiszy '=' oraz '-'.

Aplikacja pozwala też na kontrolę kierunku reflektora umocowanego do śmigłowca - w tym celu należy się posłużyć 
strzałkami.

### Argumenty wywołania

Program można uruchomić z następującymi argumentami:
* liczba punktowych źródeł światła - liczba całkowita, powinna być potęgą 2, domyślnie 16
* rodzaj podłoża - 1 dla podłoża asfaltowego wyższej jakości, 0 dla jednolitego podłoża metalowego; domyślnie 0
