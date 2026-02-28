#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "telemetry.h"

int main() {
    srand(time(NULL)); 

    float batarya_sicakligi;
    int sarj_yuzdesi;
    char kapi_durumu;
    int fren_pedali;
    int hata_var_mi = 0; 
    printf("--- ELEKTROMOBIL GUVENLIK KONTROLU ---\n");
    printf("Batarya Sicakligi (C): ");
    scanf("%f", &batarya_sicakligi);
    if (batarya_sicakligi < -100 || batarya_sicakligi > 200) {
        printf("\nSONUC: KRITIK HATA: Mantiksiz sicaklik degeri!\n");
        return 0;
    }
    printf("Sarj Yuzdesi (%%): ");
    scanf("%d", &sarj_yuzdesi);
    if (sarj_yuzdesi < 0 || sarj_yuzdesi > 100) {
        printf("\nSONUC: KRITIK HATA: Imkansiz sarj seviyesi (%%%d). Lutfen sensor verilerini kontrol edin.\n", sarj_yuzdesi);
        return 0; 
    }
    printf("Kapi Durumu (A/K): ");
    scanf(" %c", &kapi_durumu);
    if (kapi_durumu != 'A' && kapi_durumu != 'K') {
        printf("\nSONUC: KRITIK HATA: Gecersiz kapi durumu karakteri!\n");
        return 0;
    }
    printf("Fren Pedali (1:Basili, 0:Degil): ");
    scanf("%d", &fren_pedali);
    printf("\nSONUC:\n");
    if (batarya_sicakligi > 60.0) {
        printf("UYARI: Motor Asiri Isindi! Surus Engellendi.\n"); 
        hata_var_mi = 1;
    }
    if (sarj_yuzdesi < 10) {
        printf("UYARI: Batarya Kritik Seviyede! Surus Baslatilamaz.\n"); 
        hata_var_mi = 1;
    }
    if (kapi_durumu == 'A') {
        printf("UYARI: Kapilar Acik! Lutfen Kapatiniz.\n"); 
        hata_var_mi = 1;
    }
    if (fren_pedali == 0) {
        printf("UYARI: Guvenlik icin frene basarak tekrar deneyin.\n"); 
        hata_var_mi = 1;
    }
    
    // Yeni ödev başlangıcı
    if (hata_var_mi == 0) {
        printf("BASARILI: SISTEM HAZIR. MOTOR BASLATILIYOR...\n");
        sistemi_kur(batarya_sicakligi, sarj_yuzdesi);
        int secim;
        while (1) {
            printf("\n--- TUFAN ELEKTROMOBIL SURUS SIMULASYONU ---\n");
            printf("1. Gaza Bas\n");
            printf("2. Frene Bas\n");
            printf("3. Rejeneratif Frenleme Yap\n");
            printf("4. Anlik Telemetri ve Istatistikleri Oku\n");
            printf("5. Sistemi Kapat\n");
            printf("Seciminiz: ");
            if (scanf("%d", &secim) != 1) {
                while (getchar() != '\n');
                printf("Hatali giris! Lutfen 1-5 arasinda bir sayi giriniz.\n");
                continue; 
            }
            switch(secim) {
                case 1: gaza_bas(); break;
                case 2: frene_bas(); break;
                case 3: rejen_fren(); break;
                case 4: telemetri_ve_istatistik_yazdir(); break;
                case 5: sistemi_kapat(); return 0; // Döngüyü ve programı bitirir
                default: printf("Gecersiz secim! Lutfen 1-5 arasinda bir secim yapin.\n");
            }
        }
    } else {
        printf("Sistem hazir degil, gosterge paneli kapaniyor...\n");
    }

    return 0;
}
