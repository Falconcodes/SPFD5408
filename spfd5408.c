/*******************************************************
*******************************************************/
//желтый текст на синем фоне 15,15,3; 1,1,2
//желтый на коричневом 20,35,3;  5,3,1

#include <mega328p.h>
#include <spi.h>    //SPI
#include <delay.h>  //библиотека программных задержек
#include <stdio.h>  //стандартная либа ввода-вывода Си

#include <font_settings.h> //настроить используемый шрифт здесь, если стандартный планируется заменить
#include <ili9163.h>

unsigned int ovflw;

// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
 ovflw++;
}

void main(void)
{
// Input/Output Ports initialization
DDRB=(0<<DDB7) | (0<<DDB6) | (1<<DDB5) | (0<<DDB4) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (1<<PORTB1) | (0<<PORTB0);

DDRD.7=1;

// spi initialization Clock Rate: 4000,000 kHz MSB First
SPCR=(1<<SPE) | (1<<MSTR) | (1<<CPOL);
SPSR=(1<<SPI2X);

// Timer/Counter 0 initialization
TCCR0B=(0<<WGM02) | (0<<CS02) | (1<<CS01) | (0<<CS00);
// Timer/Counter 0 Interrupt(s) initialization
TIMSK0=(0<<OCIE0B) | (0<<OCIE0A) | (1<<TOIE0);

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


// Global enable interrupts
#asm("sei")


    while(1){
    
    };

}
