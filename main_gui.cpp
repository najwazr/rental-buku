// main_gui.cpp - Aplikasi Rental Buku dengan Dear ImGui
// 
// CARA COMPILE & RUN:
// 1. Download Dear ImGui dari: https://github.com/ocornut/imgui
// 2. Download GLFW dari: https://www.glfw.org/
// 3. Copy files: imgui.h, imgui.cpp, imgui_demo.cpp, imgui_draw.cpp, 
//    imgui_tables.cpp, imgui_widgets.cpp, imgui_impl_glfw.h, 
//    imgui_impl_glfw.cpp, imgui_impl_opengl3.h, imgui_impl_opengl3.cpp
// 
// Compile Windows (MinGW): 
// g++ main_gui.cpp imgui*.cpp -I./include -L./lib -lglfw3 -lopengl32 -lgdi32 -o rental_buku.exe
//
// Compile Linux:
// g++ main_gui.cpp imgui*.cpp -lglfw -lGL -ldl -o rental_buku
//
// NOTE: Untuk kemudahan, saya rekomendasikan menggunakan CMake atau IDE seperti Visual Studio/CLion
#include <windows.h>
#include <GL/gl.h>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>

// Include headers yang sudah ada
#include "buku.h"
#include "member.h"
#include "transaksi.h"
#include "filehandler.h"

using namespace std;

// Global data
vector<Buku> daftarBuku;
vector<Member> daftarMember;
vector<Transaksi> daftarTransaksi;

// Window states
enum AppState {
    STATE_MENU_UTAMA,
    STATE_KELOLA_BUKU,
    STATE_KELOLA_MEMBER,
    STATE_KELOLA_TRANSAKSI
};

AppState currentState = STATE_MENU_UTAMA;
int bukuSubMenu = 0;
int memberSubMenu = 0;
int transaksiSubMenu = 0;

// Input buffers (untuk form input)
char inputBukuId[64] = "";
char inputBukuJudul[128] = "";
char inputBukuPengarang[128] = "";
char inputBukuTahun[16] = "";
char inputMemberId[64] = "";
char inputMemberNama[128] = "";
char inputMemberAlamat[256] = "";
char inputMemberTelp[64] = "";
char inputTransaksiId[64] = "";
char inputTransaksiBukuId[64] = "";
char inputTransaksiMemberId[64] = "";
char inputTransaksiTglPinjam[64] = "";
char searchKeyword[128] = "";

// Status message
char statusMessage[256] = "Selamat datang di Sistem Rental Buku!";
float statusMessageTime = 3.0f;

// Helper functions
void ShowStatusMessage(const char* msg) {
    strcpy(statusMessage, msg);
    statusMessageTime = 3.0f;
}

Buku* CariBukuById(const string& id) {
    for (auto& buku : daftarBuku) {
        if (buku.getId() == id) return &buku;
    }
    return nullptr;
}

Member* CariMemberById(const string& id) {
    for (auto& member : daftarMember) {
        if (member.getId() == id) return &member;
    }
    return nullptr;
}

// GUI Rendering Functions
void RenderMenuUtama() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    float w = ImGui::GetWindowWidth();
    
    // Header
    ImGui::SetCursorPosY(60);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 1.0f, 1.0f));
    ImGui::SetWindowFontScale(2.5f);
    const char* title = "SISTEM RENTAL BUKU";
    ImGui::SetCursorPosX((w - ImGui::CalcTextSize(title).x) / 2);
    ImGui::Text("%s", title);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();
    
    ImGui::SetCursorPosY(120);
    ImGui::Separator();
    
    // Buttons
    ImGui::SetCursorPosY(180);
    float bw = 300, bh = 55;
    ImGui::SetCursorPosX((w - bw) / 2);
    
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.8f, 1.0f));

    if (ImGui::Button("Kelola Buku", ImVec2(bw, bh))) {
        currentState = STATE_KELOLA_BUKU;
        bukuSubMenu = 0;
    }

    ImGui::SetCursorPosX((w - bw) / 2);
    if (ImGui::Button("Kelola Member", ImVec2(bw, bh))) {
        currentState = STATE_KELOLA_MEMBER;
        memberSubMenu = 0;
    }

    ImGui::SetCursorPosX((w - bw) / 2);
    if (ImGui::Button("Kelola Transaksi", ImVec2(bw, bh))) {
        currentState = STATE_KELOLA_TRANSAKSI;
        transaksiSubMenu = 0;
    }

    ImGui::SetCursorPosX((w - bw) / 2);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.9f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.7f, 0.2f, 1.0f));
    if (ImGui::Button("Simpan Data", ImVec2(bw, bh))) {
        if (saveData("output.txt", daftarBuku, daftarMember, daftarTransaksi)) {
            ShowStatusMessage("Data berhasil disimpan ke output.txt!");
        } else {
            ShowStatusMessage("Gagal menyimpan data!");
        }
    }
    ImGui::PopStyleColor(6);
    ImGui::PopStyleVar();

    // Status message
    if (statusMessageTime > 0) {
        ImGui::SetCursorPosY(480);
        ImGui::SetCursorPosX((w - ImGui::CalcTextSize(statusMessage).x) / 2);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 1.0f, 0.3f, 1.0f));
        ImGui::Text("%s", statusMessage);
        ImGui::PopStyleColor();
    }

    // Footer info
    ImGui::SetCursorPos(ImVec2(20, ImGui::GetWindowHeight() - 40));
    ImGui::Text("Total Buku: %d | Total Member: %d | Transaksi Aktif: %d", 
                (int)daftarBuku.size(), (int)daftarMember.size(),
                (int)count_if(daftarTransaksi.begin(), daftarTransaksi.end(),
                             [](const Transaksi& t) { return t.getStatus() == "aktif"; }));

    ImGui::End();
}

