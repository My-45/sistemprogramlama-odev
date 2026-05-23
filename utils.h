#ifndef UTILS_H
#define UTILS_H

#include <sys/stat.h>

// Dosyanın metin dosyası olup olmadığını kontrol eder
int is_text_file(const char *filename);

// Dosyanın boyutunu döndürür (-1 hata durumunda)
long get_file_size(const char *filename);

// Dosyanın izinlerini döndürür
mode_t get_file_permissions(const char *filename);

// Dizin yoksa oluşturur
int create_directory(const char *path);

#endif