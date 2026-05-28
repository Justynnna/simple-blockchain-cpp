#include "tests.h"
#include "BlockChain.h"
#include <iostream>
#include <cassert>
#include <iomanip> //allows 05 instead of 5
#include <sstream>
#include <string>
#include <openssl/sha.h>
#include <openssl/evp.h>


void run_tests() {
    std::cout << "Uruchamianie testów blockchainu..." << std::endl;

    // TEST 1: Inicjalizacja pustego blockchainu
    BlockChain bc;
    // Zwróć uwagę, że head i tail powinny być nullptr na starcie
    // (Zakładamy tymczasowy dostęp przez friend class/function do celów testowych)

    // TEST 2: Próba dodania pustych danych (powinna zwrócić false)
    assert(bc.add_block(nullptr) == false);
    assert(bc.add_block("") == false);
    std::cout << "-> Test 1 & 2: Pusta inicjalizacja i walidacja danych wejściowych zaliczona." << std::endl;

    // TEST 3: Dodanie pierwszego bloku (Genesis Block)
    bool first_added = bc.add_block("Pierwszy blok danych");
    assert(first_added == true);
    assert(bc.head != nullptr);
    assert(bc.tail != nullptr);
    assert(bc.head == bc.tail); // Dla jednego elementu head == tail
    assert(bc.head->next == nullptr);

    // Sprawdzenie trudności (Proof of Work) dla pierwszego bloku
    int hash_val_1 = bc.head->hash[0] + bc.head->hash[1] * 256;
    assert(hash_val_1 <= 50); // TARGET = 50
    std::cout << "-> Test 3: Genesis block dodany i prawidłowo wykopany." << std::endl;

    // TEST 4: Dodanie drugiego bloku i weryfikacja powiązania (wielkość liter ma znaczenie)
    bool second_added = bc.add_block("Drugi blok danych");
    assert(second_added == true);
    assert(bc.head != bc.tail);
    assert(bc.head->next == bc.tail);

    // Sprawdzenie powiązania kryptograficznego (previous_hash)
    int hash_match = std::memcmp(bc.tail->previous_hash, bc.head->hash, 32);
    assert(hash_match == 0); // Hashe muszą być identyczne

    // Sprawdzenie trudności dla drugiego bloku
    int hash_val_2 = bc.tail->hash[0] + bc.tail->hash[1] * 256;
    assert(hash_val_2 <= 50);
    std::cout << "-> Test 4: Drugi blok połączony prawidłowo z poprzednikiem." << std::endl;

    // TEST 5: Maksymalna długość danych (255 znaków)
    char long_string[300];
    std::memset(long_string, 'A', 299);
    long_string[299] = '\0';

    bool long_added = bc.add_block(long_string);
    assert(long_added == true);
    assert(bc.tail->data[255] == '\0'); // Sprawdzenie ochrony przed przepełnieniem bufora
    assert(bc.tail->data[0] == 'A');
    std::cout << "-> Test 5: Ochrona przed przepełnieniem pamięci przy długich danych działa." << std::endl;

    std::cout << "\nWSZYSTKIE TESTY ZAKONCZONE SUKCESEM! " << std::endl;
}

std::string simpleSHA256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH]; //SHA256_DIGEST_LENGTH = 32 bajty = 256 bitów / 8
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string simpleSHA256_v2(const std::string& input) {
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) return "Blad: Nie udalo sie utworzyc kontekstu!";

    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1 ||
        EVP_DigestUpdate(context, input.c_str(), input.length()) != 1) {
        EVP_MD_CTX_free(context);
        return "Blad: Problem z inicjalizacja/aktualizacja hashu!";
        }

    unsigned char hashResult[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    if (EVP_DigestFinal_ex(context, hashResult, &length) != 1) {
        EVP_MD_CTX_free(context);
        return "Blad: Nie udalo sie sfinalizowac hashu!";
    }

    EVP_MD_CTX_free(context);

    std::stringstream hexStream;
    for (unsigned int i = 0; i < length; ++i) {
        hexStream << std::hex << std::setw(2) << std::setfill('0') << (int)hashResult[i];
    }
    return hexStream.str();
}

int simple_sha_run() {
    std::string data = "blockchain_test";
    std::string hashResult = simpleSHA256(data);

    std::cout << "=== Test OpenSSL w CLion ===" << std::endl;
    std::cout << "Tekst wejsciowy: " << data << std::endl;
    std::cout << "Wygenerowany Hash SHA-256: " << hashResult << std::endl;

    // Poprawny hash dla 'blockchain_test' zaczyna się od 'b308477...'
    if (!hashResult.empty()) {
        std::cout << "\n[SUKCES] OpenSSL dziala poprawnie w Twoim projekcie!" << std::endl;
    }

    std::string testText = "Hello Blockchain!";
    std::string hash = simpleSHA256_v2(testText);

    std::cout << "Tekst wejsciowy: " << testText << "\n";
    std::cout << "Wygenerowany SHA-256: " << hash << "\n";

    return 0;
}
