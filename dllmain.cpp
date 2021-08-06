#include <openssl/sha.h>
#include <string>
#include <cstring>
#include <windows.h>

extern "C" __declspec(dllexport) long mine_duinos1(char* last, char* excepted, int diff) {
    const char* lastStringBuff = last;

    unsigned char exceptedHashBytes[20] = { 0 };
    int counter_bytes = 0;
    for (const char* sym = excepted; *sym != 0; sym += 2) {
        char one[3] = { 0 };
        memcpy(one, sym, 2);
        exceptedHashBytes[counter_bytes] = (unsigned char)strtol(one, 0, 16);
        counter_bytes++;
    }

    SHA_CTX base_ctx;

    SHA1_Init(&base_ctx);
    SHA1_Update(&base_ctx, lastStringBuff, 40);
    long maximum = 100 * diff + 1;
    char conv[32] = { 0 };

    unsigned char temp_hash[20];

    for (long nonce = 1; nonce < maximum; nonce++) {
        SHA_CTX temp_ctx = base_ctx;
        std::string nb = std::to_string(nonce);
        SHA1_Update(&temp_ctx, nb.c_str(), nb.size());
        SHA1_Final(temp_hash, &temp_ctx);
        if (!memcmp(exceptedHashBytes, temp_hash, 20)) {
            return nonce;
        }
    }

    return 0;
}
