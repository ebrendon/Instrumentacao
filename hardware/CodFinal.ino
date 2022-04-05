#include <avr/io.h>
#include <avr/interrupt.h>  //biblioteca para usar as interrupções
#include <util/delay.h>   //biblioteca para o uso das rotinas de _delay_ms e _delay_us()
#include <avr/pgmspace.h>   //para o uso do PROGMEM, gravação de dados na memória flas

#define set_bit(y,bit)  (y|=(1<<bit))   // coloca em 1 o bit x da variável Y
#define clr_bit(y,bit)  (y&=~(1<<bit))  // coloca em 0 o bit x da variável Y
#define cpl_bit(y,bit)  (y^=(1<<bit))   // troca o estado lógico do bit x da variável Y
#define tst_bit(y,bit)  (y&(1<<bit))    // retorna 0 ou 1 conforme leitura do bit

unsigned int sinal, distancia, flag = 0;
int distanciaIdeal = 80;

uint8_t datah, datal;
uint16_t data;

void setup_USART(){
  UBRR0H = 0b00000000;//9600 bits/s.
  UBRR0L = 0b01100111;
  UCSR0A = 0b11000000;
  UCSR0B = 0b00011000;
  UCSR0C = 0b00000110;
}

ISR(TIMER1_CAPT_vect){ 

  cpl_bit(TCCR1B, ICES1);      //troca a borda de captura do sinal
  
  if(!tst_bit(TCCR1B,ICES1))    //lê o valor de contagem do TC1 na borda de subida do sinal
    sinal = ICR1;    //salva a primeira contagem para determinar a largura do pulso
  else              //lê o valor de contagem do TC1 na borda de descida do sinal
    distancia = (ICR1 - sinal)/58/2; /*agora ICR1 tem o valor do TC1 na borda de
                        descida do sinal, então calcula a distância */
}
void configurarTimer() {
  // TC1 com prescaler = 8, captura na borda de subida
  TCCR1B = (1 << ICES1)|(1 << CS11);  
  // habilita a interrupção por captura
  TIMSK1 = 1 << ICIE1;                
  // habilita a chave de interrupções globais
  sei();                              
   
  TCNT1H = 0x00;
  TCNT1L = 0x00;
  
  TCCR1A = 0x00;
  TCCR1C = 0x00; 
}
void reverse(char *str, int len){
    
  int i=0, j=len-1, temp;
  
  for(i=0, j=len-1; i<j; i++, j--){
    temp = str[i];
    str[i] = str[j];
    str[j] = temp;
  }
}

int intToString(int x, char str[], int d)
{
  int i = 0;
  while (x)
  {
    str[i++] = (x%10) + '0';
    x = x/10;
  } 
  
  reverse(str, i);
  str[i] = '\0';
  return i;
}

void USART_TRANSFER(uint8_t data){
  // Verifica se o buffer está ocupado, senão coloca o ender dentro do UDRn
  while((UCSR0A & (1 << UDRE0)) != (1 << UDRE0)){}
  UDR0 = data;
}


void USART_TRANSFER_STRING(char * text){
  int i=0;
  while(text[i]){
    USART_TRANSFER(text[i]);
    i++;
  }
  USART_TRANSFER('\n');
}

void USART_TRANSFER_INT(long int number){

  char string[20];
  intToString(number, string, 0);
  USART_TRANSFER_STRING(string);
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
      sensor_HC();
    }
    return aux;
}
void sensor_HC(){
  //Enviar pulso
  PORTB = 0x10;
  _delay_us(10);
  PORTB = 0x00;
      
  USART_TRANSFER_INT(distancia); 
}

int main(void)
{
  setup_USART(); //Configuração da serial
  DDRC = 0x00;
  ADCSRA = 0x87; //Liga o ADC, não inicia a conversão, não habilita o auto gatilho, zera o flag ADIF, não habilita interrupção, fator de divisão igual a 128.
  ADCSRB = 0b00000000; //Modo free running - uma conversão e iniciada sempre que o bit ADSC for setado.

  configurarTimer();
  //Trigger ligado ao PB1 como saída e Echo ligado ao PB0 como entrada
  DDRB = 0x10;
  PORTB = 0b11111101;

    while (1) 
    {
      ADMUX = 0x00;//Tensão de referência = pino AREF, resultado justificado a direita e canal 0 do ADC.
      _delay_ms(500);
      DetectarPresenca();
    }
}
