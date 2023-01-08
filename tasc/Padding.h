#ifndef TESTTASK_PADDING_H
#define TESTTASK_PADDING_H

/*
* add "return" to defines because: 
* return RET_OK; longer then RET_OK;
I think readability has not been alost
well, maybe a little)
*/
/** Error code */ 
#define RET_OK                return 0  // Successful 
#define WRONG_PARAMETER_ERROR return 1  // Invalid input parameter
#define RUNTIME_ERROR         return 2  // Others errors


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
PADDING_EXPORT int kp_pad_alloc(Data* data, size_t k, Data** padded); // **pad

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
PADDING_EXPORT int kp_unpad_alloc(Data* data, size_t k, Data** unpadded);// **unpad

/**
 * Іниціалізує data тим що є у databuf за допомогою memcpy
 *
 * @param data пусті данні
 * @param size розмір databuf, 
 * @param databuf данні які будуть скопійовані 
 *
 */
PADDING_EXPORT void dataInit(Data* data, size_t size, void* databuf);


void dataInit(Data* data, size_t size, void* databuf) {
    data->len = size;
    data->buf = (uint8_t*)malloc(sizeof(uint8_t) * data->len);
    if (data->buf == NULL) exit(-1); // 
    memcpy(data->buf, databuf, data->len);
}

int kp_pad(Data* data, size_t k) {
    // input parameter check
    if (data->len == NULL || data->len <= 0 || data->buf == NULL || (k > 255 && k < 1)) WRONG_PARAMETER_ERROR;

    const size_t kx = k - (data->len % k);                                          // padded count 
    data->buf = (uint8_t*)realloc(data->buf, (data->len + kx) * sizeof(uint8_t));   // realloc size = data.len + kx
    if (data->buf == NULL) RUNTIME_ERROR;                                           // if realloc failed

    for (int i = data->len; i < data->len + kx; i++) data->buf[i] = (uint8_t)kx;    // padded cycle
    data->len += kx;
    RET_OK;
}

int kp_unpad(Data* data, size_t k) {
    // input parameter check
    if (data->len == NULL || data->len <= 0 || data->buf == NULL || (k > 255 && k < 1)) WRONG_PARAMETER_ERROR;

    const size_t kx = data->buf[data->len - 1];                                     // padded count 
    data->buf = (uint8_t*)realloc(data->buf, (data->len - kx) * sizeof(uint8_t));   // realloc size = data.len - kx
    if (data->buf == NULL) RUNTIME_ERROR;                                           // if realloc failed

    data->len -= kx;
    RET_OK;

    /*
    * there are no cycles and mem rewrites here, as far as I understand, realloc frees old memory
    */
}

int kp_pad_alloc(Data* data, size_t k, Data** padded) {
    /*
    * just allocs new data(padded) memcpy data -> padded and kp_pad( padded )
    */
    if (data->len == NULL || data->len <= 0 || data->buf == NULL || (k > 255 && k < 1)) WRONG_PARAMETER_ERROR;
    dataInit(*padded, data->len, data->buf);
    return kp_pad(*padded, k);
}

int kp_unpad_alloc(Data* data, size_t k, Data** unpadded) {
    /*
    * just allocs new data(unpadded) memcpy data -> unpadded and kp_pad( unpadded )
    */

    if (data->len == NULL || data->len <= 0 || data->buf == NULL || (k > 255 && k < 1)) WRONG_PARAMETER_ERROR;
    dataInit(*unpadded, data->len, data->buf);
    return kp_unpad(*unpadded, k);
}


#endif//TESTTASK_PADDING_H