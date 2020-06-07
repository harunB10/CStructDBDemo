/**
 * 
 * 
 * 
 *  KAKO TESTIRATI?
 * 
 *  ----------- PART BEZ COMMAND LINE - A... --------------------
 *  U zavisnosti od IDE-a buildaj, kompajliraj i pokreni projekt. Sa command line to možeš uraditi ovako (pod uslovom da koristite gcc kompajler)
 *  gcc main.c dbfunc.c -o test
 *  Zatim ./test
 *  Onda možeš ovo:
 * 
 *  1. Promijeni dbMemory na 10 jer se traži u zadatku
 *  2. Unesi vrijednosti.. U meniju broj 2
 *  3. Spremi u DB saveDB(4)
 *  4. Ponovo pokreni program
 *  5. Pritisni 1 (initDB) da vidiš jesu li sve vrijednosti prazne
 *  6. Pritisni 5 da čita iz baze loadDB
 *  7. Pritisni opet 1 da vidiš jesu li postavljene vrijednosti kako treba
 *  8. Pritisni 7 sortData da soritira DB
 * 
 *  ------------ PART SA COMMAND LINE - OM (od TEIL D) -----------
 *  
 *  Prethodno moraš kreirati DB (npr test.txt)
 *  Pogledaj ovaj screenshot https://drive.google.com/file/d/1CMgLQL2OoYF9vT9Y2WZ4gFFR8TqjWTrz/view?usp=sharing
 *  
 *  U fajlu ima vrijednost Harun i harun
 *  Kad koristim ./test22 Harun test.txt onda će mi izbaciti samo vrijednosti koje su identične "Harun" znači veliko slovo samo. Isto tako za "harun" prikazao bi samo drugu liniju
 *  Kad koristim ./test -i Harun test.txt uključuje i malo i veliko slovo
 *  Također moguće je tražiti u više fajlova:
 *  ./test Harun test.txt test2.txt
 *  ./test -i Harun test.txt test2.txt
 * 
 * 
 * */

#include "dbfunc.h" // uključije se header dbfunc.h a on u sebi sadrži dbfunc.c gdje se vrši implementacija

int main(int argc, char *argv[]) // argc i argv su parametri koji se prosljeđuju iz command line-a... argc je broj parametara, a argv su parametri
// npr za ./test Tarik test.txt ... argv[0] je ./test argv[1] je Tarik i argv[2] je test.txt
{
  long result; // varijabla za spremanje rezultata od funkcija ... ovdje možeš provjeriti if(!result) success else fail .. to nije odrađeno
  long ignoreCase = 0;

  if (argc == 2) // ako iz command linea dolazi samo tražena riječ i jedan file path.. npr ./test Tarik test.txt
  {
    result = searchFile(argv[2], argv[1], ignoreCase); // za ./test Tarik test.txt proslijedi searchFile("test.txt", "Tarik", 0);
  }
  else if (argc >= 3) // ako iz command linea dolazi više traženih parametara npr ./test Tarik test.txt test2.txt
  {
    long i = 1;
    long searchTermIndex = 1;

    if (strcmp(argv[1], "-i") == 0) // u zadatku se traži ako se proslijedi parametar -i da se ignoreCase stavi na 1... znači da se u pretragu uključe i mala i velika slova
    {
      ignoreCase = 1;
      ++i;
      ++searchTermIndex;
    }

    for (i; i < argc; i++)
    {
      result = searchFile(argv[i], argv[searchTermIndex], ignoreCase);
    }
  }
  else
  {

    const int dbMemory = 10; // Veličina baze (struct niza) ...

    Data_t stDataT[dbMemory];     // inicijalizacija baze... zvat će se stDataT
    initItems(stDataT, dbMemory); // inicijalizacija vrijednosti
    long selection;
    do
    {
      // meni koji korisnik bira na osnovu unosa broja
      printf("Choose option: \n");
      printf("1.    Print all\n");
      printf("2.    Give a new input\n");
      printf("3.    Delete input\n");
      printf("4.    SaveDB\n");
      printf("5.    LoadDB\n");
      printf("6.    Search in file\n");
      printf("7.    Sort data\n");
      printf("8.    Add data from file\n");
      printf("9.    End program\n");

      selection = readValue("Selection");

      long item;

      // u zavisnosti koji je broj unešen odradi što se traži
      switch (selection)
      {
      case 1:
        printDB(stDataT, dbMemory);
        break;
      case 2:
        // u svakoj funkciji vraća se 0 ako je uspješno odrađena.. -1 ako ima greška, pa je tako i postavljen if else za svaki case
        item = addItem(stDataT, dbMemory);
        if (!item)
        {
          printf("\nAdded item successfully\n");
        }
        else
        {
          fprintf(stderr, "\nInsertion failed\n");
        }
        break;
      case 3:
        item = deleteItem(stDataT, dbMemory);
        if (!item)
        {
          printf("\nDeleted item successfully\n");
        }
        else
        {
          fprintf(stderr, "\nDeletion failed\n");
        }
        break;
      case 4:
        item = saveDB(stDataT, dbMemory, "test.txt");
        if (!item)
        {
          printf("\nSaved successfully\n");
        }
        else
        {
          fprintf(stderr, "\nSaving failed\n");
        }
        break;
      case 5:
        item = loadDB(stDataT, dbMemory, "test.txt");
        if (!item)
        {
          printf("\nLoaded successfully\n");
        }
        else
        {
          fprintf(stderr, "\nLoading DB failed\n");
        }
        break;
      case 6:
        item = searchFile("test.txt", "Harun", 0);
        if (!item)
        {
          printf("\nLoaded successfully\n");
        }
        else
        {
          fprintf(stderr, "\nLoading failed\n");
        }
        break;
      case 7:
        item = sortItems(stDataT, dbMemory, 2);
        if (!item)
        {
          fprintf(stderr, "\nSorted successfully\n");
        }
        else
        {
          fprintf(stderr, "\nSorting failed\n");
        }
        break;
      case 8:
        item = addFromFile(stDataT, dbMemory, "test.txt", "Harun");
        if (!item)
        {
          fprintf(stderr, "\nAdded from file successfully\n");
        }
        else
        {
          fprintf(stderr, "\nAdding from file failed\n");
        }
        break;
      default:
        break;
      }
    } while (selection != 9);
  }
}