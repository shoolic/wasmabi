# Wasmabi

TKOM – Dokumentacja końcowa

Maciej Szulik, semestr letni 2020

## Zadanie

Celem projektu jest stworzenie prostego języka ogólnego przeznaczenia, który będzie kompilowany do LLVM Intermediate Representation, a następnie docelowo do WebAssembly.

## Opis języka

- statycznie typowany, obsługujący zmiennoprzecinkowy, całkowitoliczbowy i tekstowy typ danych,
- pozwala na tworzenie i wielokrotne przypisywanie zmiennych o zadanym typie widocznych jedynie wewnątrz bloku { … }, w którym zostały zdefiniowane,
- instrukcja pętli loop,
- instrukcja warunkowa if,
- instrukcja warunkowa select zwracająca wartość,
- instrukcja print wypisująca wartość,
- wielolinijkowe komentarze zaczynające się i kończące znakiem @,
- funkcje z parametrami, wywoływane zarówno jako pojedyncza instrukcja, jak i jako wartość,
- operacje matematyczne z priorytetami i nawiasowaniem (binarne: dodawanie, odejmowanie, mnożenie, dzielenie, potęgowanie i unarne: negacja),
- operacje logiczne z priorytetami i nawiasowaniem, operatory logiczne: i, lub, negacji, 
- operatory relacji: mniejszy, większy, mniejszy równy, większy równy, równy, różny od,
- wszystkie instrukcje muszą być zawarte w obrębie definicji funkcji.

## Wejście i wyjście

### Wymagania przed użyciem

Zainstalowane:

