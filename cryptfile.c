#include<stdio.h>
#include<stdlib.h>
#include<openssl/aes.h>

int main(void) {

int filename;

printf("Enter the file name to encrypt\n");
scanf("%d", &filename);

// Function to encrypt a file using AES 256
void encryptFileAES256(const char* inputFile, const char* outputFile, const unsigned char* key) {
    // Initialize AES
    AES_KEY aesKey;
    AES_set_encrypt_key(key, 256, &aesKey);

    // Open input and output files
    FILE *inFile = fopen(inputFile, "rb");
    FILE *outFile = fopen(outputFile, "wb");

    // Encrypt file block by block
    unsigned char inBlock[16], outBlock[16];
    while (fread(inBlock, 1, 16, inFile) == 16) {
        AES_encrypt(inBlock, outBlock, &aesKey);
        fwrite(outBlock, 1, 16, outFile);
    }

    // Close files
    fclose(inFile);
    fclose(outFile);
}

return 0;
}
