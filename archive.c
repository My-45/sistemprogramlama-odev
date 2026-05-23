#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "archive.h"
#include "utils.h"

#define MAX_FILES 32
#define MAX_TOTAL_SIZE (200LL * 1024 * 1024)  // 200 MB

int build_archive(char *files[], int count, const char *output) {
    // Toplam boyut kontrolü
    long total_size = 0;
    for (int i = 0; i < count; i++) {
        // Metin dosyası kontrolü
        if (!is_text_file(files[i])) {
            return 1;
        }
        long size = get_file_size(files[i]);
        if (size < 0) {
            fprintf(stderr, "Hata: '%s' dosyasi okunamadi!\n", files[i]);
            return 1;
        }
        total_size += size;
        if (total_size > MAX_TOTAL_SIZE) {
            fprintf(stderr, "Hata: Toplam dosya boyutu 200 MB'i geciyor!\n");
            return 1;
        }
    }

    // İçerik tablosunu oluştur (önce bellekte)
    char index_buf[1024 * 1024];  // 1 MB tampon
    memset(index_buf, 0, sizeof(index_buf));
    int index_len = 0;

    for (int i = 0; i < count; i++) {
        long size = get_file_size(files[i]);
        mode_t perms = get_file_permissions(files[i]);

        char record[512];
        snprintf(record, sizeof(record), "|%s,%o,%ld|", files[i], perms, size);

        if (index_len + (int)strlen(record) >= (int)sizeof(index_buf)) {
            fprintf(stderr, "Hata: Icerik tablosu cok buyuk!\n");
            return 1;
        }
        strcat(index_buf, record);
        index_len += strlen(record);
    }

    // Arşiv dosyasını yaz
    FILE *out = fopen(output, "w");
    if (!out) {
        fprintf(stderr, "Hata: '%s' arsiv dosyasi olusturulamadi!\n", output);
        return 1;
    }

    // İlk 10 bayt: içerik bölümü boyutu (ASCII)
    fprintf(out, "%10d", index_len);

    // İçerik tablosunu yaz
    fprintf(out, "%s", index_buf);

    // Dosya içeriklerini art arda yaz
    for (int i = 0; i < count; i++) {
        FILE *in = fopen(files[i], "r");
        if (!in) {
            fprintf(stderr, "Hata: '%s' dosyasi acilamadi!\n", files[i]);
            fclose(out);
            return 1;
        }

        int c;
        while ((c = fgetc(in)) != EOF) {
            fputc(c, out);
        }
        fclose(in);
    }

    fclose(out);
    printf("Dosyalar birlestirildi.\n");
    return 0;
}