- `clang 10+`
- `wasi-sdk` [https://github.com/WebAssembly/wasi-sdk](https://github.com/WebAssembly/wasi-sdk)

### Opis
Program posiada następujące opcje:

```
-h [ --help ]                         Show help
-t [ --target ] arg                   Specify a compilation target, possible 
                                      values: wasm, native (input file must 
                                      include a main function)
-d [ --dump-ast ]                     Dump AST to yourSource.ast file
--wasi-sysroot arg (=/opt/wasi-sdk/share/wasi-sysroot)
                                      wasi sysroot path
-i [ --input ] arg                    input file *.wa
```



Na podstawie pliku wejściowego `.wa` program generuje:

- plik postaci pośredniej LLVM IR `.ll` (zawsze)
- plik określający odwzorowujący strukturę AST (na żądanie poprzez opcję `-d`)
- plik binarny WebAssembly `.wasm` (domyślnie lub poprzez opcję `-t wasm`)
- plik wykonywalny `.out` (na żądanie poprzez opcję `-t native`)

### Target
Dostępne są dwa targety kompilacji:

- native
- wasm

#### Native

Kompiluje LLVM IR do postaci pliku wykonywalnego na danej maszynie. Wewnątrz program używa do tego polecenia:

```clang -lm -o test.out test.ll```

Plik wejściowy `test.wa` musi posiadać funkcję z nagłówkiem `fn main(): int`, żeby skompilować go do tej postaci.


#### Wasm

Kompiluje LLVM IR do postaci pliku binarnego `*.wasm`. Wewnątrz program używa do tego polecenia:

```
clang --target=wasm32-unknown-wasi --sysroot=/opt/wasi-sdk/share/wasi-sysroot
 -nostartfiles -Wl,--no-entry -Wl,--export-all -o test.wasm test.ll;
```

Konieczne jest tutaj posiadanie zainstalowanego `wasi-sdk` (domyslnie w folderze `/opt/wasi-sdk/share/wasi-sysroot`).

Opcja `wasmabi --wasi-sysroot path` służy do wskazania innej niż domyślna ścieżki.

Zaleca się (zostało to przetestowane) uruchamianie plików `.wasm` przy pomocy maszyny wirtualnej [wasmtime](https://wasmtime.dev).

##### Przykłady użycia z `wasmtime`


Dla programu wejściowego `test.wa`:
```
fn add(a: int, b: int) {
  return a + b;
}
```

wywołaj komendę:

```wasmtime test.wasm --invoke add 1 2```

oczekiwany output:

```
warning: using `--invoke` with a function that takes arguments is experimental and may break in the future
warning: using `--invoke` with a function that returns values is experimental and may break in the future
3
```


### Przykłady użycia `wasmabi`

```
> wasmabi --target native --dump-ast test.wa
> ls
test.wa
test.ast
test.ll
test.out
```

```
> wasmabi test.wa
> ls
test.wa
test.ll
test.wasm
```





## Przykładowy kod

### Przykład 1 - ogólny

```
fn calc(base: int, exponent: int): int {
  return select {
    base^exponent + 1 when exponent > 0 and not base < 0,
    0 otherwise
  };
}

fn printer(): void {
  print "tekst";
}

fn main(): int {
  let x: int = calc(2, 3);

  {
    let y: int = 2;
    print calc(x, y);
  }

  printer();

  return 0;
}
```

### Przykład 2 - select

Przykład ilustruje wyłącznie konstrukcję, a nie cały kod.

```
let x: string = select {
  "Sukces" when score >= 75,
  "Moze byc" when score >= 50,
  "Porazka" otherwise
}
```

### Przykład 3 - loop

Przykład ilustruje wyłącznie konstrukcję, a nie cały kod.

```
loop x > 0 and y != 4 {
  print x;
  x = x - 1;
}
```

### Przykład 4 - if i operacje logiczne

Przykład ilustruje wyłącznie konstrukcję, a nie cały kod.

```
if not (x >= 0 or y > 10) and x != -5 {
  print "Zaakceptowano";
}
```

### Przykład 5 - operacje matematyczne
```
@ rocket math @
x = (2.1+3.7) * -0.6;
```
## Gramatyka

### Komentarz

Kod źródłowy zostanie poddany wstępnej obróbce polegającej na usunięciu białych znaków (z wyjątkiem string_literal i komentarzy), stąd nie zostały one uwzględnione w gramatyce. Został użyty również specjalny symbol printable_asii_character oznaczający dowolny znak ASCII, który można wyświetlić.

### Gramatyka

```
program = { function_definition };

function_definition = "fn", identifier, "(", function_params ")", function_return_type, body ;

function_params = [ identifier, type, { ","  identifier, type}] ;

body = "{", { statement | body } "}" ;

statement = (
  loop_statement 
  | if_statement 
  | return_statement
  | print_statement
  | variable_definition_statement 
  | variable_assignment_statement 
  | function_call_statement
  ), ";" ;

function_call_statement = identifier, "(", { value_expr, { ",", value_expr }}")" ;

loop_statement = "loop", logic_expr, body;
if_statement = "if", logic_expr, body ;
return_statement = "return", value_expr ; 
print_statement = "print", value_expr ;

variable_definition_statement = "let", identifier, type, [ assignment ] ;

variable_assignment_statement = identifier, assignment ;

assignment = assignment_operator, value_expr ;
assignment_operator = "=" ;

value_expr =  number_expr | logic_expr | select_expr
| function_call_statement | identifier | integer_literal 
| flaot_literal | string_literal ;

select_expr = "select", "{", 
  { value_expr, "when", logic_expr, "," },
  value_expr, "otherwise",
"}" ;


logic_expr = [logic_negation_operator], logic_expr,
| logic_expr, {logic_expr_operator, logic_expr}
| value_expr
| "(", logic_expr, ")" ;

logic_expr_operator = logic_operator | comparison_operator ;
logic_operator = "and" | "or" ;
logic_negation_operator = "not" ;
comparison_operator = "<" | "<=" | ">" | ">=" | "==" | "!=" ;

number_expr = [number_negation_operator], number_expr 
| number_expr, { number_operator, number_expr }
| value_expr
| "(", number_expr, ")" ;

number_operator = "+" | "-" | "*" | "/" | "^" ;
number_negation_operator = "-" ;

float_literal = integer_literal, ".", {digit} ;
integer_literal = "0" | ([ number_negation_operator ], non_zero_digit, { digit }) ;

function_return_type = type_operator, (type_name | "void"); 
type = type_operator, type_name ;
type_operator = ":" ;
type_name = "int" | "float" | "string";

identifier = letter, { letter | digit | "_"} ;

comment = "@", {? printable_asii_character ?}, "@" ;

string_literal = '"',  {? printable_asii_character ? - '"'}, '"' ;


digit = "0" | non_zero_digit ;
non_zero_digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;

letter = "A" | "B" | "C" | "D" | "E" | "F" | "G"
       | "H" | "I" | "J" | "K" | "L" | "M" | "N"
       | "O" | "P" | "Q" | "R" | "S" | "T" | "U"
       | "V" | "W" | "X" | "Y" | "Z" | "a" | "b"
       | "c" | "d" | "e" | "f" | "g" | "h" | "i"
       | "j" | "k" | "l" | "m" | "n" | "o" | "p"
       | "q" | "r" | "s" | "t" | "u" | "v" | "w"
       | "x" | "y" | "z" ;
```

## Implementacja

### Język

- `C++`

### Użyte narzędzia i biblioteki

- `CMake 3.16+`
- `Boost 1.70+`
- `LLVM 10+`
- `clang 10+`
- `wasi-sdk` [https://github.com/WebAssembly/wasi-sdk](https://github.com/WebAssembly/wasi-sdk) - konieczne do kompilowania do plików binarnych `.wasm`

### Moduły

#### Compiler

Centralny moduł programu zarządzający procesem kompilacji i innymi modułami.
Kiedy Generator stworzy LLVM IR, Compilator tworzy subproces wywołując `clang` z odpowiednimi opcjami.

Konstruktor wymaga:

- `std::string filename_` nazwa pliku wejściowego
- `std::string target_` nazwa targetu kompilacji (`wasm` lub `native`)
- `std::string wasiSysrootPath_,` ścieżka do `wasi-sdk`
- `bool dumpAST_` zmiennaokreślająca czy zapisać `AST`

Publiczne funkcje:

- `void compile()` dokonuje właściwej kompilacji, inicjalizuje moduły:
  - `SourceController`
  - `ErrorHandler`
  - `Lexer`
  - `Parser`
  - `Generator`

#### Errors

Moduł odpowiada za obsługę błędów.

##### ErrorHandler
Klasa odpowiada za wyświetlanie komunikatów z błędami. 

##### ErrorTypes
Zbiór klas odpowiadających różnego rodzaju wyjątkom, które mogą wystąpić.

Podstawowe grupy:

- `GeneralError`
  - `NoSourceFileError`

- `LexicalError`
  - `UnexpectedZeroError`
  - `ExtraDotError`
  - `ExpectedDigitErro`
  - `InvalidTokenError`
  - `NoCommentEndError`
  - `NoQuoteEndError`

- `SyntaxError`
  - `UnexpectedToken`
  - `UnknownFunRetType`
  - `UnknownVarRetType`
  - `StatementError`
  - `StatmentWithIdentifierError`
  - `VariableDefinitionStatementError`
  - `SelectExpressionError`
  - `ValueExpressionNudError`
  - `ValueExpressionLedError`
  - `UnexpectedTokenValExprError`

- `GeneratorError`
  - `StringAsOperandError`
  - `UndefinedFunctionError`
  - `UndefinedVariableError`
  - `VariableRedefintionError`
  - `FunctionReturnValueMismatchError`
  - `VoidFunretValueError`
  - `UnpritableValueError`
  - `VarAssignTypeMismatch`
  - `StringAsConditionError`

#### SourceController

Moduł zarządzający wejściem programu. Umożliwia pobieranie, podglądanie kolejnych znaków, sprawdzenie czy plik się skończył. Udostępnia także informacje o ścieżce pliku, zawartości i numeru przetwarzanej linii, kolumny, pozycji i znaku.

Konstruktor wymaga:

- `std::istream &sourceStream_` strumień danych wejściowych
- opcjonalnie `std::string sourcePath_` bezwzględna ścieżka do pliku wejściowego

#### Lexer

Moduł odpowiadający za analizę leksykalną, przetwarzający znaki, zamieniający je na tokeny poprzez funkcję `nextToken`.

Konstruktor wymaga:

- `SourceController &sourceController_`
- `ErrorHandler &errorHandler_` 

Publiczne funkcje:

- `const Token nextToken()`

#### Parser

Moduł odpowiada za analizę syntaktyczną, pobiera kolejne tokeny, tworzy z nich AST i poprzez funckję `parse` udostępnia dalej.

Konstruktor wymaga:

- `Lexer &lexer_`
- `ErrorHandler &errorHandler_`

Publiczne funkcje:

  - `std::unique_ptr<Program> parse()`

#### Generator

Moduł odpowiada za generację postaci pośredniej LLVM IR oraz za analizę semantyczną (kontrola typów i nazw).

Konstruktor wymaga:

- `std::ostream &output_` output na LLVM IR
- `std::string moduleName_` potrzebne do ustawienia nazwy modułu LLVM IR
- `std::string sourceFilePath_` ścieżka pliku wejściowego (na potrzeby wygenerowania LLVM IR ze wskazaniem na tę ścieżkę)

Publiczne funkcje:

- `void gen(Program &node)`
- i inne konieczne ze względu na wzorzec wizytora

#### Tokeny

Tokeny posiadają swój typ i wartość reprezentowaną przez `std::variant<std::string, int, float>`.

Wyszczególnione są następujące typy tokenów:

- Token koniec strumienia:
  - `Eof`

- Literały:

  - `StringLiteral`
  - `IntLiteral`
  - `FloatLiteral`

- Typy:
  - `StringTypename`
  - `IntTypename`
  - `FloatTypename`
  - `VoidTypename`

- Identyfikator (zmiennych, funkcji):
  - `Identifier`

- Średnik:
  - `Semicolon`

- Nawiasy okrągłe:
  - `OpeningParenthesis`
  - `ClosingParenthesis`

- Nawiasy klamrowe:
  - `OpeningBracket`
  - `ClosingBracket`

- Przecinek:
  - `Comma`

- Operator przypisania:
  - `AssignmentOperator`

- Operator typu:
  - `TypeOperator`

- Operatory arytmetyczne:
  - `AddOperator`
  - `SubOperator`
  - `MulOperator`
  - `DivOperator`
  - `PowOperator`

- Operatory logiczne:
  - `AndOperator`
  - `OrOperator`
  - `NotOperator`

- Operatory porównania:
  - `Equals`
  - `NotEquals`
  - `Greater`
  - `GreaterOrEqual`
  - `Less`
  - `LessOrEqual`

- Token definicji zmiennych:
  - `Let`

- Token instrukcji warunkowej:
  - `If`

- Token pętli:
  - `Loop`

- Token instrukcji wypisania:
  - `Print`

- Token definicji funkcji:
  - `Fn`

- Token instrukcji powrotu z funkcji:
  - `Return`

- Token instrukcji select:
  - `Select`

- Token instrukcji warunkowej wewnątrz select:
  - `When`

- Token domyślnej instrukcji wewnątrz select:
  - `Otherwise`

- Token oznaczający błąd:
  - `Invalid`


#### Węzły (nodes)

Węzły są strukturami tworzonymi przez Parser w procesie budowania drzewa.

Dostępne są następujące rodzaje węzłów:

- `Node` abstrakcyjna struktura bazowa 
- `Program` reprezentuje całość drzewa, czyli program 
- `Literal` (String, Int, Float)
- `VariableType` (String, Int, Float)
- `FunctionType` typy funkcji (wartość zwracana) (String, Int, Float, Void)
- `Block` blok kodu (ograniczony nawiasami klamrowymi), składa się z innych `Block` lub `Statement`
- `FunctionDefinition`
- `FunctionDefinitionParameter` deklaracja parametru w definicji funkcji
- `SelectExpressionCase` klasa reprezentująca przypadek w wyrażeniu `select`

Klasy dziedziczące po `ValueExpression` reprezentujące wyrażenie:

- `FunctionCallExpression`
- `IdentifierAsExpression`
- `UnaryExpression`
- `BinaryExpression`
- `SelectExpression`

Klasy dziedziczące po klasie `Statement` reprezentujące instrukcję:

- `LoopStatement`
- `IfStatement`
- `ReturnStatement`
- `PrintStatement`
- `VariableDefinitionStatement`
- `VariableAssignmentStatement`
- `FunctionCallStatement`


#### Helpers

##### OstreamModifieir

Umożliwa używanie kolorów i pogrubienia w konsoli.

##### VariantOstream

Umozliwa wypisanie `std::variant` na strumień.

#### Visitor

Abstrakcyjna klasa wizytora. 

##### PrintVisitor

Wypisuje drzewo AST na podany w konstruktorze strumień.

##### ParenthesisPrintVisitor

Działa jedynie dla węzłów typu `ValueExpression` i zwraca ich nawiasową postać.


## Zarządzanie projektem

### Kompilacja

#### Release

```
./create_configs.sh;
cd build/Release;
make;
cd ../..;
./bin/Release/wasmabi tests/examples/preDocs1.wa;
```

#### Debug

```
./create_configs.sh;
cd build/Debug;
make;
cd ../..;
./bin/Debug/wasmabi tests/examples/preDocs1.wa;
```

### Testy

Testy powstały z wykorzystaniem Boost Unit Test Framework.

#### Uruchomienie testów

```
./create_configs.sh;
./run_tests.sh;
```

### Output z programu w czasie testów
W `tests/Helpers/NullOstream.hpp` zdefiniowana jest nazwa`PREVENT_PRINTING`. Unieważnij (undef) ją, żeby umożliwić wyświetlenie outputu z programu w czasie testów. 

## Dodatkowe uwagi
### Linkowane funkcje 
Generator, jeśli istnieje taka konieczność, dołącza deklaracje funkcji, które Compilator później linkuje.

Możliwe funkcje to:

- printf 
- powf

### Wypisywanie na ekran

Za pomocą instrukcji `print value;` można wypisać wartość na ekran. Dopuszczalne do wypisania są wartości typu `Int`, `Float` i `String`. Automatycznie dodawany jest znak nowej linii.

### Potęgowanie 

Wartości podstawy i wykładnika zawsze sprowadzane są do typu `Float`, a następnie wywoływana jest funkcja `powf`.

### Priorytety operatorów

- `AddOperator` 10
- `SubOperator` 10
- `MulOperator` 20
- `DivOperator` 20
- `PowOperator` 40
- `Greater` 50
- `GreaterOrEqual` 50
- `Less` 50
- `LessOrEqual` 50
- `Equals` 50
- `NotEquals` 60
- `OrOperator` 60
- `AndOperator` 60
- `NotOperator` 60


### Ciągi znaków
Ciągi znaków są stałe. Generator tworzy je jako zmienne globalne unikając powtórzeń tych samych wartości.


### Zmienne

Zmienne niezainicjalizowane są inicjalizowane przez Parser podstawowymi wartościami podstawowymi dla danego typu:

- `String` - `""`
- `Int` - `0`
- `Float` - `0.0`

### Typy

- Odwzorowanie typów na LLVM IR:

  - `String` - `*i8`
  - `Int` - `i32`
  - `Float` - `float` 

- Konflikty typów
  - Typ `String` nie może wystąpić w kontekście binarnego i unarnego wyrażenia.

  - Dla binarnego wyrażenia o niezgodnych typach następuje rzutowanie do `Float`.

  - Rezultat operacji logicznej i porównania rzutowany jest do typu `Int`.

### Instrukcja return

Na potrzebny poprawności LLVM IR do każdej funkcji na jej końcu dodawana jest instrukcja zwracająca wartość podstawową danego typu.

