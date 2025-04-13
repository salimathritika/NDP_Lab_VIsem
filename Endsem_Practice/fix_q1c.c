// client.c
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int fd, r;
    struct sockaddr_in serv;
    int PORT;
    printf("Enter Port number: ");
    scanf("%d", &PORT);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("Socket creation error");
        exit(1);
    }

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    r = connect(fd, (struct sockaddr*)&serv, sizeof(serv));
    if (r < 0) {
        perror("Connection failed");
        exit(1);
    }

    char name[100], pswd[100], strbuff[100];
    int buff[10];

    printf("\nLOGIN\nEnter username: ");
    scanf("%s", name);
    send(fd, name, sizeof(name), 0);

    printf("Enter password: ");
    scanf("%s", pswd);
    send(fd, pswd, sizeof(pswd), 0);

    recv(fd, buff, sizeof(buff), 0);
    if (buff[0] != 1) {
        printf("Login failed\n");
        close(fd);
        exit(0);
    }
    printf("Login Successful\n");

    int ch = 0;
    printf("\n1.Insert\n2.Update\n3.Delete\n4.Display\n5.Exit\n");
    while (ch != 5) {
        printf("Enter choice: ");
        scanf("%d", &ch);
        buff[0] = ch;
        send(fd, buff, sizeof(buff), 0);

        switch (ch) {
            case 1: // insert
                printf("Enter name: ");
                scanf("%s", strbuff);
                send(fd, strbuff, sizeof(strbuff), 0);

                printf("Enter subject: ");
                scanf("%s", strbuff);
                send(fd, strbuff, sizeof(strbuff), 0);

                printf("Enter marks: ");
                scanf("%d", &buff[0]);
                send(fd, buff, sizeof(buff), 0);

                recv(fd, strbuff, sizeof(strbuff), 0);
                printf("%s\n", strbuff);
                break;

            case 2: // update
                printf("Enter name: ");
                scanf("%s", strbuff);
                send(fd, strbuff, sizeof(strbuff), 0);

                printf("Enter subject: ");
                scanf("%s", strbuff);
                send(fd, strbuff, sizeof(strbuff), 0);

                printf("Enter updated marks: ");
                scanf("%d", &buff[0]);
                send(fd, buff, sizeof(buff), 0);

                recv(fd, strbuff, sizeof(strbuff), 0);
                printf("%s\n", strbuff);
                break;

            case 3: // delete
                printf("Enter name to delete: ");
                scanf("%s", strbuff);
                send(fd, strbuff, sizeof(strbuff), 0);

                recv(fd, strbuff, sizeof(strbuff), 0);
                printf("%s\n", strbuff);
                break;

            case 4: // display
                recv(fd, strbuff, sizeof(strbuff), 0);
                printf("\nDatabase Contents:\n%s", strbuff);
                break;

            case 5:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice\n");
        }
    }

    close(fd);
    return 0;
}
