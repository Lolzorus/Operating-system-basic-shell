// Inclure les bibliothèques nécessaires

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>

void decouper(char*, char*, char**, int);

enum {
    MaxLigne = 1024,
    MaxMot = MaxLigne / 2,
    MaxDirs = 100,
    MaxPathLength = 512,
};

#define PROMPT "? "
#define MAX_COMMANDS 10

// Fonction pour exécuter des commandes avec des pipes et gérer les redirections
void execute_with_pipe_and_redirections(char *commands[], int num_commands, int background) {
    int pipefd[MAX_COMMANDS - 1][2];
    int input_fd = STDIN_FILENO;
    int output_fd = STDOUT_FILENO;

    // Créer les pipes
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipefd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Exécuter les commandes
    for (int i = 0; i < num_commands; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {  // Processus enfant
            // Gérer la redirection d'entrée si spécifiée
            if (input_fd != STDIN_FILENO) {
                if (dup2(input_fd, STDIN_FILENO) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }
            // Gérer la redirection de sortie si spécifiée
            if (i == num_commands - 1) {
                // Dernière commande : vérifier si une redirection de sortie est spécifiée
                if (output_fd != STDOUT_FILENO) {
                    if (dup2(output_fd, STDOUT_FILENO) == -1) {
                        perror("dup2");
                        exit(EXIT_FAILURE);
                    }
                }
            } else {
                // Pas la dernière commande : rediriger la sortie vers le pipe
                if (dup2(pipefd[i][1], STDOUT_FILENO) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }
            // Fermer tous les descripteurs de fichier du pipe
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipefd[j][0]);
                close(pipefd[j][1]);
            }
            // Exécuter la commande
            char *args[MaxMot];
            decouper(commands[i], " \t\n", args, MaxMot);
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else {  // Processus parent
            // Fermer les descripteurs de fichier appropriés
            if (input_fd != STDIN_FILENO) {
                close(input_fd);
            }
            if (i == num_commands - 1 && output_fd != STDOUT_FILENO) {
                close(output_fd);
            }
            // Mettre à jour les descripteurs de fichier pour la prochaine commande
            input_fd = pipefd[i][0];
        }
    }

    // Fermer tous les descripteurs de fichier du pipe dans le processus parent
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipefd[i][0]);
        close(pipefd[i][1]);
    }

    // Attendre que tous les processus enfants se terminent, sauf si la commande est en arrière-plan
    if (!background) {
        for (int i = 0; i < num_commands; i++) {
            wait(NULL);
        }
    }
}

// Fonction pour changer de répertoire
void moncd(char *dir) {
    // Utilise chdir pour changer de repertoire vers celui specifie par 'dir'
    if (chdir(dir) < 0) {
        perror(dir);
    } else {
        char cwd[MaxPathLength];
        // Recupere le repertoire courant apres le changement
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Répertoire changé vers %s\n", cwd);
        } else {
            perror("getcwd() error");
        }
    }
}

// Fonction pour afficher le répertoire courant
void pwd() {
    char cwd[MaxPathLength];
    // Recupere le repertoire courant
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Répertoire courant : %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

// Fonction pour créer un fichier
void create(char *filename) {
    // Ouvre un fichier avec les permissions specifiees, le cree s'il n'existe pas
    int fd = open(filename, O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("open() error");
    } else {
        printf("Fichier %s créé avec succès.\n", filename);
        close(fd);
    }
}

// Fonction pour exécuter les commandes intégrées
int execute_builtin_command(char *cmd, char *arg) {
    if (strcmp(cmd, "moncd") == 0) {
        if (arg == NULL) {
            char *dir = getenv("HOME");
            if (dir == NULL)
                dir = "/tmp";
            moncd(dir);
        } else {
            moncd(arg);
        }
        return 1;
    } else if (strcmp(cmd, "pwd") == 0) {
        pwd();
        return 1;
    } else if (strcmp(cmd, "create") == 0) {
        if (arg == NULL) {
            printf("Entrez le nom du fichier.\n");
        } else {
            create(arg);
        }
        return 1;
    } else if (strcmp(cmd, "monexit") == 0) {
        printf("Sortie du shell.\n");
        exit(EXIT_SUCCESS);
    }
    return 0;
}


void handle_background_processes(){
	pid_t pid;
	int status;
	//verifie le status des processus background
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0){
		if(WIFEXITED(status)) {
		printf("le processus en background numero %d est fini.\n", pid); }
		else if (WIFSIGNALED(status)) {
		printf("le processus de background %d a ete termine par le signal %d.? ", pid, WTERMSIG(status));  
		} 
	}
}

