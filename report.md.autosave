Laporan Analisis Garis (UTS Grafika Komputer)

Tanggal: 20 Oktober 2025

1. Pendahuluan

Laporan ini dibuat untuk Ujian Tengah Semester mata kuliah Grafika Komputer. Intinya adalah bagaimana kita bisa membuat komputer mengenali potongan garis lurus yang kita gambar di Canvas.

Tiga tujuan utama kita di sini:

Menentukan seberapa besar "kaca pembesar" (window) yang paling pas buat ngeliat garis.

Mencatat semua pola garis yang ideal dari hasil scanning data.

Bikin kode deteksi otomatis berdasarkan pola-pola yang udah kita temukan.


2. Kenapa Pilih Window 3x3? (Goal 1)

Intinya: Kita pilih window 3x3 (WINDOW_SIZE = 3).

Alasannya: Garis yang kita gambar itu tebalnya 4 pixel. Kalau pakai window 3x3, ukurannya pas banget:

Cukup kecil buat fokus ke satu bagian garis aja.

Cukup besar buat nangkap bagian padat ("inti" garis) dan bagian tepinya.

Dari hasil dump data, kita lihat pola 3x3 benar-benar konsisten merepresentasikan garis tebal, misalnya dengan pola 1 1 1, 1 1 1, 1 1 1 di tengah-tengah garis. Jadi, ini ukuran yang paling minimal dan efektif.

3. Pola-Pola Garis Ideal (Goal 2)

Setelah kita scan semua window 3x3 di Canvas, ada 5 pola utama yang paling sering muncul dan paling mewakili garis kita.

Nama Pola

Representasi 3x3 (1=Garis, 0=Background)

Deskripsi (Dilihat Langsung dari Data Dump)

Inti Garis (Solid Core)

1 1 1
1 1 1
1 1 1

Ini adalah pola "full block" yang paling sering dan paling stabil. Ditemukan di tengah-tengah garis tebal (Contoh: Window 36-42).

Tepi Atas (Upper Edge)

0 0 0
1 1 1
1 1 1

Menangkap batas atas garis. Baris paling atas di window masih kosong/putih. (Contoh: Window 11-14).

Tepi Bawah (Lower Edge)

1 1 1
1 1 1
0 0 0

Kebalikan dari Tepi Atas. Menangkap batas bawah garis, baris paling bawah sudah kosong.

Tepi Kiri/Diagonal Naik

0 1 1
1 1 1
1 1 1

Menangkap bagian di mana garis mulai agak miring ke atas atau di ujung kiri garis. (Contoh: Window 35, 48).

Tepi Kanan/Diagonal Turun

1 1 1
1 1 1
1 1 0

Menangkap transisi di ujung kanan atau saat garis mulai agak miring ke bawah. (Contoh: Window 43, 73).

4. Implementasi Deteksi Otomatis (Goal 3 - Milestone 2)

A. Cara Kerjanya

Deteksi segmen di kode kita (segmentDetection()) bekerja begini:

Simpan Pola: Pola-pola ideal di atas kita masukkan ke dalam variabel m_idealPatterns.

Cek Kecocokan: Kode akan memutar semua data window hasil scan (scannedWindows) dan membandingkan polanya satu per satu dengan m_idealPatterns secara mutlak.

Kasih Tanda: Kalau ada yang cocok, lokasi window itu langsung ditandai dengan kotak ungu $3 \times 3$ pixel di Canvas.

B. Verifikasi Kode

Pola-pola yang ada di tabel ini sudah saya masukkan persis ke dalam konstruktor DrawingCanvas untuk membuktikan bahwa hasil temuan analisis ini dipakai langsung oleh kode sumber, sesuai arahan UTS.
