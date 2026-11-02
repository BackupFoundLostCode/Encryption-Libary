//
// Created by varcolac on 15.08.26.
//
#pragma once

#include <iostream>
#include <random>
#include <vector>
#include <string>

#ifndef LABOR_FOR_ENCRYPTION_CRYPT_H
#define LABOR_FOR_ENCRYPTION_CRYPT_H

#endif //LABOR_FOR_ENCRYPTION_CRYPT_H

inline std::vector<std::bitset<8>> string_to_bitset(const std::string &str) {
    std::vector<std::bitset<8>> keyInCell;
    keyInCell.resize(str.size()/8);

    for (size_t i = 0; i < keyInCell.size(); i++){
        std::string byte = str.substr(i * 8, 8);
        keyInCell[i] = std::bitset<8>(byte);
    }

    return keyInCell;
}

inline std::vector <std::bitset<8>> ascii_to_bit(const std::string &textToBit) {
    std::vector<std::bitset<8>> bitsFromAscii = {};

    for (auto i = 0; i < textToBit.length(); i++) {
        std::bitset<8> bits(textToBit[i]);
        bitsFromAscii.push_back(bits);
    }

    return bitsFromAscii;
}

inline void bit_to_ascii(const std::vector<std::bitset<8>> &textInBit) {
    for (auto i : textInBit) {
        std::cout << static_cast<char>(i.to_ullong());
    }
}

inline int real_random_numbers() {

    int result = {};
    std::random_device rd;

    if (rd.entropy()==0) {
        std::cout << "KEIN ECHTER ZUFALL AUF DEM SYSTEM MÖGLICH!" << std::endl;
        return 404;
    }
    std::uniform_int_distribution<int> myBit (0,1);//Bereiche festlegen zwischen 0 und 1
    const int zufallswert = myBit(rd);
    result = zufallswert;

    return result;
}

inline int rest(const int number, const int modulo) {
    return number % modulo;
}

inline int rest_big_number(const int number, const int expo, const int modulo) {
    return static_cast<int>(std::pow(number, expo)) % modulo;
}

inline bool ggT(const int number, const int modulo) {
    return std::gcd(number, modulo) == 1;
}
//
//
//ERLEDIGT
namespace affineEncryption {
    //nur für das alphabet gemacht momentan.
    //Sollte das mal optimierern das geht sonst nur pro buchtsbae so das der einzeln übernimmt oder so (NOTIZ WENN REST FERTIG UND IN DER MAIN DANN GEBAUT WIRD)
    inline std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWKYZ";//Damit ich die Indexe mir anschauen kann, habs nach ner woche schon vergessen gehabt, warum und musste ne minute nachdenken, warum das hier steht. Für dich Zukunfts-Ich.


    inline int affine_Encryption(const int keyA, const int keyB, const int index) {
        //muss gelten ggt(a,m) = 1, mod = 26 weil alphabet, wenn etwas %Zn dann Ergebnis immer kleiner Zn+1.
        int result = {};

        if (ggT(keyA, 26)) {
            int chiff = ((keyA* index) + keyB) % 26;
            result = chiff;
        }
        else {
            std::cout << "FATAL ERROR!" << std::endl;
        }

        return result;
    }

    inline int affine_Decryption(const int keyA, const int keyB, const int index) {
        // X = a^-1 *(y-b) mod 26, a¹ = a*a^-1 = 1 mod 26
        int result = {};

        if (ggT(keyA, 26)) {

            int reverse = {1};
            bool run = true;

            while (run) {

                if ((keyA * reverse) % 26 == 1) {
                    //reverse sollte an dem punkt a hoch - 1 sein.
                    int dCrypt = reverse * (index - keyB) % 26;//das ist nur richtig für positive ergebnisse

                    if (dCrypt < 0) {
                        dCrypt =(dCrypt +26) %26;// %26 nochmal um es, wenn es zu groß währe weider in den restklassenring 26 zu überführen, hier aber im grunde sinnlos.
                    }
                    run = false;
                    result = dCrypt;
                }
                else {
                    reverse++;
                }
            }
        }
        return result;
    }
}
//
//
//
//ERLEDIGT
namespace streamCipher  {

