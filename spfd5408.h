////////////////////////////////////////////////////////////////////
//                                                                //
// Библиотека для работы с дислпеем по SPI                        //
//                                                                //
// ВНИМАНИЕ!!! Перед этим файлом в программе                      //
// нужно включить стандартные функции:                            //
// #include <delay.h>                                             //
// #include <stdio.h>                                             //
//                                                                //
// а также хидер с настройками шрифта:                            //
// #include <font_settings.h>                                     //
//                                                                //
////////////////////////////////////////////////////////////////////

// расставляем соединения модуля с МК
// !!! НЕ ЗАБЫВАЕМ в основной программе прописать эти ножки на выход, а также ножки SPI
#define RD   PORTC.0  
#define WR   PORTC.1
#define RS   PORTC.2
#define CS   PORTC.3
#define RST  PORTC.4

#define DDR_LCD_CONTROL DDRC.0=DDRC.1=DDRC.2=DDRC.3=DDRC.4

#define D0 PORTB.0
#define D1 PORTB.1
#define D2 PORTD.2
#define D3 PORTD.3
#define D4 PORTD.4
#define D5 PORTD.5
#define D6 PORTD.6
#define D7 PORTD.7

#define DDR_LCD_DATA DDRB.0=DDRB.1=DDRD.2=DDRD.3=DDRD.4=DDRD.5=DDRD.6=DDRD.7

#define delay delay_us(100)

unsigned int text_color_1=0xFF, text_color_2=0xFF, bg_color_1=0x0, bg_color_2=0x0;
unsigned char string[16]; //буферный массив для переноса текста на дисплей

void lcd_com(unsigned char addr){
  DDR_LCD_DATA=1;
  
  WR=1;
  RS=0;
  RD=1;
  
  PORTD=0;
  PORTB=0;
  
  WR=0;
  delay;
  WR=1;
  
  PORTD=(addr & 0b11111100);
  PORTB=(addr & 0b00000011);

  WR=0;
  delay;
  WR=1;
  
  RS=1;
}

void lcd_data(unsigned char data){
  WR=1;
  RS=1;
  RD=1;
    
  DDR_LCD_DATA=1;
  PORTD=(data & 0b11111100);
  PORTB=(data & 0b00000011);

  WR=0;
  delay;
  WR=1;
}

void lcd_data16(unsigned int data){
  WR=1;
  RS=1;
  RD=1;
    
  DDR_LCD_DATA=1;
  PORTD=((data>>8) & 0b11111100);
  PORTB=((data>>8) & 0b00000011);

  WR=0;
  delay;
  WR=1;
  
  PORTD=(data & 0b11111100);
  PORTB=(data & 0b00000011);

  WR=0;
  delay;
  WR=1;
}

unsigned char lcd_read(){
  unsigned char data;
  DDR_LCD_DATA=0;
  D0=D1=D2=D3=D4=D5=D6=D7=0;
  delay;
  
  RD=0;
  delay;
  
  data = (PIND & 0b11111100)+(PINB & 0b00000011);
  
  RD=1;
  delay;
 
  return data;
}

void lcd_write(unsigned char addr, unsigned int data){
  DDR_LCD_DATA=1; //переключаем нужные ноги МК на выход
  
  CS=0;
  delay;
///////////// ПЕРЕДАЧА АДРЕСА  
  RS=0;
  delay;
  
  //переключаем ножки data (PD2..7 и PB1..0) в соответствии с адресом (старший бит)
  PORTD=(addr & 0b11111100);
  PORTB=(addr & 0b00000011);
  
  WR=0;
  delay;
  
  WR=1;                                                                                              
  delay;
    
  //переключаем ножки data (PD2..7 и PB1..0) в соответствии с адресом (младший бит)
  PORTD=(addr & 0b11111100);
  PORTB=(addr & 0b00000011);
  
  WR=0;
  delay;
  
  WR=1;                                                                                              
  delay;
  
  RS=1;
  delay;

///////////// ПЕРЕДАЧА ИНФЫ
  
  //переключаем ножки data (PD2..7 и PB1..0) в соответствии со старшим байтом data
  PORTD=((data >> 8) & 0b11111100);
  PORTB=((data >> 8) & 0b00000011);
  
  WR=0;
  delay;
  
  WR=1;                                                                                              
  delay;
  
  //переключаем ножки data (PD2..7 и PB1..0) в соответствии с младшим байтом data
  PORTD=(data & 0b11111100);
  PORTB=(data & 0b00000011);
  
  WR=0;
  delay;
  
  WR=1;                                                                                              
  delay;
    
  CS=1;
}

