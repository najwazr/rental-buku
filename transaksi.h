#ifndef TRANSAKSI_H
#define TRANSAKSI_H

#include <string>
#include <iostream>
using namespace std;

class Transaksi {
private:
    string idTransaksi;
    string idBuku;
    string idMember;
    string tanggalPinjam;
    string tanggalKembali;
    string status; // "aktif" atau "selesai"

public:
    // Constructor
    Transaksi() : idTransaksi(""), idBuku(""), idMember(""), 
                  tanggalPinjam(""), tanggalKembali(""), status("aktif") {}
    
    Transaksi(string idTrans, string idBuku, string idMember, 
              string tglPinjam, string tglKembali = "-", string status = "aktif")
        : idTransaksi(idTrans), idBuku(idBuku), idMember(idMember),
          tanggalPinjam(tglPinjam), tanggalKembali(tglKembali), status(status) {}

    // Getter
    string getIdTransaksi() const { return idTransaksi; }
    string getIdBuku() const { return idBuku; }
    string getIdMember() const { return idMember; }
    string getTanggalPinjam() const { return tanggalPinjam; }
    string getTanggalKembali() const { return tanggalKembali; }
    string getStatus() const { return status; }

    // Setter
    void setIdTransaksi(string newId) { idTransaksi = newId; }
    void setIdBuku(string newIdBuku) { idBuku = newIdBuku; }
    void setIdMember(string newIdMember) { idMember = newIdMember; }
    void setTanggalPinjam(string newTgl) { tanggalPinjam = newTgl; }
    void setTanggalKembali(string newTgl) { tanggalKembali = newTgl; }
    void setStatus(string newStatus) { status = newStatus; }

    // Method untuk menampilkan info transaksi
    void tampilkanInfo() const {
        cout << "ID Transaksi: " << idTransaksi << endl;
        cout << "ID Buku: " << idBuku << endl;
        cout << "ID Member: " << idMember << endl;
        cout << "Tanggal Pinjam: " << tanggalPinjam << endl;
        cout << "Tanggal Kembali: " << tanggalKembali << endl;
        cout << "Status: " << status << endl;
    }

    // Method untuk format string (untuk save ke file)
    string toFileFormat() const {
        return idTransaksi + "|" + idBuku + "|" + idMember + "|" + 
               tanggalPinjam + "|" + tanggalKembali + "|" + status;
    }
};

#endif