// fonction du custom man
void display_man(){
	// on appelle le man
	execlp("man", "man", "my_shell.1", NULL);
	}
		


int main(int argc, char *argv[]) {
    char ligne[MaxLigne];
    char *mot[MaxMot];
    char *dirs[MaxDirs];

    // Enregistre le gestionnaire de signal pour SIGCHLD (terminaison des processus enfants)
    signal(SIGCHLD, handle_background_processes);

    // Découper UNE COPIE de PATH en répertoires
    decouper(strdup(getenv("PATH")), ":", dirs, MaxDirs);

    // Lire et traiter chaque commande suivant qu'il y ait un pipe ou non (| comme séparateur)
    for (printf(PROMPT); fgets(ligne, sizeof ligne, stdin) != 0; printf(PROMPT)) {
    handle_background_processes();
        decouper(ligne, "|", mot, MaxMot); // identification du pipe dans la ligne

        int nombre_commande = 0; // on initialise à 0 le nombre de commande

        // On compte le nombre de commande. Tant qu'on détecte des commandes on incrémente le compteur
        while (mot[nombre_commande] != NULL) {
            nombre_commande++;
        }

        // Vérifier si la commande est en arrière-plan
        int background = 0;
        if (nombre_commande > 0) {
            if (strcmp(mot[nombre_commande - 1], "&") == 0) {
                background = 1;
                mot[nombre_commande - 1] = NULL;
            }
        }

        // Exécuter les commandes avec pipe si nécessaire
        if (nombre_commande > 1) {
            execute_with_pipe_and_redirections(mot, nombre_commande, background);
            continue;
        }
	
	if (strcmp(mot[0], "man\n") == 0) {
		display_man();
		continue;
	}
	
        // Exécuter la commande simple
	char *args[MaxMot];
	decouper(mot[0], " \t\n", args, MaxMot);

	// Vérifier si c'est une commande intégrée
	if (execute_builtin_command(args[0], args[1])) {
	    continue;
	}

	// Vérifier les redirections
	int input_redirection = 0, output_redirection = 0;
	char *input_file = NULL, *output_file = NULL;
	for (int i = 0; args[i] != NULL; i++) {
	    if (strcmp(args[i], "<") == 0) {
		input_redirection = 1;
		input_file = args[i + 1];
		args[i] = NULL;
	    } else if (strcmp(args[i], ">") == 0) {
		output_redirection = 1;
		output_file = args[i + 1];
		args[i] = NULL;
	    }
	}

	    // Vérifier si la commande est en arrière-plan
	    background = 0;
	    for (int i =0; args[i] != NULL; i++){
		if (strcmp(args[i], "&") == 0) {
		background = 1;
		args[i] = NULL;
		break;
		}
	    }

	    pid_t pid = fork();
	    if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	    } else if (pid == 0) {  // Processus enfant
		// Gérer la redirection d'entrée si spécifiée
		if (input_redirection) {
		    int fd = open(input_file, O_RDONLY);
		    if (fd == -1) {
		        perror("open");
		        exit(EXIT_FAILURE);
		    }
		    if (dup2(fd, STDIN_FILENO) == -1) {
		        perror("dup2");
		        exit(EXIT_FAILURE);
		    }
		    close(fd);
		}
		// Gérer la redirection de sortie si spécifiée
		if (output_redirection) {
		    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		    if (fd == -1) {
		        perror("open");
		        exit(EXIT_FAILURE);
		    }
		    if (dup2(fd, STDOUT_FILENO) == -1) {
		        perror("dup2");
		        exit(EXIT_FAILURE);
		    }
		    close(fd);
		}
		// Exécuter la commande
		execvp(args[0], args);
		perror("execvp");
		exit(EXIT_FAILURE);
	    } else {  // Processus parent
		if (!background) {
		    wait(NULL);;
		}
	    }

    }

    return 0;
}
