/*============================================================== */
/* File        : main.c                                          */
/* Deskripsi   : Program Pencarian Rute Bus TransJakarta         */
/*               menggunakan ADT Non Binary Tree statis dan      */
/*               ADT Stack Dinamis                               */
/*============================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "NBT_static.h"
#include "stack_dinamis.h"

#define ROOT 1
#define BIAYA_PER_HALTE 3500

/*========================= FUNGSI BANTU =========================*/

int strcasecmp_local(const char *a, const char *b);

/* Hilangkan newline di akhir string hasil fgets */
void HapusNewline(char *s) {
    int len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[--len] = '\0';
    }
}

/* Jika input diawali "halte " (case-insensitive), buang awalan itu */
void BuangAwalanHalte(char *s, char *hasil) {
    if (strlen(s) >= 6) {
        char awal[7];
        strncpy(awal, s, 6);
        awal[6] = '\0';
        if (strcasecmp_local(awal, "halte ") == 0) {
            strcpy(hasil, s + 6);
            return;
        }
    }
    strcpy(hasil, s);
}

/* strcasecmp portable sederhana */
int strcasecmp_local(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 1;
        }
        a++; b++;
    }
    return (*a == '\0' && *b == '\0') ? 0 : 1;
}

/* Format angka jarak (float) -> "14,7" */
void FormatJarak(float val, char *out) {
    sprintf(out, "%.1f", val);
    for (int i = 0; out[i] != '\0'; i++) {
        if (out[i] == '.') out[i] = ',';
    }
}

/* Format biaya (int rupiah) -> "Rp10.500,00" */
void FormatBiaya(int val, char *out) {
    char angka[20];
    sprintf(angka, "%d", val);
    int len = strlen(angka);

    char hasil[30];
    int hi = 0;
    for (int i = 0; i < len; i++) {
        hasil[hi++] = angka[i];
        int sisaDigit = len - i - 1;
        if (sisaDigit > 0 && sisaDigit % 3 == 0) {
            hasil[hi++] = '.';
        }
    }
    hasil[hi] = '\0';

    sprintf(out, "Rp%s,00", hasil);
}

/*========================= TAMPILAN MENU =========================*/

void TampilkanMenu() {
    printf("=========================================================\n");
    printf("|             PROGRAM RUTE BUS TRANSJAKARTA             |\n");
    printf("=========================================================\n");
    printf("Selamat datang di Rute Bus TransJakarta wilayah Jakarta\n\n");
    printf("Silahkan pilih menu layanan di bawah ini!\n");
    printf("1. Cek Ketersediaan Halte\n");
    printf("2. Rute Bus dari Blok M menuju suatu Halte\n");
    printf("3. Rute Bus dari suatu Halte menuju BLOK M\n");
    printf("4. Rute Bus dari suatu Halte menuju Halte lain\n");
    printf("5. Daftar Pemberhentian Halte terakhir\n");
    printf("6. Keluar\n");
    printf("Pilihan : ");
}

/*========================= MENU 1 =========================*/

void Menu1(Isi_Tree T, int n) {
    char input[60], nama[60];

    printf("\nSilahkan Input Nama Halte : ");
    fgets(input, sizeof(input), stdin);
    HapusNewline(input);
    BuangAwalanHalte(input, nama);

    int idx = FindIndex(T, n, nama);

    if (idx != -1) {
        printf("\nHalte %s TERSEDIA\n", T[idx].nama);
    } else {
        printf("\nMohon maaf! Halte %s TIDAK TERSEDIA,\n", nama);
        printf("Berikut daftar halte yang tersedia\n");
        printf("=============================================\n");
        printf("|        DAFTAR HALTE YANG TERSEDIA         |\n");
        printf("=============================================\n");
        PrintLevelOrder(T, n);
    }
}

/*========================= MENU 2 =========================*/

void Menu2(Isi_Tree T, int n) {
    char input[60], nama[60];

    printf("=====================================================================\n");
    printf("|             RUTE BUS DARI BLOK M MENUJU SUATU HALTE               |\n");
    printf("=====================================================================\n");
    printf("Silahkan input Halte yang ingin Anda tuju : ");
    fgets(input, sizeof(input), stdin);
    HapusNewline(input);
    BuangAwalanHalte(input, nama);

    int idx = FindIndex(T, n, nama);

    if (idx == -1) {
        printf("Mohon maaf Halte %s TIDAK TERSEDIA\n\n", nama);
        printf("Silahkankan untuk memilih Halte lain,\n");
        printf("atau melihat Daftar Halte yang TERSEDIA pada menu 1\n\n");
        printf("Terima Kasih!\n");
        printf("======================================================================\n");
        return;
    }

    printf("Halte %s TERSEDIA\n\n", T[idx].nama);

    int path[MAKS_NODE], len;
    GetPathArray(T, ROOT, idx, path, &len);

    printf("Untuk menuju Halte %s, silahkan mengikuti jalur Halte berikut ini!\n", T[idx].nama);
    for (int i = 0; i < len; i++) {
        printf("Halte %s", T[path[i]].nama);
        if (i != len - 1) printf(" -> ");
    }
    printf("\n\n");

    float totalJarak = 0;
    for (int i = 1; i < len; i++) {
        totalJarak += T[path[i]].jarak;
    }
    int pemberhentian = len;
    int biaya = pemberhentian * BIAYA_PER_HALTE;

    char strJarak[20], strBiaya[30];
    FormatJarak(totalJarak, strJarak);
    FormatBiaya(biaya, strBiaya);

    printf("KETERANGAN\n");
    printf("TOTAL JARAK YANG DITEMPUH \t: %s Kilometer\n", strJarak);
    printf("BANYAK PEMBERHENTIAN \t\t: %d Halte Bus\n", pemberhentian);
    printf("ESTIMASI BIAYA \t\t\t: %s\n\n", strBiaya);

    printf("Selamat berjalan dan hati-hati dalam perjalanan!\n");
    printf("======================================================================\n");
}