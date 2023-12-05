#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_USERS 100
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20

#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 50

// �Τᵲ�c
struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

// �ǥ͵��c
struct Student {
    char name[MAX_NAME_LENGTH];
    int score;
};

// �װŦr�Ŧ�]�h���e��ťզr�š^
void trimString(char* str) {
    int len = strlen(str);
    int start, end;

    // ���Ĥ@�ӫD�ťզr��
    for (start = 0; start < len && isspace((unsigned char)str[start]); start++) {
        // do nothing
    }

    // ���̫�@�ӫD�ťզr��
    for (end = len - 1; end >= 0 && isspace((unsigned char)str[end]); end--) {
        // do nothing
    }

    // �ƻs�װū᪺�r�Ŧ�
    int i, j = 0;
    for (i = start; i <= end; i++) {
        str[j++] = str[i];
    }

    // �b�s�������K�[ null �r��
    str[j] = '\0';
}

// ���J�Τ�H��
int loadUsers(struct User users[]) {
    FILE* file = fopen("users.txt", "r");
    if (file == NULL) {
        return 0; // ��󥴶}����
    }

    int count = 0;
    while (fscanf(file, "%s %s", users[count].username, users[count].password) == 2) {
        count++;
    }

    fclose(file);
    return count;
}

// �g�J�Τ�H��
void saveUsers(struct User users[], int count) {
    FILE* file = fopen("users.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }
    fclose(file);
}

// �n��
int loginUser(struct User users[], int count, const char* username, const char* password) {
    // �װťΤ��J���r�Ŧ�
    char trimmedUsername[MAX_USERNAME_LENGTH];
    char trimmedPassword[MAX_PASSWORD_LENGTH];
    strncpy(trimmedUsername, username, MAX_USERNAME_LENGTH);
    strncpy(trimmedPassword, password, MAX_PASSWORD_LENGTH);
    trimString(trimmedUsername);
    trimString(trimmedPassword);

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, trimmedUsername) == 0 && strcmp(users[i].password, trimmedPassword) == 0) {
            return 1; // �n�����\
        }
    }
    return 0; // �n������
}

// ���U�Τ�
void registerUser(struct User users[], int* count) {
    if (*count >= MAX_USERS) {
        printf("�L�k�K�[��h�Τ�C\n");
        return;
    }

    char newUsername[MAX_USERNAME_LENGTH];
    char newPassword[MAX_PASSWORD_LENGTH];

    printf("�п�J�s�Τ�W�١G");
    scanf("%s", newUsername);

    // �ˬd�s�Τ�W�O�_�w�s�b
    for (int i = 0; i < *count; i++) {
        if (strcmp(users[i].username, newUsername) == 0) {
            printf("�Τ�W�w�s�b�A�п�ܨ�L�Τ�W�C\n");
            return;
        }
    }

    printf("�п�J�s�K�X�G");
    scanf("%s", newPassword);

    // �N�s�Τ�H���K�[��Ʋ�
    strncpy(users[*count].username, newUsername, MAX_USERNAME_LENGTH);
    strncpy(users[*count].password, newPassword, MAX_PASSWORD_LENGTH);
    (*count)++;

    // �O�s�Τ�H������
    saveUsers(users, *count);

    printf("���U���\�I�w��A%s�C\n", newUsername);
}

// ���J�ǥͫH��
int loadStudents(struct Student students[]) {
    FILE* file = fopen("students.txt", "r");
    if (file == NULL) {
        return 0; // ��󥴶}����
    }

    int count = 0;
    while (fscanf(file, "%s %d", students[count].name, &students[count].score) == 2) {
        count++;
    }

    fclose(file);
    return count;
}

// �g�J�ǥͫH��
void saveStudents(struct Student students[], int count) {
    FILE* file = fopen("students.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d\n", students[i].name, students[i].score);
    }
    fclose(file);
}

// �K�[�s�ǥ�
void addStudent(struct Student students[], int* count) {
    if (*count >= MAX_STUDENTS) {
        printf("�L�k�K�[��h�ǥ͡C\n");
        return;
    }

    printf("�п�J�s�ǥͪ��W�١G");
    scanf("%s", students[*count].name);

    // �ˬd��J�O�_���Ʀr
    while (1) {
        printf("�п�J���Z�G");
        if (scanf("%d", &students[*count].score) == 1) {
            break;
        }
        else {
            printf("��J�L�ġA�п�J���Ī��Ʀr�G");
            // �M����J�w�İ�
            while (getchar() != '\n') {
                // do nothing
            }
        }
    }

    (*count)++;
    saveStudents(students, *count);
    printf("�K�[�ǥͦ��\�I\n");

    // ��ܧ�s���e��
    /*printf("\n��s�᪺�ǥͦ��Z�G\n");
    for (int i = 0; i < *count; i++) {
        printf("%s�A���Z�G%d\n", students[i].name, students[i].score);
    }*/
}

