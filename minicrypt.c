#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/blake2.h>
#include <openssl/twofish.h>

int main(int argc, char *argv[]) {
    char filename[256];
    if (argc > 1) {
        strcpy(filename, argv[1]);
    } else {
        printf("Enter the file name to encrypt: ");
        scanf("%s", filename);
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error reading file: %s\n", filename);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    unsigned char *data = malloc(file_size);
    fread(data, 1, file_size, file);
    fclose(file);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    BLAKE2b(hash, NULL, NULL, SHA256_DIGEST_LENGTH, data, file_size, NULL, 0);

    unsigned char key[SHA256_DIGEST_LENGTH];
    SHA256(hash, SHA256_DIGEST_LENGTH, key);

    Twofish_key_schedule schedule;
    Twofish_set_key(&schedule, key, 256);

    unsigned char *encrypted = malloc(file_size);
    Twofish_encrypt(&schedule, encrypted, data, file_size);

    char output_filename[260];
    snprintf(output_filename, sizeof(output_filename), "%s.foo", filename);
    file = fopen(output_filename, "wb");
    if (!file) {
        printf("Error writing encrypted file: %s\n", output_filename);
        free(data);
        free(encrypted);
        return 1;
    }
    fwrite(encrypted, 1, file_size, file);
    fclose(file);

    printf("File encrypted successfully: %s\n", output_filename);

    free(data);
    free(encrypted);
    return 0;
}
