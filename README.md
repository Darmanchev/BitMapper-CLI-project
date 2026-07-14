# BitMapper CLI

BitMapper is a C++ command-line image editor for the ASCII Netpbm formats **PBM (P1)**, **PGM (P2)** and **PPM (P3)**. I built it as an object-oriented programming project to work with polymorphism, manual object ownership and file parsing without using an image-processing library.

## Features

- load one or more images into a session;
- add images to the active session;
- negative, grayscale and monochrome transformations;
- rotate left/right and flip horizontally/vertically;
- paste an image into another image of the same format;
- automatically extend the destination canvas when needed;
- save all session images under generated or custom names;
- list files in the current session.

## Design choices

`Image` defines the common interface, while `PBMImage`, `PGMImage` and `PPMImage` contain format-specific pixels and transformations. `Session` groups loaded images and `SessionManager` owns the session lifecycle. `CommandHandler` is kept separate so parsing terminal commands does not become part of the image classes.

The most difficult part was writing the same operation for three different pixel representations: boolean, grayscale and RGB. Paste was another edge case because the target may have to grow while keeping its old pixels in place. Working without an external library made those details visible.

## Build

Requirements: a compiler with C++17 support.

```bash
git clone https://github.com/Darmanchev/BitMapper-CLI-project.git
cd BitMapper-CLI-project
g++ -std=c++17 -Wall -Wextra *.cpp -o bitmapper
./bitmapper
```

## Commands

| Command | Description |
| --- | --- |
| `load file1.ppm file2.ppm` | start a session with images |
| `add file.pgm` | add an image to the current session |
| `negative` | invert pixel values |
| `grayscale` | convert colors to grayscale values |
| `monochrome` | convert to black and white |
| `rotate left` / `rotate right` | rotate all images |
| `flip top` / `flip left` | vertical or horizontal flip |
| `paste src.ppm dest.ppm 20 10` | paste at `(x, y)`; formats must match |
| `list session` | show files in the active session |
| `save` | save using timestamped names |
| `saveas out1.ppm out2.ppm` | save using custom names |
| `exit` | leave the program |

Example files are included in the repository, so a quick session can start with:

```text
load test3.ppm
negative
rotate right
saveas result.ppm
exit
```

## Current status and next steps

The P1/P2/P3 parser and main transformations are implemented. Undo/redo is not complete: the current history code does not restore the previous pixel state reliably, so it should not yet be treated as a finished feature. The next steps are snapshot/command-based undo, tests for malformed files and transformations, binary P4/P5/P6 support, safer ownership with smart pointers and a CMake build.
