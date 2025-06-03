# TEALFuzz

(WIP)

## Installing AFL
TODO

## Transpiler usage
TODO

## Compilation
To compile the generated template, run:
```
afl-clang-fast -o fuzz_target FuzzAVM.generated.cpp -lstdc++
```
from the root folder.

## Starting a fuzzing campaign

To start a fuzzing campaign, once the fuzz target is generated and compiled,
run:
```
afl-fuzz -i seeds -o out -- ./fuzz_target
```
from the root folder.