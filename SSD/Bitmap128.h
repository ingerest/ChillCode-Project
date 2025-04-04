#pragma once
#include <iostream>
#include <cstdint>

class Bitset128 {
private:
    uint64_t high; // 상위 64비트
    uint64_t low;  // 하위 64비트

public:
    Bitset128() : high(0), low(0) {}

    // 특정 비트 설정 (1로 만들기)
    void setBit(int index) {
        if (index < 0 || index >= 128) return;
        if (index < 64)
            low |= (1ULL << index);
        else
            high |= (1ULL << (index - 64));
    }

    // 특정 비트 해제 (0으로 만들기)
    void clearBit(int index) {
        if (index < 0 || index >= 128) return;
        if (index < 64)
            low &= ~(1ULL << index);
        else
            high &= ~(1ULL << (index - 64));
    }

    // 특정 비트 읽기 (0 또는 1 반환)
    bool getBit(int index) const {
        if (index < 0 || index >= 128) return false;
        if (index < 64)
            return (low & (1ULL << index)) != 0;
        else
            return (high & (1ULL << (index - 64))) != 0;
    }

    // 특정 비트 반전 (1 ↔ 0 토글)
    void toggleBit(int index) {
        if (index < 0 || index >= 128) return;
        if (index < 64)
            low ^= (1ULL << index);
        else
            high ^= (1ULL << (index - 64));
    }

    // AND 연산
    void bitwiseAnd(const Bitset128& other) {
        low &= other.low;
        high &= other.high;
    }

    // OR 연산
    void bitwiseOr(const Bitset128& other) {
        low |= other.low;
        high |= other.high;
    }

    // XOR 연산
    void bitwiseXor(const Bitset128& other) {
        low ^= other.low;
        high ^= other.high;
    }

    // 전체 클리어 (0으로 초기화)
    void clearAll() {
        low = 0;
        high = 0;
    }
};