#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include <sstream>
#include <vector>
#include "buku.h"
#include "member.h"
#include "transaksi.h"

using namespace std;

// Fungsi untuk split string berdasarkan delimiter
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Load data buku dari file
bool loadBuku(const string& filename, vector<Buku>& daftarBuku) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        vector<string> data = split(line, '|');
        if (data.size() >= 5) {
            Buku buku(data[0], data[1], data[2], stoi(data[3]), data[4] == "1");
            daftarBuku.push_back(buku);
        }
    }
    file.close();
    return true;
}

// Load data member dari file
bool loadMember(const string& filename, vector<Member>& daftarMember) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        vector<string> data = split(line, '|');
        if (data.size() >= 4) {
            Member member(data[0], data[1], data[2], data[3]);
            daftarMember.push_back(member);
        }
    }
    file.close();
    return true;
}

// Load data transaksi dari file
bool loadTransaksi(const string& filename, vector<Transaksi>& daftarTransaksi) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        vector<string> data = split(line, '|');
        if (data.size() >= 6) {
            Transaksi transaksi(data[0], data[1], data[2], data[3], data[4], data[5]);
            daftarTransaksi.push_back(transaksi);
        }
    }
    file.close();
    return true;
}

// Save semua data ke file
bool saveData(const string& filename, 
              const vector<Buku>& daftarBuku,
              const vector<Member>& daftarMember,
              const vector<Transaksi>& daftarTransaksi) {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Simpan buku
    file << "[BUKU]\n";
    for (const auto& buku : daftarBuku) {
        file << buku.toFileFormat() << "\n";
    }

    // Simpan member
    file << "[MEMBER]\n";
    for (const auto& member : daftarMember) {
        file << member.toFileFormat() << "\n";
    }

    // Simpan transaksi
    file << "[TRANSAKSI]\n";
    for (const auto& transaksi : daftarTransaksi) {
        file << transaksi.toFileFormat() << "\n";
    }

    file.close();
    return true;
}

// Load semua data dari satu file (input.txt)
bool loadAllData(const string& filename,
                 vector<Buku>& daftarBuku,
                 vector<Member>& daftarMember,
                 vector<Transaksi>& daftarTransaksi) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    string line;
    string section = "";
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        if (line == "[BUKU]") {
            section = "BUKU";
            continue;
        } else if (line == "[MEMBER]") {
            section = "MEMBER";
            continue;
        } else if (line == "[TRANSAKSI]") {
            section = "TRANSAKSI";
            continue;
        }

        vector<string> data = split(line, '|');
        
        if (section == "BUKU" && data.size() >= 5) {
            Buku buku(data[0], data[1], data[2], stoi(data[3]), data[4] == "1");
            daftarBuku.push_back(buku);
        } else if (section == "MEMBER" && data.size() >= 4) {
            Member member(data[0], data[1], data[2], data[3]);
            daftarMember.push_back(member);
        } else if (section == "TRANSAKSI" && data.size() >= 6) {
            Transaksi transaksi(data[0], data[1], data[2], data[3], data[4], data[5]);
            daftarTransaksi.push_back(transaksi);
        }
    }

    file.close();
    return true;
}

#endif