    inline std::vector<std::bitset<8>> stream_key_OTP(const std::vector<std::bitset<8>> &messageBitLenght) {

        std::vector<std::bitset<8>> key;
        key.resize(messageBitLenght.size());

        for (auto i = 0; i < messageBitLenght.size(); i++) {
            for (auto j = 0; j < 8; j++) {
                key[i][j] = real_random_numbers();
            }
        }
        std::cout << "\nTHIS IS YOUR KEY, SAFE THE KEY IN BIT: " << std::endl;
        for (auto i : key) {
            std::cout << i;
        }

        return key;
    }
    //
    inline std::vector<std::bitset<8>> stream_verschlusseln_OTP(const std::vector<std::bitset<8>> &textInBit, const std::vector<std::bitset<8>> &strom) {
        std::vector<std::bitset<8>> message;
        message.resize(textInBit.size());

        for (auto i = 0; i < message.size(); i++) {
            for (auto j = 0; j < 8; j++) {
                message[i][j] = (textInBit[i][j] + strom[i][j])%2;
            }
        }

        return message;
    }
    //
    inline void stream_entschlusseln_OTP(const std::string &keyToString, const std::string &verMessage) {
        std::vector<std::bitset<8>> keyInCell;
        std::vector< std::bitset<8>> newMessage;
        std::vector<std::bitset<8>> message = string_to_bitset(verMessage);

        message.resize(keyToString.size()/8);
        keyInCell.resize(keyToString.size()/8);
        newMessage.resize(keyToString.size()/8);

        if (keyToString.size()%8 !=0 || keyToString.size() !=  verMessage.size()) {
            std::cout << "Nicht genug Bit vorhanden, Key und Nachricht müssen die gleiche anzahl Bit haben." << std::endl;
            std::cout << "Länge Key: " << keyToString.length() << std::endl;
            std::cout << "Länge der verschlüsselten Nachricht: : " << verMessage.size() << std::endl;

            exit(1);
        }

        keyInCell = string_to_bitset(keyToString);

        for (auto i = 0; i < newMessage.size(); i++) {
            for (auto j = 0; j < 8; j++) {
                newMessage[i][j] = (keyInCell[i][j] + message[i][j])%2;
            }
        }

        std::cout << "Your Message:" << std::endl;
        bit_to_ascii(newMessage);
    }

}
//
//
namespace advance {

    inline void right_shift_register(const int chiffts) {
        std::bitset<8> shiftRegister;

        for (auto i = 0; i < 8; i++) {
            shiftRegister[i] = real_random_numbers();
        }

        std::cout << "Das gestezte bitset:" << std::endl;
        for (auto i = 0; i < shiftRegister.size(); i++) {
            std::cout <<shiftRegister[shiftRegister.size() - 1 -i];//minus 1 und dann minus i weill ich die minus - brauche um auf generell das maximale element zu kommen wegen der ertsen index zahl die ja 0 ist. somit habe ich hier eine länge von 8 ohne -1 komme ich so als erstes auf index 8 den gibts ja nicht. das währe eine länge von 9, nicht 8.
        }

        std::cout << "\n" << std::endl;
        std::cout << "EIN VERSUCH:" << std::endl;
        for (auto i = 0; i < chiffts; i++) {

            bool lastBit2 = shiftRegister[0];
            shiftRegister >>= 1;
            shiftRegister[7]= lastBit2;

            for (auto j = 0; j < shiftRegister.size(); j++) {
                std::cout <<shiftRegister[shiftRegister.size() - 1 -j];//minus 1 und dann minus j weill ich die minus - brauche um auf generell das maximale element zu kommen wegen der ertsen index zahl die ja 0 ist. somit habe ich hier eine länge von 8 ohne -1 komme ich so als erstes auf index 8 den gibts ja nicht. das währe eine länge von 9, nicht 8.
            }
            std::cout << "" << std::endl;
        }
    }

