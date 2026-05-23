#ifndef ARCHIVE_H
#define ARCHIVE_H

// Arşiv oluşturma fonksiyonu
// files: dosya adları dizisi
// count: dosya sayısı
// output: çıktı arşiv dosyası adı
// Başarıda 0, hata durumunda 1 döndürür
int build_archive(char *files[], int count, const char *output);

#endif