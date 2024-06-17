#include <xc.h>

#define _XTAL_FREQ 4000000  // Fréquence d'horloge

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

void init_ADC() {
    ADCON0 = 0x01;  // Activer ADC et sélectionner AN0
    ADCON1 = 0x80;  // Vref+ = Vdd, Vref- = Vss, Justification à droite
}

unsigned int read_ADC() {
    __delay_us(20);  // Temps d'acquisition
    ADCON0bits.GO_nDONE = 1;  // Démarrer la conversion ADC
    while (ADCON0bits.GO_nDONE);  // Attendre la fin de la conversion
    return ((ADRESH << 8) + ADRESL);  // Retourner le résultat de l'ADC
}

void main() {
    unsigned int adc_value;
    float temperature;
    char buffer[10];

    init_UART();
    init_ADC();

    while (1) {
        adc_value = read_ADC();
        temperature = (adc_value * 5.0 / 1024) * 100;  // Convertir la valeur ADC en température
        sprintf(buffer, "Temp: %.2fC\n", temperature);
        UART_send_string(buffer);
        __delay_ms(1000);  // Attendre 1 seconde avant de mesurer à nouveau
    }
}
