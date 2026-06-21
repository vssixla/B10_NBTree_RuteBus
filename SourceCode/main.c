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