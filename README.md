# Symulacja pandemii

## Kompilacja
W celu skompilowania projektu należy wywołać skrypt `build.sh`. Można mu przekazać argument `r` albo `release`,
żeby program został skompilowany z optymalizacjami. Jeżeli przekaże mu się argument `d`, `debug` albo nie
przekaże mu się żadnego argumentu wtedy program zostanie skompilowany w trybie debug. Skompilowany program
będzie umieszczony w `./target/<debug|release>/simulation`.

Aby usunąć wszystkie pliki kompilatora, skompilowane programy oraz katalog `plots/` można użyć skryptu `clean.sh`.

## Testy
Jeżeli CMake wykryje zainstalowaną bibliotekę GoogleTest, to dodatkowa zostaną skompilowane testy.
Ich plik wykonywalny będzie umieszczony jako `/target/<debug|release>/tests`.

## Używanie
Program `simulation` może zostać uruchomiony w 3 trybach: test, random, file. Kontroluje to flaga --type.
W trybie "test" program przeprowadza symulację dla 3 testowych osób. W trybie "random" losuje n osób w
mieście o rozmiarze 1.0. A w trybie "file" wczytuje początkowe informacje o osobach z podanego pliku.

Poszczególne klatki symulacji będą zapisywane w jako `plots/frame_xxxx.png` jeżeli flaga --save
zostanie przekazana do programu.

W celu zobaczenia wszystkich informacji o dostępnych opcjach wywołaj `simulation --help`.

## Struktura projektu
Projekt składa się z kilku plików i klas. Poniżej prezentuję najważniejsze klasy i w nich najważniejsze metody:
	- City: Klasa opisująca miasto. Jej dwie najważniejsze funkcje to `run_simulation` oraz statyczna
funkcja `from_config`.
	- Person: Klasa opisująca osobę. Zawiera informacje o jej położeniu, prędkości, promieniu zarażenia i stanie choroby.
Jej najważniejsza funkcja to `move`.
	- Vector2d: Klasa reprezentująca dwuwymiarowy wektor. Jej ważnymi funkcjami jest `distance` i `reflect`.
	- Line2d: Klasa reprezentująca linię prostą opisaną równaniem Ax + By + C = 0.
Jej ważnymi funkcjami są `point_position` i `intersection`.
	- SimulationBaseException: Bazowa klasa, po której wszystkie moje klasy wyjątków dziedziczą.

W funkcji main parsuję argumenty cli, tworzę obiekt City a następnie uruchamiam jego symulację.
