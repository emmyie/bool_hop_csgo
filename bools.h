#pragma once
#include <iostream>
#include <Windows.h>
#include <string>

struct bool_bit {
    bool value;
};

struct bool_byte {
    bool_bit bits[8];

    unsigned char decode() {
        unsigned char decoded = 0x0;

        for (int i = 0; i < 8; i++)
            decoded |= bits[i].value << (7 - i);

        return decoded;
    }

    std::string to_binary() {
        std::string ret;

        for (int i = 0; i < 8; i++)
            ret = std::to_string((int)bits[i].value) + ret;

        return ret;
    }
};

struct bool_int {
    bool_byte bytes[4];

    unsigned int decode() {
        unsigned int decoded = 0;

        for (int i = 0; i < 32; i++)
            decoded |= bytes[i / 8].bits[i % 8].value << (31 - i);

        return decoded;
    }

    std::string to_binary() {
        std::string ret;

        for (int i = 0; i < 32; i++)
            ret = std::to_string((int)bytes[i / 8].bits[i].value) + ret;

        return ret;
    }

    bool is() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 8; j++)
                if (bytes[i].bits[j].value)
                    return true;
    }
};

bool_byte encode(const char byte) {
    bool_byte bb = { 0 };

    for (int i = 0; i < 8; i++)
        bb.bits[i].value = (byte >> (7 - i)) & !!1;

    return bb;
}

bool_byte encode(const unsigned char byte) {
    bool_byte bb = { 0 };

    for (int i = 0; i < 8; i++)
        bb.bits[i].value = (byte >> (7 - i)) & !!1;

    return bb;
}

bool_int encode(const int bytes) {
    bool_int bi = { 0 };

    for (int i = 0; i < 32; i++) {
        bi.bytes[i / 8].bits[i % 8].value = (bytes >> (31 - i)) & !!1;
    }

    return bi;
}

bool_int encode(const unsigned int bytes) {
    bool_int bi = { 0 };

    for (int i = 0; i < 32; i++) {
        bi.bytes[i / 8].bits[i % 8].value = (bytes >> (31 - i)) & !!1;
    }

    return bi;
}

std::string char_to_binary(unsigned char byte) {
    std::string ret;
    for (int i = 0; i < 8; i++)
        ret = std::to_string((byte >> i) & !!1) + ret;

    return ret;
}

std::string int_to_binary(unsigned int bytes) {
    std::string ret;
    for (int i = 0; i < 32; i++) {
        const int bit = (bytes >> i) & !!1;
        ret = std::to_string(bit) + ret;
    }

    return ret;
}