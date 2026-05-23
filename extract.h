#ifndef EXTRACT_H
#define EXTRACT_H

// Arşiv açma fonksiyonu
// archive: açılacak arşiv dosyası adı
// dir: dosyaların çıkarılacağı dizin
// Başarıda 0, hata durumunda 1 döndürür
int extract_archive(const char *archive, const char *dir);

#endif