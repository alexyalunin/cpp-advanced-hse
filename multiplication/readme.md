# Умножение чисел

Эта задача предназначена для знакомства с тестирующей системой. Вам предстоит реализовать функцию `Multiply`, которая перемножает 2 числа, в файле `multiplication.cpp`. Перед тем, как приступить к решению, настройте окружение согласно инструкции на вики.

### Структура задачи

В каждой задаче обычно есть как минимум 3-4 файла:

* `CMakeLists.txt` --- cmake-файл для сборки задачи.
* `test.cpp` --- сами тесты для задачи.
* `*.h` --- некоторый .h файл, который содержит объявления функций/классов, которые необходимо реализовать.
* `*.cpp` --- файлы, в которых вы должны реализовать то, что требуется в условии задачи (в данном случае `multiplication.cpp`).
* `.tester.json` --- файл с метаданными, которыми пользуется тестирующая система. Поле `allow_change` показывает,
  какие файлы можно менять.

Вам разрешено изменять только файлы, содержащие решение задачи, --- в противном случае система не примет вашу посылку.
Когда вы делаете посылку, система запускает `test.cpp`, и в случае успешного завершения задача вам засчитывается.

Обратите внимание, что в некоторых задачах `test.cpp` может либо вообще не содержать тестов, либо содержать не все тесты, которые будут на сервере --- об этом всегда сообщается в условии задачи.
В этом случае вы сами должны написать тесты и протестировать всю реализованную функциональность.

### Сборка

В целом любая IDE должна работать. Обращайтесь к Google, о том как настроить CMake проект в вашей IDE. С самим CMake мы ещё познакомимся. Ниже будут general инструкции для Linux & OSX и Windows.

### Линтеры

В работе с кодом в индустрии очень хорошо иметь линтер, который по заданным правилам приводит ваш код к единому стилю.
Мы будем использовать [C++ Google Code Style](https://google.github.io/styleguide/cppguide.html) с некоторыми различиями.

* Исключения будут разрешены
* Отступы в 4 пробела
* Максимальная длина строки в 100 символов
* Будем миксовать С++17 и C++20

Если ваш код не будет соответствовать кодстайлу, проверка будет падать с ошибкой. Чтобы привести его к стилю, используйте `clang-format -i file.cpp`, конфигурацию `clang-format` можно прочитать [здесь](../../.clang-format).

#### Linux & OSX

Создадим директорию со сборкой:
```sh
# В директории cpp-advanced-hse.
mkdir build
cd build
```

Запустим cmake:
```sh
cmake ..
```

Если на этом шаге вы получаете странные ошибки, проверьте что вы создали директорию `build` в корне репозитория, а не в директории с задачей.

При этом будет выведена некоторая служебная информация, в частности используемый компилятор. Для корректной сборки необходим `g++` версии не ниже 10.3 или `clang++` версии не ниже 11.0.0. Это потребуется для задач связанных с C++20. Некоторые задачи могут компилироваться и с меньшими версиями, ответственность лежит на вас. Сервер собирает всё с clang-11.

Наконец, соберем нашу задачу `test.cpp`:
```sh
make test_multiplication
```

Все задачи включены в один проект, поэтому если вы попытаетесь собрать проект целиком, могут возникнуть ошибки. Поэтому при сборке нужно указывать конкретный исполняемый файл, который вам нужен.

Теперь в `build` есть исполняемый файл `test_multiplication`, который и нужно запустить.

Обратите внимание, что помимо этого на сервере также осуществляется тестирование с включенными санитайзерами, которые проверяют наличие неопределенного поведения и некорректной работы с памятью в программе. 
Осуществить такую сборку просто:
```sh
# В директории cpp-advanced-hse.
mkdir asan_build
cd asan_build
cmake -DCMAKE_BUILD_TYPE=ASAN ..
make test_multiplication
```

Если вы реализуете решение в этой задаче просто как `a * b`, то тесты в asan-сборке должны упасть.
Изучите ошибку, поймите, почему тест упал, и исправьте решение.

#### Windows

Если вы используете mingw и cygwin, то все шаги такие же, разве что asan сборку вы сделать не сможете. 
Чтобы работать из Visual Studio 2017, сделайте следующее:

1. Выберите Open Folder и откройте папку с задачей.
2. Зайдите в Cmake/Change Cmake Settings.
3. В тех конфигурациях, которые вам нужны, поменяйте содержимое buildRoot на "${workspaceRoot}\\\\build".
4. Теперь вы можете использовать Cmake/Build all для сборки. Запускать из Visual Studio (без дебага) неудобно, это лучше делать из терминала. В директории с задачей после
сборки должна появиться папка build, где и находится исполняемый файл.

Обратите внимание, что компилятор MSVC может давать иные предупреждения/ошибки компиляции, чем те, которые вы можете получить на сервере, где используется clang.

### Замечания

Обратите внимание на модульность, возможно это в первый раз, когда вы пишете
решение не в одном файле. Мы ещё разберём cmake.

