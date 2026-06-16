/*==============================================================*/
/* File        : NBT_static.h                                   */
/* Deskripsi   : ADT Non Binary Tree Statis untuk data Halte    */
/*               (Nama Halte + Jarak ke node tsb dari parent)   */
/*==============================================================*/

#ifndef NBT_STATIC_H
#define NBT_STATIC_H

#define MAKS_NODE 33   /* index 1..32 dipakai, index 0 tidak dipakai */
#define nil 0          /* nilai nil untuk ps_fs/ps_nb/ps_pr          */

/* Satu elemen Tree : Nama Halte + Jarak (dari parent) + pointer statis */
typedef struct {
    char  nama[50];   /* Nama Halte                       */
    float jarak;      /* Jarak dari parent ke halte ini   */
    int   ps_fs;      /* Pointer ke First Son (anak pertama) */
    int   ps_nb;      /* Pointer ke Next Brother (saudara)   */
    int   ps_pr;      /* Pointer ke Parent                   */
} TElmtTree;

typedef TElmtTree Isi_Tree[MAKS_NODE];

/* ===== Modul Program ===== */

/* Membangun Non Binary Tree dengan membaca file teks "filename" */
/* n akan diisi dengan banyaknya node yang berhasil dibaca        */
void BuildTree(Isi_Tree T, int *n, const char *filename);

/* Mencari index node berdasarkan nama (case-insensitive)         */
/* Mengirimkan -1 jika tidak ditemukan                             */
int FindIndex(Isi_Tree T, int n, const char *nama);

/* Mengirimkan true (1) jika node pada index idx adalah daun       */
int IsLeaf(Isi_Tree T, int idx);

/* Mencari jalur (path) dari root ke target dengan PreOrder        */
/* memanfaatkan ADT Stack Dinamis. Hasil disimpan di path[]        */
/* dengan urutan root -> ... -> target. len diisi panjang path     */
/* Mengirimkan 1 jika ketemu, 0 jika tidak ketemu                   */
int GetPathArray(Isi_Tree T, int root, int target, int path[], int *len);

/* Menampilkan semua nama halte secara Level Order, dinumbering    */
void PrintLevelOrder(Isi_Tree T, int n);

#endif