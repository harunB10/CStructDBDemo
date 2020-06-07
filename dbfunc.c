#include "dbfunc.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
** initItems je funkcija koja inicijalizira vrijednosti na defaultne.
** Za ubacivanje vrijednosti u niz od struct-a (Data_t items) korištena je strcpy funkcija.
** Prvi parameter ove funkcije je placeholder odnosno mjesto gdje se ubacuje, a drugi vrijednost koja se ubacuje
*/

void initItems(Data_t *items, long len)
{
  for (long i = 0; i < len; i++)
  {
    strcpy(items[i].name, "");
    strcpy(items[i].brand, "");
    items[i].year = 0;
    strcpy(items[i].invNr, "");
  }
}

/*
** printItem ispisuje vrijednosti elemenata iz niza koji se prosljeđuju. Ovdje se radi o konkretnom elementu, npr:
** ako je Data_t niz[3], ovdje se prosljeđuje npr element niz[0]. Zbog toga se koriste strelice jer se pristupa preko pointera
*/

void printItem(Data_t *item)
{
  printf("\nName:\t %s", item->name);
  printf("\nBrand:\t %s\n", item->brand);
  printf("\nYear:\t %ld\n", item->year);
  printf("\nInvNr:\t %s\n", item->invNr);
}

/*
** printDB ispisuje sve elemente u struct nizu. Isčitava sve elemente i prosljeđuje ih jedan po jedan u funkciju printItem
** koja je već proslijeđena.
*/


void printDB(Data_t *items, long len)
{
  printf("\nDatabase entries:\n");
  for (long i = 0; i < len; i++)
  {
    printf("----------Entry: %ld----------", i + 1);
    printItem(&items[i]);
  }
}

/*
** readValue čita unos sa tastature
*/

long readValue(char *name)
{
  if ((name != NULL) && (name[0] == '\0')) // provjera je li prazno
  {
    fprintf(stderr, "Name cannot be empty nor null");
  }

  char input[TEXT_LEN] = "";
  int length, i;

  scanf("%s", input);
  length = strlen(input);
  for (i = 0; i < length; i++)
    if (!isdigit(input[i]))  // pomoću isDigit provjerava se da li je unešena vrijednost broj.. ako je slovo treba izbaciti error
    {
      fprintf(stderr, "\nEntered input is not a number\n");
      return -1;
    }

  printf("\n%s: ", name);

  long number = atol(input); // atol pretvara char u long ... atoi npr pretvara char u int.

  if (number < 0)
  {
    fprintf(stderr, "Number must be positive");
  }

  return number;
}

/*
** readString je slična kao readValue
*/

void readString(char *name, char *text, long len)
{
  if ((name != NULL) && (name[0] == '\0'))
  {
    fprintf(stderr, "Name cannot be empty nor null");
  }

  if (len < 0)
  {
    fprintf(stderr, "Lenght must be greater than 0");
  }
  else
  {
    printf("\n%s: ", name);
    scanf("%s", text);
  }
}

/*
** addItem ubacuje element u struct niz
*/

long addItem(Data_t *items, long len)
{
  for (long i = 0; i < len; i++)
  {
    if (items[i].name[0] == '\0')
    {
      char temp[TEXT_LEN];
      readString("Name", temp, TEXT_LEN);
      strcpy(items[i].name, temp);
      readString("Brand", temp, TEXT_LEN);
      strcpy(items[i].brand, temp);
      items[i].year = readValue("Year");
      readString("InvNr", temp, TEXT_LEN);
      strcpy(items[i].invNr, temp);

      printf("\n----------Added item:----------\n");
      printItem(&items[i]);
      return 0;
    }
  }
  return -1;
}

/*
** ovo je pomoćna funkcija za qsort koja sortira po imenu. Postavlja pointere na Data_t i onda provjerava po abecednom redu
*/

long compareDataByName(const void *v1, const void *v2)
{
  const Data_t *u1 = v1;
  const Data_t *u2 = v2;
  if (*u1->name == '\0') // ovo se koristi jer se u zadatku traži da prazna polja idu na zadnje mjesto u nizu
    return (*u2->name == '\0') ? 0 : 1;
  if (*u2->name == '\0')
    return -1;
  return strcmp(u1->name, u2->name);
}

/*
** ovo je pomoćna funkcija za qsort koja sortira po invnr
*/

