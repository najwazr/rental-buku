#include <iostream>
#include <vector>
#include <algorithm>
#include "buku.h"
#include "member.h"
#include "transaksi.h"
#include "filehandler.h"
#include "utils.h"

using namespace std;

// Global vectors untuk menyimpan data
vector<Buku> daftarBuku;
vector<Member> daftarMember;
vector<Transaksi> daftarTransaksi;

// Deklarasi fungsi
void menuUtama();
void menuBuku();
void menuMember();
void menuTransaksi();
void tambahBuku();
void editBuku();
void hapusBuku();
void tampilkanSemuaBuku();
void cariBuku();
void tambahMember();
void editMember();
void hapusMember();
void tampilkanSemuaMember();
void cariMember();
void pinjamBuku();
void kembalikanBuku();
void tampilkanTransaksi();
Buku* cariBukuById(const string& id);
Member* cariMemberById(const string& id);

int main() {
    // Load data dari file input.txt
    cout << "Memuat data dari input.txt...\n";
    if (loadAllData("input.txt", daftarBuku, daftarMember, daftarTransaksi)) {
        cout << "Data berhasil dimuat!\n";
    } else {
        cout << "File input.txt tidak ditemukan. Memulai dengan data kosong.\n";
    }
    pause();

    // Jalankan menu utama
    menuUtama();

    // Save data ke file output.txt saat program selesai
    cout << "\nMenyimpan data ke output.txt...\n";
    if (saveData("output.txt", daftarBuku, daftarMember, daftarTransaksi)) {
        cout << "Data berhasil disimpan!\n";
    } else {
        cout << "Gagal menyimpan data!\n";
    }

    return 0;
}

void menuUtama() {
    int pilihan;
    do {
        clearScreen();
        tampilkanHeader("SISTEM RENTAL BUKU");
        cout << "1. Kelola Buku\n";
        cout << "2. Kelola Member\n";
        cout << "3. Kelola Transaksi\n";
        cout << "4. Simpan Data Manual\n";
        cout << "0. Keluar\n";
        
        pilihan = inputAngka("Pilih menu: ");

        switch (pilihan) {
            case 1: menuBuku(); break;
            case 2: menuMember(); break;
            case 3: menuTransaksi(); break;
            case 4:
                if (saveData("output.txt", daftarBuku, daftarMember, daftarTransaksi)) {
                    cout << "\nData berhasil disimpan ke output.txt!\n";
                } else {
                    cout << "\nGagal menyimpan data!\n";
                }
                pause();
                break;
            case 0:
                cout << "\nTerima kasih telah menggunakan aplikasi!\n";
                break;
            default:
                cout << "\nPilihan tidak valid!\n";
                pause();
        }
    } while (pilihan != 0);
}

void menuBuku() {
    int pilihan;
    do {
        clearScreen();
        tampilkanHeader("KELOLA BUKU");
        cout << "1. Tambah Buku\n";
        cout << "2. Edit Buku\n";
        cout << "3. Hapus Buku\n";
        cout << "4. Tampilkan Semua Buku\n";
        cout << "5. Cari Buku\n";
        cout << "0. Kembali\n";
        
        pilihan = inputAngka("Pilih menu: ");

        switch (pilihan) {
            case 1: tambahBuku(); break;
            case 2: editBuku(); break;
            case 3: hapusBuku(); break;
            case 4: tampilkanSemuaBuku(); break;
            case 5: cariBuku(); break;
            case 0: break;
            default:
                cout << "\nPilihan tidak valid!\n";
                pause();
        }
    } while (pilihan != 0);
}

void tambahBuku() {
    clearScreen();
    tampilkanHeader("TAMBAH BUKU");
    
    string id = inputString("ID Buku: ");
    string judul = inputString("Judul: ");
    string pengarang = inputString("Pengarang: ");
    int tahun = inputAngka("Tahun Terbit: ");
    
    Buku bukuBaru(id, judul, pengarang, tahun, true);
    daftarBuku.push_back(bukuBaru);
    
    cout << "\nBuku berhasil ditambahkan!\n";
    pause();
}

