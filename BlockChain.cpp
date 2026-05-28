#include "BlockChain.h"
#include <iostream>
#include <openssl/evp.h>

/* computes hash for given block and sets it */
bool BlockChain::compute_hash(Block* block) {
    memset(block->hash, 0, 32);
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) return false;
    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1 ||
        EVP_DigestUpdate(context, &(block->nonce), sizeof(block->nonce)) != 1 ||
        EVP_DigestUpdate(context, block->data, 256) != 1 ||
        EVP_DigestUpdate(context, block->previous_hash, 32) != 1) {
        EVP_MD_CTX_free(context);
        return false;
        }
    unsigned char hashResult[EVP_MAX_MD_SIZE];
    unsigned int length = 0;
    if (EVP_DigestFinal_ex(context, hashResult, &length) != 1) {
        EVP_MD_CTX_free(context);
        return false;
    }
    memcpy(block->hash, hashResult, 32);
    EVP_MD_CTX_free(context);
    return true;
}
/* ckecks if (hash < target) and returns if successful*/
bool BlockChain::mine_block(Block* block) {
    while (true) {
        if (!compute_hash(block)) return false;
        int number = block->hash[0] + block->hash[1] * 256;
        if (number <= TARGET) return true;

        block->nonce++;
    }
}

bool BlockChain::add_block(const char *new_data) {
    if (! new_data || new_data[0] == '\0') return false;
    Block* new_block = new Block();
    std::strncpy(new_block->data, new_data, 255);
    new_block->data[255] = '\0';
    if (head) memcpy(new_block->previous_hash, tail->hash, 32);

    if (mine_block(new_block)) {
        if (!head) head = new_block;
        else tail->next = new_block;
        tail = new_block;
        return true;
    }
    delete new_block;
    return false;
}

void BlockChain::print() {
    Block* current = head;
    int index = 0;
    while (current) {
        std::cout << "Block: " << index++;
        std::cout << "\nData: " << current->data;
        std::cout << "\nNonce: " << current->nonce;
        std::cout << "\nPrevious hash: ";
        for(int i = 0; i < 32; i++) printf("%02x", current->previous_hash[i]);
        std::cout << "\nHash:          ";
        for(int i = 0; i < 32; i++) printf("%02x", current->hash[i]);
        std::cout << "\n----------------\n";
        current = current->next;
    }
    if (!head) {
        std::cout << "Blockchain is empty. \n";
    }

}
