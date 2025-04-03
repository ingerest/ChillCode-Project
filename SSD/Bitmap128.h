#pragma once
#include <iostream>
#include <cstdint>

class Bitset128 {
private:
    uint64_t high; // ���� 64��Ʈ
    uint64_t low;  // ���� 64��Ʈ

public:
    Bitset128() : high(0), low(0) {}

    // Ư�� ��Ʈ ���� (1�� �����)
    void setBit(int index) {
        if (index < 0 || index >= 128) return;
        if (index < 64)
            low |= (1ULL << index);
        else
            high |= (1ULL << (index - 64));
    }

    // Ư�� ��Ʈ ���� (0���� �����)
    void clearBit(int index) {
        if (index < 0 || index >= 128) return;
        if (index < 64)
            low &= ~(1ULL << index);
        else
            high &= ~(1ULL << (index - 64));
    }

    // Ư�� ��Ʈ �б� (0 �Ǵ� 1 ��ȯ)
    bool getBit(int index) const {
        if (index < 0 || index >= 128) return false;
        if (index < 64)
            return (low & (1ULL << index)) != 0;
        else
            return (high & (1ULL << (index - 64))) != 0;
    }

    // Ư�� ��Ʈ ���� (1 �� 0 ���)
    void toggleBit(int index) {
        if (index < 0 || index >= 128) return;
        if (index < 64)
            low ^= (1ULL << index);
        else
            high ^= (1ULL << (index - 64));
    }

    // AND ����
    void bitwiseAnd(const Bitset128& other) {
        low &= other.low;
        high &= other.high;
    }

    // OR ����
    void bitwiseOr(const Bitset128& other) {
        low |= other.low;
        high |= other.high;
    }

    // XOR ����
    void bitwiseXor(const Bitset128& other) {
        low ^= other.low;
        high ^= other.high;
    }

    // ��ü Ŭ���� (0���� �ʱ�ȭ)
    void clearAll() {
        low = 0;
        high = 0;
    }
};