void editBuku() {
    clearScreen();
    tampilkanHeader("EDIT BUKU");
    
    string id = inputString("Masukkan ID Buku yang akan diedit: ");
    Buku* buku = cariBukuById(id);
    
    if (buku == nullptr) {
        cout << "\nBuku tidak ditemukan!\n";
        pause();
        return;
    }
    
    cout << "\nData buku saat ini:\n";
    buku->tampilkanInfo();
    cout << "\n";
    
    string judul = inputString("Judul baru (kosongkan jika tidak berubah): ");
    if (!judul.empty()) buku->setJudul(judul);
    
    string pengarang = inputString("Pengarang baru (kosongkan jika tidak berubah): ");
    if (!pengarang.empty()) buku->setPengarang(pengarang);
    
    cout << "Tahun terbit baru (0 jika tidak berubah): ";
    int tahun;
    cin >> tahun;
    clearInputBuffer();
    if (tahun != 0) buku->setTahunTerbit(tahun);
    
    cout << "\nBuku berhasil diupdate!\n";
    pause();
}

void hapusBuku() {
    clearScreen();
    tampilkanHeader("HAPUS BUKU");
    
    string id = inputString("Masukkan ID Buku yang akan dihapus: ");
    
    // Cek apakah buku sedang dipinjam
    for (const auto& trans : daftarTransaksi) {
        if (trans.getIdBuku() == id && trans.getStatus() == "aktif") {
            cout << "\nBuku sedang dipinjam! Tidak dapat dihapus.\n";
            pause();
            return;
        }
    }
    
    auto it = remove_if(daftarBuku.begin(), daftarBuku.end(),
                        [&id](const Buku& b) { return b.getId() == id; });
    
    if (it != daftarBuku.end()) {
        daftarBuku.erase(it, daftarBuku.end());
        cout << "\nBuku berhasil dihapus!\n";
    } else {
        cout << "\nBuku tidak ditemukan!\n";
    }
    pause();
}

void tampilkanSemuaBuku() {
    clearScreen();
    tampilkanHeader("DAFTAR SEMUA BUKU");
    
    if (daftarBuku.empty()) {
        cout << "Belum ada data buku.\n";
        pause();
        return;
    }
    
    for (const auto& buku : daftarBuku) {
        buku.tampilkanInfo();
        cout << "----------------------------------------\n";
    }
    pause();
}

void cariBuku() {
    clearScreen();
    tampilkanHeader("CARI BUKU");
    
    cout << "1. Cari berdasarkan Judul\n";
    cout << "2. Cari berdasarkan Pengarang\n";
    cout << "3. Filter berdasarkan Status (Tersedia/Dipinjam)\n";
    
    int pilihan = inputAngka("Pilih metode pencarian: ");
    
    if (pilihan == 1) {
        string keyword = inputString("Masukkan kata kunci judul: ");
        bool found = false;
        for (const auto& buku : daftarBuku) {
            if (buku.getJudul().find(keyword) != string::npos) {
                buku.tampilkanInfo();
                cout << "----------------------------------------\n";
                found = true;
            }
        }
        if (!found) cout << "Tidak ada buku yang ditemukan.\n";
    } else if (pilihan == 2) {
        string keyword = inputString("Masukkan kata kunci pengarang: ");
        bool found = false;
        for (const auto& buku : daftarBuku) {
            if (buku.getPengarang().find(keyword) != string::npos) {
                buku.tampilkanInfo();
                cout << "----------------------------------------\n";
                found = true;
            }
        }
        if (!found) cout << "Tidak ada buku yang ditemukan.\n";
    } else if (pilihan == 3) {
        cout << "1. Tersedia\n";
        cout << "2. Dipinjam\n";
        int status = inputAngka("Pilih status: ");
        bool tersedia = (status == 1);
        
        bool found = false;
        for (const auto& buku : daftarBuku) {
            if (buku.isTersedia() == tersedia) {
                buku.tampilkanInfo();
                cout << "----------------------------------------\n";
                found = true;
            }
        }
        if (!found) cout << "Tidak ada buku yang ditemukan.\n";
    }
    
    pause();
}

