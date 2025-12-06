//Compiler Pengecekan Ekspresi Matematika Sederhana

#include <iostream>
#include <regex>
#include <string>
using namespace std;

bool cekInput(const string& expr) {
    // Salin ekspresi agar bisa dimodifikasi tanpa mengubah input asli
    string expr_checked = expr;

    // Tambahkan tanda '*' otomatis di antara angka dan tanda kurung buka
    // Misal: 3(4+5) → 3*(4+5)
    expr_checked = regex_replace(expr_checked, regex(R"((\d)\s*\()"), "$1*(");

    // Cek karakter yang diizinkan
    regex pattern(R"(^[0-9+\-*/^().\sA-Za-z]+$)");
    if (!regex_match(expr_checked, pattern)) {
        cout << "Ditemukan karakter tidak valid.\n";
        for (size_t i = 0; i < expr_checked.size(); ++i) {
            if (!regex_match(string(1, expr_checked[i]), regex(R"([0-9+\-*/^().\sA-Za-z])"))) {
                cout << expr_checked << "\n";
                cout << string(i, ' ') << "^\n";
                cout << "Posisi kesalahan: indeks " << i << " (karakter '" << expr_checked[i] << "')\n";
                return false;
            }
        }
        return false;
    }

    // Fungsi matematika harus diikuti tanda kurung buka "("
    regex wrong_func(R"((sin|cos|tan|sqrt|log|pow)(?!\s*\())");
    smatch func_mismatch;
    if (regex_search(expr_checked, func_mismatch, wrong_func)) {
        cout << "Fungsi matematika tidak diikuti tanda kurung buka.\n";
        string bad = func_mismatch.str();
        size_t pos = func_mismatch.position();
        cout << expr_checked << "\n";
        cout << string(pos, ' ') << "^\n";
        cout << "Kesalahan pada: \"" << bad << "\" di indeks " << pos << "\n";
        return false;
    }

    // Hapus fungsi yang valid, untuk memudahkan pengecekan selanjutnya
    regex func_pattern(R"((sin|cos|tan|sqrt|log|pow))");
    string expr_no_funcs = regex_replace(expr_checked, func_pattern, "");

    // Huruf tidak dikenali
    smatch m;
    if (regex_search(expr_no_funcs, m, regex("[A-Za-z]"))) {
        size_t pos = m.position();
        cout << "Huruf tidak dikenali pada ekspresi.\n";
        cout << expr_checked << "\n";
        cout << string(pos, ' ') << "^\n";
        cout << "Posisi kesalahan: indeks " << pos << " (karakter '" << expr_checked[pos] << "')\n";
        return false;
    }

    // Operator berurutan
    if (regex_search(expr_checked, m, regex(R"([\+\-\*/\^]{2,})"))) {
        size_t pos = m.position();
        cout << "Ditemukan operator ganda atau berurutan.\n";
        cout << expr_checked << "\n";
        cout << string(pos, ' ') << "^\n";
        cout << "Kesalahan dimulai di indeks " << pos << "\n";
        return false;
    }

    // Angka berurutan tanpa operator
    if (regex_search(expr_checked, m, regex(R"(\d+\s+\d+)"))) {
        size_t pos = m.position();
        cout << "Angka berurutan tanpa operator di antaranya.\n";
        cout << expr_checked << "\n";
        cout << string(pos, ' ') << "^\n";
        cout << "Kesalahan di indeks " << pos << "\n";
        return false;
    }

    // Ekspresi mulai dengan operator salah
    if (regex_search(expr_checked, m, regex(R"(^[\*/\^])"))) {
        cout << "Ekspresi tidak boleh diawali dengan *, /, atau ^.\n";
        cout << expr_checked << "\n^\n";
        cout << "Kesalahan di awal ekspresi.\n";
        return false;
    }

    // Ekspresi diakhiri dengan operator
    if (regex_search(expr_checked, m, regex(R"([\+\-\*/\^]$)"))) {
        cout << "Ekspresi tidak boleh diakhiri dengan operator.\n";
        cout << expr_checked << "\n";
        cout << string(expr_checked.size() - 1, ' ') << "^\n";
        cout << "Kesalahan di akhir ekspresi.\n";
        return false;
    }

    // Operator langsung setelah '(' 
    if (regex_search(expr_checked, m, regex(R"(\(\s*[\*/\^])"))) {
        size_t pos = m.position() + 1;
        cout << "Operator tidak boleh langsung setelah '('.\n";
        cout << expr_checked << "\n";
        cout << string(pos, ' ') << "^\n";
        cout << "Kesalahan di indeks " << pos << "\n";
        return false;
    }

    // Operator langsung sebelum ')'
    if (regex_search(expr_checked, m, regex(R"([\+\-\*/\^]\s*\))"))) {
        size_t pos = m.position();
        cout << "Operator tidak boleh langsung sebelum ')'.\n";
        cout << expr_checked << "\n";
        cout << string(pos, ' ') << "^\n";
        cout << "Kesalahan di indeks " << pos << "\n";
        return false;
    }

    // Kurung seimbang
    int balance = 0;
    for (size_t i = 0; i < expr_checked.size(); ++i) {
        if (expr_checked[i] == '(') balance++;
        else if (expr_checked[i] == ')') balance--;
        if (balance < 0) {
            cout << "Tanda ')' muncul sebelum pasangannya '('.\n";
            cout << expr_checked << "\n";
            cout << string(i, ' ') << "^\n";
            cout << "Posisi kesalahan: indeks " << i << "\n";
            return false;
        }
    }
    if (balance != 0) {
        cout << "Jumlah tanda '(' dan ')' tidak seimbang.\n";
        return false;
    }

    // Kurung kosong
    if (regex_search(expr_checked, m, regex(R"(\(\s*\))"))) {
        size_t pos = m.position();
        cout << "Tanda kurung kosong tidak diperbolehkan.\n";
        cout << expr_checked << "\n";
        cout << string(pos, ' ') << "^\n";
        cout << "Kesalahan di indeks " << pos << "\n";
        return false;
    }

    return true;
}

int main() {
    string math;
    int op;

    do {
        cout << "Pilih Mode:\n";
        cout << "1. Cek Validitas Ekspresi Matematika\n";
        cout << "2. Keluar\n";
        cin >> op;
        cin.ignore();

        switch (op) {
            case 1:
                cout << "Input Ekspresi Matematika: ";
                getline(cin, math);

                if (cekInput(math)) {
                    cout << "----------------------------" << endl;
                    cout << "Ekspresi valid." << endl;
                    cout << "----------------------------" << endl;
                } else {
                    cout << "----------------------------" << endl;
                    cout << "Ekspresi tidak valid." << endl;
                    cout << "----------------------------" << endl;
                }
                break;

            case 2:
                cout << "Keluar dari program." << endl;
                break;

            default:
                cout << "Input salah.\n\n";
                break;
        }
    } while (op != 2);
}

