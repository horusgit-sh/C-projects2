#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FILENAME "/var/note/note"

int print_note(int, int, char *);
int find_user_note(int, int);
int search_note(char *, char *);
void fatal(char *);

int main(int argc, char *argv[]) {
    int userid, fd, printing = 1;
    char searchstring[100] = "";

    if (argc > 1) {
        strncpy(searchstring, argv[1], sizeof(searchstring) - 1);
    }

    userid = getuid();
    fd = open(FILENAME, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    while (printing) {
        printing = print_note(fd, userid, searchstring);
    }

    printf("----[ end of note data ]----\n");
    close(fd);
    return 0;
}

int print_note(int fd, int uid, char *searchstring) {
    int note_length;
    char note_buffer[100];

    note_length = find_user_note(fd, uid);
    if (note_length == -1) {
        return 0;
    }

    if (note_length >= sizeof(note_buffer)) {
        fatal("Note too long");
    }

    if (read(fd, note_buffer, note_length) != note_length) {
        fatal("Error reading note");
    }
    note_buffer[note_length] = '\0';

    if (search_note(note_buffer, searchstring)) {
        printf("%s\n", note_buffer);
    }
    return 1;
}

int find_user_note(int fd, int user_uid) {
    char uid_str[5] = "";
    char byte;
    int length = 0;

    while (1) {
        if (read(fd, uid_str, 4) != 4) {
            return -1;
        }
        uid_str[4] = '\0';
        int note_uid = atoi(uid_str);

        if (read(fd, &byte, 1) != 1) {
            return -1;
        }

        if (note_uid == user_uid) {
            break;
        }

        while (byte != '\n') {
            if (read(fd, &byte, 1) != 1) {
                return -1;
            }
        }
    }

    off_t start_pos = lseek(fd, 0, SEEK_CUR);
    while (read(fd, &byte, 1) == 1 && byte != '\n') {
        length++;
    }
    lseek(fd, start_pos, SEEK_SET);

    printf("[DEBUG] found a %d byte note for user id %d\n", length, user_uid);
    return length;
}

int search_note(char *note, char *keyword) {
    if (strlen(keyword) == 0) {
        return 1;
    }
    return strstr(note, keyword) != NULL;
}

void fatal(char *message) {
    fprintf(stderr, "!! Fatal Error !! %s\n", message);
    exit(-1);
}