void menuMember() {
    int pilihan;
    do {
        clearScreen();
        tampilkanHeader("KELOLA MEMBER");
        cout << "1. Tambah Member\n";
        cout << "2. Edit Member\n";
        cout << "3. Hapus Member\n";
        cout << "4. Tampilkan Semua Member\n";
        cout << "5. Cari Member\n";
        cout << "0. Kembali\n";
        
        pilihan = inputAngka("Pilih menu: ");

        switch (pilihan) {
            case 1: tambahMember(); break;
            case 2: editMember(); break;
            case 3: hapusMember(); break;
            case 4: tampilkanSemuaMember(); break;
            case 5: cariMember(); break;
            case 0: break;
            default:
                cout << "\nPilihan tidak valid!\n";
                pause();
        }
    } while (pilihan != 0);
}

void tambahMember() {
    clearScreen();
    tampilkanHeader("TAMBAH MEMBER");
    
    string id = inputString("ID Member: ");
    string nama = inputString("Nama: ");
    string alamat = inputString("Alamat: ");
    string noTelp = inputString("No. Telp: ");
    
    Member memberBaru(id, nama, alamat, noTelp);
    daftarMember.push_back(memberBaru);
    
    cout << "\nMember berhasil ditambahkan!\n";
    pause();
}

void editMember() {
    clearScreen();
    tampilkanHeader("EDIT MEMBER");
    
    string id = inputString("Masukkan ID Member yang akan diedit: ");
    Member* member = cariMemberById(id);
    
    if (member == nullptr) {
        cout << "\nMember tidak ditemukan!\n";
        pause();
        return;
    }
    
    cout << "\nData member saat ini:\n";
    member->tampilkanInfo();
    cout << "\n";
    
    string nama = inputString("Nama baru (kosongkan jika tidak berubah): ");
    if (!nama.empty()) member->setNama(nama);
    
    string alamat = inputString("Alamat baru (kosongkan jika tidak berubah): ");
    if (!alamat.empty()) member->setAlamat(alamat);
    
    string noTelp = inputString("No. Telp baru (kosongkan jika tidak berubah): ");
    if (!noTelp.empty()) member->setNoTelp(noTelp);
    
    cout << "\nMember berhasil diupdate!\n";
    pause();
}

void hapusMember() {
    clearScreen();
    tampilkanHeader("HAPUS MEMBER");
    
    string id = inputString("Masukkan ID Member yang akan dihapus: ");
    
    // Cek apakah member memiliki transaksi aktif
    for (const auto& trans : daftarTransaksi) {
        if (trans.getIdMember() == id && trans.getStatus() == "aktif") {
            cout << "\nMember memiliki transaksi aktif! Tidak dapat dihapus.\n";
            pause();
            return;
        }
    }
    
    auto it = remove_if(daftarMember.begin(), daftarMember.end(),
                        [&id](const Member& m) { return m.getId() == id; });
    
    if (it != daftarMember.end()) {
        daftarMember.erase(it, daftarMember.end());
        cout << "\nMember berhasil dihapus!\n";
    } else {
        cout << "\nMember tidak ditemukan!\n";
    }
    pause();
}

void tampilkanSemuaMember() {
    clearScreen();
    tampilkanHeader("DAFTAR SEMUA MEMBER");
    
    if (daftarMember.empty()) {
        cout << "Belum ada data member.\n";
        pause();
        return;
    }
    
    for (const auto& member : daftarMember) {
        member.tampilkanInfo();
        cout << "----------------------------------------\n";
    }
    pause();
}

void cariMember() {
    clearScreen();
    tampilkanHeader("CARI MEMBER");
    
    string keyword = inputString("Masukkan kata kunci nama: ");
    bool found = false;
    
    for (const auto& member : daftarMember) {
        if (member.getNama().find(keyword) != string::npos) {
            member.tampilkanInfo();
            cout << "----------------------------------------\n";
            found = true;
        }
    }
    
    if (!found) {
        cout << "Tidak ada member yang ditemukan.\n";
    }
    pause();
}

