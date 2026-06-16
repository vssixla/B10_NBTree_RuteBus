/*==============================================================*/
/* File        : stack_dinamis.c                                */
/* Deskripsi   : Implementasi ADT Stack Dinamis (Linked List)   */
/*==============================================================*/

#include "stack_dinamis.h"
#include <stdio.h>

/* Konstruktor */
/* Membuat stack kosong */
void CreateStack(Stack *S) { //mengatur pointer TOP ke Nil (kosong),
    AddressTop(S) = Nil; //belum ada node yang ditunjuk
}

/* Validator */
/* Mengecek apakah stack kosong */
boolean IsEmpty(Stack S) {
    return (AddressTop(&S) == Nil); //jika TOP menunjuk ke Nil, berarti stack kosong
}

/* Getter */
/* Mengambil nilai TOP */
infotype Top(Stack S) {
    if (!IsEmpty(S)) {
        return AddressTop(&S)->info; //Mengakses field 'info' dari node yang ditunjuk TOP
    }
    return -1;
}

/* Push */
/* Menambahkan elemen ke TOP */
void Push(Stack *S, infotype X) {
    address P;

    /* Alokasi node baru */
    P = (address) malloc(sizeof(ElmtList));

    if (P != Nil) {
        P->info = X; // Mengisi node baru dengan data X
        P->next = AddressTop(S); /* menunjuk ke TOP lama */
        AddressTop(S) = P;       /* TOP pindah ke node baru */
    } else {
        printf("Alokasi gagal!\n"); //Kalau memori komputer penuh
    }
}

/* Pop */
/* Menghapus elemen TOP */
void Pop(Stack *S, infotype *X) {
    address P;

    if (!IsEmpty(*S)) {
        P = AddressTop(S);       // P menunjuk ke node yang akan dihapus (TOP saat ini)
        *X = P->info;            // Mengambil datanya untuk dikembalikan ke user
        AddressTop(S) = P->next; // TOP digeser ke node di bawahnya
        free(P);                 // Menghapus node P dari memori
    } else {
        printf("Stack kosong!\n");
    }
}

/* hitung jumlah */
int Count(Stack S) {
    int count = 0;
    address P = S.TOP; //Mulai dari puncak

    while (P != Nil) {     //Selama belum sampai ke ujung (Nil)
        count++;        //Tambah hitungan
        P = P->next;    //Pindah ke node berikutnya
    }
    return count;
}