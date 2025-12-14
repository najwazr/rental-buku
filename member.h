#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <iostream>
using namespace std;

class Member {
private:
    string id;
    string nama;
    string alamat;
    string noTelp;

public:
    // Constructor
    Member() : id(""), nama(""), alamat(""), noTelp("") {}
    
    Member(string id, string nama, string alamat, string noTelp)
        : id(id), nama(nama), alamat(alamat), noTelp(noTelp) {}

    // Getter
    string getId() const { return id; }
    string getNama() const { return nama; }
    string getAlamat() const { return alamat; }
    string getNoTelp() const { return noTelp; }

    // Setter
    void setId(string newId) { id = newId; }
    void setNama(string newNama) { nama = newNama; }
    void setAlamat(string newAlamat) { alamat = newAlamat; }
    void setNoTelp(string newNoTelp) { noTelp = newNoTelp; }

    // Method untuk menampilkan info member
    void tampilkanInfo() const {
        cout << "ID Member: " << id << endl;
        cout << "Nama: " << nama << endl;
        cout << "Alamat: " << alamat << endl;
        cout << "No. Telp: " << noTelp << endl;
    }

    // Method untuk format string (untuk save ke file)
    string toFileFormat() const {
        return id + "|" + nama + "|" + alamat + "|" + noTelp;
    }
};

#endif