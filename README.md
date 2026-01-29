# Image Processor (BMP)

## Overview

This project implements an image processing system in C** for manipulating BMP (Bitmap) images** at the pixel level. The processor reads BMP files, applies a set of custom image filters, and outputs the transformed image while preserving the BMP file structure.

The project demonstrates low-level image handling, binary file processing, and modular filter design, and was developed as part of an academic software engineering assignment.

---

## Key Features

* Reads and writes **BMP image files**
* Pixel-level image manipulation
* Custom image filters implemented in C
* Modular separation of BMP handling, pixel logic, and filters
* Command-line driven execution
* Makefile-based build system

---

## Technologies Used

* **C (ANSI C)**
* **BMP image format**
* **Makefile**
* Standard C libraries (`stdio.h`, `stdlib.h`, etc.)

---

## Project Structure

```
Image-Processor/
├── BmpProcessor.c       # BMP file parsing and writing logic
├── BmpProcessor.h
├── PixelProcessor.c    # Pixel-level operations
├── PixelProcessor.h
├── RojasFilters.c      # Custom image filters
├── Makefile            # Build configuration
├── ...
```

---

## Components

### BMP Processor

**`BmpProcessor.c / .h`**

* Handles reading BMP headers and pixel data
* Writes modified pixel data back to a valid BMP file
* Ensures file format integrity during processing

---

### Pixel Processor

**`PixelProcessor.c / .h`**

* Provides low-level access to pixel data
* Supports per-pixel transformations
* Acts as the foundation for filter operations

---

### Filters

**`RojasFilters.c`**

* Implements custom image filters
* Applies transformations such as pixel modification and image effects
* Designed to be easily extensible with new filters

---

## How It Works

1. The BMP image file is loaded and parsed
2. Pixel data is extracted and stored in memory
3. Selected filters are applied to the pixel data
4. The modified image is written to a new BMP file

This approach allows precise control over image data while maintaining BMP compatibility.

---

## Build Instructions

### Requirements

* GCC or compatible C compiler
* Unix-like environment (Linux, macOS, or WSL)

### Compile

```bash
make
```
or
```bash
gcc RojasFilters.c PixelProcessor.c BmpProcessor.c -o RojasFilters.out
```

This generates the image processor executable.

---

## Running the Program

```bash
./RojasFilters.out -i test2-1.bmp -o out.bmp -f b
./RojasFilters.out -i test2-1.bmp -o out.bmp -f c
```


