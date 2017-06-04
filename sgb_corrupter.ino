//Define stuff for Arduino register voodoo magic
#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))

//Pin connected to ST_CP of 74HC595, green wire
#define latch_pin 2
//Pin connected to SH_CP of 74HC595, yellow wire
#define clock_pin 3
//Pin connected to DS of 74HC595, blue wire
#define data_pin 4

//Pin connected to WRAM Data Corruption Button
#define wram_data_button 19
//Pin connected to WRAM Address Corruption Button
#define wram_addr_button 18
//Pin connected to Cartridge Data Corruption Button
#define cart_data_button 16
//Pin connected to Cartridge Address Corruption Button
#define cart_addr_button 17

//Pin connected to WRAM Data Corruption Status LED
#define wram_data_led 12
//Pin connected to WRAM Address Corruption Status LED
#define wram_addr_led 7
//Pin connected to Cartridge Data Corruption Status LED
#define cart_data_led 6
//Pin connected to Cartridge Address Corruption Status LED
#define cart_addr_led 5

void setup() {
  //Set shift register output pins
  pinMode(latch_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);

  //Set inputs for WRAM and Cartridge corruption buttons
  pinMode(wram_data_button, INPUT);
  pinMode(wram_addr_button, INPUT);
  pinMode(cart_data_button, INPUT);
  pinMode(cart_addr_button, INPUT);

  //Set outputs for WRAM and Cartridge status LEDs
  pinMode(wram_data_led, OUTPUT);
  pinMode(wram_addr_led, OUTPUT);
  pinMode(cart_data_led, OUTPUT);
  pinMode(cart_addr_led, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  corrupt_vram(analogRead(0), analogRead(1));
  corrupt_cart_wram(digitalRead(wram_data_button), digitalRead(wram_addr_button), digitalRead(cart_data_button), digitalRead(cart_addr_button));
  /*
  if(digitalRead(wram_data_button) == HIGH){
    digitalWrite(wram_data_led, HIGH);
    delay(1000);
    digitalWrite(wram_data_led, LOW);
    //Serial.println("wram data pressed");
  }
  if(digitalRead(wram_addr_button) == HIGH){
    digitalWrite(wram_addr_led, HIGH);
    delay(1000);
    digitalWrite(wram_addr_led, LOW);
    //Serial.println("wram addr pressed");
  }
  if(digitalRead(cart_data_button) == HIGH){
    digitalWrite(cart_data_led, HIGH);
    delay(1000);
    digitalWrite(cart_data_led, LOW);
    //Serial.println("cart data pressed");
  }
  if(digitalRead(cart_addr_button) == HIGH){
    digitalWrite(cart_addr_led, HIGH);
    delay(1000);
    digitalWrite(cart_addr_led, LOW);
    //Serial.println("cart addr pressed");
  }
  /*
  if(digitalRead(5) == LOW){
    Serial.println("4 pressed");
  }
  /*
  randShift = random(8);
  switch (randShift){
    case 0:
      shiftByte = 0;
      break;
    //case 1:
      //shiftByte = 1;
      //break;
    case 2:
      shiftByte = 2;
      break;
    case 3:
      shiftByte = 4;
      break;
    case 4:
      shiftByte = 8;
      break;
    case 5:
      shiftByte = 16;
      break;
    case 6:
      shiftByte = 32;
      break;
    case 7:
      shiftByte = 64;
      break;
    case 8:
      shiftByte = 128;
      break;
  }
  digitalWrite(latchPin, LOW);
  // shift out the bits:
  shiftOut(dataPin, clockPin, LSBFIRST, 0);  
  digitalWrite(latchPin, HIGH);
  // pause before next value:
  Serial.print(analogRead(0));
  Serial.print(" ");
  Serial.print(analogRead(1));
  Serial.print(" ");
  Serial.println();
  /*
  //Corrupt arduino digpin 0, sram A0
  SET(PORTB, 0);  
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  CLR(PORTB, 0);
  
  //Corrupt arduino digpin 1, sram D0
  SET(PORTD, 0);  
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  CLR(PORTD, 0);
  
  //Corrupt arduino digpin 2, vram A0
  SET(PORTD, 0);  
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  CLR(PORTD, 0);
  
  //Corrupt arduino digpin 3, vram D0
  SET(PORTD, 0);  
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  CLR(PORTD, 0);
  
  //Corrupt arduino digpin 4, cart A0
  SET(PORTD, 0);  
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  CLR(PORTD, 0);
  
  //Corrupt arduino digpin 5, cart D0
  SET(PORTD, 0);  
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  CLR(PORTD, 0);
  delayMicroseconds(10000);
  */
}

void corrupt_vram(int addr_pot, int data_pot){
  int addr_shift = ((((addr_pot*255)/1000)));
  int data_shift = ((((data_pot*255)/1000)));
  //Serial.println(addr_shift);
  //Serial.println(data_shift);
  //int rand_addr = 255 >> addr_shift;
  //int rand_data = 255 >> data_shift;
  int rand_addr = addr_shift;
  int rand_data = data_shift;
  rand_data = rand_data&0b11110111;
  rand_addr = rand_addr&0b11110111;
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, rand_addr);  
  digitalWrite(latch_pin, HIGH);
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, rand_data);  
  digitalWrite(latch_pin, HIGH);
  delay(10);
}

void corrupt_cart_wram(bool wram_data_button_status, bool wram_addr_button_status, bool cart_data_button_status, bool cart_addr_button_status)
{
  if(wram_data_button_status)
  {
    digitalWrite(wram_data_led, HIGH);
    corrupt_line(2);
    Serial.println("Wram Data Button Pressed");
    digitalWrite(wram_data_led, LOW);
  }

  if(wram_addr_button_status)
  {
    digitalWrite(wram_addr_led, HIGH);
    corrupt_line(1);
    Serial.println("Wram Address Button Pressed");
    digitalWrite(wram_addr_led, LOW);
  }

  if(cart_data_button_status)
  {
    digitalWrite(cart_data_led, HIGH);
    corrupt_line(3);
    Serial.println("Cart Data Button Pressed");
    digitalWrite(cart_data_led, LOW);
  }

  if(cart_addr_button_status)
  {
    digitalWrite(cart_addr_led, HIGH);
    corrupt_line(0);
    Serial.println("Cart Address Button Pressed");
    digitalWrite(cart_addr_led, LOW);
  }
}

void corrupt_line(int port)
{
  SET(PORTB, port);  
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  CLR(PORTB, port);
}

