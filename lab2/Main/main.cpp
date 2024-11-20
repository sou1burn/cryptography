#include "BMP.h"
#include "FEAL.h"
using namespace lab2;

/*void test_feal_crypt() {
    // Define a simple key and plaintext for testing
    Key key;
    
    // Initialize FEAL_crypt with a valid number of rounds
    int rounds = 4; // For testing
    lab2::FEAL_crypt feal(rounds, key);

    // Create a test block of data
    Block plaintext(16, 0);  // 16 bytes of zero
    for (size_t i = 0; i < plaintext.size(); ++i) {
        plaintext[i] = static_cast<byte>(i);  // Incremental data for testing
    }

    // Encrypt the plaintext
    feal.encrypt(plaintext, key);
    std::cout << "Encrypted Data: ";
    for (auto b : plaintext) {
        std::cout << static_cast<int>(b) << " ";
    }
    std::cout << std::endl;

    // Decrypt the data
    feal.decrypt(plaintext, key);
    std::cout << "Decrypted Data: ";
    for (auto b : plaintext) {
        std::cout << static_cast<int>(b) << " ";
    }
    std::cout << std::endl;

    // Check if the decrypted data matches the original plaintext
    for (size_t i = 0; i < 16; ++i) {
        if (plaintext[i] != static_cast<byte>(i)) {
            std::cerr << "Error: Decryption did not return the original data!" << std::endl;
            return;
        }
    }

    std::cout << "Test passed successfully!" << std::endl;
}
*/

int main(int argc, char** argv)
{
    if (argc != 4) 
    {
        std::cerr << "Usage: " << argv[0] << " <input_bmp> <output_encrypted_bmp> <output_decrypted_bmp>\n";
        return 1;
    }

    
    std::string input_bmp = argv[1];
    std::string output_encrypted_bmp = argv[2];
    std::string output_decrypted_bmp = argv[3];

    try
    {
        BmpReader bmp_reader;
        Key key;
        for (size_t i = 0; i < key.size(); ++i)
        {
            std::cout << "key[" << i << "]: " << static_cast<int>(key[i]);
        }
        std::cout << std::endl;
        Block test_block = {0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF};

        FEAL_crypt feal(32, key);
        Block encrypted = test_block;
        feal.encrypt_block(encrypted);

        Block decrypted = encrypted;
        feal.decrypt_block(decrypted);

        if (test_block == decrypted)
        {
            std::cout << "Encryption and decryption are working correctly.\n";
        }
        else
        {
            for (size_t i = 0; i < test_block.size(); ++i)
            {   
                std::cout << "encrypted[" << i << "]: " << static_cast<int>(encrypted[i]) << " decrypted[" << i << "]: " << static_cast<int>(decrypted[i])  << " original[" << i << "]: " << static_cast<int>(test_block[i]) << "\n";
            }
            std::cout << "Error in encryption/decryption.\n";
        }
        size_t block_size = 8; 

        
        bmp_reader.encrypt_bmp(input_bmp, output_encrypted_bmp, block_size, key);
        std::cout << "Encryption complete. Encrypted BMP saved to " << output_encrypted_bmp << std::endl;

        
        bmp_reader.decrypt_bmp(output_encrypted_bmp, output_decrypted_bmp, block_size, key);
        std::cout << "Decryption complete. Decrypted BMP saved to " << output_decrypted_bmp << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;

}

