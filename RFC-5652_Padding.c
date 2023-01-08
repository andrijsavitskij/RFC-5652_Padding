#define _CRT_SECURE_NO_WARNINGS
#include "Padding.h"

int fullRandomTest();
int reedbleRandomTest();

int main(){

    //Tasks
    Data data1; const int k1 = 0x10;
    dataInit(&data1, 16, (uint8_t[]) { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f });
    Data data2; const int k2 = 0x0f;
    dataInit(&data2, 07, (uint8_t[]) { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 });
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
    colorPrintData(data2, 07, Green);

    printf("\nk = %i\n", k3);
    printData(data3);
    kp_unpad(&data3, k3);
    printData(data3);
    //end tasks

    //Data data;
    //dataInit(&data, 8, (uint8_t[]) { 0xfd, 0x3d, 0xa1, 0x00, 0x20, 0xcc, 0xe7, 0xff });
    //dataExamplePadUnpad(data,0x0a, Yellow);
    
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

}

char* randomColorT_T() {
    switch (rand() % 7) {
    case 0: return Red;
    case 1: return Green;
    case 2: return Yellow;
    case 3: return Blue;
    case 4: return Purple;
    case 5: return Cyan;
    case 6: return White;
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