void RenderKelolaBuku() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Buku", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 1.0f, 1.0f));
    ImGui::SetWindowFontScale(1.8f);
    ImGui::Text("KELOLA BUKU");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();
    ImGui::Separator();

    // Sidebar
    ImGui::BeginChild("Sidebar", ImVec2(220, 0), true);
    if (ImGui::Selectable("Daftar Buku", bukuSubMenu == 0)) bukuSubMenu = 0;
    if (ImGui::Selectable("Tambah Buku", bukuSubMenu == 1)) bukuSubMenu = 1;
    if (ImGui::Selectable("Edit Buku", bukuSubMenu == 2)) bukuSubMenu = 2;
    if (ImGui::Selectable("Hapus Buku", bukuSubMenu == 3)) bukuSubMenu = 3;
    if (ImGui::Selectable("Cari Buku", bukuSubMenu == 4)) bukuSubMenu = 4;
    ImGui::Separator();
    if (ImGui::Button("Kembali", ImVec2(-1, 40))) {
        currentState = STATE_MENU_UTAMA;
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Main content
    ImGui::BeginChild("Content", ImVec2(0, 0), true);
    
    if (bukuSubMenu == 0) { // Daftar Buku
        ImGui::Text("DAFTAR SEMUA BUKU");
        ImGui::Separator();
        
        if (daftarBuku.empty()) {
            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Belum ada data buku.");
        } else {
            if (ImGui::BeginTable("BukuTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | 
                                  ImGuiTableFlags_ScrollY)) {
                ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 80);
                ImGui::TableSetupColumn("Judul", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Pengarang", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Tahun", ImGuiTableColumnFlags_WidthFixed, 70);
                ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 100);
                ImGui::TableHeadersRow();

                for (const auto& buku : daftarBuku) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", buku.getId().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", buku.getJudul().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", buku.getPengarang().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", buku.getTahunTerbit());
                    ImGui::TableNextColumn();
                    if (buku.isTersedia()) {
                        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Tersedia");
                    } else {
                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Dipinjam");
                    }
                }
                ImGui::EndTable();
            }
        }
    } 
    else if (bukuSubMenu == 1) { // Tambah Buku
        ImGui::Text("TAMBAH BUKU BARU");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("ID Buku:");
        ImGui::InputText("##BukuId", inputBukuId, sizeof(inputBukuId));
        ImGui::Text("Judul:");
        ImGui::InputText("##BukuJudul", inputBukuJudul, sizeof(inputBukuJudul));
        ImGui::Text("Pengarang:");
        ImGui::InputText("##BukuPengarang", inputBukuPengarang, sizeof(inputBukuPengarang));
        ImGui::Text("Tahun Terbit:");
        ImGui::InputText("##BukuTahun", inputBukuTahun, sizeof(inputBukuTahun));

        ImGui::Spacing();
        if (ImGui::Button("Simpan", ImVec2(150, 40))) {
            if (strlen(inputBukuId) > 0 && strlen(inputBukuJudul) > 0) {
                int tahun = atoi(inputBukuTahun);
                Buku bukuBaru(inputBukuId, inputBukuJudul, inputBukuPengarang, tahun, true);
                daftarBuku.push_back(bukuBaru);
                ShowStatusMessage("Buku berhasil ditambahkan!");
                memset(inputBukuId, 0, sizeof(inputBukuId));
                memset(inputBukuJudul, 0, sizeof(inputBukuJudul));
                memset(inputBukuPengarang, 0, sizeof(inputBukuPengarang));
                memset(inputBukuTahun, 0, sizeof(inputBukuTahun));
                bukuSubMenu = 0;
            } else {
                ShowStatusMessage("ID dan Judul tidak boleh kosong!");
            }
        }
    }

    if (statusMessageTime > 0) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", statusMessage);
    }

    ImGui::EndChild();
    ImGui::End();
}

