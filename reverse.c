#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Määritellään puskuri, joka on kooltaan 1024 tavua
#define BUFFER_SIZE 1024

// Rakenne solmulle linkitettyyn listaan
typedef struct Node {
    char *line;
    struct Node *next;
} Node;

// Funktio, joka lisää uuden rivin linkitetyn listan alkuun
Node *add_line(Node *head, const char *line) {
    // Varataan muistia
    Node *new_node = (Node *)malloc(sizeof(Node));

    if (!new_node) {
        // Tulostetaan virheviesti, jos muistinvaraus epäonnistuu
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    // Selvitetään annetun rivin pituus
    size_t len = strlen(line);

    // Tarkistetaan, päättyykö rivi rivinvaihtomerkkiin
    if (len == 0 || line[len - 1] != '\n') {
        // Jos rivi ei pääty rivinvaihtoon, lisätään sellainen
        char *fixed_line = malloc(len + 2); // Varataan muistia alkuperäiselle riville + rivinvaihdolle + nollamerkille
        if (!fixed_line) {
            fprintf(stderr, "malloc failed\n"); // Virheilmoitus muistinvarauksen epäonnistuessa
            exit(1);
        }
        strcpy(fixed_line, line);
        fixed_line[len] = '\n';  // Lisätään rivinvaihtomerkki loppuun
        fixed_line[len + 1] = '\0';  // Lisätään nollamerkki merkkijonon lopetukseksi
        new_node->line = fixed_line; // Asetetaan solmun line-osoitin uuteen merkkijonoon
    } else {
        // Jos rivi päättyy jo rivinvaihtoon, kopioidaan se sellaisenaan
        new_node->line = strdup(line);
    }

    if (!new_node->line) {
        fprintf(stderr, "malloc failed\n"); // Virheilmoitus muistinvarauksen epäonnistuessa
        exit(1);
    }
 
    new_node->next = head; // Uusi solmu lisätään listan alkuun
    return new_node; // Palautetaan uusi listan pääsolmu
}

// Funktio vapauttaa linkitetyn listan muistin
void free_list(Node *head) {
    Node *temp;
    while (head) {
        temp = head; // Tallennetaan nykyinen solmu tilapäismuuttujaan
        head = head->next; // Siirrytään seuraavaan solmuun
        free(temp->line); // Vapautetaan rivin muistialue
        free(temp); // Vapautetaan solmun muistialue
    }
}
 
int main(int argc, char *argv[]) {
    // Tarkistetaan, että ohjelman käyttö on oikein (enintään 2 argumenttia)
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    FILE *input = stdin;  // Oletuksena luetaan standard inputista
    FILE *output = stdout; // Oletuksena tulostetaan standard outputiin

    // Jos käyttäjä on antanut syötetiedoston nimen, avataan tiedosto lukemista varten
    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    // Jos käyttäjä on antanut tulostetiedoston nimen, avataan se kirjoittamista varten
    if (argc == 3) {
        // Tarkistetaan, että syöte- ja tulostetiedostot ovat eri tiedostot
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "error: input and output file must differ\n");
            exit(1);
        }

        // Avataan tiedosto kirjoittamista varten
        output = fopen(argv[2], "w");
        if (!output) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
    }

    Node *head = NULL;  // Alustetaan listan pää osoittamaan tyhjään
    char *line = NULL;  // Osoitin getline-funktion käyttämään muistiin
    size_t len = 0;  // Pituus muuttuja getline-funktiolle

    // Luetaan rivit ja tallennetaan ne linkitettyyn listaan
    while (getline(&line, &len, input) != -1) {
        head = add_line(head, line);  // Lisätään uusi rivi listan alkuun
    }
    free(line);  // Vapautetaan getline-funktion varaama muisti

    // Tarkistetaan, oliko syöte tyhjä
    if (head == NULL) {
        fprintf(stderr, "error: input is empty\n");
        exit(1);
    }

    // Käydään linkitetty lista läpi ja tulostetaan rivit käänteisessä järjestyksessä
    Node *current = head;
    while (current) {
        fprintf(output, "%s", current->line);  // Tulostetaan nykyinen rivi
        current = current->next;  // Siirrytään seuraavaan solmuun
    }

    // Vapautetaan muistialueet ja suljetaan tiedostot
    free_list(head);  // Vapautetaan kaikki linkitetyn listan solmut
    if (input != stdin) fclose(input);  // Suljetaan tiedosto, jos se avattiin
    if (output != stdout) fclose(output);  // Suljetaan tiedosto, jos se avattiin

    return 0;
}
