#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
 
#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100
 
// Polkujen taulukko, jossa oletuksena vain "/bin"
char *path[MAX_ARGS] = {"/bin", NULL};
 
void print_error() {
    char error_message[30] = "An error has occurred\n"; // Virheviestin tulostus stderr:iin
    write(STDERR_FILENO, error_message, strlen(error_message));
}
 
// Suorittaa annetun komennon
void execute_command(char **args) {
    if (args[0] == NULL) return;
 
    // Käsitellään sisäänrakennetut komennot: exit, cd ja path
    if (strcmp(args[0], "exit") == 0) {
        if (args[1] != NULL) {
            print_error();
        } else {
            exit(0);
        }
        return;
    } else if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL || args[2] != NULL || chdir(args[1]) != 0) {
            print_error();
        }
        return;
    } else if (strcmp(args[0], "path") == 0) {
        int i = 0;
        // Jos komennon jälkeen ei polkuja, tyhjennetään taulukko
        if (args[1] == NULL) {
            path[0] = NULL;
        } else {
            while (args[i + 1] != NULL && i < MAX_ARGS - 1) {
                path[i] = args[i + 1];  // Asetetaan polut
                i++;
            }
            path[i] = NULL;  // NULL loppuun
        }
        return;
    }
 
    // Child-prosessi komentojen suorittamista varten
    pid_t pid = fork();
    if (pid == 0) {
        char exec_path[MAX_INPUT_SIZE];
        for (int i = 0; path[i] != NULL; i++) {
            snprintf(exec_path, sizeof(exec_path), "%s/%s", path[i], args[0]);
            if (access(exec_path, X_OK) == 0) {
                execv(exec_path, args);  // Suoritetaan komento
            }
        }
        print_error();  // Error, jos komentoa ei löydy
        exit(1);
    } else if (pid > 0) {  // Parent-prosessi odottaa child-prosessin valmistumista
        wait(NULL);
    } else {
        print_error();
    }
}
 
// Parsii ja suorittaa käyttäjän syötteen
void parse_input(char *input) {
    char *args[MAX_ARGS];
    char *redirect_file = NULL;
    int i = 0, background = 0;
 
    // Pilkotaan syöte osiin
    char *token = strtok(input, " \t\n");
    while (token != NULL) {
        if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t\n");
            if (token == NULL || strtok(NULL, " \t\n") != NULL) {
                print_error();
                return;
            }
            redirect_file = token;
        } else if (strcmp(token, "&") == 0) {
            background = 1;
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
 
    if (i > 0) {
        int saved_stdout = dup(STDOUT_FILENO);
        if (redirect_file) {
            int fd = open(redirect_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd < 0) {
                print_error();
                return;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        
        if (background) { // Taustalla suoritus
            if (fork() == 0) {
                execute_command(args);
                exit(0);
            }
        } else {
            execute_command(args); // Normaali suoritus
        }
 
        if (redirect_file) {
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
        }
    }
}
 
// Käynnistää shellin ja käsittelee joko käyttäjän syötteen tai batch-tiedoston
void run_shell(FILE *input_stream) {
    char input[MAX_INPUT_SIZE];
    while (1) {
        if (input_stream == stdin) {
            printf("wish> ");
        }
        if (!fgets(input, MAX_INPUT_SIZE, input_stream)) {
            break;
        }
        parse_input(input);
    }
    exit(0);
}
 
int main(int argc, char *argv[]) { // Pääohjelma
    if (argc > 2) {
        print_error();
        exit(1);
    }
    FILE *input_stream = (argc == 2) ? fopen(argv[1], "r") : stdin;
    if (input_stream == NULL) {
        print_error();
        exit(1);
    }
    run_shell(input_stream);
    return 0;
}