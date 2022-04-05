#include <avr/io.h>       //definições do componente especificado
#include <avr/interrupt.h>  //biblioteca para usar as interrupções
#include <util/delay.h>   //biblioteca para o uso das rotinas de _delay_ms e _delay_us()
#include <avr/pgmspace.h>   //para o uso do PROGMEM, gravação de dados na memória flas

#define set_bit(y,bit)  (y|=(1<<bit)) //coloca em 1 o bit x da variável Y
#define clr_bit(y,bit)  (y&=~(1<<bit))  //coloca em 0 o bit x da variável Y
#define cpl_bit(y,bit)  (y^=(1<<bit)) //troca o estado lógico do bit x da variável Y
#define tst_bit(y,bit)  (y&(1<<bit))  //retorna 0 ou 1 conforme leitura do bit

unsigned int sinal, distancia, flag = 0;

ISR(TIMER1_CAPT_vect){
//  if(flag == 0){
//    Serial.println("ICR1");
//    Serial.println(ICR1);
//    sinal = ICR1;
//    flag = 1;
//  }
//  else{
//    distancia = ((ICR1-sinal)/58);
//    flag = 0;
//  }
//  
//  //Muda o evento para borda de descida
//  TCCR1B = 0x10;

  cpl_bit(TCCR1B, ICES1);      //troca a borda de captura do sinal
  
  if(!tst_bit(TCCR1B,ICES1))    //lê o valor de contagem do TC1 na borda de subida do sinal
    sinal = ICR1;    //salva a primeira contagem para determinar a largura do pulso
  else              //lê o valor de contagem do TC1 na borda de descida do sinal
    distancia = (ICR1 - sinal)/58/2; /*agora ICR1 tem o valor do TC1 na borda de
                        descida do sinal, então calcula a distância */
}

void setup(){
  Serial.begin(9600);
    
    //Trigger ligado ao PB1 como saída e Echo ligado ao PB0 como entrada
    DDRB = 0x10;
    PORTB = 0b11111101;
    
    TCCR1B = (1<<ICES1)|(1<<CS11);  // TC1 com prescaler = 8, captura na borda de subida
    TIMSK1 = 1<<ICIE1;        //habilita a interrupção por captura
    sei();              //habilita a chave de interrupções globais
     
    TCNT1H = 0x00;
    TCNT1L = 0x00;
    
    TCCR1A = 0x00;
    TCCR1C = 0x00;
      
    Serial.println("Lendo dados do sensor...");
}

void loop(){
    //Enviar pulso
    PORTB = 0x10;
    _delay_us(10);
    PORTB = 0x00;
  
    Serial.print(distancia);
    Serial.println(" cm");
    _delay_ms(500);
}
