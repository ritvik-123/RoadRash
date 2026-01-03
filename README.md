# RoadRash (OpenGL / GLUT)

> **Course/Assignment:** Advance Game Dev : Final Project
> **Author:** Ritvik Mahapatra 
> **Language/Tech:** C++, OpenGL, GLUT / FreeGLUT

---

## 1) Overview

This repository contains an OpenGL/GLUT-based game or assignment developed in C++.

### Project-Specific Description
> This is our final project: Road Rash
> - This game has three levels. We have to avoid obstacles and reach the target distance to advance to the next level
> - Different levels have different environmental setups, nighttime mode, and subtle visual differences.  
> - Used OpenGL spheres to implement obstacles and a spritesheet to implement the main bike and its actions

---

## 2) Requirements / Dependencies

- C++ Compiler (GCC / MinGW / MSVC)
- OpenGL
- GLUT or FreeGLUT
- Code::Blocks (recommended for Windows users)

---

## 3) How to Build & Run

### Option A — Windows (Code::Blocks)
1. Install **Code::Blocks** with **MinGW**.
2. Open `Project1.cbp` in Code::Blocks.
3. Make sure the compiler is properly configured.
4. Click **Build & Run (F9)**.

### Option B — Windows (Command Line – MinGW)
```bash
g++ main.cpp -o RoadRash.exe -lopengl32 -lglu32 -lfreeglut
