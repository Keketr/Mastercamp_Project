#include <xc.h>

#define _XTAL_FREQ 4000000  // Fréquence d'horloge
#define TRIG RB0
#define ECHO RB1

void init_UART() {
    // Initialisation du UART pour la communication série
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    SPBRG = 25;  // Baud rate 9600, Fosc = 4MHz
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
}

void UART_send_char(char c) {
    while (!TXSTAbits.TRMT);
    TXREG = c;
}

void UART_send_string(const char* s) {
    while (*s) {
        UART_send_char(*s++);
    }
}

void init_HCSR04() {
    TRISB0 = 0;  // RB0 comme sortie (Trig)
    TRISB1 = 1;  // RB1 comme entrée (Echo)
}

unsigned int measure_distance() {
    unsigned int duration = 0;
    unsigned int distance = 0;

    TRIG = 0;  // Trig bas
    __delay_us(2);
    TRIG = 1;  // Trig haut
    __delay_us(10);
    TRIG = 0;  // Trig bas

    while (!ECHO);  // Attendre le début de l'echo
    while (ECHO) {  // Mesurer la durée de l'echo
        __delay_us(1);
        duration++;
    }

    distance = (duration * 0.034) / 2;  // Calculer la distance
    return distance;
}

void main() {
    unsigned int distance;
    char buffer[10];

    init_UART();
    init_HCSR04();

    while (1) {
        distance = measure_distance();
        sprintf(buffer, "Dist: %ucm\n", distance);
        UART_send_string(buffer);
        __delay_ms(1000);  // Attendre 1 seconde avant de mesurer à nouveau
    }
}