long compareDataByInvNr(const void *v1, const void *v2)
{
  const Data_t *u1 = v1;
  const Data_t *u2 = v2;
  if (*u1->invNr == '\0')
    return (*u2->invNr == '\0') ? 0 : 1;
  if (*u2->invNr == '\0')
    return -1;
  return strcmp(u1->invNr, u2->invNr);
}

/*
** ovo je pomoćna funkcija za qsort koja sortira po godini. Postavlja pointere na Data_t i onda provjerava po abecednom redu
*/

long compareDataByYear(const void *v1, const void *v2)
{
  const Data_t *u1 = v1;
  const Data_t *u2 = v2;

  return (u1->year > u2->year) - (u1->year < u2->year);
}

/*
** deleteItem briše element iz niza. Prvo se traži od korisnika da unese index elementa koji se želi brisati
** NAPOMENA!!! nizovi u većini programskih jezika, pa i u C kreću od nule. Tako da, ukoliko korisnik želi
** obrisati prvi element niza unijet će 0 !!!
*/

long deleteItem(Data_t *items, long len)
{

  long index = readValue("\nEnter the index of element that you want to delete:\n");

  if (len < index)
  {
    fprintf(stderr, "\nIndex is out of range\n"); // provjera da li je korisnik unio index koji je van dometa
    return -1;
  }

  else if (items[index].name[0] == '\0')
  {
    fprintf(stderr, "\nElement at index %ld is already blank\n", index); // provjera da li je element na tom indeksu prazan
    return -1;
  }

  else
  {

    strcpy(items[index].name, "");
    strcpy(items[index].brand, "");
    strcpy(items[index].invNr, "");
    items[index].year = 0;

    qsort(items, len, sizeof(Data_t), compareDataByName); // soritra se niz tako da sve prazne elemente stavlja na zadnje pozicije
  }

  return 0;
}

/*
** saveDB sprema elemente niza u file koji se prosljeđuje u outputFile.
*/

long saveDB(Data_t *items, long len, char *outputFile)
{
  FILE *fp;

  if ((fp = fopen(outputFile, "w+"))) // provjera da li se fajl može otvoriit. w+ znači pisanje u fajl i overwrite svaki put kad se funkcija pozove
  {

    // ovo je da bi se napravio alignment kolona u fileovima tako da ljepše izgleda
    fprintf(fp, "%-20s %-20s %-20s %-4s\n", "#Name", "#Brand", "#Inventar-Nr.", "#Jahr");

    for (long i = 0; i < len; i++)
    {
      fprintf(fp, "%-20s %-20s %-20s %-4ld\n", items[i].name, items[i].brand, items[i].invNr, items[i].year);

      if (feof(fp)) //ako je dosegnut kraj fajla (zbog manjka memorije npr.)
      {
        printf("\n --------------------------");
      }
      else
      {
        printf("\n Failure."); // greška prilikom unosa
      }
    }

    if(fp)
    {
    fclose(fp);
    } // zatvaranje konekcije sa fajlom
  }

  else
  {
    fprintf(stderr, "Error while opening file\n"); // greška ako se fajl nije mogao otvoriti
    return -1;
  }

  return 0;
}


/*
** loadDB čita iz fajla podatke i ubacuje ih u struct niz
*/

long loadDB(Data_t *items, long len, char *inputFile)
{
  FILE *fp;
  char ch;
  char name[TEXT_LEN];
  char brand[TEXT_LEN];
  char invNr[TEXT_LEN];
  long year;
  long i;

  if ((fp = fopen(inputFile, "r")))
  {

    while ((ch = getc(fp) != '\n') && ch != EOF)
      ; // ignoriši prvu liniju jer je header... #Name #Brand #InvNr #Year ... Ovo se ne sprema u varijable
    printf("Reading....\n");
    for (i = 0; i < len && (fscanf(fp, "%s %s %s %ld", name, brand, invNr, &year) != EOF); i++)
    {
      if (name[0] == '\0' || brand[0] == '\0' || invNr[0] == '\0')
      {
        fprintf(stderr, "Failed... File format is wrong\n");
        initItems(items, len);
      }
      else
      {
        strcpy(items[i].name, name);
        strcpy(items[i].brand, brand);
        strcpy(items[i].invNr, invNr);
        items[i].year = year;

        memset(name, 0, sizeof name);
        memset(brand, 0, sizeof brand);
        memset(invNr, 0, sizeof invNr);
        printf("\n----------Loaded items: ----------\n");
      }
    }
    printf("\n----------------------------------\n");
  }
  else
  {
    fprintf(stderr, "File Error");
    return -1;
  }

  return 0;
}


/*
** saveFile pretražuje fajl po zadanom key-u
*/

