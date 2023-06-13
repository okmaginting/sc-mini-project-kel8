/*
Nama Kelompok 8:
221402007_Riama Raulina Boru Silaban
221402054_Okma Sangaptaras Ginting
221402066_Tasya Amelia
221402089_Christy Simarmata

Mini Project: Membuat Text Editor
*/

#include <iostream>
#include <fstream> 
#include <vector> 

using namespace std;

class TextEditor {
    private:
    //deklarasi atribut
    string currentFileName; 
    vector<string> lines; 
    vector<string> undoStack; 
    vector<string> redoStack; 
    bool isModified; 

    public:
    TextEditor() : isModified(false) {}  // Inisialisasi isModified dengan false di konstruktor

    //untuk merefresh layar agar tidak menumpuk
    void clearScreen() {
        system("cls");
    }

    //fungsi-fungsi yang terdapat pada text editor
    void createNewFile();
    void selectFile();
    void displayFile();
    void editFile();
    void editLine();
    void saveFile();
    void countCharacters();
    void undo();
    void redo();
    void deleteFile();
    void displayMenu();
    void run();
};

//kode untuk membuat file baru
void TextEditor::createNewFile() {
    //tempat user menginput nama file
    string fileName;
    cout << "Masukkan nama file baru: ";
    getline(cin, fileName); 

    ofstream file(fileName);  // Membuat file baru dari variable fileName
    
    //pemanggilan file yang sudah dibuat
    if (file.is_open()) { 
        currentFileName = fileName; 
        //menghapus konten yang mungkin sebelumnya ada
        lines.clear(); 
        undoStack.clear();
        redoStack.clear();
        isModified = false;
        cout << "File \"" << fileName << "\" berhasil dibuat." << endl;
    } else { 
        cout << "Gagal membuat file \"" << fileName << "\"." << endl;
    }

    cout << "Tekan ENTER untuk kembali ke menu...";
    cin.ignore(); 
    cin.get(); 
}

