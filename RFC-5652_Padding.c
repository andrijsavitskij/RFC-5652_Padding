#define _CRT_SECURE_NO_WARNINGS
//#include "Padding.h"
#include "Padding.h"

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
int fullRandomTest();
int reedbleRandomTest();
void printData(const Data);
void colorPrintData(const Data data, size_t colorStart, char* colorStr);
uint8_t* randomData(size_t size);

int main(){

    //Tasks
    Data data1; const int k1 = 0x10;
    dataInit(&data1, 16, (uint8_t[]) { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f });
    Data data2; const int k2 = 0x0f;
    dataInit(&data2, 7, (uint8_t[]) { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 });
    Data data3; const int k3 = 0x10;
    dataInit(&data3, 32, (uint8_t[]) {
        0x10, 0x10, 0x10, 0xa10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
            , 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
    });

    printf("k = %i\n", k1);
    printData(data1);
    kp_pad(&data1, k1);
    colorPrintData(data1, 16, Red);

    printf("\nk = %i\n", k2);
    printData(data2);
    kp_pad(&data2, k2);
    colorPrintData(data2, 7, Green);

    printf("\nk = %i\n", k3);
    printData(data3);
    kp_unpad(&data3, k3);
    printData(data3);
    //end tasks

    //full random is:    size(1 - 255) k(1 - 255) 
    //reedble random is: size(5 - 30)  k(2 - 31)
    l1:printf("\nStart random test?(Y/N  (R -reedble rundom test)):");
    char ch = getchar();

    switch (ch){
    case 'N': {
        printf("exit");
        exit(0);
        break;
    }
    case 'Y': {
        printf("Start, will stop if get error or CTR+C :) \n");
        srand(NULL);

        while (!fullRandomTest());

        break;
    }
    case 'R': {
        printf("Start, will stop if get error or CTR+C :) \n");
        srand(NULL);
        while (!reedbleRandomTest());

        break;
    }
    default: {
        printf("\twrong input\n");
        goto l1;
        break;
    }
    }

    return 0;
}

char* randomColorT_T() {
    switch (rand() % 6) {
    case 0: return Red;
    case 1: return Green;
    case 2: return Yellow;
    case 3: return Blue;
    case 4: return Purple;
    case 5: return Cyan;
    }
}

uint8_t* randomData(size_t size) {
    uint8_t* buf = calloc(size, sizeof(uint8_t));

    for (int i = 0; i < size; i++) buf[i] = rand() % 0xff;
    return buf;
}

int fullRandomTest() {
    Data data;
    size_t size = (rand() % 0xfe) + 0x01;
    size_t k = (rand() % 0xfe) + 0x01;

    dataInit(&data, size, randomData(size));
    return dataExamplePadUnpad(data, k, randomColorT_T());
}

int reedbleRandomTest() {
    Data data;
    size_t size = (rand() % 0x19) + 0x05;
    size_t k = (rand() % 0x1d) + 0x02;

    dataInit(&data, size, randomData(size));
    return dataExamplePadUnpad(data, k, randomColorT_T());
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

