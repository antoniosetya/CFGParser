/*  NIM/Nama		: 13516002/Antonio Setya
	Tanggal			: 28 September 2017
	Topik praktikum : ADT Mesin Kata Versi 1 */
/* File: mesintoken.h */
/* Definisi Mesin Token: Model Akuisisi Versi I */

#include "boolean.h"
#include "mesinkar.h"
#include "mesintoken.h"

boolean EndToken;
Token CToken;
char CC;
boolean EOP;

void IgnoreBlank() {
/* Mengabaikan satu atau beberapa BLANK
   I.S. : CC sembarang 
   F.S. : CC ≠ BLANK atau CC = MARK */
	while ((CC == BLANK) && (CC != MARK)) {
		ADV();
	}
}

void STARTTOKEN() {
/* I.S. : CC sembarang 
   F.S. : EndToken = true, dan CC = MARK; 
          atau EndToken = false, CToken adalah Token yang sudah diakuisisi,
          CC karakter pertama sesudah karakter terakhir Token */
	START();
	IgnoreBlank();
	if (CC == MARK) {
		EndToken = true;
	}
	else {
		EndToken = false;
		SalinToken();
	}
}

void ADVTOKEN() {
/* I.S. : CC adalah karakter pertama kata yang akan diakuisisi 
   F.S. : CToken adalah Token terakhir yang sudah diakuisisi, 
          CC adalah karakter pertama dari kata berikutnya, mungkin MARK
          Jika CC = MARK, maka EndToken = true      
   Proses : Akuisisi kata menggunakan procedure SalinKata */
	IgnoreBlank();
	if (CC == MARK) {
		EndToken = true;
	}
	else {
		SalinToken();
	}
}

void SalinToken() {
/* Mengakuisisi Token dan menyimpan hasilnya dalam CToken
   I.S. : CC adalah karakter pertama dari Token
   F.S. : CToken berisi Token yang sudah diakuisisi; 
          CC = BLANK atau CC = MARK; 
          CC adalah karakter sesudah karakter terakhir yang diakuisisi.
          Jika panjang kata melebihi NMax, maka sisa kata "dipotong" */
	int i = 1;
	CToken.val = 0;
	while (!(EOP || (CC == BLANK)) && (i <= NMax)) {
		if((CC != '+') && (CC != '-') && (CC != '*') && (CC != '/') && (CC != '^')) {
			CToken.tkn = 'b';
			CToken.val = (CToken.val * 10) + ((int) CC - '0');
		}
		else {
			CToken.tkn = CC;
			CToken.val = -1;
		}
		ADV();
		if (!(EOP || (CC == BLANK))) {
			i++;
		}
	}
	IgnoreBlank();
}
