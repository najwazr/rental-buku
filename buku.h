#ifndef BUKU_H
#define BUKU_H

#include <string>
#include <iostream>
using namespace std;

class Buku {
private:
    string id;
    string judul;
    string pengarang;
    int tahunTerbit;
    bool tersedia;

public:
    // Constructor
    Buku() : id(""), judul(""), pengarang(""), tahunTerbit(0), tersedia(true) {}
    
    Buku(string id, string judul, string pengarang, int tahun, bool tersedia = true) 
        : id(id), judul(judul), pengarang(pengarang), tahunTerbit(tahun), tersedia(tersedia) {}

    // Getter
    string getId() const { return id; }
    string getJudul() const { return judul; }
    string getPengarang() const { return pengarang; }
    int getTahunTerbit() const { return tahunTerbit; }
    bool isTersedia() const { return tersedia; }

    // Setter
    void setId(string newId) { id = newId; }
    void setJudul(string newJudul) { judul = newJudul; }
    void setPengarang(string newPengarang) { pengarang = newPengarang; }
    void setTahunTerbit(int newTahun) { tahunTerbit = newTahun; }
    void setTersedia(bool status) { tersedia = status; }

    // Method untuk menampilkan info buku
    void tampilkanInfo() const {
        cout << "ID: " << id << endl;
        cout << "Judul: " << judul << endl;
        cout << "Pengarang: " << pengarang << endl;
        cout << "Tahun Terbit: " << tahunTerbit << endl;
        cout << "Status: " << (tersedia ? "Tersedia" : "Dipinjam") << endl;
    }

    // Method untuk format string (untuk save ke file)
    string toFileFormat() const {
        return id + "|" + judul + "|" + pengarang + "|" + 
               to_string(tahunTerbit) + "|" + (tersedia ? "1" : "0");
    }
};

#endif