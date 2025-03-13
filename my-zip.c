#include <stdio.h>
#include <stdlib.h>
 
// Funktio, joka suorittaa "zip"-toiminnon
void my_zip(int argc, char *argv[]) {
    if (argc < 2) { // Tarkistetaan, että vähintään yksi tiedosto annettu
        fprintf(stderr, "my-zip: file1 [file2 ...]\n"); // Tulostetaan virheviesti, jos tiedostoja ei ole annettu
        exit(1); // Ohjelman lopetus
    }
 
    FILE *fp; // Tiedostopointteri
    int count; // Laskuri merkkiesiintymille
    char current, previous;
    int first_char = 1;  // Seuraa ensimmäisen merkin esiintymiä
 
    // Käydään läpi kaikki annettu tiedostopolut (argv[1] ... argv[n])
    for (int i = 1; i < argc; i++) {
        fp = fopen(argv[i], "r"); // Yritetään avata tiedosto lukemista varten
        if (fp == NULL) { // Jos tiedostoa ei voida avata, tulostetaan virheviesti ja lopetetaan ohjelma
            fprintf(stderr, "my-zip: cannot open file %s\n", argv[i]);
            exit(1);
        }
 
        while ((current = fgetc(fp)) != EOF) { // Luetaan tiedostoa merkki kerrallaan
            if (first_char) {
                previous = current;  // Asetetaan ensimmäinen merkki edelliseksi
                count = 1;           // Lasketaan, että ensimmäinen merkki esiintyy kerran
                first_char = 0;      // Asetetaan lippu epätodeksi
                continue;            // Siirrytään seuraavaan merkkiin
            }

            // Jos nykyinen merkki on sama kuin edellinen, kasvatetaan laskuria
            if (current == previous) {
                count++;
            } else {
                fwrite(&count, sizeof(int), 1, stdout);  // Kirjoitetaan laskuri (4 tavua)
                fwrite(&previous, sizeof(char), 1, stdout);  // Kirjoitetaan merkki (1 tavu)

                // Nollataan laskuri ja edellinen merkki uuden merkin käsittelyä varten
                previous = current;
                count = 1;
            }
        }
        fclose(fp); // Suljetaan tiedosto käsittelyn jälkeen
    }

    if (!first_char) {
        fwrite(&count, sizeof(int), 1, stdout); // Kirjoitetaan viimeinen laskuri
        fwrite(&previous, sizeof(char), 1, stdout); // Kirjoitetaan viimeinen merkki
    }
}

// Pääohjelma
int main(int argc, char *argv[]) {
    my_zip(argc, argv);
    return 0;
}
