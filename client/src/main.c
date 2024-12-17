#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>

#define SERVER_PORT 4567
#define CLIENT_PORT 4643

int main (int argc, char** argv) {

    printf("                                  ,'\\\n");
    printf("    _.----.        ____         ,'  _\\   ___    ___     ____\n");
    printf("_,-'       `.     |    |  /`.   \\,-'    |   \\  /   |   |    \\  |`.\n");
    printf("\\      __    \\    '-.  | /   `.  ___    |    \\/    |   '-.   \\ |  |\n");
    printf(" \\.    \\ \\   |  __  |  |/    ,','_  `.  |          | __  |    \\|  |\n");
    printf("   \\    \\/   /,' _`.|      ,' / / / /   |          ,' _`.|     |  |\n");
    printf("    \\     ,-'/  /   \\    ,'   | \\/ / ,`.|         /  /   \\  |     |\n");
    printf("     \\    \\ |   \\_/  |   `-.  \\    `'  /|  |    ||   \\_/  | |\\    |\n");
    printf("      \\    \\ \\      /       `-.`.___,-' |  |\\  /| \\      /  | |   |\n");
    printf("       \\    \\ `.__,'|  |`-._    `|      |__| \\/ |  `.__,'|  | |   |\n");
    printf("        \\_.-'       |__|    `-._ |              '-.|     '-.| |   |\n");
    printf("                                `'                            '-._|\n");

    /* Connexion de l'utilisateur via le terminal */
    char player[BUFSIZ]; memset(player, 0, BUFSIZ);
    if (argc > 1) {
        strcpy(player, argv[1]);
        printf("Dresseur %s, Bienvenue dans l'arène Pokemon!\n", player);
    } else {
        printf("Quel est ton nom ?\n");
        fgets(player, BUFSIZ, stdin); player[strlen(player)-1] = 0;
        printf("Dresseur %s, Bienvenue dans l'arène Pokemon!\n", player);
    }

    /* Initialisation du socket, bind et connect */
    int client_fd = socket(AF_INET, SOCK_STREAM, 0); perror("socket ");
        if (client_fd == -1) return EXIT_FAILURE;

    struct sockaddr_in client_addr = {
        .sin_addr.s_addr = INADDR_ANY,
        .sin_family = AF_INET,
        .sin_port = htons(CLIENT_PORT)
    };

    int error = bind(client_fd, (struct sockaddr*)&client_addr, sizeof(client_addr)); perror("bind ");
        if (error == -1 ) return EXIT_FAILURE;

    struct sockaddr_in server_addr = {
        .sin_addr.s_addr = INADDR_ANY,
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT)
    }; 

    error = connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)); perror(" connect ");
        if (error == -1) return EXIT_FAILURE;

    /* Envoi du nom du player et reception du message de bienvenue */
    error = send(client_fd, player, strlen(player) -1, 0); perror("send ");
        if (error == -1) { close(client_fd); return EXIT_FAILURE; }

    char welcome[BUFSIZ]; memset(welcome, 0 , BUFSIZ);
    error = recv(client_fd, welcome, sizeof(welcome)-1, 0); perror("welcome ");
        if (error == -1) { close(client_fd); return EXIT_FAILURE; }
    printf("%s\n", welcome);

return 0;
}