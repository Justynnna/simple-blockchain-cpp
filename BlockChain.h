#ifndef BLOCKCHAIN_BLOCKCHAIN_H
#define BLOCKCHAIN_BLOCKCHAIN_H
#include <cstdint>
#include <cstring>
#include <iterator>

class Block {
public:
    unsigned char hash[32];
    unsigned char previous_hash[32];
    uint32_t nonce;
    char data[256];
    Block* next;
    Block() {
        nonce = 0;
        next = nullptr;
        std::memset(hash, 0, 32);
        std::memset(previous_hash, 0, 32);
        std::memset(data, 0, 256);
    }
};
class BlockChain {
    Block* head;
    Block* tail;
    const uint16_t TARGET = 50;
    bool compute_hash(Block* block);
    bool mine_block(Block* block);
public:
    BlockChain() : head(nullptr), tail(nullptr) {}
    ~BlockChain() {
        Block* current = head;
        while (current) {
            Block* nextBlock = current->next;
            delete current;
            current = nextBlock;
        }
    }
    bool add_block(const char* new_data);
    friend void run_tests();
    void print();
};


#endif //BLOCKCHAIN_BLOCKCHAIN_H