/*==============================================================*/
/* File        : stack_dinamis.h                                */
/* Deskripsi   : ADT Stack Dinamis (Linked List)                */
/*               infotype = int -> menyimpan index halte di Tree*/
/*==============================================================*/

#ifndef STACK_DINAMIS_H
#define STACK_DINAMIS_H

#include <stdlib.h>

#define Nil NULL

typedef int infotype;   /* infotype berupa index node pada Isi_Tree */

typedef struct elmtlist {
    infotype info;
    struct elmtlist *next;
} ElmtList;

typedef ElmtList *address;

typedef struct {
    address TOP;
} Stack;

#define AddressTop(S) ((*S).TOP)

typedef int boolean;
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

/* Konstruktor */
void CreateStack(Stack *S);

/* Validator */
boolean IsEmpty(Stack S);

/* Getter */
infotype Top(Stack S);

/* Push */
void Push(Stack *S, infotype X);

/* Pop */
void Pop(Stack *S, infotype *X);

/* Hitung jumlah elemen */
int Count(Stack S);

#endif