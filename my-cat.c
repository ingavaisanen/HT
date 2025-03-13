#include <stdio.h>
#include <stdlib.h>

// Yksinkertainen versio UNIX-komennosta 'cat'
// Ohjelma lukee yhden tai useamman tiedoston sisällön ja tulostaa sen näytölle

int main(int argc, char *argv[]) {
    // Tarkistetaan, onko käyttäjä antanut tiedostoja komentorivillä
    // Jos ei ole, ohjelma lopetetaan onnistuneesti
    if (argc < 2) {
        return 0;
    }

    // Käydään läpi jokainen tiedostonimi, joka on annettu komentorivillä
    for (int i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if (!file) {
            // Jos fopen() epäonnistuu, tulostetaan virheilmoitus ja lopetetaan ohjelma
            printf("my-cat: cannot open file\n");
            return 1;
        }

        char buffer[4096];  
        while (fgets(buffer, sizeof(buffer), file)) {
            // Luetaan rivi tiedostosta ja tulostetaan se näytölle
            printf("%s", buffer);
        }

        fclose(file); // Suljetaan tiedosto, kun sen käsittely on valmis
    }

    return 0;
}
