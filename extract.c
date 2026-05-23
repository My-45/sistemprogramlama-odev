#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "extract.h"
#include "utils.h"

#define MAX_FILES 32

typedef struct {
    char name[256];
    mode_t perms;
    long size;
} FileEntry;

int extract_archive(const char *archive, const char *dir) {
    FILE *f = fopen(archive, "r");
    if (!f) {
        fprintf(stderr, "Arsiv dosyasi uygunsuz veya bozuk!\n");
        return 1;
    }

    // İlk 10 bayt: içerik bölümü boyutu
    char size_buf[11];
    if (fread(size_buf, 1, 10, f) != 10) {
        fprintf(stderr, "Arsiv dosyasi uygunsuz veya bozuk!\n");
        fclose(f);
        return 1;
    }
    size_buf[10] = '\0';
    int index_size = atoi(size_buf);

    if (index_size <= 0) {
        fprintf(stderr, "Arsiv dosyasi uygunsuz veya bozuk!\n");
        fclose(f);
        return 1;
    }

    // İçerik tablosunu oku
    char *index_buf = malloc(index_size + 1);
    if (!index_buf) {
        fprintf(stderr, "Hata: Bellek ayrilamadi!\n");
        fclose(f);
        return 1;
    }

    if (fread(index_buf, 1, index_size, f) != (size_t)index_size) {
        fprintf(stderr, "Arsiv dosyasi uygunsuz veya bozuk!\n");
        free(index_buf);
        fclose(f);
        return 1;
    }
    index_buf[index_size] = '\0';

    // İçerik tablosunu parse et
    FileEntry entries[MAX_FILES];
    int entry_count = 0;

    char *token = strtok(index_buf, "|");
    while (token != NULL && entry_count < MAX_FILES) {
        if (strlen(token) > 0) {
            char name[256];
            unsigned int perms;
            long size;
            if (sscanf(token, "%255[^,],%o,%ld", name, &perms, &size) == 3) {
                strcpy(entries[entry_count].name, name);
                entries[entry_count].perms = (mode_t)perms;
                entries[entry_count].size = size;
                entry_count++;
            }
        }
        token = strtok(NULL, "|");
    }
    free(index_buf);

    // Hedef dizini oluştur
    if (strcmp(dir, ".") != 0) {
        if (create_directory(dir) != 0) {
            fclose(f);
            return 1;
        }
    }

    // Dosyaları çıkar
    for (int i = 0; i < entry_count; i++) {
        char filepath[512];
        snprintf(filepath, sizeof(filepath), "%s/%s", dir, entries[i].name);

        FILE *out = fopen(filepath, "w");
        if (!out) {
            fprintf(stderr, "Hata: '%s' dosyasi olusturulamadi!\n", filepath);
            fclose(f);
            return 1;
        }

        // Dosya içeriğini oku ve yaz
        long remaining = entries[i].size;
        int c;
        while (remaining > 0 && (c = fgetc(f)) != EOF) {
            fputc(c, out);
            remaining--;
        }
        fclose(out);

        // İzinleri geri yükle
        chmod(filepath, entries[i].perms);
    }

    fclose(f);

    printf("%s dizininde", dir);
    for (int i = 0; i < entry_count; i++) {
        if (i == entry_count - 1)
            printf(" ve %s", entries[i].name);
        else
            printf(" %s,", entries[i].name);
    }
    printf(" dosyalari acildi.\n");

    return 0;
}