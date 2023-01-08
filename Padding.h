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

#include "some.h"

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

int kp_pad_alloc(Data* data, size_t k, Data* padded) {
    /*
    * just allocs new data(padded) memcpy data -> padded and kp_pad( padded )
    */
    if (data->len == NULL || data->len <= 0 || data->buf == NULL || (k > 255 && k < 1)) WRONG_PARAMETER_ERROR;
    dataInit(padded, data->len, data->buf);
    return kp_pad(padded, k);
}

int kp_unpad_alloc(Data* data, size_t k, Data* unpadded) {
    /*
    * just allocs new data(unpadded) memcpy data -> unpadded and kp_pad( unpadded )
    */

    if (data->len == NULL || data->len <= 0 || data->buf == NULL || (k > 255 && k < 1)) WRONG_PARAMETER_ERROR;
    dataInit(unpadded, data->len, data->buf);
    return kp_unpad(unpadded, k);
}

#endif//TESTTASK_PADDING_H
