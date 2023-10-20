// Nama : Ferdy Rizkiawan
// NIM : L0122064
// Kelas : B

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <stack>
#include <queue>
#include <utility>
#include <set>
#include <map>

using namespace std;

#define F first
#define S second

struct Product // barang
{
    string name;
    int stock, price;
};

const int dx[] = {1, 0, -1, 0}; // sumbu x pada maze
const int dy[] = {0, 1, 0, -1}; // sumbu y pada maze
int n = 5; // total produk/barang
int par[10]; // disjoint set

map<string, Product> mp; // database
queue<pair<string, string>> q; // pertanyaan consumer ke seller

// deklarasi fungsi
void isi_database();
void print_batas();
void customer(string user);
void seller();
void menu_customer(string user);
void menu_seller();
void pilih_customer(string user);
void pilih_seller();
void lanjut_customer(string user);
void lanjut_seller();
void run();
void login();
int in(int i, int j);
int find(int x);
void merge(int x, int y);
void print_batas();

// mengisi nilai awal map dan par
void isi_database() 
{
    Product product[5];

    product[0].name = "Notebook";
    product[0].stock = 10;
    product[0].price = 3000;

    product[1].name = "Pulpen";
    product[1].stock = 12;
    product[1].price = 2000;
    
    product[2].name = "Pensil";
    product[2].stock = 3;
    product[2].price = 2000;

    product[3].name = "Tipex";
    product[3].stock = 7;
    product[3].price = 7000;

    product[4].name = "Stabilo";
    product[4].stock = 2;
    product[4].price = 5000;

    for (int i = 0; i < 5; i++)
    {
        string code = "00";
        code += (char)i+48;
        mp[code] = product[i];
        par[i] = i;
    }

    merge(0, 1);
    merge(2, 4);
}

// mencari parent/perwakilan kelompok
int find(int x)
{
    if (par[x] == x) return x;
    return find(par[x]);
}

// menggabungkan kedua barang dalam satu kelompok
void merge(int x, int y)
{
    int parX = find(x);
    int parY = find(y);

    par[parX] = parY;
}

// mengetahui apakah petak yang dituju di dalam range 
int in(int i, int j)
{
    return i >= 0 && i < 8 && j >= 0 && j < 10;
}

// garis horizontal supaya program terlihat lebih rapi
void print_batas() 
{
    for (int i = 0; i < 70; i++) cout << "-";
    cout << endl;
}