void RenderKelolaMember() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Member", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 1.0f, 1.0f));
    ImGui::SetWindowFontScale(1.8f);
    ImGui::Text("KELOLA MEMBER");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();
    ImGui::Separator();

    ImGui::BeginChild("Sidebar", ImVec2(220, 0), true);
    if (ImGui::Selectable("Daftar Member", memberSubMenu == 0)) memberSubMenu = 0;
    if (ImGui::Selectable("Tambah Member", memberSubMenu == 1)) memberSubMenu = 1;
    ImGui::Separator();
    if (ImGui::Button("Kembali", ImVec2(-1, 40))) {
        currentState = STATE_MENU_UTAMA;
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Content", ImVec2(0, 0), true);
    
    if (memberSubMenu == 0) { // Daftar Member
        ImGui::Text("DAFTAR SEMUA MEMBER");
        ImGui::Separator();
        
        if (daftarMember.empty()) {
            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Belum ada data member.");
        } else {
            if (ImGui::BeginTable("MemberTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | 
                                  ImGuiTableFlags_ScrollY)) {
                ImGui::TableSetupColumn("ID");
                ImGui::TableSetupColumn("Nama");
                ImGui::TableSetupColumn("Alamat");
                ImGui::TableSetupColumn("No. Telp");
                ImGui::TableHeadersRow();

                for (const auto& member : daftarMember) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", member.getId().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", member.getNama().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", member.getAlamat().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", member.getNoTelp().c_str());
                }
                ImGui::EndTable();
            }
        }
    }
    else if (memberSubMenu == 1) { // Tambah Member
        ImGui::Text("TAMBAH MEMBER BARU");
        ImGui::Separator();

        ImGui::Text("ID Member:");
        ImGui::InputText("##MemberId", inputMemberId, sizeof(inputMemberId));
        ImGui::Text("Nama:");
        ImGui::InputText("##MemberNama", inputMemberNama, sizeof(inputMemberNama));
        ImGui::Text("Alamat:");
        ImGui::InputText("##MemberAlamat", inputMemberAlamat, sizeof(inputMemberAlamat));
        ImGui::Text("No. Telp:");
        ImGui::InputText("##MemberTelp", inputMemberTelp, sizeof(inputMemberTelp));

        ImGui::Spacing();
        if (ImGui::Button("Simpan", ImVec2(150, 40))) {
            if (strlen(inputMemberId) > 0 && strlen(inputMemberNama) > 0) {
                Member memberBaru(inputMemberId, inputMemberNama, inputMemberAlamat, inputMemberTelp);
                daftarMember.push_back(memberBaru);
                ShowStatusMessage("Member berhasil ditambahkan!");
                memset(inputMemberId, 0, sizeof(inputMemberId));
                memset(inputMemberNama, 0, sizeof(inputMemberNama));
                memset(inputMemberAlamat, 0, sizeof(inputMemberAlamat));
                memset(inputMemberTelp, 0, sizeof(inputMemberTelp));
                memberSubMenu = 0;
            }
        }
    }

    if (statusMessageTime > 0) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", statusMessage);
    }

    ImGui::EndChild();
    ImGui::End();
}

