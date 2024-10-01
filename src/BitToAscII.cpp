#include "BitToAscII.h"

// Função para ler os bits e retornar o caractere ASCII correspondente
char BitToAscii::readAsciiChar() {
    int asciiValue = readBits();  // Lê os bits e monta o valor ASCII
    return (char)asciiValue;      // Converte o valor numérico para caractere
}

// Função para ler o valor dos bits dos pinos e formar o valor ASCII
int BitToAscii::readBits() {
    uint8_t value = 0;
    for ( u_int8_t i = 0; i < 8; i++) {
        int bitValue = digitalRead(_bitPins[i]); 
        value |= (bitValue << i);  
    }
    return value;
}
