# MON_SHELL(1)		Custom man pour projet systeme d'exploitation		MON_SHELL(1)

NAME
       mon_shell - un petit shell contenant quelques commandes

SYNOPSIS
       mon_shell [options] [commands]

DESCRIPTION
       mon_shell est un mini-shell capable d'executer quelques commandes, deplacement dans les repertoires et creations de fichiers.

# COMMANDES PRINCIPALES
       -moncd [repertoire]
       		Change le repertoire courant.

       -pwd
       		Affiche le repertoire courant.
       		
       -create [fichier]
       		Creer un fichier dans le repertoire courant.
       
       -monexit
       		Sortie du shell.
       
       -sleep [temps] &
       		& pour lancer sleep en arriere plan.

# INFOS ADDITIONELLES
mon_shell gere les commandes shell standard (ls, pwd, touch etc). Il supporte aussi les redirections input/output, les pipes et les commandes en background.


# DOCUMENTATION DES COMMANDES
       moncd: Change le répertoire de travail courant.
       pwd: Affiche le répertoire de travail courant.
       create: Crée un fichier.
       monexit: Termine le shell.

# PROCÉDURES
       execute_with_pipe_and_redirections:
       Exécute des commandes avec des pipes
       et gère les redirections.
       
       moncd: Change de répertoire.
       
       pwd: Affiche le répertoire courant.
       
       create: Crée un fichier.
       
       execute_builtin_command: Exécute les commandes intégrées.

# REDIRECTIONS
       <: Redirige l'entrée standard d'une commande
       depuis un fichier.
       
       >: Redirige la sortie standard d'une commande
       vers un fichier.

# PIPES
       | : Permet d'exécuter des commandes avec des
       pipes pour diriger la sortie de l'une vers
       l'entrée de l'autre.


# MODIFICATIONS APPORTÉES AU SHELL INITIAL
       Ajout de la gestion des redirections
       pour les commandes simples.
       
       Ajout de la possibilité de spécifier des
       commandes en arrière-plan avec le symbole '&'.

# EXEMPLES D'UTILISATION
       Utilisation de moncd:
              $ moncd 1
              Répertoire changé vers /chemin/vers/le/dossier/1

       Utilisation de pwd:
              $ pwd
              Répertoire courant: /chemin/vers/le/dossier/1

       Création d'un fichier avec create:
              $ create fichier.txt
              Fichier fichier.txt créé avec succès.

       Utilisation de monexit pour quitter le shell:
              $ monexit
              Sortie du shell.

       Exemple d'utilisation de redirection de sortie:
              $ ls > fichiers.txt

       Exemple d'utilisation de redirection d'entrée:
              $ cat < fichier.txt

       Exemple d'utilisation de pipe:
              $ ls | grep .txt

## english

# MON_SHELL(1)
        Custom man page for Operating System project

# NAME
      mon_shell - A small shell with some commands

# SYNOPSIS
      mon_shell [options] [commands]

# DESCRIPTION
      mon_shell is a mini-shell capable of executing some commands, changing directories, and creating files.

# MAIN COMMANDS
      -moncd [directory]
      Change the current directory.

      -pwd
      Displays the current directory.
      
      -create [file]
      Creates a file in the current directory.
      
      -monexit
      Exits the shell.
      
      -sleep [time] &
      The & runs the sleep command in the background.

# ADDITIONAL INFORMATION
      mon_shell handles standard shell commands (such as ls, pwd, touch, etc.). It also supports input/output redirection, pipes, and background commands.

# COMMAND DOCUMENTATION
      moncd: Changes the working directory.
      pwd: Displays the current working directory.
      create: Creates a file.
      monexit: Terminates the shell.

# PROCEDURES
      execute_with_pipe_and_redirections:
      Executes commands with pipes and handles redirections.

      moncd: Changes the directory.
      pwd: Displays the current directory.
      create: Creates a file.
      execute_builtin_command: Executes built-in commands.

# REDIRECTIONS
      <: Redirects standard input from a file.
      
      >: Redirects standard output to a file.

# PIPES
      |: Executes commands with pipes to direct the output of one command into the input of another.
      
      CHANGES MADE TO THE INITIAL SHELL
      Added handling of redirections for simple commands.
      Added the ability to specify background commands using the & symbol.


# EXAMPLES OF USE
      Using moncd:
      $ moncd 1
      Directory changed to /path/to/directory/1
      
      Using pwd:
      $ pwd
      Current directory: /path/to/directory/1
      
      Creating a file with create:
      $ create fichier.txt
      File fichier.txt created successfully.
      
      Using monexit to quit the shell:
      $ monexit
      Exiting the shell.
      
      Example of output redirection:
      $ ls > files.txt
      
      Example of input redirection:
      $ cat < file.txt
      
      Example of using a pipe:
      $ ls | grep .txt