//  //отправить SPI-комманду модулю, список команд см. выше в строках #define 
//  unsigned int lcd_read(unsigned char addr){
//  unsigned int data=0, i;
//  
//  DDR_LCD_DATA=1; //переключаем нужные ноги МК на выход
//  
//  RS=0;
//  RD=1;
//  CS=0;
//  delay;
/////////////// ПЕРЕДАЧА НОМЕРА РЕГИСТРА  
//  
////  RS=0;
////  delay;
//  
//  //переключаем ножки data (PD2..7 и PB1..0) в соответствии с адресом (пустой старший бит)
//  PORTD=0;
//  PORTB=0;
//  delay;
//  
//  WR=0;
//  delay;
//  
//  WR=1;                                                                                              
//  delay;
//    
//  //переключаем ножки data (PD2..7 и PB1..0) в соответствии с адресом (младший бит)
//  PORTD=(addr & 0b11111100);
//  PORTB=(addr & 0b00000011);
//  delay;
//  
//  WR=0;
//  delay;
//  
//  WR=1;                                                                                              
//  delay;
//  
//  RS=1;
//  delay;
//
///*//////////// ПЕРЕДАЧА КОМАНДЫ  
//  RS=0;
//  delay;
//  
//  //переключаем ножки data (PD2..7 и PB1..0) в соответствии с командой
//  PORTD=0;
//  PORTB=0;
//  PORTD.2=1;
//  delay;
//  
//  WR=0;
//  delay;
//  
//  WR=1;                                                                                              
//  delay;
//  
//  RS=1;
//  delay;
//  */
//
/////////////// ПРИЕМ ИНФЫ
//  
//  DDR_LCD_DATA=0; //переключаем нужные ноги МК на вход
//  PORTB=PORTD=255;
//  
//  for (i=1; i<5; i++) {
//  RD=0;
//  delay;
//  
//  //принимаем данные с шины data и сдвигаем на 1 байт влево, пишем в переменную data
////  data= (((PIND & 0b11111100) + (PINB & 0b00000011)) << 8);
//  data = data + (PIND & 0b11111100) + (PINB & 0b00000011);    
//  
//  RD=1;
//  delay;
//  }
//  
//  CS=1;
//  
//  return data;
//}

////отправить SPI-комманду модулю, список команд см. выше в строках #define 
//void lcd_com(unsigned char command){
//  CS=0;
//                                                                                                
//  CS=1;
//}

////отправить очередной байт данных после предшествующей команды lcd_com
//void lcd_send(unsigned char byte){
//  CS=0;
//  
//  CS=1;
//  
//}

//void lcd_send(unsigned char byte){
//  DC=1;
//  CS=0;
//  spi(byte);
//  CS=1;
//  DC=0;
//}

//задать цвет выводимого текста, диапазон 0..63 для зеленого и 0..31 для остальных
void text_color(unsigned char red, unsigned char green, unsigned char blue){
text_color_1 = (red<<3) + (green>>3);
text_color_2 = ((green & 0b00011111)<<5) + blue;
}

//Задать цвет фона, на котором выводится текст, диапазон 0..63 для зеленого и 0..31 для остальных
void bg_color(unsigned char red, unsigned char green, unsigned char blue){
bg_color_1 = (red<<3) + (green>>3);
bg_color_2 = ((green & 0b00011111)<<5) + blue;
}

//задать диапазон по X для вывода (номера столбцов пикселей начала и конца вывода, диапазон 0..127)
//void lcd_x_band (unsigned char start, unsigned char end){
//  lcd_com(PAGE_ADDR);
//  lcd_send(0);
//  lcd_send(start);
//  lcd_send(0);
//  lcd_send(end);
//}

////задать диапазон по Y для вывода (номера строк пикселей начала и конца, диапазон 0..127)
//void lcd_y_band (unsigned char start, unsigned char end){
//  lcd_com(COLUMN_ADDR);
//  lcd_send(0);
//  lcd_send(start);
//  lcd_send(0);
//  lcd_send(end);
//}

