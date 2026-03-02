# 🛠 BitMapper-CLI: Multi-Format Image Processor

**BitMapper-CLI** is a C++ command-line tool designed for processing and transforming raster images in the **Netpbm family (PBM, PGM, PPM)**. This project bridges the gap between low-level hardware logic and high-level software architecture, focusing on memory efficiency and polymorphic design.


## 🌟 Key Features

* **Full Netpbm Support:** * **PBM (P1):** Portable BitMap (Monochrome/Black & White).
    * **PGM (P2):** Portable GrayMap (Grayscale).
    * **PPM (P3):** Portable PixMap (Full Color RGB).
* **Session Management:** Handle multiple images simultaneously using a robust `SessionManager`.
* **Polymorphic Core:** A unified `Image` interface allowing seamless transitions between formats (e.g., converting PPM to Grayscale).
* **Graphic Transformations:**
    * `negative`: Invert pixel values.
    * `rotate`: 90-degree rotations (left/right).
    * `flip`: Horizontal and vertical mirroring.
    * **Smart Paste:** Insert one image into another with automatic canvas resizing.
* **Undo/Redo System:** A complete command history for every active session to ensure non-destructive editing.

## 🏗 System Architecture

The project follows SOLID principles to ensure scalability:
1.  **Command Layer (`CommandHandler`):** Handles input tokenization, command routing, and error reporting.
2.  **Session Layer (`SessionManager` & `Session`):** Manages the lifecycle of image edits and history stacks.
3.  **Data Layer (`PBM/PGM/PPMImage`):** Implements format-specific logic for file I/O and pixel manipulation.



## 🛠 Installation & Usage

### Prerequisites
* A C++ compiler supporting **C++17** (e.g., `g++` or `clang`).

### Build Instructions
```bash
# Compile all modules
g++ -std=c++17 *.cpp -o bitmapper

# Run the application
./bitmapper