//kode untuk memilih file
void TextEditor::selectFile() {
    string fileName;
    cout << "Masukkan nama file yang ingin dipilih: ";
    cin >> fileName; 
    ifstream file(fileName); 
    if (file.is_open()) { 
        currentFileName = fileName; 
        lines.clear(); 
        //untuk membaca dan menyimpan setiap baris dalam file
        string line; 
        while (getline(file, line)) { 
        lines.push_back(line); 
        }
        file.close(); //file ditutup
        isModified = false; 
        cout << "File \"" << fileName << "\"berhasil dipilih." << endl;
        } else { 
        cout << "File \"" << fileName << "\" tidak ditemukan." << endl;
    }
    cout << "Tekan ENTER untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

//kode tampil isi file
void TextEditor::displayFile() {
if (currentFileName.empty()) { // jika belum ada file yang dipilih
            cout << "File belum dipilih." << endl;
            cout << "Tekan ENTER untuk kembali ke menu...";
            cin.ignore();
            cin.get();
            return;
        }

    if (lines.empty()) { 
        cout << "File kosong." << endl;
    } 
    else {
         cout << "Isi file \"" << currentFileName << "\":" << endl; 
        for (const string& line : lines) { 
            cout << line << endl; 
        }
    }
    cout << "Tekan ENTER untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

//kode edit file
void TextEditor::editFile() {
    if (currentFileName.empty()) { 
        cout << "File belum dipilih." << endl;
        cout << "Tekan ENTER untuk kembali ke menu...";
        cin.ignore();
        cin.get();
        return;
    }

    clearScreen(); 

    //jika sudah ada file yang dipilih
    cout << "===== EDIT MODE =====" << endl;
    cout << "Ketikkan isi file. Tekan ENTER untuk membuat baris baru." << endl;
    cout << "Tekan Ctrl+S untuk menyimpan dan kembali ke menu." << endl;
    cout << "=================================" << endl;

    undoStack.clear(); 
    redoStack.clear(); 
    isModified = false; 

    string input; 
    string currentLine; 
    while (true) { 
        cout << "> "; 
        getline(cin, input); 

            //jika user menekan Ctrl+S untuk menyimpan
            if (input == "\x13") {  
                if (!currentLine.empty()) { 

                    lines.push_back(currentLine); 
                    undoStack.push_back(currentLine); 
                    isModified = true; 
                    currentLine.clear(); 
                }
                saveFile(); //memanggil fungsi saveFile() untuk menyimpan perubahan
                break;
            }

            if (input.empty()) { // jika user menginput garis kosong(menekan tombol Enter)
                lines.push_back(currentLine);
                undoStack.push_back(currentLine);
                currentLine.clear();
            } 
            else { // jika user menginput selain Ctrl+S dan Enter
                currentLine += input; 
                currentLine += '\n'; 
            }

            clearScreen();
            cout << "===== EDIT MODE =====" << endl;
            cout << "Ketikkan isi file. Tekan ENTER untuk membuat baris baru." << endl;
            cout << "Tekan Ctrl+S untuk menyimpan dan kembali ke menu." << endl;
            cout << "=================================" << endl;
            cout << currentLine; 
        }
}

//kode edit line
void TextEditor::editLine() {
if (currentFileName.empty()) { // jika belum ada file yang dipilih
            cout << "File belum dipilih." << endl;
            cout << "Tekan ENTER untuk kembali ke menu...";
            cin.ignore();
            cin.get();
            return;
        }

    if (lines.empty()) { // jika file kosong
        cout << "File kosong." << endl;
        return;
    }

    int lineIndex;
    cout << "Masukkan nomor baris yang ingin diubah: ";
    cin >> lineIndex;
    cin.ignore();

    if (lineIndex >= 1 && lineIndex <= lines.size()) {
        string& line = lines[lineIndex - 1]; // mengambil referensi ke baris yang ingin diubah
        cout << "Baris asli: " << line << endl;

        string oldWord, newWord;
        cout << "Masukkan kata yang ingin dihapus: ";
        cin >> oldWord;
        cout << "Masukkan kata pengganti: ";
        cin >> newWord;

        size_t pos = 0;
        while ((pos = line.find(oldWord, pos)) != string::npos) {
            line.replace(pos, oldWord.length(), newWord); // mengganti kata lama dengan kata baru
            pos += newWord.length();
        }

        cout << "Baris setelah diubah: " << line << endl;
        isModified = true; // menandakan bahwa file telah dimodifikasi
    } else {
        cout << "Nomor baris tidak valid." << endl;
    }

    cout << "Tekan ENTER untuk kembali ke menu...";
    cin.get();
}

//kode save file
void TextEditor::saveFile() {
    if (isModified) { //jika ada perubahan dalam file yang digunakan

        // untuk menulis perubahan ke dalam file yang digunakan
        ofstream file(currentFileName); 
        if (file.is_open()) { 
            for (const string& line : lines) { 
                file << line; 
            }
            file.close(); //menutup file yang digunakan setelah selesai ditulis
            isModified = false; 
            cout << "File \"" << currentFileName << "\" berhasil disimpan." << endl;
        }
        else { 
                cout << "Gagal menyimpan file \"" << currentFileName << "\"." << endl;
        }
    }
    else { //jika tidak ada perubahan dalam file yang digunakan
        cout << "Tidak ada perubahan pada file \"" << currentFileName << "\"." << endl;
    }
        cout << "Tekan ENTER untuk kembali ke menu...";
        cin.ignore();
        cin.get();
}

//kode menghitung karakter dan baris
void TextEditor::countCharacters() {
if (currentFileName.empty()) { // jika belum ada file yang dipilih
            cout << "File belum dipilih." << endl;
            cout << "Tekan ENTER untuk kembali ke menu...";
            cin.ignore();
            cin.get();
            return;
        }

    //untuk memeriksa apakah vektor lines kosong atau tidak
    if (lines.empty()) { 
        cout << "File kosong." << endl;
    } 
    else { 
        int characterCount = 0; 
        int lineCount = lines.size();
        for (const string& line : lines) {
            characterCount += line.size(); 
        }
       
        cout << "Jumlah huruf dalam file \"" << currentFileName << "\": " << characterCount << endl;
        cout << "Jumlah baris dalam file \"" << currentFileName << "\": " << lineCount << endl;
        }
    cout << "Tekan ENTER untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

//kode untuk meng-undo
void TextEditor::undo() {
if (currentFileName.empty()) { 
            cout << "File belum dipilih." << endl;
            cout << "Tekan ENTER untuk kembali ke menu...";
            cin.ignore();
            cin.get();
            return;
        }

    if (undoStack.empty()) { 
        cout << "Tidak ada operasi undo yang tersedia." << endl;
        cout << "Tekan ENTER untuk kembali ke menu...";
        cin.ignore();
        cin.get();
        return;
    }

    //jika tidak kosong
    redoStack.push_back(lines.back());
    lines.pop_back(); 
    cout << "Operasi undo berhasil." << endl;
    cout << "Tekan ENTER untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

//kode untuk me-redo
void TextEditor::redo() {
if (currentFileName.empty()) { 
            cout << "File belum dipilih." << endl;
            cout << "Tekan ENTER untuk kembali ke menu...";
            cin.ignore();
            cin.get();
            return;
        }

    if (redoStack.empty()) { 
        cout << "Tidak ada operasi redo yang tersedia." << endl;
        cout << "Tekan ENTER untuk kembali ke menu...";
        cin.ignore();
        cin.get();
        return;
    }

    // jika stack redo tidak kosong
    lines.push_back(redoStack.back()); //memindahkan kembali baris terakhir ke vektor lines
    redoStack.pop_back(); //baris yang dipindahkan ke vektor lines dihapus dari vektor redoSctack
    cout << "Operasi redo berhasil." << endl;
    cout << "Tekan ENTER untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

//kode untuk men-delete
void TextEditor::deleteFile() {
if (currentFileName.empty()) { 
            cout << "File belum dipilih." << endl;
            cout << "Tekan ENTER untuk kembali ke menu...";
            cin.ignore();
            cin.get();
            return;
        }

        //jika file sudah dipilih
        cout << "Apakah Anda yakin ingin menghapus file \"" << currentFileName << "\"? (Y/N): "; 
        char confirmation; // variable untuk menyimpan pilihan user
        cin >> confirmation; 
        cin.ignore();

      
        if (confirmation == 'Y' || confirmation == 'y') { 
            if (remove(currentFileName.c_str()) == 0) { 
                cout << "File berhasil dihapus." << endl;
                currentFileName.clear(); 
                lines.clear(); 
            } else { 
                cout << "Gagal menghapus file." << endl;
            }
        } 
     
        else if (confirmation == 'N' || confirmation == 'n') {
            cout << "Penghapusan file dibatalkan." << endl;
        } 

        else {
            cout << "Pilihan tidak valid." << endl;
        }

        cout << "Tekan ENTER untuk kembali ke menu...";
        cin.ignore();
        cin.get();
}

//kode untuk menu
void TextEditor::displayMenu() {

    cout << "+========= Eight(8)ditor =========+" << endl;
    cout << "|------Teks Editor Sederhana------|" << endl;
    cout << "+=================================+" << endl;
    cout << "1. Buat file baru" << endl;
    cout << "2. Pilih file" << endl;
    cout << "3. Tampilkan isi file" << endl;
    cout << "4. Edit file" << endl;
    cout << "5. Edit baris" << endl;
    cout << "6. Hitung jumlah huruf dan baris" << endl;
    cout << "7. Undo" << endl;
    cout << "8. Redo" << endl;
    cout << "9. Delete" << endl;
    cout << "0. Keluar" << endl;
    cout << "===================================" << endl;
    cout << "Pilihan: ";


    if (isModified) { 
        cout << " [TEKS TELAH DIMODIFIKASI]";
    }
    cout << endl;
}

//kode switch case
void TextEditor::run() {
    int choice;
    while (true) {
        clearScreen();
        displayMenu(); 
        cin >> choice; 
        cin.ignore();

        //switch case menu
        clearScreen();
        switch (choice) {
            case 1:
                createNewFile();
                break;
            case 2:
                selectFile();
                break;
            case 3:
                displayFile();
                break;
            case 4:
                editFile();
                break;
            case 5:
                editLine();
                break;
            case 6:
                countCharacters();
                break;
            case 7:
                undo();
                break;
            case 8:
                redo();
                break;
            case 9:
                deleteFile();
                break;
            case 0:
                return;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                cout << "Tekan ENTER untuk kembali ke menu...";
                cin.ignore();
                cin.get();
                break;
        }
    }
}

int main() {
    TextEditor te; 
    te.run(); 
    
    return 0;
}
