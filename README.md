# 📚 Rental Buku (Rent A Book App)
*Desktop App Sederhana untuk Manajemen Rental Buku*

![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)
![C++](https://img.shields.io/badge/C++-11-00599C.svg)

Sistem Rental Buku adalah aplikasi berbasis Object-Oriented Programming (OOP) yang memungkinkan pengelolaan data buku, member, dan transaksi peminjaman dengan interface grafis yang modern dan user-friendly.

Fitur:
- Manajemen Buku: tambah, edit, hapus, dan cari buku
- Manajemen Member: elola data pelanggan rental
- Transaksi Peminjaman: proses pinjam dan pengembalian buku
- Dual Interface (sesuai preferensi): GUI (Dear ImGui) dan Console 
- Persistensi Data: load/save data dari/ke file
- Color Coding: status visual (tersedia/dipinjam/aktif)

Pre-installation
- C++ Compiler (GCC 7.0+/MSVC 2019+/Clang)
- OpenGL 3.0+
- Git

Steps:
1. Clone Repository
```bash
git clone https://github.com/username/rental-buku.git
cd rental-buku
```

2. Dependencies
- Imgui Files
https://github.com/ocornut/imgui → "Code" → "Download ZIP" → Extract file yang dibutuhkan:
```
imgui/
├── imgui.h
├── imgui.cpp
├── imgui_demo.cpp
├── imgui_draw.cpp
├── imgui_tables.cpp
├── imgui_widgets.cpp
├── imgui_internal.h
├── imconfig.h
├── imstb_rectpack.h
├── imstb_textedit.h
├── imstb_truetype.h
└── backends/
    ├── imgui_impl_glfw.h
    ├── imgui_impl_glfw.cpp
    ├── imgui_impl_opengl3.h
    └── imgui_impl_opengl3.cpp
```

- GLFW Files (Windows): https://www.glfw.org/download.html 
1) Extract: include/GLFW/* → rental-buku/include/GLFW/
2) Extract: lib-mingw-w64/libglfw3.a → rental-buku/lib/

# Compile
**1. Console Version (Windows)**
```bash
g++ main.cpp -o rental_buku.exe
```
to run:
```bash
rental_buku
```

**2. GUI Version**
```bash
g++ src/main_gui.cpp imgui/*.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp ^
    -Iimgui -Iimgui/backends -Iinclude ^
    -Llib -lglfw3 -lopengl32 -lgdi32 ^
    -o rental_buku_gui.exe
```
to run:
```bash
rental_buku_gui
```

References:
- [Dear ImGui](https://github.com/ocornut/imgui) - immediate mode GUI library
- [GLFW](https://www.glfw.org/) - multi-platform window library
- [OpenGL](https://www.opengl.org/) - Graphics API

*Last updated: December 2024*
