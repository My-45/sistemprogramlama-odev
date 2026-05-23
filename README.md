# tarsau - Arşivleme Programı

Linux ortamında çalışan, sıkıştırma yapmayan bir arşivleme programı.

## Derleme

make

## Kullanım

### Arşiv Oluşturma
./tarsau -b dosya1.txt dosya2.txt -o arsiv.sau

### Arşiv Açma
./tarsau -a arsiv.sau hedef_dizin

## Dosya Formatı
.sau arşiv dosyası iki bölümden oluşur:
- İlk 10 bayt: içerik tablosunun boyutu
- İçerik tablosu: |dosyaadi,izinler,boyut| formatında
- Dosya içerikleri: art arda ASCII formatında

## Gereksinimler
- Linux / WSL
- gcc
- make
