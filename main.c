#include <stdio.h>
#include <string.h>
#include "archive.h"
#include "extract.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Kullanim: tarsau -b dosya1 dosya2 ... -o arsiv.sau\n");
        fprintf(stderr, "          tarsau -a arsiv.sau [dizin]\n");
        return 1;
    }

    if (strcmp(argv[1], "-b") == 0) {
        // Arşiv oluşturma modu
        char *output = "a.sau";  // varsayılan çıktı adı
        int file_count = 0;
        char *files[32];

        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-o") == 0) {
                if (i + 1 < argc) {
                    output = argv[i + 1];
                    i++;
                }
            } else {
                if (file_count >= 32) {
                    fprintf(stderr, "Hata: En fazla 32 dosya arsivlenebilir!\n");
                    return 1;
                }
                files[file_count++] = argv[i];
            }
        }

        if (file_count == 0) {
            fprintf(stderr, "Hata: Hic dosya belirtilmedi!\n");
            return 1;
        }

        return build_archive(files, file_count, output);

    } else if (strcmp(argv[1], "-a") == 0) {
        // Arşiv açma modu
        if (argc < 3) {
            fprintf(stderr, "Hata: Arsiv dosyasi belirtilmedi!\n");
            return 1;
        }
        char *archive = argv[2];
        char *dir = (argc >= 4) ? argv[3] : ".";

        return extract_archive(archive, dir);

    } else {
        fprintf(stderr, "Hata: Gecersiz parametre '%s'\n", argv[1]);
        return 1;
    }

    return 0;
}