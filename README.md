# Battleship

A terminal-based Battleship game written in C++. Built as a practice project while learning C++.

## Features

- **Solo mode** — four ships are hidden on a 4x4 board; find them all within eight misses
- **Vs Computer mode** — place your own fleet, then trade shots with the computer until one side is sunk
- Colored terminal output (ANSI escape codes)
- Input validation for non-numeric input, out-of-range coordinates, and repeated shots

## Build

```bash
g++ -std=c++17 -Wall -Wextra -o battleship battleship.cpp
```

Run it:

```bash
./battleship
```

On Windows:

```bash
g++ -std=c++17 -Wall -Wextra -o battleship.exe battleship.cpp
battleship.exe
```

## How to play

Pick a mode from the menu, then enter coordinates as a row followed by a column, each between 0 and 3.

Board symbols:

| Symbol | Meaning |
| --- | --- |
| `0` | Untouched cell |
| `S` | Your own ship (shown only on your board) |
| `X` (green) | Hit |
| `X` (red) | Miss |

In solo mode only misses count against you, so a hit costs nothing. Firing at a cell you already tried does not use up an attempt either.

## Notes

Colors rely on ANSI escape codes. These work in most modern terminals, including Windows Terminal and PowerShell on Windows 10 and later. Older Windows consoles may print the escape sequences as raw text.

## License

MIT
