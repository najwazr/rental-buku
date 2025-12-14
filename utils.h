#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <limits>
#include <cstdlib>
using namespace std;

// Fungsi untuk clear screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Fungsi untuk pause
void pause() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Fungsi untuk membersihkan input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Fungsi untuk validasi input angka
int inputAngka(const string& prompt) {
    int angka;
    while (true) {
        cout << prompt;
        if (cin >> angka) {
            clearInputBuffer();
            return angka;
        } else {
            cout << "Input tidak valid! Masukkan angka.\n";
            clearInputBuffer();
        }
    }
}

// Fungsi untuk input string (dengan getline)
string inputString(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

// Fungsi untuk tampilkan header
void tampilkanHeader(const string& judul) {
    cout << "\n";
    cout << "========================================\n";
    cout << "  " << judul << "\n";
    cout << "========================================\n";
}

// Fungsi untuk konfirmasi (y/n)
bool konfirmasi(const string& pesan) {
    char pilihan;
    cout << pesan << " (y/n): ";
    cin >> pilihan;
    clearInputBuffer();
    return (pilihan == 'y' || pilihan == 'Y');
}

#endif