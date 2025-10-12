# CombatTUItion
Тестовое задание по направлению Программирование для Леста Академии 2025
- Язык выполнения: C++
- Визуальное оформление: NCurses/PDCurses TUI

> [!IMPORTANT]  
> Windows версия работает менее стабильно, чем Linux

## Сборка
### Windows
```powershell
mkdir build
cd .\build\

cmake .. -G "Visual Studio 17 2022"
cmake --build .
```

### Linux
```console
$ mkdir build
$ cd build/

$ cmake .. -G Ninja
$ ninja
```
