#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
    FILE *pCli;
    errno_t err = fopen_s(&pCli, "eula.png", "rb");

    fseek(pCli, 0, SEEK_END);
    long fsize = ftell(pCli);
    fseek(pCli, 0, SEEK_SET);

    void *data = malloc(fsize);
    memset(data, 0, fsize);
    fread(data, 1, fsize - 13, pCli);

    FILE *pSer;
    err = fopen_s(&pSer, "check.png", "wb");
    fwrite(data, 1, fsize - 13, pSer);

    fread(data, 1, 13, pCli);
    data = (void *)(const char *)data;
    fwrite(data, 1, 13, pSer);

    fclose(pCli);
    free(data);
    fclose(pSer);

    return 0;
}
