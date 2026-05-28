# Simple Blockchain in C++

A simple, educational implementation of Blockchain technology written in C++ using the OpenSSL cryptographic library. This project demonstrates the core mechanisms behind blockchain networks, including block structure, SHA-256 hashing, and the Proof of Work consensus algorithm (block mining).

---

## 🚀 Key Features
* **Cryptographic Security**: Utilizes the industry-standard SHA-256 algorithm via OpenSSL EVP.
* **Proof of Work**: Implements a block mining mechanism with a configurable difficulty level (`TARGET`).
* **Dynamic Structure**: A blockchain network built on top of an efficient, singly linked list.
* **Memory Safe**: Built-in safeguards against memory leaks, buffer overflows, and pointer errors.

---

## 🛠️ Structures and Functions Overview

### `Block` Class (The Chain Link)
Represents a single block in the chain. It stores the payload data, cryptographic headers, and a pointer to the next element.

*   `Block()` (Constructor) – Initializes a new block, zeros out the byte arrays for hashes, sets `nonce` to 0, and sets the `next` pointer to `nullptr`. This prevents "garbage data" from corrupting the memory.

---

### `BlockChain` Class (The Network Manager)
Handles the core business logic of the network, manages the `head` (start) and `tail` (end) pointers, and coordinates the mining process.

#### 🔒 Private Functions (Cryptographic Logic):

*   `bool compute_hash(Block* block)`
    *   **Purpose**: Computes a unique digital signature (SHA-256 hash) for the given block.
    *   **Mechanism**: Combines three elements into a single data stream: the current random number (`nonce`), the text payload (`data`), and the previous block's hash (`previous_hash`). The result is saved into `block->hash`.
    *   **Significance**: Ensures data immutability. Changing even a single character in the data completely alters the resulting hash.

*   `bool mine_block(Block* block)`
    *   **Purpose**: Implements the Proof of Work consensus algorithm (mining).
    *   **Mechanism**: Runs a loop that continuously updates the `block->nonce` and recalculates the hash. The loop runs until the leading bytes of the generated hash (interpreted as an integer value) are lower than or equal to the `TARGET` threshold. Includes an overflow safeguard to prevent infinite loops.

#### 🔓 Public Functions (User Interface):

*   `BlockChain()` (Constructor)
    *   **Purpose**: Initializes an empty blockchain by setting both `head` and `tail` pointers to `nullptr`.

*   `~BlockChain()` (Destructor)
    *   **Purpose**: Safely releases RAM when the program finishes execution.
    *   **Mechanism**: Traverses the entire chain from start to finish, sequentially using `delete` on each block to prevent memory leaks.

*   `bool add_block(const char* new_data)`
    *   **Purpose**: Appends new information to the blockchain.
    *   **Mechanism**: Validates incoming data (rejects empty strings). Safely copies the text into a newly allocated block. If the chain is not empty, it links the block by copying the current `tail->hash` into the new block's `previous_hash`. It then triggers `mine_block` and, upon success, appends it to the end of the list.

*   `void print()`
    *   **Purpose**: Displays the entire contents of the blockchain on the screen.
    *   **Mechanism**: Iterates through every block from first to last, printing block indices, text data, the solved `nonce`, and full cryptographic hashes formatted as hexadecimal strings.

---

## 💻 System Requirements
To compile this project, you need a compiler that supports **C++11** or newer and the **OpenSSL** library installed on your system (specifically the `openssl/evp.h` headers).
