#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

void encryptFileAES256(const char* inputFile, const char* outputFile, const unsigned char* key) {
    FILE *inFile = fopen(inputFile, "rb");
    FILE *outFile = fopen(outputFile, "wb");

    fseek(inFile, 0, SEEK_END);
    long fileSize = ftell(inFile);
    rewind(inFile);

    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);

    AES_KEY aesKey;
    AES_set_encrypt_key(key, 256, &aesKey);

    fwrite(iv, 1, AES_BLOCK_SIZE, outFile);

    unsigned char inputBuffer[AES_BLOCK_SIZE];
    unsigned char outputBuffer[AES_BLOCK_SIZE];

    while (fread(inputBuffer, 1, AES_BLOCK_SIZE, inFile) == AES_BLOCK_SIZE) {
        AES_cbc_encrypt(inputBuffer, outputBuffer, AES_BLOCK_SIZE, &aesKey, iv, AES_ENCRYPT);
        fwrite(outputBuffer, 1, AES_BLOCK_SIZE, outFile);
    }

    if (fileSize % AES_BLOCK_SIZE != 0) {
        size_t remaining = AES_BLOCK_SIZE - (fileSize % AES_BLOCK_SIZE);
        unsigned char padding[remaining];
        memset(padding, (unsigned char)remaining, remaining);
        AES_cbc_encrypt(padding, outputBuffer, AES_BLOCK_SIZE, &aesKey, iv, AES_ENCRYPT);
        fwrite(outputBuffer, 1, AES_BLOCK_SIZE, outFile);
    }

    fclose(inFile);
    fclose(outFile);
}

int main() {
    const char* inputFile = "input.txt";
    const char* outputFile = "output.ongang";
    const unsigned char key[] = "01234567890123456789012345678901";

    encryptFileAES256(inputFile, outputFile, key);

    return 0;
}
