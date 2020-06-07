// Ovo je header file. Tu se definiše struktura podataka i prototipi funkcija koji se koriste
// implementacija se vrši u dbfunc.c

#include <stdio.h>
#include <string.h>

#define TEXT_LEN 100

// varijable

struct Data_s{
  char name[TEXT_LEN];
  char brand[TEXT_LEN];
  char invNr[TEXT_LEN];
  long year;
};
typedef struct Data_s Data_t;


//prototipi funkcija 
void initItems(Data_t *items, long int len);
void printItem(Data_t *item);
void printDB(Data_t *items, long len);
long readValue (char *name);
void readString(char *name, char *text, long len);
long addItem(Data_t *items, long len);
long deleteItem(Data_t *items, long len);
long saveDB(Data_t *items, long len, char *outputFile);
long loadDB(Data_t *items, long len, char *inputFile);
long searchFile(char *inputFile, char *key, long ignoreCase);
long sortItems(Data_t *items, long len, long which);
long addFromFile(Data_t *items, long len, char *inputFile, char *key);