void RenderKelolaTransaksi() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Transaksi", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 1.0f, 1.0f));
    ImGui::SetWindowFontScale(1.8f);
    ImGui::Text("KELOLA TRANSAKSI");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();
    ImGui::Separator();

    ImGui::BeginChild("Sidebar", ImVec2(220, 0), true);
    if (ImGui::Selectable("Daftar Transaksi", transaksiSubMenu == 0)) transaksiSubMenu = 0;
    if (ImGui::Selectable("Pinjam Buku", transaksiSubMenu == 1)) transaksiSubMenu = 1;
    if (ImGui::Selectable("Kembalikan", transaksiSubMenu == 2)) transaksiSubMenu = 2;
    ImGui::Separator();
    if (ImGui::Button("Kembali", ImVec2(-1, 40))) {
        currentState = STATE_MENU_UTAMA;
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Content", ImVec2(0, 0), true);
    
    if (transaksiSubMenu == 0) { // Daftar Transaksi
        ImGui::Text("DAFTAR SEMUA TRANSAKSI");
        ImGui::Separator();
        
        if (daftarTransaksi.empty()) {
            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Belum ada data transaksi.");
        } else {
            if (ImGui::BeginTable("TransTable", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | 
                                  ImGuiTableFlags_ScrollY)) {
                ImGui::TableSetupColumn("ID Trans");
                ImGui::TableSetupColumn("ID Buku");
                ImGui::TableSetupColumn("ID Member");
                ImGui::TableSetupColumn("Tgl Pinjam");
                ImGui::TableSetupColumn("Tgl Kembali");
                ImGui::TableSetupColumn("Status");
                ImGui::TableHeadersRow();

                for (const auto& trans : daftarTransaksi) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", trans.getIdTransaksi().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", trans.getIdBuku().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", trans.getIdMember().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", trans.getTanggalPinjam().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", trans.getTanggalKembali().c_str());
                    ImGui::TableNextColumn();
                    if (trans.getStatus() == "aktif") {
                        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Aktif");
                    } else {
                        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Selesai");
                    }
                }
                ImGui::EndTable();
            }
        }
    }
    else if (transaksiSubMenu == 1) { // Pinjam Buku
        ImGui::Text("PINJAM BUKU");
        ImGui::Separator();

        ImGui::Text("ID Buku:");
        ImGui::InputText("##TransBukuId", inputTransaksiBukuId, sizeof(inputTransaksiBukuId));
        ImGui::Text("ID Member:");
        ImGui::InputText("##TransMemberId", inputTransaksiMemberId, sizeof(inputTransaksiMemberId));
        ImGui::Text("Tanggal Pinjam:");
        ImGui::InputText("##TransTglPinjam", inputTransaksiTglPinjam, sizeof(inputTransaksiTglPinjam));

        ImGui::Spacing();
        if (ImGui::Button("Pinjam", ImVec2(150, 40))) {
            Buku* buku = CariBukuById(inputTransaksiBukuId);
            Member* member = CariMemberById(inputTransaksiMemberId);
            
            if (buku && member && buku->isTersedia()) {
                string idTrans = "T" + to_string(daftarTransaksi.size() + 1);
                Transaksi trans(idTrans, inputTransaksiBukuId, inputTransaksiMemberId, 
                               inputTransaksiTglPinjam);
                daftarTransaksi.push_back(trans);
                buku->setTersedia(false);
                ShowStatusMessage(("Transaksi berhasil! ID: " + idTrans).c_str());
                memset(inputTransaksiBukuId, 0, sizeof(inputTransaksiBukuId));
                memset(inputTransaksiMemberId, 0, sizeof(inputTransaksiMemberId));
                memset(inputTransaksiTglPinjam, 0, sizeof(inputTransaksiTglPinjam));
                transaksiSubMenu = 0;
            } else {
                ShowStatusMessage("Buku/Member tidak valid atau buku tidak tersedia!");
            }
        }
    }

    if (statusMessageTime > 0) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", statusMessage);
    }

    ImGui::EndChild();
    ImGui::End();
}

// Main function
int main(int, char**) {
    // Load data
    if (loadAllData("input.txt", daftarBuku, daftarMember, daftarTransaksi)) {
        ShowStatusMessage("Data berhasil dimuat dari input.txt");
    }

    // Setup GLFW
    if (!glfwInit())
        return -1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Sistem Rental Buku", nullptr, nullptr);
    if (!window)
        return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Update status message timer
        if (statusMessageTime > 0)
            statusMessageTime -= io.DeltaTime;

        // Render based on state
        switch (currentState) {
            case STATE_MENU_UTAMA: RenderMenuUtama(); break;
            case STATE_KELOLA_BUKU: RenderKelolaBuku(); break;
            case STATE_KELOLA_MEMBER: RenderKelolaMember(); break;
            case STATE_KELOLA_TRANSAKSI: RenderKelolaTransaksi(); break;
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Save data on exit
    saveData("output.txt", daftarBuku, daftarMember, daftarTransaksi);

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}