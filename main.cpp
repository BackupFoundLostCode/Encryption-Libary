#include <bitset>
#include <vector>
#include <limits>

#include "crypt.h"


int main() {

    int input = {};
    std::cout << "[1] OTP\n[2] Trivium\n[3] AES\n" << std::endl;
    std::cin >> input;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (input) {

        case 1: {

            int input2 = {};
            std::cout << "[1] Encrypt Message\n[2] Decrypt Message" <<std::endl;
            std::cin >> input2;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (input2 == 1) {
                std::string message = {};
                std::cout << "Insert Message:" <<std::endl;
                std::getline(std::cin,message);
                std::vector<std::bitset<8>> message_in_bin = ascii_to_bit(message);

                std::vector<std::bitset<8>> key_stream = streamCipher::stream_key_OTP(message_in_bin);
                std::vector<std::bitset<8>> encrypt_message = streamCipher::stream_verschlusseln_OTP(message_in_bin,key_stream);

                std::cout <<"\nDie Verschlüsselte Nachricht:" <<std::endl;
                for (auto i : encrypt_message) {
                    std::cout << i;
                }
            }
            else if (input2 == 2) {
                std::string encrypted_message2 ={};
                std::string key = {};

                std::cout << "Insert Message:" <<std::endl;
                std::cin >> encrypted_message2;
                std::cout << "Insert Key:" <<std::endl;
                std::cin >> key;

                streamCipher::stream_entschlusseln_OTP(key,encrypted_message2);
            }
            else {
                std::cout << "Invalid Number!" <<std::endl;
            }
        }
            break;
        case 2: {

            int input2 = {};
            std::cout << "[1] Encrypt Message\n[2] Decrypt Message" <<std::endl;
            std::cin >> input2;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (input2 == 1) {
                std::string message = {};
                std::cout << "Insert Message:" <<std::endl;
                std::getline(std::cin,message);

                std::vector<std::bitset<8>> message_in_bit = ascii_to_bit(message);
                std::vector<std::bitset<8>> keystream = advance::trivium::keystream_trivium_for_ASCII(message_in_bit);

                std::cout << "New generated Key: " << std::endl;
                for (auto i : keystream) {
                    std::cout << i;
                }

                std::vector<std::bitset<8>> encrypt_message = advance::trivium::verschlusseln_trivium_for_ASCII(keystream, message_in_bit);

                std::cout << "\nVerschlüsselte Nachricht:" << std::endl;
                for (auto i : encrypt_message) {
                    std::cout << i;
                }
            }
            else if (input2 == 2) {

                std::string encrypted_message2 ={};
                std::string key = {};

                std::cout << "Insert Message:" <<std::endl;
                std::cin >> encrypted_message2;
                std::cout << "Insert Key:" <<std::endl;
                std::cin >> key;

                advance::trivium::entschlusseln_trivium_for_ASCII(key,encrypted_message2);
            }
            else {
                std::cout << "Invalid Number!" <<std::endl;
            }
        }
            break;
        case 3: {
            //AES:

        }
            break;
        default: {
            std::cout << "Invalid Number!" <<std::endl;
        };
    }

    return 0;
}
