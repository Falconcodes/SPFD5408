////////////////////////////////////////////////////////////////////
//                                                                //
// ���������� ��� ������ � �������� �� SPI                        //
//                                                                //
// ��������!!! ����� ���� ������ � ���������                      //
// ����� �������� ����������� �������:                            //
// #include <delay.h>                                             //
// #include <stdio.h>                                             //
//                                                                //
// � ����� ����� � ����������� ������:                            //
// #include <font_settings.h>                                     //
//                                                                //
////////////////////////////////////////////////////////////////////

// ����������� ���������� ������ � ��
// !!! �� �������� � �������� ��������� ��������� ��� ����� �� �����, � ����� ����� SPI
#define RD   PORTC.0  // � ��������� ������� ���������� "A0"
#define WR   PORTC.1
#define RS   PORTC.2
#define CS   PORTC.3
#define RST  PORTC.4

//������� ��� �������, �������� ������� � ��
#define SLEEP_IN     0x10
#define SLEEP_OUT    0x11
#define INV_OFF      0x20
#define INV_ON       0x21
#define GAMMA_SET    0x26
#define DISP_OFF     0x28
#define DISP_ON      0x29
#define COLUMN_ADDR  0x2A
#define PAGE_ADDR    0x2B
#define RAM_WRITE    0x2C
#define RAM_CONTROL  0x36
#define IDLE_OFF     0x38
#define IDLE_ON      0x39
#define PIXEL_FORMAT 0x3A

unsigned int text_color_1=0xFF, text_color_2=0xFF, bg_color_1=0x0, bg_color_2=0x0;
unsigned char string[16]; //�������� ������ ��� �������� ������ �� �������

//��������� SPI-�������� ������, ������ ������ ��. ���� � ������� #define 
void lcd_com(unsigned char command){
  CS=0;
  spi(command);                                                                                              
  CS=1;
}

//��������� ��������� ���� ������ ����� �������������� ������� lcd_com
void lcd_send(unsigned char byte){
  CS=0;
  spi(byte);
  CS=1;
  DC=0;
}

//void lcd_send(unsigned char byte){
//  DC=1;
//  CS=0;
//  spi(byte);
//  CS=1;
//  DC=0;
//}

//������ ���� ���������� ������, �������� 0..63 ��� �������� � 0..31 ��� ���������
void text_color(unsigned char red, unsigned char green, unsigned char blue){
text_color_1 = (red<<3) + (green>>3);
text_color_2 = ((green & 0b00011111)<<5) + blue;
}

//������ ���� ����, �� ������� ��������� �����, �������� 0..63 ��� �������� � 0..31 ��� ���������
void bg_color(unsigned char red, unsigned char green, unsigned char blue){
bg_color_1 = (red<<3) + (green>>3);
bg_color_2 = ((green & 0b00011111)<<5) + blue;
}

//������ �������� �� X ��� ������ (������ �������� �������� ������ � ����� ������, �������� 0..127)
void lcd_x_band (unsigned char start, unsigned char end){
  lcd_com(PAGE_ADDR);
  lcd_send(0);
  lcd_send(start);
  lcd_send(0);
  lcd_send(end);
}

//������ �������� �� Y ��� ������ (������ ����� �������� ������ � �����, �������� 0..127)
void lcd_y_band (unsigned char start, unsigned char end){
  lcd_com(COLUMN_ADDR);
  lcd_send(0);
  lcd_send(start);
  lcd_send(0);
  lcd_send(end);
}

//������ ���� ����� ������, �������� 0..63 ��� �������� � 0..31 ��� ���������
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

//������ ������ ���� �� ����� ��������� 
//!!!� ��������� ���� ���� ��� ������� (bgcolor();) ������ ����� �������!!! 
//�������� ������� �� ������ ����� ��������� ������ ����� (CHAR_HEIGHT)
//����� ��� ������, �������� 0..63 ��� �������� � 0..31 ��� ���������
lcd_fill_str(unsigned char str_number, unsigned char red, unsigned char green, unsigned char blue){
  unsigned int i;
  unsigned char spi_1, spi_2;
  
  //���������� ������� �������
  lcd_y_band(str_number*CHAR_HEIGHT, str_number*CHAR_HEIGHT + CHAR_HEIGHT-1);
  lcd_x_band(0, 127);
  
  spi_1 = (red<<3) + (green>>3);
  spi_2 = ((green & 0b00011111)<<5) + blue; 
   
  CS=0;
  spi(0x2C); //�������� � ������� ����� ������                                                                                               
  DC=1;
     
  for (i=0; i<(128*CHAR_HEIGHT); i++){
  spi(spi_1); //first byte
  spi(spi_2); //second byte
  }
  DC=0;
  CS=1;
  
  bg_color(red, green, blue);  //����������� ������������ ������ ���, ��� � ������� ������
}

//��������� ��������� ������������� � ��������� �������, ����� �������� �� ����� �������� �� ��
void lcd_init(){        
    //���������� �����
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

    //��������� ������ ����������� ���������� ������ � ������� R-G-B
    //���������� ��������� ������ - �������� �� ��������� �����
    lcd_com(RAM_CONTROL);
    lcd_send(0b11101000);

    //���������� ������ (���.65536 = 16 ��� RGB)
    lcd_com(PIXEL_FORMAT);
    lcd_send(0b01100101);
    
    lcd_fill(1,1,1);  //������� ������
}

//����� ������ ������� � �����.. ����� ������� (��� �����)
void lcd_typechar(unsigned char char_code){
unsigned char char_column, column_pix;
lcd_fill(1,1,1);
lcd_x_band(0, 127);
lcd_y_band(0, CHAR_HEIGHT-1);
   CS=0;
   spi(0x2C);                                                                                                
   DC=1;
 for (char_column=0;char_column<(CHAR_WIGHT);char_column++) { //���� ��������� ������� �� ��������
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

//����� ���������� �������, ���������������� �� ������ � ���������� ����������� �������� sprintf()
void lcd_type(unsigned char y, unsigned char x, unsigned char text[16]){
unsigned char char_column, column_pix, posit, sym_code, column_byte;
lcd_y_band(CHAR_HEIGHT*y, CHAR_HEIGHT*y+CHAR_HEIGHT-1);
lcd_x_band(8*x, 127);
   CS=0;
   spi(0x2C);                                                                                                
   DC=1;
 for (posit=0; posit<(32-x); posit++){  //���� ������ �������� �� �������
 if (text[posit] == 0) break; //���� ����� �� ������� ����� ������ - ���������� �����, ������� �� �����
 sym_code=text[posit]-0x20;
  for (char_column=0; char_column<(CHAR_ARRAY_NAME[sym_code][0]); char_column++) { //���� ��������� ������� �� ��������, ������ ������� ������� �� ������� ascii
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

//������ ����� ������ ������ �� �������
void lcd_text(unsigned char y, unsigned char x, char flash *fmtstr){
sprintf(string, fmtstr);
lcd_type(y, x, string);
}

//������ ����� �� ������� ������ � ����� ���������� ���� unsigned int
void lcd_1var(unsigned char y, unsigned char x, char flash *fmtstr, unsigned int var1){
sprintf(string, fmtstr, var1);
lcd_type(y, x, string);
}

//������ ����� �� ������� ������ � ���� ���������� ���� unsigned int
void lcd_2var(unsigned char y, unsigned char x, char flash *fmtstr, unsigned int var1, unsigned int var2){
sprintf(string, fmtstr, var1, var2);
lcd_type(y, x, string);
}
