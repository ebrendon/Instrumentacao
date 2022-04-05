#include <avr/io.h>       //definições do componente especificado
#include <avr/interrupt.h>  //biblioteca para usar as interrupções
#include <util/delay.h>   //biblioteca para o uso das rotinas de _delay_ms e _delay_us()
#include <avr/pgmspace.h>   //para o uso do PROGMEM, gravação de dados na memória flas

#define set_bit(y,bit)  (y|=(1<<bit))   // coloca em 1 o bit x da variável Y
#define clr_bit(y,bit)  (y&=~(1<<bit))  // coloca em 0 o bit x da variável Y
#define cpl_bit(y,bit)  (y^=(1<<bit))   // troca o estado lógico do bit x da variável Y
#define tst_bit(y,bit)  (y&(1<<bit))    // retorna 0 ou 1 conforme leitura do bit

unsigned int sinal, distancia, flag = 0;

ISR(TIMER1_CAPT_vect) { 
  // ICES1 = 1
  cpl_bit(TCCR1B, ICES1);       // troca a borda de captura do sinal
  // ICES1 = 0
  if(!tst_bit(TCCR1B, ICES1))   // lê o valor de contagem do TC1 na borda de subida do sinal
    sinal = ICR1;               // salva a primeira contagem para determinar a largura do pulso
  else                          // lê o valor de contagem do TC1 na borda de descida do sinal
    distancia = (ICR1 - sinal)/58/2; // agora ICR1 tem o valor do TC1 na borda de descida do sinal, então calcula a distância
}

void USART_CONFIG()
{
  UBRR0H = 0b00000000;  // seta 9600 de baud rate
  UBRR0L = 0b01100111;  
  UCSR0A = 0b11000000;  // limpa RXC E TXC
  UCSR0B = 0b00011000;  // seta RXEN e TXEN, para receber e transmitir
  UCSR0C = 0b00000110;  // seta tamanho dos dados em 8 bits
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
  intToString(number,string, 0);
  USART_TRANSFER_STRING(string);
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

int main(void) {
    USART_CONFIG();
  
    // Trigger ligado ao PB1 como saída e Echo ligado ao PB0 como entrada
    DDRB = 0b00000010;
    PORTB = 0b11111101;
  
    configurarTimer();

     // Enviar pulso de 10 us no Trigger para ativar o sensor.     
     PORTB = 0b00000010;
     _delay_us(10);
     PORTB = 0x00;
 
    while(1) { 
      USART_TRANSFER_INT(distancia); 
      _delay_ms(1000);
    }
}
