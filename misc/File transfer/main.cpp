#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

#define OPCODE_SIZE 1
#define LENGTH_SIZE 4
#define PAYLOAD_SIZE 9999
#define BUFF_SIZE OPCODE_SIZE + LENGTH_SIZE + PAYLOAD_SIZE + 1
#define OPCODE_INVALID -1
#define OPCODE_ENCODE 0
#define OPCODE_DECODE 1
#define OPCODE_TRANSFER 2
#define OPCODE_ERROR 3

long enterLong();
int enterMode();
int enterKey();
int countDigit(int);
const char *numToStr(int, int);
int strToNum(const char *);
const char *createOpcodeAndLength(int, int);
const char *generateTmpFileName();
int encode(int, int);
int decode(int, int);
void encode(int, void *, int);
void decode(int, void *, int);

int main() {
    srand(time(NULL));

    // Client creates request
    int mode = enterMode(), key = enterKey();

    // Client sends request to server
    printf("Cli to ser: %s\n", createOpcodeAndLength(mode, countDigit(key)));
    printf("Cli to ser: %s\n", numToStr(key, 0));

    // Client enters file path
    char clientFilePath[BUFF_SIZE];
    printf("Enter file path: ");
    gets_s(clientFilePath, BUFF_SIZE);
    printf("File path is %s\n", clientFilePath);

    // Client opens file
    FILE *pCli;
    errno_t err = fopen_s(&pCli, clientFilePath, "rb");
    if (err != 0) {
        printf("Error %d: cannot open file at %s\n", err, clientFilePath);
        return -1;
    }

    // Client reads file and sends it to server
    // Server saves temporary file
    void *buff = malloc(BUFF_SIZE);
    char serverFilePath[BUFF_SIZE];
    strcpy_s(serverFilePath, generateTmpFileName());
    FILE *pSer;
    err = fopen_s(&pSer, serverFilePath, "wb");

    while (!feof(pCli)) {
        memset(buff, 0, BUFF_SIZE);
        int ret = fread(buff, 1, PAYLOAD_SIZE, pCli);

        printf("Cli to ser: %s\n", createOpcodeAndLength(OPCODE_TRANSFER, ret));
        printf("Cli to ser: blahblah\n");

        fwrite(buff, 1, ret, pSer);
    }
    printf("Cli to ser: %s\n", createOpcodeAndLength(OPCODE_TRANSFER, 0));

    fclose(pCli);
    fclose(pSer);

    // Server reads, encodes / decodes, then sends file to client
    // Client saves file
    err = fopen_s(&pSer, serverFilePath, "rb");
    if (mode == OPCODE_ENCODE) {
        strcat_s(clientFilePath, ".enc");
    } else if (mode == OPCODE_DECODE) {
        strcat_s(clientFilePath, ".dec");
    }
    err = fopen_s(&pCli, clientFilePath, "wb");

    while (!feof(pSer)) {
        memset(buff, 0, BUFF_SIZE);
        int ret = fread(buff, 1, PAYLOAD_SIZE, pSer);

        printf("Ser to cli: %s\n", createOpcodeAndLength(OPCODE_TRANSFER, ret));
        printf("Ser to cli: stuff of bytes\n");

        if (mode == OPCODE_ENCODE) {
            encode(key, buff, ret);
        } else if (mode == OPCODE_DECODE) {
            decode(key, buff, ret);
        }

        fwrite(buff, 1, ret, pCli);
    }
    printf("Ser to cli: %s\n", createOpcodeAndLength(OPCODE_TRANSFER, 0));

    // Close stuff
    free(buff);
    fclose(pCli);
    fclose(pSer);

    if (remove(serverFilePath) == 0) {
        printf("Server file %s deleted\n", serverFilePath);
    } else {
        printf("Error %d: cannot delete server file %s\n", errno, serverFilePath);
    }

    return 0;
}

/**
 * Safe way to get a long number from standard input.
 * This function will not stop until a valid long is entered.
 *
 * @return number from standard input
 */
long enterLong() {
    long n;
    char *p, s[20];

    while (fgets(s, sizeof(s), stdin)) {
        n = strtol(s, &p, 10);
        if (*p == '\n' && p != s) {
            break;
        }
    }

    return n;
}

int enterMode() {
    int mode = OPCODE_INVALID;
    while (mode == OPCODE_INVALID) {
        printf("Choose mode:\t[%d] Encode\t[%d] Decode\n", OPCODE_ENCODE, OPCODE_DECODE);
        mode = enterLong();
        if (mode != OPCODE_ENCODE && mode != OPCODE_DECODE) {
            mode = OPCODE_INVALID;
        }
    }

    return mode;
}

int enterKey() {
    int key = -1;
    while (key == -1) {
        printf("Enter key in range [0, 255]: ");
        key = enterLong();
        if (key < 0 || key > 255) {
            key = -1;
        }
    }

    return key;
}

int countDigit(int n) {
    if (n == 0) {
        return 1;
    }

    int count = 0;
    while (n != 0) {
        n /= 10;
        ++count;
    }

    return count;
}

const char *numToStr(int num, int fixedSize) {
    char *result = (char *)malloc(sizeof(char) * BUFF_SIZE), tmp[BUFF_SIZE];
    strcpy_s(result, BUFF_SIZE, "");

    if (fixedSize > 0) {
        int numLength = countDigit(num);
        for (int i = 0; i < fixedSize - numLength; ++i) {
            strcat_s(result, BUFF_SIZE, "0");
        }
    }

    sprintf_s(tmp, BUFF_SIZE, "%d", num);
    strcat_s(result, BUFF_SIZE, tmp);

    return result;
}

int strToNum(const char *str) {
    int res = 0, digit = 1;
    for (int i = strlen(str) - 1; i >= 0; --i) {
        res += digit * (int)(str[i] - '0');
        digit *= 10;
    }

    return res;
}

const char *createOpcodeAndLength(int opcode, int length) {
    char *res = (char *)malloc(sizeof(char) * BUFF_SIZE);
    strcpy_s(res, BUFF_SIZE, "");
    strcat_s(res, BUFF_SIZE, numToStr(opcode, OPCODE_SIZE));
    strcat_s(res, BUFF_SIZE, numToStr(length, LENGTH_SIZE));

    return res;
}

const char *generateTmpFileName() {
    char *res = (char *)malloc(sizeof(char) * BUFF_SIZE);
    strcpy_s(res, BUFF_SIZE, "");
    strcat_s(res, BUFF_SIZE, "tmp_");
    strcat_s(res, BUFF_SIZE, numToStr(rand() % 10000, 4));

    return res;
}

int encode(int k, int c) {
    int res = c + 128;
    res = (res + k) % 256;
    res -= 128;

    return res;
}

int decode(int k, int c) {
    int res = c + 128;
    res = (res - k + 256) % 256;
    res -= 128;

    return res;
}

void encode(int k, void *buff, int length) {
    for (int i = 0; i < length; ++i) {
        ((char *)buff)[i] = encode(k, ((char *)buff)[i]);
    }
}

void decode(int k, void *buff, int length) {
    for (int i = 0; i < length; ++i) {
        ((char *)buff)[i] = decode(k, ((char *)buff)[i]);
    }
}