long searchFile(char *inputFile, char *key, long ignoreCase)
{
  if (inputFile[0] == '\0')
  {
    fprintf(stderr, "File name is empty");
  }

  FILE *fp;
  int line_num = 1;
  int find_result = 0;
  char temp[512];

  if ((fp = fopen(inputFile, "r")) == NULL)
  {
    return -1;
  }

  if (ignoreCase == 0) // ako je ignoreCase 0 onda se gleda malo i veliko slovo... npr. Tarik ili tarik nije isto u ovom slučaju
  {
    while (fgets(temp, 512, fp) != NULL)
    {
      if ((strstr(temp, key)) != NULL) // funkcija koja provjerava da li u char nizu ima tražena riječ - više o ovome na http://www.cplusplus.com/reference/cstring/strstr/
      {
        printf("%s Line: %d, %s\n", inputFile, line_num,
               temp);
        find_result++; // broj nađenih rezultata
      }
      line_num++; // linija u kojoj je nađen term
    }
  }

  if (ignoreCase != 0) // ako je ignoreCase 1 ili bilo koji broj osim 0 onda veliko i malo slovo nije bitno... https://pubs.opengroup.org/onlinepubs/009695399/functions/strcasecmp.html
  {
    while (fgets(temp, 512, fp) != NULL)
    {
      if ((strcasecmp(temp, key)) > 1)
      {
        printf("%s Line: %d, %s\n", inputFile, line_num,
               temp);
        find_result++;
      }
      line_num++;
    }
  }

  if (find_result == 0)
  {
    printf("\nCouldn't find a match.\n");
    return -1;
  }

  if (fp)
  {
    fclose(fp);
  }
  return 0;
}

/*
** saveFile pretražuje fajl po zadanom key-u ... Zavisno od which parametra koriste se pomoćne funkcije za qsort. 
** više detalja o pomoćnim funkcijama je napisano iznad
*/

long sortItems(Data_t *items, long len, long which)
{
  if (which == 1)
  {
    printf("\nSorting by Inventar - Nr....\n");
    qsort(items, len, sizeof(Data_t), compareDataByInvNr);
  }

  if (which == 2)
  {
    printf("\nSorting by Year....\n");
    qsort(items, len, sizeof(Data_t), compareDataByYear);
  }

  else
  {
    printf("\nWhich parameter is invalid... Please choose 1 (sort by InvNr.) or 2 (sort by Year) only\n");
    return -1;
  }
  return 0;
}


/*
** addFromFile pretražuje fajl po zadanom key-u i settuje vrijednosti niza structa od tog rezultata što je pronađen
*/

long addFromFile(Data_t *items, long len, char *inputFile, char *key)
{

  if (inputFile[0] == '\0')
  {
    fprintf(stderr, "File name is empty");
  }

  FILE *fp;
  int line_num = 1;
  int find_result = 0;
  char temp[512];
  char *foundResults[3];

  if ((fp = fopen(inputFile, "r")) == NULL)
  {
    return -1;
  }

  while (fgets(temp, 512, fp) != NULL)
  {
    if ((strstr(temp, key)) != NULL)
    {
      if (line_num == 1)
      {
        line_num++; //ignoriši prvu liniju fajla jer je to header #Name #Brand itd
        continue;
      }

      char *split = strtok(temp, " "); //strtok funkcija razdvaja riječ po riječ.. npr "Danas je lijep dan" razdvojit će na "danas", "lijep", "dan"
      char *word;
      char foundResults[3];
      long i = 0;
      while (split != NULL)
      {
        if (find_result >= len)
        {
          fprintf(stderr, "\nThere is no more memory in DB...\n"); // provjera ako broj pronađenih rezultata je veći od mjesta u struct nizu (bazi)
          return -1;
        }
        // ovdje se vrši settiranje vrijednosti za items... spremi se jedna riječ onda se ponovo poziva strtok da pređe na slijedeću riječ
        strcpy(items[find_result].name, split);
        split = strtok(NULL, " ");
        strcpy(items[find_result].brand, split);
        split = strtok(NULL, " ");
        strcpy(items[find_result].invNr, split);
        split = strtok(NULL, " ");
        items[find_result].year = atol(split);
        split = strtok(NULL, " ");
      }
      find_result++;
    }
    line_num++;
  }

  if (find_result == 0) // nema pronađenih rezultata
  {
    printf("\nCouldn't find a match.\n");
    return -1;
  }

  if (fp)
  {
    fclose(fp);
  }

  return 0;
}
