#include <avr/io.h>
#include <util/delay.h>   //biblioteca para o uso das rotinas de _delay_ms e _delay_us()

uint8_t datah, datal;
uint16_t data;
int distanciaIdeal = 80;

void setup_USART(){
  UBRR0H = 0b00000000;//9600 bits/s.
  UBRR0L = 0b01100111;
  UCSR0A = 0b11000000;
  UCSR0B = 0b00011000;
  UCSR0C = 0b00000110;
}

int DetectarPresenca(){
    int aux;
      
    ADCSRA =  ADCSRA | 0x40; //Inicia a conversão ADCS = 1.
    
    while((ADCSRA & 0x10) != 0x10){} //Espera a conversão ser finalizada - ADIF=1?
    
    datal = ADCL;
    datah = ADCH;
    ADCSRA = ADCSRA | 0x10; //Zera flag

    data = ((datah << 8) | (datal));
    
    if(data > distanciaIdeal){
      aux = 0;
    }else{
      aux = 1;
    }
    Serial.print(aux);
    return aux;
}
int main(void)
{
  setup_USART(); //Configuração da serial
  DDRC = 0x00;
  ADCSRA = 0x87; //Liga o ADC, não inicia a conversão, não habilita o auto gatilho, zera o flag ADIF, não habilita interrupção, fator de divisão igual a 128.
  ADCSRB = 0b00000000; //Modo free running - uma conversão e iniciada sempre que o bit ADSC for setado.
  
    while (1) 
    {
      ADMUX = 0x00;//Tensão de referência = pino AREF, resultado justificado a direita e canal 0 do ADC.
      _delay_ms(500);
      DetectarPresenca();
    }
}