// �d�߾ǥͦ��Z
void queryStudent(struct Student students[], int count, const char* name) {
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].name, name) == 0) {
            printf("�ǥ͡G%s�A���Z�G%d\n", students[i].name, students[i].score);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("�䤣��W��%s���ǥ͡C\n", name);
    }
}

// �ƦW�ǥͦ��Z
void rankStudents(struct Student students[], int count) {
    // ��ǥͫ����Z�i��ƦW
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (students[j].score < students[j + 1].score) {
                // �洫��Ӿǥͪ���m
                struct Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    // ��ܱƦW
    printf("�ǥͦ��Z�ƦW�G\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s�A���Z�G%d\n", i + 1, students[i].name, students[i].score);
    }

    // ��ܧ�s���e��
    /*printf("\n��s�᪺�ǥͦ��Z�ƦW�G\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s�A���Z�G%d\n", i + 1, students[i].name, students[i].score);
    }*/
}

// �M���Τ�H��
void clearUsers(struct User users[], int* count) {
    *count = 0;
    printf("�w�M���Ҧ��Τ�H���C\n");
}

// �M���ǥͫH��
void clearStudents(struct Student students[], int* count) {
    *count = 0;
    printf("�w�M���Ҧ��ǥͫH���C\n");
}

int main() {
    struct User users[MAX_USERS];
    int userCount = loadUsers(users);

    struct Student students[MAX_STUDENTS];
    int studentCount = loadStudents(students);

    printf("�w��Ө즨�Z�n���t�ΡI\n");

    while (1) {
        printf("�п�ܾާ@�G\n");
        printf("1. �Τ�n��\n");
        printf("2. �Τ���U\n");
        printf("3. �K�[�s�ǥ�\n");
        printf("4. �d�߾ǥͦ��Z\n");
        printf("5. ��ܾǥͦ��Z�ƦW\n");
        printf("6. �M���Τ�H��\n");
        printf("7. �M���ǥͫH��\n");
        printf("8. �h�X\n");

        int choice;

        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 8 ) {
            // �M����J�w�İ�
            while (getchar() != '\n') {
                // do nothing
            }
            system("cls");
            printf("��J�L�ġA�Э��s��J�C\n");
            continue;
        }
        /*if (scanf("%d", &choice) == 228)
        {
            while (getchar() != '\n') {
                // do nothing
            }
            printf("�A�n!!�������j�ӭ�\n");
            choice = 8;
        }*/
        switch (choice) {
        case 1: {
            char username[MAX_USERNAME_LENGTH];
            char password[MAX_PASSWORD_LENGTH];

            printf("�п�J�Τ�W�١G");
            scanf("%s", username);

            printf("�п�J�K�X�G");
            scanf("%s", password);

            if (loginUser(users, userCount, username, password)) {
                printf("�n�����\�I�w��A%s�C\n", username);
            }
            else {
                printf("�n�����ѡA�Τ�W�٩αK�X���~�C\n");
            }
            break;
        }
        case 2:
            registerUser(users, &userCount);
            break;
        case 3:
            addStudent(students, &studentCount);
            break;
        case 4: {
            char name[MAX_NAME_LENGTH];
            printf("�п�J�ǥͦW�١G");
            scanf("%s", name);
            queryStudent(students, studentCount, name);
            break;
        }
        case 5:
            rankStudents(students, studentCount);
            break;
        case 6:
            clearUsers(users, &userCount);
            break;
        case 7:
            clearStudents(students, &studentCount);
            break;
        case 8:
            saveUsers(users, userCount); // �b�h�X�e�O�s�Τ�H��
            saveStudents(students, studentCount); // �b�h�X�e�O�s�ǥͫH��
            printf("���¨ϥΡA�A���I\n");
            return 0;
        default:
            // �M����J�w�İ�
            while (getchar() != '\n') {
                // do nothing
            }
            system("cls");
            printf("��J�L�ġA�Э��s��J�C\n");
        }
        system("pause");
        system("cls");
    }

    return 0;
}

