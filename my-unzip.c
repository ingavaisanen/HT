#include <stdio.h>
#include <stdlib.h>
 
// Funktio, joka suorittaa purkamistoiminnon
void my_unzip(int argc, char *argv[]) {
    if (argc < 2) { // Tarkistetaan, että vähintään yksi tiedosto annettu
        fprintf(stderr, "my-unzip: file1 [file2 ...]\n"); // Tulostetaan virheviesti, jos tiedostoja ei ole annettu
        exit(1); // Ohjelman lopetus
    }
 
    FILE *fp;           // Tiedostopointteri
    int count;          // Laskuri merkkiesiintymille
    char character;     // Merkki, joka toistetaan
 
    for (int i = 1; i < argc; i++) { // Käydään läpi kaikki annettu tiedostopolut (argv[1] ... argv[n])
        fp = fopen(argv[i], "rb"); // Yritetään avata tiedosto lukemista varten
        if (fp == NULL) { // Jos tiedostoa ei voida avata, tulostetaan virheviesti ja lopetetaan ohjelma
            fprintf(stderr, "my-unzip: cannot open file %s\n", argv[i]);
            exit(1);
        }
 
        // Luetaan tiedostoa: ensin laskuri (count) ja sitten merkki (character)
        // Tämä toistetaan niin kauan, kun tiedostosta löytyy vielä lukuun sopivia tietoja
        while (fread(&count, sizeof(int), 1, fp) == 1 && fread(&character, sizeof(char), 1, fp) == 1) {
            for (int j = 0; j < count; j++) {
                // Toistetaan merkki 'count' kertaa
                putchar(character); // Merkin tulostus
            }
        }
        fclose(fp); // Tiedoston sulkeminen käsittelyn jälkeen
    }
}
 
// Pääohjelma
int main(int argc, char *argv[]) {
    my_unzip(argc, argv);
    return 0;
}
