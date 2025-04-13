// server.c
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>

typedef struct {
    char nm[100];
    int marks;
    char sub[50];
} database;

char name[] = "admin";
char pswd[] = "admin";

database db[50];
int dbn = 0;
int fd, nfd, r;

int update(char n[], char s[], int m) {
    for (int i = 0; i < dbn; i++) {
        if (strcmp(db[i].nm, n) == 0 && strcmp(db[i].sub, s) == 0) {
            db[i].marks = m;
            return 1;
        }
    }
    return 0;
}

int deletedb(char n[]) {
    int i, j, flag = 0;
    for (i = 0; i < dbn; i++) {
        if (strcmp(db[i].nm, n) == 0) {
            flag = 1;
            for (j = i; j < dbn - 1; j++) {
                db[j] = db[j + 1];
            }
            dbn--;
            i--; // check this index again after shifting
        }
    }
    return flag;
}

void display() {
    char str[2000] = "Name\tSubject\tMarks\n";
    char a[50];

    for (int i = 0; i < dbn; i++) {
        strcat(str, db[i].nm);
        strcat(str, "\t");
        strcat(str, db[i].sub);
        strcat(str, "\t");
        sprintf(a, "%d", db[i].marks);
        strcat(str, a);
        strcat(str, "\n");
    }

    if (send(nfd, str, sizeof(str), 0) < 0) {
        perror("Send failed");
        close(nfd);
        close(fd);
        exit(1);
    }
}

int main() {
    int port;
    printf("Enter port number: ");
    scanf("%d", &port);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("Socket creation error");
        exit(1);
    }

    printf("Socket created\n");

    struct sockaddr_in cli, ser;
    ser.sin_family = AF_INET;
    ser.sin_port = htons(port);
    ser.sin_addr.s_addr = INADDR_ANY;

    if (bind(fd, (struct sockaddr*)&ser, sizeof(ser)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(fd, 5);
    printf("Listening on port %d...\n", port);

    socklen_t clisize = sizeof(cli);
    nfd = accept(fd, (struct sockaddr*)&cli, &clisize);
    if (nfd < 0) {
        perror("Accept failed");
        exit(1);
    }
    printf("Connection established\n");

    char n[100], p[100];
    int buff[10];
    char strbuff[100];

    recv(nfd, n, sizeof(n), 0);
    recv(nfd, p, sizeof(p), 0);

    if (strcmp(n, name) == 0 && strcmp(p, pswd) == 0) {
        buff[0] = 1;
        send(nfd, buff, sizeof(buff), 0);
        printf("Login successful\n");
    } else {
        buff[0] = 0;
        send(nfd, buff, sizeof(buff), 0);
        printf("Login failed\n");
        close(nfd);
        close(fd);
        exit(0);
    }

    int ch = 0;
    while (ch != 5) {
        recv(nfd, buff, sizeof(buff), 0);
        ch = buff[0];
        switch (ch) {
            case 1: { // insert
                recv(nfd, strbuff, sizeof(strbuff), 0);
                strcpy(db[dbn].nm, strbuff);
                recv(nfd, strbuff, sizeof(strbuff), 0);
                strcpy(db[dbn].sub, strbuff);
                recv(nfd, buff, sizeof(buff), 0);
                db[dbn].marks = buff[0];
                dbn++;
                strcpy(strbuff, "Success");
                send(nfd, strbuff, sizeof(strbuff), 0);
                break;
            }
            case 2: { // update
                char nm[100], su[100];
                recv(nfd, nm, sizeof(nm), 0);
                recv(nfd, su, sizeof(su), 0);
                recv(nfd, buff, sizeof(buff), 0);
                if (update(nm, su, buff[0]))
                    strcpy(strbuff, "Success");
                else
                    strcpy(strbuff, "Not Successful");
                send(nfd, strbuff, sizeof(strbuff), 0);
                break;
            }
            case 3: { // delete
                recv(nfd, strbuff, sizeof(strbuff), 0);
                if (deletedb(strbuff))
                    strcpy(strbuff, "Success");
                else
                    strcpy(strbuff, "Not Successful");
                send(nfd, strbuff, sizeof(strbuff), 0);
                break;
            }
            case 4:
                display();
                break;
            case 5:
                printf("Exiting server...\n");
                break;
            default:
                printf("Invalid option received\n");
        }
    }

    close(nfd);
    close(fd);
    return 0;
}
