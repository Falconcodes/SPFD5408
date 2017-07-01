/*******************************************************
*******************************************************/
//желтый текст на синем фоне 15,15,3; 1,1,2
//желтый на коричневом 20,35,3;  5,3,1

#include <mega328p.h>
//#include <spi.h>    //SPI
#include <delay.h>  //библиотека программных задержек
#include <stdio.h>  //стандартная либа ввода-вывода Си

//#include <font_settings.h> //настроить используемый шрифт здесь, если стандартный планируется заменить
#include <spfd5408.h>

unsigned int ovflw, lcd_get, i;

// Timer 0 overflow interrupt service routine
//interrupt [TIM0_OVF] void timer0_ovf_isr(void)
//{
// ovflw++;
//}

void main(void)
{
//LCD Control lines initialization
DDR_LCD_CONTROL=1;
CS=RS=RD=WR=RST=1;

//LCD Data lines Initialisation
DDR_LCD_DATA=1;
D0=D1=D2=D3=D4=D5=D6=D7=0;


// spi initialization Clock Rate: 4000,000 kHz MSB First
//SPCR=(1<<SPE) | (1<<MSTR) | (1<<CPOL);
//SPSR=(1<<SPI2X);

// Timer/Counter 0 initialization
//TCCR0B=(0<<WGM02) | (0<<CS02) | (1<<CS01) | (0<<CS00);
// Timer/Counter 0 Interrupt(s) initialization
//TIMSK0=(0<<OCIE0B) | (0<<OCIE0A) | (1<<TOIE0);

// USART initialization
UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (0<<U2X0) | (0<<MPCM0);
UCSR0B=(0<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (0<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
UCSR0C=(0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
UBRR0H=0x00;
UBRR0L=0x67;

//lcd_get = lcd_read(0xEF);

  DDR_LCD_DATA=1;
 
  RST=0;
  delay;
  RST=1;
/*  
  DDR_LCD_DATA=1;
  
  WR=1;
  RS=0;
  RD=1;
  CS=0;
  
  PORTD=0;
  PORTB=0;
  
  WR=0;
  delay;
  WR=1;
  
  PORTD=(0x0 & 0b11111100);
  PORTB=(0x0 & 0b00000011);

  WR=0;
  delay;
  WR=1;
  
  RS=1;
  DDR_LCD_DATA=0;
  D0=D1=D2=D3=D4=D5=D6=D7=0;
  delay;
  
for (i=0;i<2; i++){  
  RD=0;
  delay;
  lcd_get += (((PIND & 0b11111100)+(PINB & 0b00000011))<<(i*8));
  RD=1;
  delay;
}
  
  CS=1;
*/  

CS=0;  
lcd_com(0x0);
lcd_get = lcd_read();
CS=1;
  
printf("DATA= %u", lcd_get);

/*
lcd_init();
lcd_fill(1,1,1);

text_color(20,25,3);
bg_color(5,3,1);

lcd_fill_str(0, 1, 1, 2);

lcd_text(0,0,"Уст.");
lcd_text(0,6,"рН = 6,50");

lcd_fill_str(1, 1, 5, 1);

lcd_text(1,0,"Факт.");
lcd_text(1,6,"рН = 6,42");

lcd_fill_str(2, 1, 1, 2);

lcd_text(2,0,"Уст.");
lcd_text(2,6,"t°=38.0°С");

lcd_fill_str(3, 1, 5, 1);

lcd_text(3,0,"Факт.");
lcd_text(3,6,"t°=38.4°С");

lcd_fill_str(4, 1, 1, 2);

lcd_text(4,0,"Прошло");
lcd_text(4,9,"45 мин");

lcd_fill_str(5, 1, 5, 1);

lcd_text(5,0,"Осталось");
lcd_text(5,9,"15 мин");

lcd_fill_str(6, 1, 1, 2);

lcd_text(6,0,"Прогресс  75%%");

lcd_fill_str(7, 1, 5, 1);

lcd_text(7,0,"Погода хорошая");

lcd_fill_str(8, 1, 1, 2);

lcd_text(8,0,"Валюты в жопе");

*/

// Global enable interrupts
#asm("sei")


    while(1){
    
    };

}
