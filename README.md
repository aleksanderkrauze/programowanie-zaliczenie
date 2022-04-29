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
zostanie przekazana do programu. W celu połączenia klatek w animację można wywołać skrypt `make_animation.sh`
(na maszynach na których jest zainstalowany program convert). Łączy on wszystkie klatki znajdujące się
w katalogu `plots/` w plik `plots/animation.gif`. Moża mu także przekazać jako argument nazwę pliku
z animacją (np. `make_animation.sh a.gif` połączy klatki w plik `a.gif`).

W celu zobaczenia wszystkich informacji o dostępnych opcjach wywołaj `simulation --help`.

## Znane bugi
Biblioteka matplotlib nie usuwa automatycznie stworzonych figur. Ponieważ nakładka matplotlibcpp jest dość
prymitywna nie posiada ona funkcji, która by pozwalała na ręczne zamknięcie figury. Oznacza to, że program
posiada stały i nie do uniknięcia **wyciek pamięci**. Na serwerze `pracownia.okwf.fuw.edu.pl` oznacza to,
że **program zostanie zabity** po wygenerowaniu ~600 klatek.

## Dependences
Poza oczywistą biblioteką `libpython`, której wymaga ten projekt jest jeszcze kilka innych.
Znajdują się one razem z kodem źródłowym tego programu pod kontrolą repozytorium git w katalogu `3rd_party/`.
Są one budowane a następnie statycznie linkowane do programu simulation.

### matplotlibcpp
Biblioteka [matplotlibcpp](https://github.com/lava/matplotlib-cpp) jest używana jako binding do biblioteki
matplotlib. Jest udostępniona na licencji MIT. Jej dokładna wersja nie jest mi znana.

### TCLAP
Biblioteka [tclap](http://tclap.sourceforge.net/) jest użyta do parsowania argumentów linii poleceń.
Jest udostępniona na licencji MIT. Wersja z której korzystam to `tclap-1.4.0-rc1`.
