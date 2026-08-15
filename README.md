# Atary — Terminal Arcade Game (C, ncurses)

A retro arcade-style game that runs in the terminal: pilot a small airplane, dodge and shoot down falling barriers, and rack up points before you get hit. Built in C with `ncurses` for a university programming course.

## Overview

The airplane sits at the bottom of the play field and moves left/right. Barriers fall from the top of the screen at random intervals; shooting one down scores a point (and shoots down any barriers connected to it in a row). The game speeds up as your score climbs, and it ends the moment a barrier reaches the airplane.

Before playing, the game asks for the player's first name, last name, and username, and displays them alongside the live score throughout the round.

## Controls

| Key | Action |
|-----|--------|
| `m` / `M` | Move airplane right |
| `b` / `B` | Move airplane left |
| `Space` | Shoot |
| `p` / `P` | Pause / resume |
| `s` | Start the game (after entering player info) |
| `q` / `Q` | Quit after game over |

## How It Works

- The play field is a fixed-size character grid (`width` × `height`) that's redrawn each frame using `ncurses` colored blocks.
- Falling barriers are inserted at random columns on a timer and shifted down each tick; shots fired by the player move upward and clear any barrier they hit.
- A non-blocking keyboard read (`kbhit`, implemented via raw terminal mode + `fcntl`) lets the game keep animating while waiting for input.
- Collision between the airplane and a barrier ends the game and shows the final score.

## Tech Stack

- C
- `ncurses` (rendering, color pairs, windows)
- POSIX terminal I/O (`termios`, `fcntl`) for non-blocking key detection
- Linux environment

## Repository Structure

```
.
├── atary.c    # Full game source — screen logic, game loop, input handling
└── README.md
```

## Building & Running

Requires `ncurses` development headers.

```bash
# Install ncurses (Debian/Ubuntu)
sudo apt-get install libncurses5-dev libncursesw5-dev

# Compile
gcc atary.c -o atary -lncurses

# Run
./atary
```

For the colors to render correctly, run it in a terminal that supports ANSI colors (most modern terminal emulators do).

## Notes

This was a course assignment focused on practicing 2D arrays, structs, and real-time terminal rendering/input in C — it isn't intended as production software.