void menuTransaksi() {
    int pilihan;
    do {
        clearScreen();
        tampilkanHeader("KELOLA TRANSAKSI");
        cout << "1. Pinjam Buku\n";
        cout << "2. Kembalikan Buku\n";
        cout << "3. Tampilkan Semua Transaksi\n";
        cout << "4. Tampilkan Transaksi Aktif\n";
        cout << "0. Kembali\n";
        
        pilihan = inputAngka("Pilih menu: ");

        switch (pilihan) {
            case 1: pinjamBuku(); break;
            case 2: kembalikanBuku(); break;
            case 3: tampilkanTransaksi(); break;
            case 4: {
                clearScreen();
                tampilkanHeader("TRANSAKSI AKTIF");
                bool found = false;
                for (const auto& trans : daftarTransaksi) {
                    if (trans.getStatus() == "aktif") {
                        trans.tampilkanInfo();
                        cout << "----------------------------------------\n";
                        found = true;
                    }
                }
                if (!found) cout << "Tidak ada transaksi aktif.\n";
                pause();
                break;
            }
            case 0: break;
            default:
                cout << "\nPilihan tidak valid!\n";
                pause();
        }
    } while (pilihan != 0);
}

void pinjamBuku() {
    clearScreen();
    tampilkanHeader("PINJAM BUKU");
    
    string idBuku = inputString("ID Buku: ");
    Buku* buku = cariBukuById(idBuku);
    
    if (buku == nullptr) {
        cout << "\nBuku tidak ditemukan!\n";
        pause();
        return;
    }
    
    if (!buku->isTersedia()) {
        cout << "\nBuku sedang dipinjam!\n";
        pause();
        return;
    }
    
    string idMember = inputString("ID Member: ");
    Member* member = cariMemberById(idMember);
    
    if (member == nullptr) {
        cout << "\nMember tidak ditemukan!\n";
        pause();
        return;
    }
    
    string idTransaksi = "T" + to_string(daftarTransaksi.size() + 1);
    string tanggalPinjam = inputString("Tanggal Pinjam (DD-MM-YYYY): ");
    
    Transaksi transaksiBaru(idTransaksi, idBuku, idMember, tanggalPinjam);
    daftarTransaksi.push_back(transaksiBaru);
    buku->setTersedia(false);
    
    cout << "\nTransaksi peminjaman berhasil!\n";
    cout << "ID Transaksi: " << idTransaksi << "\n";
    pause();
}

void kembalikanBuku() {
    clearScreen();
    tampilkanHeader("KEMBALIKAN BUKU");
    
    string idTransaksi = inputString("ID Transaksi: ");
    bool found = false;
    
    for (auto& trans : daftarTransaksi) {
        if (trans.getIdTransaksi() == idTransaksi && trans.getStatus() == "aktif") {
            string tanggalKembali = inputString("Tanggal Kembali (DD-MM-YYYY): ");
            trans.setTanggalKembali(tanggalKembali);
            trans.setStatus("selesai");
            
            Buku* buku = cariBukuById(trans.getIdBuku());
            if (buku != nullptr) {
                buku->setTersedia(true);
            }
            
            cout << "\nBuku berhasil dikembalikan!\n";
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "\nTransaksi tidak ditemukan atau sudah selesai!\n";
    }
    pause();
}

void tampilkanTransaksi() {
    clearScreen();
    tampilkanHeader("DAFTAR SEMUA TRANSAKSI");
    
    if (daftarTransaksi.empty()) {
        cout << "Belum ada data transaksi.\n";
        pause();
        return;
    }
    
    for (const auto& trans : daftarTransaksi) {
        trans.tampilkanInfo();
        cout << "----------------------------------------\n";
    }
    pause();
}

Buku* cariBukuById(const string& id) {
    for (auto& buku : daftarBuku) {
        if (buku.getId() == id) {
            return &buku;
        }
    }
    return nullptr;
}

Member* cariMemberById(const string& id) {
    for (auto& member : daftarMember) {
        if (member.getId() == id) {
            return &member;
        }
    }
    return nullptr;
}