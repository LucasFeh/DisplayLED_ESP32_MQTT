#ifndef BIT_TO_ASCII_H
#define BIT_TO_ASCII_H

#include <Arduino.h>

#define BIT0 13
#define BIT1 12
#define BIT2 14
#define BIT3 27
#define BIT4 26
#define BIT5 25
#define BIT6 33
#define BIT7 32


// Caso os bits cheguem todos de uma vez, é necessario estabelecer 8 pinos de leitura pra cada bit
// Porém se os bits vierem um a um, é necessario apenas 1 pino de leitura para fazer a cosntrução do caracter ASCII

class BitToAscii {
  public:
    // Construtor que recebe o array de pinos GPIO e o número de pinos (bits)
     BitToAscii(uint8_t bit0 = BIT0, uint8_t bit1 = BIT1, uint8_t bit2 = BIT2, uint8_t bit3 = BIT3,
               uint8_t bit4 = BIT4, uint8_t bit5 = BIT5, uint8_t bit6 = BIT6, uint8_t bit7 = BIT7){

        _bitPins[0] = bit0;
        _bitPins[1] = bit1;
        _bitPins[2] = bit2;
        _bitPins[3] = bit3;
        _bitPins[4] = bit4;
        _bitPins[5] = bit5;
        _bitPins[6] = bit6;
        _bitPins[7] = bit7;

        for (int i = 0; i < 8; i++) {
            pinMode(_bitPins[i], INPUT);
        }

    };

    // Função para ler os bits e retornar o caractere ASCII correspondente
    char readAsciiChar();
    
  private:
     uint8_t _bitPins[8];
    // Função para ler o valor de entrada dos pinos e formar um valor numérico
    int readBits();
};

#endif