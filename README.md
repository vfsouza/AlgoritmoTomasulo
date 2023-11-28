# Algoritmo de Tomasulo

# Classes

## `Instruction.cpp`

Contem os números dos registradores usados na instrução 
e a operação que será feita, podendo ser **ADD**, **SUB**, **MULT** e **DIV**.
Além disso, possui o clock da issue, do começo e do final da execução e do writeback.

## `RegisterStatus.cpp`

Fornece o **status** do registrador, se está ocupado ou não.

## `ReservationStation.cpp`

Contém os valores necessários para cada **Reservation Station**, como se está ocupada,
a operação que está sendo realizada, latência, etc.

# Implementação do algoritmo

## `AlgoritmoTomasulo.cpp`

É definido várias variáveis globais para determinar a arquitetura, como o número de reservation
stations de **ADD**, **MULT** e **DIV**, as latências de cada operação, o clock global, etc.
Também são feitas as funções para executar cada etapa do processo como a `ISSUE()`, `EXECUTE()`
e `WRITEBACK()`.
Também são definidas funções para auxiliar no print das tabelas de tempo, status do registradores, etc.

## Como executar

1. Nas primeiras linhas do código temos as variáveis `NumAddReservationStation = 1`, `NumMultReservationStation = 2` e `NumDivReservationStation = 2`.
Para mudar o número de Reservation Stations para cada operação deve primeiro mudar esses números e, depois, inicializar e adicionar mais
Reservation Stations ao vetor `ResStation`.
2. O arquivo a ser lido pelo código se chama `instrucoes.txt`, para mudar o número de intruções você deve seguir a formatação que ja está nele.
Além disso, deve-se garantir que o número de registradores do `.txt` e o número de registradores no vetor `RegisterStatus` seja o mesmo.
3. Agora, basta apenas compilar e executar o projeto.

## Exemplo de Execução

### Instruções usadas

| Instruções       |
|------------------|
| ADD F1, F2, F3   |
| ADD F4, F1, F5   |
| SUB F6, F7, F8   |
| MULT F9, F4, F10 |
| DIV F11, F12, F6 |
| MULT F8, F1, F5  |
| MULT F7, F2, F3  |

### Saída 

| Inst | Issue | Execute | WB | SystemClock |
|------|-------|---------|----|-------------|
| 0    | 1     | 2-5     | 6  |             |
| 1    | 7     | 8-11    | 12 |             |
| 2    | 13    | 14-17   | 18 |             |
| 3    | 14    | 15-26   | 27 |             |
| 4    | 15    | 19-30   | 31 |             |
| 5    | 28    | 29-40   | 41 |             |
| 6    | 32    | 33-44   | 45 | 45          |
