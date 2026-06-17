/*==============================================================*/
/* File        : NBT_static.c                                   */
/* Deskripsi   : Implementasi ADT Non Binary Tree Statis        */
/*==============================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "NBT_static.h"
#include "stack_dinamis.h"

/* ===== Fungsi bantu (helper) lokal ===== */

/* Bandingkan dua string tanpa membedakan huruf besar/kecil */
static int SamaTanpaCase(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        a++; b++;
    }
    return (*a == '\0' && *b == '\0');
}

/* Ubah string angka dengan koma (2,3) menjadi float (2.3) */
static float ParseJarak(const char *s) {
    char buf[32];
    int i = 0;
    while (s[i] != '\0' && i < 31) {
        buf[i] = (s[i] == ',') ? '.' : s[i];
        i++;
    }
    buf[i] = '\0';
    return (float) atof(buf);
}

/* Ambil teks setelah tanda '=' pada satu baris, dan hilangkan newline */
static int AmbilNilai(const char *baris, char *out) {
    const char *p = strchr(baris, '=');
    if (p == NULL) return 0;
    p++; /* lewati '=' */
    while (*p == ' ') p++; /* lewati spasi */

    int i = 0;
    while (p[i] != '\0' && p[i] != '\n' && p[i] != '\r') {
        out[i] = p[i];
        i++;
    }
    out[i] = '\0';
    return 1;
}

/*==============================================================*/
/* BuildTree : Membangun Non Binary Tree dari file teks          */
/*==============================================================*/
void BuildTree(Isi_Tree T, int *n, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Gagal membuka file %s!\n", filename);
        *n = 0;
        return;
    }

    /* inisialisasi semua node kosong */
    for (int i = 0; i < MAKS_NODE; i++) {
        T[i].nama[0] = '\0';
        T[i].jarak   = 0;
        T[i].ps_fs   = nil;
        T[i].ps_nb   = nil;
        T[i].ps_pr   = nil;
    }

    char baris[100];
    char nilai[50];
    nilai[0] = '\0';
    *n = 0;

    /* Baca baris pertama: ROOT = ... */
    if (fgets(baris, sizeof(baris), fp) != NULL) {
        AmbilNilai(baris, nilai);
        *n = 1;
        strcpy(T[1].nama, nilai);
        T[1].jarak = 0;
        T[1].ps_pr = nil;
        T[1].ps_fs = nil;
        T[1].ps_nb = nil;
    }

    /* Baca grup 3 baris : Parent = .. / Node = .. / Jarak = .. */
    char namaParent[50], namaNode[50], jarakStr[50];

    while (fgets(baris, sizeof(baris), fp) != NULL) {
        if (strncmp(baris, "Parent", 6) != 0) continue;
        AmbilNilai(baris, namaParent);

        if (fgets(baris, sizeof(baris), fp) == NULL) break;
        AmbilNilai(baris, namaNode);

        if (fgets(baris, sizeof(baris), fp) == NULL) break;
        AmbilNilai(baris, jarakStr);

        (*n)++;
        int idx = *n;

        strcpy(T[idx].nama, namaNode);
        T[idx].jarak = ParseJarak(jarakStr);
        T[idx].ps_fs = nil;
        T[idx].ps_nb = nil;

        /* cari index parent */
        int pi = FindIndex(T, idx - 1, namaParent);
        if (pi == -1) pi = 1; /* fallback ke root jika tidak ketemu */
        T[idx].ps_pr = pi;

        /* pasang sebagai anak (first son / next brother) */
        if (T[pi].ps_fs == nil) {
            T[pi].ps_fs = idx;
        } else {
            int temp = T[pi].ps_fs;
            while (T[temp].ps_nb != nil) {
                temp = T[temp].ps_nb;
            }
            T[temp].ps_nb = idx;
        }
    }

    fclose(fp);
}

/*==============================================================*/
/* FindIndex : Mencari index node berdasarkan nama (case-ins)    */
/*==============================================================*/
int FindIndex(Isi_Tree T, int n, const char *nama) {
    for (int i = 1; i <= n; i++) {
        if (T[i].nama[0] != '\0' && SamaTanpaCase(T[i].nama, nama)) {
            return i;
        }
    }
    return -1;
}

/*==============================================================*/
/* IsLeaf : true jika node tidak punya anak                      */
/*==============================================================*/
int IsLeaf(Isi_Tree T, int idx) {
    return (T[idx].ps_fs == nil);
}

/*==============================================================*/
/* FindPathRec : PreOrder + Stack untuk mencari jalur            */
/* Sesuai konsep : kunjungi, tanyakan, PUSH, jika daun & bukan    */
/* target -> POP, lanjut ke saudara berikutnya                   */
/*==============================================================*/
static int FindPathRec(Isi_Tree T, int curr, int target, Stack *S) {
    Push(S, curr);                 /* kunjungi & push node ini      */

    if (curr == target) {
        return 1;                  /* ketemu, stack tidak di-pop    */
    }

    int anak = T[curr].ps_fs;
    while (anak != nil) {
        if (FindPathRec(T, anak, target, S)) {
            return 1;
        }
        anak = T[anak].ps_nb;       /* pergi ke anak lain (saudara)  */
    }

    /* sudah sampai ke daun & bukan target -> kembali (POP) */
    infotype dummy;
    Pop(S, &dummy);
    return 0;
}

/*==============================================================*/
/* GetPathArray : ambil isi stack jadi array urutan root->target  */
/*==============================================================*/
int GetPathArray(Isi_Tree T, int root, int target, int path[], int *len) {
    Stack S, S2;
    CreateStack(&S);

    if (!FindPathRec(T, root, target, &S)) {
        return 0; /* tidak ketemu */
    }

    /* Stack S isinya : TOP = target ... bawah = root            */
    /* Pindahkan ke Stack S2, supaya saat di-POP urutannya        */
    /* menjadi root -> ... -> target (sesuai "stack baru")        */
    CreateStack(&S2);
    while (!IsEmpty(S)) {
        infotype x;
        Pop(&S, &x);
        Push(&S2, x);
    }

    *len = 0;
    while (!IsEmpty(S2)) {
        infotype x;
        Pop(&S2, &x);
        path[(*len)++] = x;
    }

    return 1;
}

/*==============================================================*/
/* PrintLevelOrder : tampilkan semua nama halte secara berurutan */
/*==============================================================*/
void PrintLevelOrder(Isi_Tree T, int n) {
    int no = 1;
    for (int i = 1; i <= n; i++) {
        if (T[i].nama[0] != '\0') {
            printf("%d. Halte %s\n", no, T[i].nama);
            no++;
        }
    }
}