// memilih menu
void pilih_customer(string user)
{
    print_batas();

    int pilih;
    cout << "Pilih : "; cin >> pilih;

    if (pilih == 1)
    {
        print_batas();

        cout << "Berikut daftar barang yang tersedia :" << endl;
        cout << "| NO | Nama\t\t| Harga\t| Stok\t| Kode\t|" << endl;

        int num = 1;
        string lmt = "00";
        lmt += (char)n+48-1;

        // tampilkan list barang dalam bentnuk tabel (kurang rapi)
        for (string code = "000"; code <= lmt; code[2]++)
        {
            cout << "| " << num << "  ";
            cout << "| " << mp[code].name << "\t\t";
            cout << "| " << mp[code].price << "\t";
            cout << "| " << mp[code].stock << "\t";
            cout << "| " << code << "\t|" << endl;
            num++;
        }
        cout << endl << "*maaf jika tabel kurang rapi" << endl;

        print_batas();

        int saldo, total;
        string code;
        set<int> st;

        cout << "Masukkan saldo awal kamu : "; cin >> saldo;
        cout << endl;

        for (int i = 0; ; i++)
        {
            string ans;
            if (i > 0) // jika udah pernah beli, tampilkan sisa saldo
            {
                cout << "Sisa saldo : " << saldo << endl;
                cout << endl;

                if (st.size() > 1)
                {
                    // Jika sudah pernah beli > 1 barang, 
                    // cek apakah barang barangnya berasal dari kelompok yang berbeda.
                    //      Jika iya, maka mendapatkan voucher berdasarkan banyaknya kelompok
                    //      barang yang diberli
                    // Lalu, outputkan saldo terbaru

                    int tambahan = 500*(st.size()-1);
                    saldo += tambahan;

                    cout << "Anda sudah membeli " << st.size() << " barang dari kelompok yang berbeda." << endl;
                    cout << "Selamat Anda mendapatkan tambahan saldo sebanyak " << tambahan << "!" << endl << endl;
                    cout << "Saldo terbaru : " << saldo << endl << endl;
                }

                // tanya apakah masih ingin beli atau tidak
                cout << "Masih ada yang ingin kamu beli?" << endl;

                cout << "Ketik \"Ya\" / \"Tidak\" : "; 
                cin >> ans;

                if (ans == "Tidak") break;
                cout << endl;
            }
            
            // mengecek apakah uang cukup atau tidak
            bool kurang = true;
            for (int i = 0; i < n; i++)
            {
                string code = "00";
                code += (char)i+48;
                
                if (saldo >= mp[code].price)
                {
                    kurang = false;
                    break;
                }
            }

            if (kurang)
            {
                cout << "Yahh, uang kamu gk cukup buat beli :(" << endl;
                break;
            }

            // masukkan kode barang yang ingin dibeli
            cout << "Barang apa yang ingin kamu beli?" << endl; 
            for (int j = 0; ; j++)
            {
                cout << "Masukkan kode barang : "; cin >> code;
                if (mp.find(code) != mp.end()) break;
                cout << "Kode yang Anda masukkan salah!" << endl << endl;
            }

            // jika stok habis
            if (!mp[code].stock) 
            {   
                cout << endl;
                cout << "Maaf ya, stock nya udah habis :(" << endl;
                print_batas();
                continue;
            }

            cout << "Total pembelian : ";
            cin >> total; // input ingin beli berapa
            
            if (total > mp[code].stock) // jika stok kurang
            {
                cout << endl << "Maaf, stock nya sisa " << mp[code].stock;     
                cout << ". Gimana " << mp[code].stock << " aja gpp?" << endl;

                cout << "Ketik \"Ya\" / \"Tidak\" : "; cin >> ans;
                if (ans == "Tidak") 
                {
                    print_batas();
                    continue;
                } 

                total = mp[code].stock;    
            }
            
            cout << endl;
            if (saldo < total*mp[code].price) // ternyata gak cukup
            {
                cout << "Hmm, ternyata uang kamu gak cukup :|" << endl;
            }
            else // uang nya cukup 
            {
                cout << "Anda membeli " << total << " " << mp[code].name;
                cout << " dengan harga Rp" << total*mp[code].price <<  "!" << endl;

                mp[code].stock -= total; // stok barang dikurangi banyaknya pembelian 
                saldo -= mp[code].price*total;  // saldo berkurang
                st.insert(find((int)code[2]-48)); // masukkan kelompok barang ke dalam set
            }
            print_batas();
        }

        cout << endl << "Terimakasih sudah belanja di Tukupedia!" << endl;

        lanjut_customer(user);
    }
    else if (pilih == 2)
    {
        print_batas();

        cout << "Pertanyaan :" << endl;
        cin.ignore();

        string que;
        getline(cin, que); // input pertanyaan

        // simpan penanya dan pertanyaan ke dalam queue
        q.push({user, que}); 

        cout << endl << "Terimakasih sudah bertanya!" << endl;

        lanjut_customer(user);
    }
    else if (pilih == 3)
    {
        print_batas();

        // bentuk maze (-1 : dinding, 0 : jalan)
        int a[8][10] = {{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                        {-1, 0, 0, 0, 0, 0, -1, 0, 0, 0},
                        {-1, 0, 0, -1, -1, 0, 0, 0, -1, -1},
                        {-1, -1, 0, 0, -1, -1, -1, 0, 0, -1},
                        {-1, 0, 0, 0, -1, 0, -1, -1, -1, -1},
                        {-1, 0, -1, 0, -1, 0, -1, 0, 0, -1},
                        {-1, 0, -1, 0, 0, 0, 0, 0, -1, -1},
                        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};

        int d[8][10]; // untuk menyimpan jarak
        stack<pair<int, int>> s; 

        memset(d, -1, sizeof(d));
        s.push({1, 9}); d[1][9] = 0;
        
        while (!s.empty()) // dfs
        {
            pair<int, int> u = s.top();
            s.pop();
            
            for (int k = 0; k < 4; k++)
            {
                int ii = u.F + dx[k], jj = u.S + dy[k];
                if (!in(ii, jj)) continue;
                if (a[ii][jj] == -1) continue;
                if (d[ii][jj] != -1) continue;
                d[ii][jj] = d[u.F][u.S] + 1;
                s.push({ii, jj});
            }
        }

        // deskripsi mini-game
        cout << "                              MINI-GAME" << endl << endl;

        cout << "Anda tahu permainan maze? Kalau melihat gambar berikut ini pasti tahu." << endl;
        cout << "Gambar : https://tinyurl.com/maze-minigame" << endl << endl;
        
        cout << "Hidup seorang mahasiswa semester akhir bernama Mas Bayem Sore. Mengetahui" << endl;
        cout << "ada duit di dalam maze tersebut, dia jadi ingin bermain. Karena dia yakin" << endl;
        cout << "bahwa jika dia punya duit, maka dia punya kuasa. Pemikiran yang aneh..." << endl << endl;

        cout << "Tugas kita adalah menaruh duit tersebut sejauh mungkin dari pintu keluar" << endl;
        cout << "supaya Mas Bayem Sore kesulitan wkwkw" << endl;
        
        // mulai bermain hingga ditemukan jawaban yang benar
        bool farthest = false;
        while (!farthest)
        {
            print_batas();

            int baris, kolom;
            cout << "Petak mana yang terjauh?" << endl;
            cout << "Baris : "; cin >> baris;
            cout << "Kolom : "; cin >> kolom;

            cout << endl;
            if (!(baris >= 1 && baris <= 8) || !(kolom >= 1 && kolom <= 10))
            {
                cout << "Petak yang Anda tuju berada di luar jangkauan!" << endl;
                cout << "Coba lagi yaa.." << endl;
                continue;
            }
            
            int jarak = d[baris-1][kolom-1];
            if (jarak == -1) 
            {
                cout << "Petak yang Anda tuju merupakan sebuah dinding!" << endl;
                cout << "Coba lagi yaa.." << endl;
            }
            else
            {
                if (jarak == 21) 
                {
                    cout << "Anda berhasil menaruh duit di petak terjauh yaitu sebanyak " << jarak << " langkah :)" << endl;
                    farthest = true;
                }
                else
                {
                    cout << "Petak yang Anda tuju hanya membutuhkan " << jarak << " langkah :(" << endl;
                    cout << "Coba lagi yaa.." << endl;
                }
            }
        }
        
        lanjut_customer(user);
    }
    else if (pilih == 4)
    {
        cout << endl;
        return;
    }
    else
    {
        cout << endl;
        cout << "Anda memasukkan angka yang salah!" << endl;
        pilih_customer(user);
    }
}

// tampilan sebelum kembali ke menu
void lanjut_customer(string user)
{
    print_batas();

    cout << "Kembali ke menu?" << endl;
    cout << "Ketik \"Okeh\"/\"Ya\"/\"Gas\" : ";

    string s;
    cin >> s;

    // apapun inputnya pasti akan kembali ke menu
    menu_customer(user);
}

// menampilkan menu
void menu_customer(string user)
{
    print_batas();

    cout << "Menu :" << endl;
    cout << "1. Beli barang" << endl;
    cout << "2. Ada yang ingin kamu tanyakan?" << endl;
    cout << "3. Bosan? Ingin bermain mini-game?" << endl;
    cout << "4. Keluar" << endl;

    pilih_customer(user);
}

// tampilan awal dan akhir jika login sebagai costumer
void customer(string user)
{
    cout << "Anda berhasil login sebagai customer!" << endl;

    menu_customer(user);
    
    cout << "SAMPAI JUMPA :)" << endl;

    login();
}

// memilih menu
void pilih_seller()
{
    print_batas();

    int pilih;
    cout << "Pilih : "; cin >> pilih;

    if (pilih == 1)
    {
        if (n == 10)
        {
            cout << endl << "Anda sudah tidak bisa menambah barang baru (max 10)!" << endl;
            lanjut_seller();
            return;
        }

        print_batas();

        string kode = "00";
        kode += (char)n+48; // kode dibuat secara otomatis 
        n++;

        Product barang;
        cin.ignore();

        cout << "Nama barang : "; getline(cin, barang.name);
        cout << "Stok barang : "; cin >> barang.stock;
        cout << "Harga barang : "; cin >> barang.price;

        mp[kode] = barang; // masukkan ke dalam map (database)
        par[n-1] = n-1; // set parent/perwakilan awal

        cout << endl << barang.name << " berhasil dimasukkan!" << endl;

        lanjut_seller();
    }
    else if (pilih == 2)
    {
        print_batas();

        string kode;
        cout << "Kode barang : "; cin >> kode;

        int num;
        cout << "Tambahan stok : "; cin >> num;

        mp[kode].stock += num; // tambah stok

        cout << endl << "Stok " << mp[kode].name << " berhasil ditambah!" << endl;

        lanjut_seller();
    }
    else if (pilih == 3)
    {
        print_batas();

        string code;
        cout << "Kode barang : "; cin >> code;

        int price;
        cout << "Harga terbaru : "; cin >> price;

        mp[code].price = price; /// ganti harga

        cout << endl << "Harga " << mp[code].name << " berhasil diubah!" << endl;

        lanjut_seller();
    }
    else if (pilih == 4)
    {
        print_batas();

        string kode, kode2;
        cout << "Kode barang pertama : "; cin >> kode;
        cout << "Kode barang kedua : "; cin >> kode2;

        merge((int)kode[2]-48, (int)kode2[2]-48); // disjoint set

        cout << endl << "Berhasil mengelompokkan barang dengan kode " << kode << " dan " << kode2 << "!" << endl;

        lanjut_seller();
    }
    else if (pilih == 5)
    {
        if (q.empty())
        {
            cout << endl << "Tidak ada pertanyaan dari customer!" << endl;
            lanjut_seller();
            return;
        }

        print_batas();

        int x = 0;
        while (!q.empty())
        {
            if (++x > 1)
                print_batas();

            // ambil penanya dan pertanyaan dari queue
            string user = q.front().F;
            string que = q.front().S;
            q.pop(); 

            cout << "Dari : " << user << endl;
            cout << "Pertanyaan : " << que << endl;

            string ans;
            cin.ignore();

            cout << endl << "Jawaban : ";
            getline(cin, ans); // input jawaban

            cout << endl << "Jawaban berhasil dikirim ke " << user << "!" << endl;
        }

        lanjut_seller();
    }
    else if (pilih == 6)
    {
        print_batas();

        cout << "Berikut daftar barang yang tersedia :" << endl;
        cout << "| NO | Nama\t\t| Harga\t| Stok\t| Kode\t|" << endl;

        int num = 1;
        string lmt = "00";
        lmt += (char)n+48-1;

        // output list barang dalam bentuk tabel (kurang rapi)
        for (string code = "000"; code <= lmt; code[2]++)
        {
            cout << "| " << num << "  ";
            cout << "| " << mp[code].name << "\t\t";
            cout << "| " << mp[code].price << "\t";
            cout << "| " << mp[code].stock << "\t";
            cout << "| " << code << "\t|" << endl;
            num++;
        }

        set<int> st; // mengetahui ada berapa kelompok barang
        for (int i = 0; i < n; i++) st.insert(find(i));

        int sz = st.size();
        vector<vector<string>> kel(sz);

        int cnt = 0; // mengisi vector
        for (auto it: st)
        {
            for (int i = 0; i < n; i++)
            {
                if (it == find(i))
                {
                    string code = "00";
                    code += (char)i+48;
                    kel[cnt].push_back(code);
                }
            }
            cnt++;
        }

        // output kelompok barang A, B, C, dst.
        for (int i = 0; i < st.size(); i++)
        {
            char c = (char)i+65;
            cout << "Kelompok " << c << " : ["<< mp[kel[i][0]].name;
            for (int j = 1; j < kel[i].size(); j++)
            {
                cout << ", ";
                cout << mp[kel[i][j]].name;
            }
            cout << "]" << endl;
        }

        lanjut_seller();
    }
    else if (pilih == 7)
    {
        cout << endl;
        return;
    }
    else
    {
        cout << endl;
        cout << "Anda memasukkan angka yang salah!" << endl;
        pilih_seller();
    }
}

// tampilan sebelum kembali ke menu
void lanjut_seller()
{
    print_batas();

    cout << "Kembali ke menu?" << endl;
    cout << "Ketik \"Okeh\"/\"Ya\"/\"Gas\" : ";

    string s;
    cin >> s;

    // apapun inputnya pasti akan kemkbali ke menu
    menu_seller();
}

// menampilkan menu
void menu_seller()
{
    print_batas();

    cout << "Menu :" << endl;
    cout << "1. Memasukkan barang baru" << endl;
    cout << "2. Menambah stok barang" << endl;
    cout << "3. Mengganti harga barang" << endl;
    cout << "4. Mengelompokkan 2 barang" << endl;
    cout << "5. Menjawab pertanyaan dari costumer" << endl;
    cout << "6. Melihat daftar lengkap barang beserta kelompoknya" << endl;
    cout << "7. Keluar" << endl;

    pilih_seller();
}

// tampilan awal dan akhir jika login sebagai seller
void seller()
{
    cout << "Anda berhasil login sebagai seller!" << endl;
    
    menu_seller();

    cout << "SEMANGAT :)" << endl;

    login();
}

// tampilan kembali login atau mengakhiri program
void login()
{
    print_batas();

    cout << "Ketik \"Login\" = Kembali ke login" << endl;
    cout << "Ketik \"Selesai\" = Akhiri program" << endl << endl;
    cout << "Pilih : ";

    string ans;
    cin >> ans;

    if (ans == "Login") // kembali ke tampilan input username dan password
    {
        print_batas();
        cout << "                                LOGIN" << endl;
        run();
    }
    else 
    {
        print_batas();

        // memastikan ulang apakah beneran ingin keluar
        cout << "Apakah kau yakin ingin mengakhiri program?" << endl;
        cout << "Ketik \"Ya\" / \"Tidak\" : ";

        string ans2;
        cin >> ans2;

        if (ans2 == "Tidak") login(); // ditanyakan ulang ingin login atau selesai
        else 
        {
            print_batas();
            // program berakhir
            cout << "             TERIMAKASIH SUDAH MENGGUNAKAN APP TUKUPEDIA!" << endl;
        }
    }
}

// login dengan username dan password
void run()
{
    print_batas();

    string user, pass;
    cout << "Username : "; cin >> user;
    cout << "Password : "; cin >> pass;

    print_batas();

    // contoh username seller : admin, admin0, admin18, admin123, admin129319023
    // contoh username customer : ferdyrizkiawan, halohalo, xkljaklsdf
    ((user.substr(0, 5) == "admin" && pass == "123") ? seller() : customer(user));
}

// main program
int main()
{
    isi_database(); 
    print_batas();

    cout << "                     SELAMAT DATANG DI TUKUPEDIA" << endl;
    cout << "            Di sini kalian bisa membeli peralatan kuliah!" << endl;

    run();

    print_batas();

    return 0;
}