    inline void right_shift_register_feedback(const int chiffts) {
        std::bitset<8> shiftRegister;

        for (auto i = 0; i < 8; i++) {
            shiftRegister[i] = real_random_numbers();
        }

        std::cout << "\n" << std::endl;
        std::cout << "EIN VERSUCH (Pseudoranddomzahlen über Schieberegister):" << std::endl;
        for (auto i = 0; i < chiffts; i++) {

            bool feedback = shiftRegister[2] ^ shiftRegister[4];
            shiftRegister >>= 1;
            shiftRegister[7]= feedback;

            for (auto j = 0; j < shiftRegister.size(); j++) {
                std::cout <<shiftRegister[shiftRegister.size() - 1 -j];//minus 1 und dann minus j weill ich die minus - brauche um auf generell das maximale element zu kommen wegen der ertsen index zahl die ja 0 ist. somit habe ich hier eine länge von 8 ohne -1 komme ich so als erstes auf index 8 den gibts ja nicht. das währe eine länge von 9, nicht 8.
            }
            std::cout << "" << std::endl;
        }
    }

    namespace trivium {

        inline std::vector<std::bitset<8>> keystream_trivium_for_ASCII(const std::vector<std::bitset<8>> &message_in_bit) {
            std::bitset<93> a;
            std::bitset<84> b;
            std::bitset<111> c;
            std::bitset<80> iv_trivium;
            std::bitset<80> key;

            std::cout << "Die register ohne die key und iv bit: " << std::endl;
            std::cout << "A: " << a << " Länge: " << a.size() << std::endl;
            std::cout << "B: " << b << " Länge: " << b.size() << std::endl;
            std::cout << "C: " << c << " Länge: " << c.size() << std::endl;
            std::cout << "" << std::endl;

            std::vector<std::bitset<8>> keystream;
            keystream.resize(message_in_bit.size());

            for (auto i = 0; i < 80; i++) {
                iv_trivium[i] = real_random_numbers();
                key[i] = real_random_numbers();
            }

            std::cout << "IV: " << iv_trivium << " länge: "<< iv_trivium.size() << std::endl;
            std::cout << "KEY: " << key << " länge: "<< key.size() << std::endl;
            std::cout << "" << std::endl;

            for (auto i = 0; i < 80; i++) {
                a[i] = key[i];
                b[i] = iv_trivium[i];
            }

            c[108]= true;
            c[109]= true;
            c[110]= true;

            std::cout << "Der gefüllte a mit den iv: " << a << " länge: " << a.size() <<std::endl;
            std::cout << "Der gefüllte b mit den key: " << b << " Länge: " << b.size() <<std::endl;
            std::cout << "Der gefüllte c mit den einsen: " << c << " länge: " << c.size() <<std::endl;
            std::cout << "" << std::endl;

            for (auto i =0; i < 1152; i++) {

                bool betweenA = a[65]^a[92];
                bool betweenB = b[68]^b[83] ;
                bool betweenC = c[65]^c[110];

                bool insertB = betweenA ^ (a[90]&a[91]) ^ b[77];
                bool insertC = betweenB ^ (b[81]&b[82]) ^ c[86];
                bool insertA = betweenC ^ (c[108]&c[109]) ^ a[68];

                a <<=1;
                b <<=1;
                c <<=1;

                a[0] = insertA;
                b[0] = insertB;
                c[0] = insertC;
            }

            for (auto &i : keystream) {
                for (int bit = 0; bit < 8; bit++) {

                    bool betweenA = a[65]^a[92];
                    bool betweenB = b[68]^b[83] ;
                    bool betweenC = c[65]^c[110];

                    bool insertB = betweenA ^ (a[90]&a[91]) ^ b[77];
                    bool insertC = betweenB ^ (b[81]&b[82]) ^ c[86];
                    bool insertA = betweenC ^ (c[108]&c[109]) ^ a[68];

                    i[bit] = betweenA ^ betweenB ^betweenC;

                    a <<=1;
                    b <<=1;
                    c <<=1;

                    a[0] = insertA;
                    b[0] = insertB;
                    c[0] = insertC;
                }
            }

            return keystream;
        }
        //
        inline void entschlusseln_trivium_for_ASCII(const std::string &keyToString, const std::string &verMessage) {
            std::vector<std::bitset<8>> keyInCell;
            std::vector< std::bitset<8>> newMessage;
            std::vector<std::bitset<8>> message = string_to_bitset(verMessage);

            message.resize(keyToString.size()/8);
            keyInCell.resize(keyToString.size()/8);
            newMessage.resize(keyToString.size()/8);

            if (keyToString.size()%8 !=0 || keyToString.size() !=  verMessage.size()) {
                std::cout << "Nicht genug Bit vorhanden, Key und Nachricht müssen die gleiche anzahl Bit haben." << std::endl;
                std::cout << "Länge Key: " << keyToString.length() << std::endl;
                std::cout << "Länge der verschlüsselten Nachricht: : " << verMessage.size() << std::endl;

                exit(1);
            }

            keyInCell = string_to_bitset(keyToString);

            for (auto i = 0; i < newMessage.size(); i++) {
                for (auto j = 0; j < 8; j++) {
                    newMessage[i][j] = (keyInCell[i][j] + message[i][j])%2;
                }
            }

            std::cout << "Your Message:" << std::endl;
            bit_to_ascii(newMessage);
        }
        //
        inline std::vector<std::bitset<8>> verschlusseln_trivium_for_ASCII(const std::vector<std::bitset<8>> &keystream_trivium, const std::vector<std::bitset<8>> &message_in_bit) {
            std::vector<std::bitset<8>> message;
            message.resize(message_in_bit.size());

            for (auto i = 0; i < message.size(); i++) {
                for (auto j = 0; j < 8; j++) {
                    message[i][j] = (message_in_bit[i][j] + keystream_trivium[i][j])%2;
                }
            }

            return message;
        }
    }
    namespace DES {
        /* 64 Bit pro Block.
         * 56 Bit im key.
         * Die 64 Klartext werden in der mitte geteilt in jeweils Li und Ri.
         * 16 Runden.
         * Ri wird in f-Funktion gesteckt, dann das Ergebnis daraus XOR mit Li.
         * Li = Ri-1.
         * Ri = Li-1 XOR f(Ri-1,Ki).
         */
        inline std::vector<std::bitset<64>> verschlusseln_DES_ASCII(std::vector<std::bitset<8>> &messageIn_bit) {
            std::vector<std::bitset<56>> key;//doch 64? unsicher buch sagt so KI bei Nachfrage 64 aber es werden 56. Nicht sicher ob ich dann einfach 56 nehmen kann oder es wichtig ist was mit den fehlenden 8 passiert.
            std::vector<std::bitset<64>> message_block;//Ein Klartextblock, eventuell nichtmal nötig.
            std::vector<std::bitset<32>> l;//Linke Hälfte des Blocks
            std::vector<std::bitset<32>> r;//Rechte hälfte des Blocks.
            std::vector<std::bitset<64>> message_all_blocks;//Dachte ich speicher pro Zell einen Blck, und am ende aben die ganze nachricht verschlüsselt. Speichert also L16 und R16 n einer Zelle zusammen.

            return message_all_blocks;
        }

    }
    //
    namespace AES {
        namespace Modi {

        }
        //Auswahl der Modi, Auswahl der Keysizes (erstmal nur 128 Bit).
        inline std::vector<std::bitset<128>> verschlusselung_AES_ASCII() {
            std::vector<std::bitset<128>> crypt_block;//Jeder Block in aes sind ja festgelegte anzahl bist, der hier ist einfach für einen ausgabeblock für AES 128.
            std::vector<std::bitset<128>> all_crypt_blocks;//Hier speicher ich dann einfach die einzenen 128 bit blöcke bis ich die ganze nachricht mit Padding (oder ohne? muss nochmal schauen wie genau das mit den Modi ist)


            return all_crypt_blocks;
        }

    }
}