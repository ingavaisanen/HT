#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Yksinkertainen versio UNIX-komennosta 'grep'
// Ohjelma etsii käyttäjän antaman termin tiedostosta ja tulostaa rivin, jolta se löytyy

void grep_file(const char *search_term, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        // Jos tiedoston avaaminen epäonnistuu, tulostaa virheviestin ja lopettaa ohjelman
        printf("my-grep: cannot open file\n");
        exit(1);
    }
    
    char *line = NULL;
    size_t len = 0;

    // Lue tiedostoa rivi kerrallaan
    while (getline(&line, &len, fp) != -1) {
        // Jos hakusana löytyy riviltä, tulosta se
        if (strstr(line, search_term) != NULL) {
            printf("%s", line);
        }
    }
    
    // Vapauta muistialue ja sulje tiedosto
    free(line);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    // Tarkista, onko argumentteja vähintään 2 (hakusana ja mahdollinen tiedostonimi)
    if (argc < 2) {
        printf("my-grep: searchterm [file ...]\n");
        exit(1);
    }
    
    // Hae hakusana argumenteista
    const char *search_term = argv[1];
    
    // Jos argumentteja on vain 2, luetaan syöte standardisyötteestä
    if (argc == 2) {
        char *line = NULL;
        size_t len = 0;
        // Lue rivi kerrallaan standardisyötteestä
        while (getline(&line, &len, stdin) != -1) {
            // Jos hakusana löytyy riviltä, tulosta se
            if (strstr(line, search_term) != NULL) {
                printf("%s", line);
            }
        }
        // Vapauta muistialue
        free(line);
    } else {
        // Jos tiedostonimiä on useampi, käsitellään ne
        for (int i = 2; i < argc; i++) {
            // Etsitään hakusanaa tiedostoista
            grep_file(search_term, argv[i]);
        }
    }
    
    return 0;
}