/*
//залить весь экран цветом, диапазон 0..63 для зеленого и 0..31 для остальных
void lcd_fill(unsigned char red, unsigned char green, unsigned char blue) { 
  unsigned int i;
  unsigned char spi_1, spi_2;
  spi_1 = (red<<3) + (green>>3);
  spi_2 = ((green & 0b00011111)<<5) + blue; 
   
  CS=0;
  spi(0x2C);                                                                                                
  DC=1;
     
  for (i=0; i<16384; i++){
  spi(spi_1); //first byte
  spi(spi_2); //second byte
  }
  DC=0;
  CS=1;
}

//залить цветом одну из строк полностью 
//!!!И НАЗНАЧИТЬ цвет фона под текстом (bgcolor();) равным цвету заливки!!! 
//Диапазон заливки по высоте равен указанной высоте шифта (CHAR_HEIGHT)
//цвета как обычно, диапазон 0..63 для зеленого и 0..31 для остальных
lcd_fill_str(unsigned char str_number, unsigned char red, unsigned char green, unsigned char blue){
  unsigned int i;
  unsigned char spi_1, spi_2;
  
  //определяем границы заливки
  lcd_y_band(str_number*CHAR_HEIGHT, str_number*CHAR_HEIGHT + CHAR_HEIGHT-1);
  lcd_x_band(0, 127);
  
  spi_1 = (red<<3) + (green>>3);
  spi_2 = ((green & 0b00011111)<<5) + blue; 
   
  CS=0;
  spi(0x2C); //включаем у дисплея режим вывода                                                                                               
  DC=1;
     
  for (i=0; i<(128*CHAR_HEIGHT); i++){
  spi(spi_1); //first byte
  spi(spi_2); //second byte
  }
  DC=0;
  CS=1;
  
  bg_color(red, green, blue);  //присваиваем последующему тексту фон, как у залитой строки
}

//процедуры начальной инициализации и настройки дисплея, можно добавить по вкусу комманды из ДШ
void lcd_init(){        
    //аппаратный сброс
    RST=0;
    delay_ms(1);
    RST=1;
    delay_ms(1);

    //display-on
    CS=0;
    spi(DISP_ON);
    CS=1;

    //sleep-out
    lcd_com(SLEEP_OUT);

    //настройка режима направления заполнения экрана и порядка R-G-B
    //нормальное положение экрана - стороной со штырьками вверх
    lcd_com(RAM_CONTROL);
    lcd_send(0b11101000);

    //количество цветов (уст.65536 = 16 бит RGB)
    lcd_com(PIXEL_FORMAT);
    lcd_send(0b01100101);
    
    lcd_fill(1,1,1);  //заливка черным
}

//вывод одного символа в верхн.. левую позицию (для теста)
void lcd_typechar(unsigned char char_code){
unsigned char char_column, column_pix;
lcd_fill(1,1,1);
lcd_x_band(0, 127);
lcd_y_band(0, CHAR_HEIGHT-1);
   CS=0;
   spi(0x2C);                                                                                                
   DC=1;
 for (char_column=0;char_column<(CHAR_WIGHT);char_column++) { //цикл отрисовки символа по столбцам
   for (column_pix=0;column_pix<8;column_pix++){
     if((CHAR_ARRAY_NAME[char_code-2][2*char_column+1]) & (1<<column_pix))
       {
        spi(text_color_1);
        spi(text_color_2);
       }
       else 
       {
        spi(bg_color_1);
        spi(bg_color_2);
       } 
   }
   
   for (column_pix=0;column_pix<(CHAR_HEIGHT-8);column_pix++){
     if((CHAR_ARRAY_NAME[char_code-2][2*char_column+2]) & (1<<column_pix))
       {
        spi(text_color_1);
        spi(text_color_2);
       }
       else 
       {
        spi(bg_color_1);
        spi(bg_color_2);
       } 
   }   
 }

DC=0;
CS=1;
};

//Вывод строкового массива, сгенерированного из текста и переменных стандартной функцией sprintf()
void lcd_type(unsigned char y, unsigned char x, unsigned char text[16]){
unsigned char char_column, column_pix, posit, sym_code, column_byte;
lcd_y_band(CHAR_HEIGHT*y, CHAR_HEIGHT*y+CHAR_HEIGHT-1);
lcd_x_band(8*x, 127);
   CS=0;
   spi(0x2C);                                                                                                
   DC=1;
 for (posit=0; posit<(32-x); posit++){  //цикл вывода символов по очереди
 if (text[posit] == 0) break; //если дошли до символа конца строки - прекращаем вывод, выходим из цикла
 sym_code=text[posit]-0x20;
  for (char_column=0; char_column<(CHAR_ARRAY_NAME[sym_code][0]); char_column++) { //цикл отрисовки символа по столбцам, ширина символа берется из массива ascii
  column_byte=CHAR_ARRAY_NAME[sym_code][2*char_column+1]; 
   for (column_pix=0;column_pix<8;column_pix++){
     if((column_byte) & (1<<column_pix))
       {
        spi(text_color_1);
        spi(text_color_2);                 //c  CHAR_WIGHT
       }
       else 
       {
        spi(bg_color_1);
        spi(bg_color_2);
       } 
   }
   column_byte=CHAR_ARRAY_NAME[sym_code][2*char_column+2];
   for (column_pix=0;column_pix<(CHAR_HEIGHT-8);column_pix++){
     if((column_byte) & (1<<column_pix))
       {
        spi(text_color_1);
        spi(text_color_2);
       }
       else 
       {
        spi(bg_color_1);
        spi(bg_color_2);
       } 
   }   
  }  
 }
DC=0;
CS=1;
};

//прямой вывод только текста на дисплей
void lcd_text(unsigned char y, unsigned char x, char flash *fmtstr){
sprintf(string, fmtstr);
lcd_type(y, x, string);
}

//прямой вывод на дисплей текста и одной переменной типа unsigned int
void lcd_1var(unsigned char y, unsigned char x, char flash *fmtstr, unsigned int var1){
sprintf(string, fmtstr, var1);
lcd_type(y, x, string);
}

//прямой вывод на дисплей текста и двух переменных типа unsigned int
void lcd_2var(unsigned char y, unsigned char x, char flash *fmtstr, unsigned int var1, unsigned int var2){
sprintf(string, fmtstr, var1, var2);
lcd_type(y, x, string);
}

*/