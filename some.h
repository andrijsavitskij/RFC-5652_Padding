#ifndef SOME_H
#define SOME_H

#if defined(_WIN32)
#if defined(PADDING_EXPORTING)
#define PADDING_EXPORT __declspec(dllexport)
#else
#define PADDING_EXPORT __declspec(dllimport)
#endif
#else
#define PADDING_EXPORT
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

// colors for consol
#define Black "\033[0;30m"
#define Red "\033[0;31m"
#define Green "\033[0;32m"
#define Yellow "\033[0;33m"
#define Blue "\033[0;34m"
#define Purple "\033[0;35m"
#define Cyan "\033[0;36m"
#define White "\033[0;37m"
#define Standart "\033[0m"

struct Data_st {
    /* Буфер для даних */
    uint8_t* buf;
    /* Довжина буферу */
    size_t len;
};

typedef struct Data_st Data;

/**
 * Доповнює вхідні дані згідно з описом із rfc5652 розділ 6.3.
 *
 * Функція модифікує вхідний буфер з даними.
 *
 * @param data дані, що необхідно доповнити
 * @param k розмір блоку, до якого доповнюються дані
 *
 * @return код помилки
 */
PADDING_EXPORT int kp_pad(Data* data, size_t k);

/**
 * Доповнює вхідні дані згідно з описом із rfc5652 розділ 6.3 і повертає новий
 * буфер з доповненими даними.
 *
 * Функція не модифікує вхідний буфер з даними.
 *
 * @param data дані, що необхідно доповнити
 * @param k розмір блоку, до якого доповнюються дані
 * @param padded доповнені дані
 *
 * @return код помилки
 */
PADDING_EXPORT int kp_pad_alloc(Data* data, size_t k, Data* padded); // **pad

/**
 * Знімає доповнення з вхідних даних згідно з описом із rfc5652 розділ 6.3.
 *
 * Функція модифікує вхідний буфер з даними.
 *
 * @param data доповнені дані, з яких необхідно зняти доповнення
 * @param k розмір блоку, до якого доповнювалися дані
 *
 * @return код помилки
 */
PADDING_EXPORT int kp_unpad(Data* data, size_t k);

/**
 * Знімає доповнення з вхідних даних згідно з описом із rfc5652 розділ 6.3 і
 * повертає новий буфер з даними без доповнення.
 *
 * Функція не модифікує вхідний буфер з даними.
 *
 * @param data доповнені дані, з яких необхідно зняти доповнення
 * @param k розмір блоку, до якого доповнювалися дані
 * @param unpadded дані, з яких знято доповнення
 *
 * @return код помилки
 */
PADDING_EXPORT int kp_unpad_alloc(Data* data, size_t k, Data* unpadded);// **unpad

/**
 * Повний приклад роботи коду
 *
 * @param data данні без доповнення
 * @param k розмір блоку, до якого доповнювалися дані
 * @param collor колір доданої частини
 * 
 * @return код помилки
 */
int dataExamplePadUnpad(Data data, size_t k, char* collor);

//print data 
void printData(const Data);

//colorful print data
void colorPrintData(const Data data, size_t colorStart, char* colorStr);

/**
 * Іниціалізує data тим що є у databuf за допомогою memcpy
 *
 * @param data пусті данні
 * @param size розмір databuf, 
 * @param databuf данні які будуть скопійовані 
 *
 */
void dataInit(Data* data, size_t size, void* databuf);

                                // fun

void printData(const Data data) {
    printf("data.buf = ");
    for (int i = 0; i < data.len; i++) printf("%x ", data.buf[i] & 0xff);
    printf("\ndata.len = %llu\n", data.len);
}

void colorPrintData(const Data data, size_t colorStart, char* colorStr) {
    printf("data.buf = ");
    for (int i = 0; i < data.len; i++) {
        if (i >= colorStart)printf("%s%x ", colorStr, data.buf[i] & 0xff);
        else printf("%x ", data.buf[i] & 0xff);
    }
    printf("%s\ndata.len = %llu\n", Standart, data.len);
}

int dataExamplePadUnpad(Data data, size_t k, char* collor) {
    size_t stlen = data.len;
    int err = 0;

    printf("\n\n%sk = %llu%s\n\tSTART DATA: \n", collor, k, Standart);
    printData(data);
    err = kp_pad(&data, k);
    if (err) return err;

    printf("\tPADDED DATA:\n");
    colorPrintData(data, stlen, collor);

    printf("\tUNPADDED DATA:\n");
    err = kp_unpad(&data, k);
    if (err) return err;
    printData(data);

    RET_OK;
}

#endif//TESTTASK_PADDING_H