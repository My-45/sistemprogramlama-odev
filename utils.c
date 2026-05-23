#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include "utils.h"

int is_text_file(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "%s giris dosyasinin formati uyumsuzdur!\n", filename);
        return 0;
    }

    int c;
    while ((c = fgetc(f)) != EOF) {
        // ASCII kontrolü: 0-8, 11, 12, 14-31 arası kontrol karakterleri metin değil
        if ((c < 9 || (c > 13 && c < 32)) && c != 0) {
            fclose(f);
            fprintf(stderr, "%s giris dosyasinin formati uyumsuzdur!\n", filename);
            return 0;
        }
    }

    fclose(f);
    return 1;
}

long get_file_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        return -1;
    }
    return (long)st.st_size;
}

mode_t get_file_permissions(const char *filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        return 0;
    }
    return st.st_mode & 0777;
}

int create_directory(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        // Zaten var
        return 0;
    }
    if (mkdir(path, 0755) != 0) {
        fprintf(stderr, "Hata: '%s' dizini olusturulamadi!\n", path);
        return -1;
    }
    return 0;
}