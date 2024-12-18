#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define SERVER_PORT 4567
int nb_players = 0;

void* pthreadplayer (void* arg) {

    int client_fd = (long int) arg;

    /* reception du nom du player et envoi du message de bienvenue */
    char player[BUFSIZ]; memset(player, 0, BUFSIZ);
    int reponse_length = recv(client_fd, player, sizeof(player), 0); perror("recv name player ");
        if (reponse_length <= 0) { close(client_fd); perror ("close ");
        printf("Joueur %s s'est déconnecté :)\n", player);
        nb_players--;
        return NULL; }
     printf("Joueur %s s'est connecté :)\n", player);

    char welcome[BUFSIZ*2]; memset(welcome, 0 , BUFSIZ*2);
    sprintf(welcome, "Dresseur %s, Bienvenue dans l'arène Pokemon!\n", player);
    int error = send(client_fd, welcome, strlen(welcome) -1, 0); perror("send welcome ");
    if (error == -1) { close(client_fd); return NULL; }

    return NULL;
}

int main () {

    /* Initialisation du socket (socket, bind, listen) */
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); perror("socket ");
        if (server_fd == -1) return EXIT_FAILURE;

    struct sockaddr_in server_addr = {
        .sin_addr.s_addr = INADDR_ANY,
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT)
    };

    // Reutilise le meme port en empechant le bind : already in use
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockpopt(SO_REUSEADDR) failed ");

    #ifdef SO_REUSEPORT
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0)
           perror("setsockopt(SO_REUSEPORT) failed ");
    #endif

    int error = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)); perror("bind ");
        if (error == -1) return EXIT_FAILURE;

    error = listen(server_fd, 0); perror("listen ");
        if (error == - 1) return EXIT_FAILURE;
    printf("Server listen in port: %d\n", SERVER_PORT);


    /* On lance un pthread pour une connexion simultanée */  
    pthread_t thread;
    struct sockaddr_in client_addr;
    socklen_t len;

    while(1) {
        printf("Wait for a player to connect...\n"); 
        long int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len); perror("accept ");
            if (client_fd == -1) return EXIT_FAILURE;
        nb_players++;
        printf("Joueurs connectés : %d\n", nb_players);
        pthread_create(&thread, NULL, pthreadplayer, (void*)client_fd); perror("pthread_create ");
        printf("Thread starts...\n");
        nb_players = 0;
    }

}   