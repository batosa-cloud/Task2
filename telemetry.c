#include <stdio.h>
#include <stdlib.h>
#include "telemetry.h"
static float hiz = 0.0f;
static int batarya_yuzdesi;
static float motor_sicakligi;
static float batarya_sicakligi;
static float hizlanma_kayitlari[MAX_KAYIT];
static float yavaslama_kayitlari[MAX_KAYIT];
static float rejen_kayitlari[MAX_KAYIT];
static int gaza_basma_sayisi = 0;
static int frene_basma_sayisi = 0;
static int rejen_sayisi = 0;
void sistemi_kur(float baslangic_sicaklik, int baslangic_sarj) {
    motor_sicakligi = baslangic_sicaklik;
    batarya_sicakligi = baslangic_sicaklik;
    batarya_yuzdesi = baslangic_sarj;
    hiz = 0.0f;
}
void gaza_bas() {
    if (batarya_yuzdesi <= 1) {
        printf("\n[UYARI] Batarya Tukendi! Arac hareket ettirilemez.\n");
        return;
    }
    
    // Rastgele float
    float artis = (rand() % 81 + 20) / 10.0f; 
    
    if (gaza_basma_sayisi < MAX_KAYIT) {
        hizlanma_kayitlari[gaza_basma_sayisi++] = artis;
    }
    
    hiz += artis;
    if (hiz > 70.0f) hiz = 70.0f; // Max hız sınırladım
    batarya_yuzdesi -= 2;
    if (batarya_yuzdesi < 0) batarya_yuzdesi = 0;
    motor_sicakligi += 5.0f;
    batarya_sicakligi += 2.0f;
    printf("\n[BILGI] Gaza basildi. Arac %.1f km/s hizlandi. Motor: %.1f C, Batarya: %.1f C\n", artis, motor_sicakligi, batarya_sicakligi);
    
    if (motor_sicakligi > 90.0f || batarya_sicakligi > 70.0f) {
        printf("\nKRITIK HATA: Asiri Isinma! Sistem acil kapatiliyor.\n");
        exit(1); // kapatmak için 
    }
}

void frene_bas() {
    if (hiz <= 0.0f) {
        printf("\n[UYARI] Arac duruyorken frene basilamaz!\n");
        return;
    }
    
    // yeni float
    float dusus = (rand() % 101 + 50) / 10.0f; 
    if (dusus > hiz) dusus = hiz; // error case kontrolu gibi bişey negatif hızı engelle
    if (frene_basma_sayisi < MAX_KAYIT) {
        yavaslama_kayitlari[frene_basma_sayisi++] = dusus;
    }  
    hiz -= dusus;
    motor_sicakligi -= 3.0f;
    batarya_sicakligi -= 1.0f;
    // Ortam sıcaklığını normalde tutmak için 
    if (motor_sicakligi < 20.0f) motor_sicakligi = 20.0f;
    if (batarya_sicakligi < 20.0f) batarya_sicakligi = 20.0f;
    printf("\n[BILGI] Frene basildi. Arac %.1f km/s yavasladi. Motor: %.1f C, Batarya: %.1f C\n", dusus, motor_sicakligi, batarya_sicakligi);
}
void rejen_fren() {
    if (hiz <= 0.0f) {
        printf("\n[UYARI] Arac duruyorken rejeneratif fren yapilamaz!\n");
        return;
    }
    
    // yine float
    float dusus = (rand() % 81 + 20) / 10.0f; 
    if (dusus > hiz) dusus = hiz;
    if (rejen_sayisi < MAX_KAYIT) {
        rejen_kayitlari[rejen_sayisi++] = dusus;
    }
    hiz -= dusus;
    batarya_yuzdesi += 1;
    if (batarya_yuzdesi > 100) batarya_yuzdesi = 100; // Batarya taşmasının diye parametre kontrol
    motor_sicakligi -= 2.0f;
    if (motor_sicakligi < 20.0f) motor_sicakligi = 20.0f;
    batarya_sicakligi += 1.0f; 
    printf("\n[BILGI] Rejeneratif fren devrede. Arac %.1f km/s yavasladi. Batarya sarj oluyor.\n", dusus);
    if (motor_sicakligi > 90.0f || batarya_sicakligi > 70.0f) {
        printf("\nKRITIK HATA: Asiri Isinma! Sistem acil kapatiliyor.\n");
        exit(1);
    }
}

void telemetri_ve_istatistik_yazdir() {
    printf("\n--- ANLIK TELEMETRI ---\n");
    printf("Guncel Hiz: %.1f km/s\n", hiz);
    printf("Batarya: %%%d\n", batarya_yuzdesi);
    printf("Motor Sicakligi: %.1f C\n", motor_sicakligi);
    printf("Batarya Sicakligi: %.1f C\n", batarya_sicakligi);
    
    printf("\n--- SURUS ISTATISTIKLERI ---\n");
    
    float top_hizlanma = 0.0f;
    for(int i=0; i<gaza_basma_sayisi; i++) top_hizlanma += hizlanma_kayitlari[i];
    float ort_hizlanma = (gaza_basma_sayisi > 0) ? (top_hizlanma / gaza_basma_sayisi) : 0.0f;
    printf("Gaza Basma Sayisi: %d (Ortalama Artis: %.2f km/s)\n", gaza_basma_sayisi, ort_hizlanma);
    
    float top_yavaslama = 0.0f;
    for(int i=0; i<frene_basma_sayisi; i++) top_yavaslama += yavaslama_kayitlari[i];
    float ort_yavaslama = (frene_basma_sayisi > 0) ? (top_yavaslama / frene_basma_sayisi) : 0.0f;
    printf("Frene Basma Sayisi: %d (Ortalama Dusus: %.2f km/s)\n", frene_basma_sayisi, ort_yavaslama);
    
    float top_rejen = 0.0f;
    for(int i=0; i<rejen_sayisi; i++) top_rejen += rejen_kayitlari[i];
    float ort_rejen = (rejen_sayisi > 0) ? (top_rejen / rejen_sayisi) : 0.0f;
    printf("Rejeneratif Frenleme Sayisi: %d (Ortalama Dusus: %.2f km/s)\n", rejen_sayisi, ort_rejen);
}

void sistemi_kapat() {
    printf("\n[BILGI] Sistem kapatiliyor... Son Surus Istatistikleri:\n");
    telemetri_ve_istatistik_yazdir();
    printf("\n[BILGI] Motor guvenli bir sekilde kapatildi. Iyi gunler!\n");
}
