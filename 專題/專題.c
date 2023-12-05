#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_USERS 100
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20

#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 50

// 用戶結構
struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

// 學生結構
struct Student {
    char name[MAX_NAME_LENGTH];
    int score;
};

// 修剪字符串（去除前後空白字符）
void trimString(char* str) {
    int len = strlen(str);
    int start, end;

    // 找到第一個非空白字符
    for (start = 0; start < len && isspace((unsigned char)str[start]); start++) {
        // do nothing
    }

    // 找到最後一個非空白字符
    for (end = len - 1; end >= 0 && isspace((unsigned char)str[end]); end--) {
        // do nothing
    }

    // 複製修剪後的字符串
    int i, j = 0;
    for (i = start; i <= end; i++) {
        str[j++] = str[i];
    }

    // 在新的結尾添加 null 字符
    str[j] = '\0';
}

// 載入用戶信息
int loadUsers(struct User users[]) {
    FILE* file = fopen("users.txt", "r");
    if (file == NULL) {
        return 0; // 文件打開失敗
    }

    int count = 0;
    while (fscanf(file, "%s %s", users[count].username, users[count].password) == 2) {
        count++;
    }

    fclose(file);
    return count;
}

// 寫入用戶信息
void saveUsers(struct User users[], int count) {
    FILE* file = fopen("users.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }
    fclose(file);
}

// 登錄
int loginUser(struct User users[], int count, const char* username, const char* password) {
    // 修剪用戶輸入的字符串
    char trimmedUsername[MAX_USERNAME_LENGTH];
    char trimmedPassword[MAX_PASSWORD_LENGTH];
    strncpy(trimmedUsername, username, MAX_USERNAME_LENGTH);
    strncpy(trimmedPassword, password, MAX_PASSWORD_LENGTH);
    trimString(trimmedUsername);
    trimString(trimmedPassword);

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, trimmedUsername) == 0 && strcmp(users[i].password, trimmedPassword) == 0) {
            return 1; // 登錄成功
        }
    }
    return 0; // 登錄失敗
}

// 註冊用戶
void registerUser(struct User users[], int* count) {
    if (*count >= MAX_USERS) {
        printf("無法添加更多用戶。\n");
        return;
    }

    char newUsername[MAX_USERNAME_LENGTH];
    char newPassword[MAX_PASSWORD_LENGTH];

    printf("請輸入新用戶名稱：");
    scanf("%s", newUsername);

    // 檢查新用戶名是否已存在
    for (int i = 0; i < *count; i++) {
        if (strcmp(users[i].username, newUsername) == 0) {
            printf("用戶名已存在，請選擇其他用戶名。\n");
            return;
        }
    }

    printf("請輸入新密碼：");
    scanf("%s", newPassword);

    // 將新用戶信息添加到數組
    strncpy(users[*count].username, newUsername, MAX_USERNAME_LENGTH);
    strncpy(users[*count].password, newPassword, MAX_PASSWORD_LENGTH);
    (*count)++;

    // 保存用戶信息到文件
    saveUsers(users, *count);

    printf("註冊成功！歡迎，%s。\n", newUsername);
}

// 載入學生信息
int loadStudents(struct Student students[]) {
    FILE* file = fopen("students.txt", "r");
    if (file == NULL) {
        return 0; // 文件打開失敗
    }

    int count = 0;
    while (fscanf(file, "%s %d", students[count].name, &students[count].score) == 2) {
        count++;
    }

    fclose(file);
    return count;
}

// 寫入學生信息
void saveStudents(struct Student students[], int count) {
    FILE* file = fopen("students.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d\n", students[i].name, students[i].score);
    }
    fclose(file);
}

// 添加新學生
void addStudent(struct Student students[], int* count) {
    if (*count >= MAX_STUDENTS) {
        printf("無法添加更多學生。\n");
        return;
    }

    printf("請輸入新學生的名稱：");
    scanf("%s", students[*count].name);

    // 檢查輸入是否為數字
    while (1) {
        printf("請輸入成績：");
        if (scanf("%d", &students[*count].score) == 1) {
            break;
        }
        else {
            printf("輸入無效，請輸入有效的數字：");
            // 清除輸入緩衝區
            while (getchar() != '\n') {
                // do nothing
            }
        }
    }

    (*count)++;
    saveStudents(students, *count);
    printf("添加學生成功！\n");

    // 顯示更新的畫面
    /*printf("\n更新後的學生成績：\n");
    for (int i = 0; i < *count; i++) {
        printf("%s，成績：%d\n", students[i].name, students[i].score);
    }*/
}

// 查詢學生成績
void queryStudent(struct Student students[], int count, const char* name) {
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].name, name) == 0) {
            printf("學生：%s，成績：%d\n", students[i].name, students[i].score);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("找不到名為%s的學生。\n", name);
    }
}

// 排名學生成績
void rankStudents(struct Student students[], int count) {
    // 對學生按成績進行排名
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (students[j].score < students[j + 1].score) {
                // 交換兩個學生的位置
                struct Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    // 顯示排名
    printf("學生成績排名：\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s，成績：%d\n", i + 1, students[i].name, students[i].score);
    }

    // 顯示更新的畫面
    /*printf("\n更新後的學生成績排名：\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s，成績：%d\n", i + 1, students[i].name, students[i].score);
    }*/
}

// 清除用戶信息
void clearUsers(struct User users[], int* count) {
    *count = 0;
    printf("已清除所有用戶信息。\n");
}

// 清除學生信息
void clearStudents(struct Student students[], int* count) {
    *count = 0;
    printf("已清除所有學生信息。\n");
}

int main() {
    struct User users[MAX_USERS];
    int userCount = loadUsers(users);

    struct Student students[MAX_STUDENTS];
    int studentCount = loadStudents(students);

    printf("歡迎來到成績登錄系統！\n");

    while (1) {
        printf("請選擇操作：\n");
        printf("1. 用戶登錄\n");
        printf("2. 用戶註冊\n");
        printf("3. 添加新學生\n");
        printf("4. 查詢學生成績\n");
        printf("5. 顯示學生成績排名\n");
        printf("6. 清除用戶信息\n");
        printf("7. 清除學生信息\n");
        printf("8. 退出\n");

        int choice;

        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 8 ) {
            // 清除輸入緩衝區
            while (getchar() != '\n') {
                // do nothing
            }
            system("cls");
            printf("輸入無效，請重新輸入。\n");
            continue;
        }
        /*if (scanf("%d", &choice) == 228)
        {
            while (getchar() != '\n') {
                // do nothing
            }
            printf("你好!!杜長勳大帥哥\n");
            choice = 8;
        }*/
        switch (choice) {
        case 1: {
            char username[MAX_USERNAME_LENGTH];
            char password[MAX_PASSWORD_LENGTH];

            printf("請輸入用戶名稱：");
            scanf("%s", username);

            printf("請輸入密碼：");
            scanf("%s", password);

            if (loginUser(users, userCount, username, password)) {
                printf("登錄成功！歡迎，%s。\n", username);
            }
            else {
                printf("登錄失敗，用戶名稱或密碼錯誤。\n");
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
            printf("請輸入學生名稱：");
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
            saveUsers(users, userCount); // 在退出前保存用戶信息
            saveStudents(students, studentCount); // 在退出前保存學生信息
            printf("謝謝使用，再見！\n");
            return 0;
        default:
            // 清除輸入緩衝區
            while (getchar() != '\n') {
                // do nothing
            }
            system("cls");
            printf("輸入無效，請重新輸入。\n");
        }
        system("pause");
        system("cls");
    }

    return 0;
}

