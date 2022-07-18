#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET); /* Object of class Adafruit_SSD1306 */
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
#define SSD1306_LCDHEIGHT 64
unsigned int selectioncounter = 0;
const int selectionselectpin = 12;
const int selectionpin = 11;
const int voltagepin = A1;
unsigned int freq;
unsigned int freq1;
unsigned int freq2;
unsigned int freq3;
unsigned int pulsed;
unsigned int pulsed1;
unsigned int pulsed2;
unsigned int pulsed3;
unsigned int voltagecounter;
unsigned int voltageinput;
unsigned int voltage;
unsigned int Top;
unsigned int bdelay;
unsigned int bdelay1;
unsigned int hpw;
unsigned int oft;
float clktime;
unsigned int pw;
bool selectionselectcounter = false;
bool irindicator2 = false;
int Pwmpin = 9; //Pwm Output Pin  ;
//float EMA_a = 0.2;      //initialization of EMA alpha
  //  int EMA_S = 0;          //initialization of EMA S


//int Button=1;//Button to change the frequency
void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); /* Initialize display with address 0x3C */
  display.clearDisplay(); /* Clear display */
  display.setCursor(15,40); /* Set x,y coordinates */
  display.setTextSize(2); /* Select font size of text. Increases with size of argument. */
  display.setTextColor(WHITE); /* Color of text*/
  display.println("H.V.P.C."); /* Text to be displayed */
  display.display();
  delay(500);
  pinMode(3,OUTPUT);//Pwm pin as Output
  pinMode(selectionselectpin, INPUT);
  pinMode(selectionpin, INPUT);
  pinMode(voltagepin, INPUT);
  pinMode(9,OUTPUT);//timer1 pwm
  //pinMode(Button, INPUT);//Button as Input
  parametersetup1();
  
  
   TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS21);
  OCR2A=142;
 
//for frequency  range 0 to 4 setting prescale of clock to 256(due to comparatively large delay)
 if (freq<=4)
 {
  Top = 16000000 / (256*freq);
  clktime= 1/freq;
  pw= (pulsed*Top)/(1000000*clktime);
  bdelay= (16*Top)/(1000000*clktime);
  bdelay1=bdelay*2;
  hpw=pw/2-bdelay;
  oft=Top-pw;

  TCNT1=0;
  OCR1A = hpw;
  TCCR1A = _BV(COM1A0); //Non-inverted Mode
  TCCR1B = _BV(WGM12) | _BV(CS12); //Prescalar 256
  TCCR1C=  _BV(FOC1A);
  TIMSK1=  _BV(OCIE1A);
  sei(); 
 }
 
//for frequency  range 4 to 33 setting prescale of clock to 64
   else if (freq>4 & freq<=33)
  {
  Top = 16000000 / (64 * freq);
  clktime= 1/freq;
  pw= (pulsed*Top)/(1000000*clktime);
  bdelay= (5*Top)/(1000000*clktime);
  bdelay1=bdelay*2;
  hpw=pw/2-bdelay;
  oft=Top-pw;

  TCNT1=0;
  OCR1A = hpw;
  TCCR1A = _BV(COM1A0); //Non-inverted Mode
  TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10); //Prescalar 64
  TCCR1C=  _BV(FOC1A);
  TIMSK1=  _BV(OCIE1A);
  sei();
 }

//for frequency  range 33 to 250 setting prescale of clock to 8
  else if(freq>33 & freq<=250)
  {
  Top = 16000000 / (8 * freq);
  clktime= 1/freq;
  pw= (pulsed*Top)/(1000000*clktime);
  bdelay= (3*Top)/(1000000*clktime);
  bdelay1=bdelay*2;
  hpw=pw/2-bdelay;
  oft=Top-pw;

  TCNT1=0;
  OCR1A = hpw;
  TCCR1A = _BV(COM1A0); //Non-inverted Mode
  TCCR1B = _BV(WGM12) | _BV(CS11); //Prescalar 8
  TCCR1C=  _BV(FOC1A);
  TIMSK1=  _BV(OCIE1A);
  sei();
 }
 else
 {
  Top = 16000000 /freq;
  clktime= 1/freq;
  pw= (pulsed*Top)/(1000000*clktime);
  bdelay= (3*Top)/(1000000*clktime);
  bdelay1=bdelay*2;
  hpw=pw/2-bdelay;
  oft=Top-pw;

  TCNT1=0;
  OCR1A = hpw;
  TCCR1A = _BV(COM1A0); //Non-inverted Mode
  TCCR1B = _BV(WGM12) | _BV(CS10); //Prescalar 1
  TCCR1C=  _BV(FOC1A);
  TIMSK1=  _BV(OCIE1A);
  sei();
  
  }
}

void loop() {
   
//voltageinput=analogRead(voltagepin);
//  voltagecounter = map(voltageinput,0,1024,0,65);
//  EMA_S = (EMA_a*voltagecounter) + ((1-EMA_a)*EMA_S);   //run the EMA   
//   voltage=map(EMA_S,0,50,0,160);
//  voltage= int(voltagecounter/25);
 if (digitalRead(selectionpin) == HIGH)
    {
      delay(5);
      if (digitalRead(selectionpin) == HIGH)
      {
        while (digitalRead(selectionpin) == HIGH);
        delay(5);
        while (digitalRead(selectionpin) == HIGH);
        
        voltagecounter = voltagecounter+1;
        if (voltagecounter>=51)
        {
          voltagecounter=0;
          
          }
      }
    }
    if (digitalRead(selectionselectpin) == HIGH)
    {
      delay(5);
      if (digitalRead(selectionselectpin) == HIGH)
      {
        while (digitalRead(selectionselectpin) == HIGH);
        delay(5);
        while (digitalRead(selectionselectpin) == HIGH);
        
        voltagecounter = voltagecounter-1;
        if (voltagecounter>=51)
        {
          voltagecounter= 50;
          
          }
      }
    }
    voltage = map(voltagecounter,0,55,0,200);
    OCR2B = voltagecounter;
    display.clearDisplay();
   display.setTextSize(1);
    display.setTextColor(WHITE);
   display.setCursor(10, 5); // position to display
   display.println("F:   Hz  p.w:   us");
    display.setCursor(25, 5); // position to display
    display.println(freq); // text to display
    display.setCursor(90, 5); // position to display
    display.println(pulsed); // text to display
    display.setTextSize(2);
    display.setTextColor(WHITE);
   display.setCursor(10, 30); // position to display
   display.println("V int :   ");
   display.setCursor(90, 30); // position to display
   display.println(voltagecounter);
   display.setCursor(90, 36); // position to display
   display.println("__");
   display.display();
   
}


ISR(TIMER1_COMPA_vect)
  {
    const unsigned int wavetable[] = {hpw,bdelay1,hpw,oft};
    unsigned int index=0;

    OCR1A = wavetable[index];
    index++;
    TCNT1=0;
     if (index>=4)
     {
      index=0;
      }
    
  }

//implementing embedded interface code for setting frequency and pulseduration of device  

void parametersetup1()
{

  selectionselectcounter = true;
  selectioncounter=0;
  while (selectionselectcounter == true)
  {
    if (digitalRead(selectionpin) == HIGH)
    {
      delay(10);
      if (digitalRead(selectionpin) == HIGH)
      {
        while (digitalRead(selectionpin) == HIGH);
        delay(10);
        while (digitalRead(selectionpin) == HIGH);
        selectioncounter = selectioncounter + 1;

        if (selectioncounter == 10)
        {
          selectioncounter = 0;
        }
        freq1 = selectioncounter;
      }
    }
    
   display.clearDisplay();
   display.setTextSize(1);
    display.setTextColor(WHITE);
   display.setCursor(20, 10); // position to display
   display.println("Frequency:     HZ                       Pulse width:     us");
   
    display.setCursor(80, 13); // position to display
    display.println("_"); // text to display
    
    display.setCursor(80, 10); // position to display
    display.println(freq1); // text to display
   
    display.setCursor(86, 10); // position to display
    display.println(freq2); // text to display
   
    display.setCursor(92, 10); // position to display
    display.println(freq3); // text to display
   
    display.setCursor(80, 26); // position to display
    display.println(pulsed); // text to display
    display.display(); // show on OLED


    if (digitalRead(selectionselectpin) == HIGH) //Detection button interface to=HIGH
    {
      delay(10); //Delay 10ms for the elimination of key leading-edge jitter
      if (digitalRead(selectionselectpin) == HIGH) //Confirm button is pressed
      {
        while (digitalRead(selectionselectpin) == HIGH); //Wait for key
        delay(10); //delay 10ms for the elimination of key trailing-edge jitter
        while (digitalRead(selectionselectpin) == HIGH); //Confirm button release
        selectionselectcounter = false;
      }
    }
  }
  selectionselectcounter = true;
  selectioncounter=0;
  while (selectionselectcounter == true)
  {
    if (digitalRead(selectionpin) == HIGH)
    {
      delay(5);
      if (digitalRead(selectionpin) == HIGH)
      {
        while (digitalRead(selectionpin) == HIGH);
        delay(5);
        while (digitalRead(selectionpin) == HIGH);
        selectioncounter = selectioncounter + 1;
        if (selectioncounter == 10)
        {
          selectioncounter = 0;
        }
        freq2 = selectioncounter;
      }
    }
    
   display.clearDisplay();
   display.setTextSize(1);
    display.setTextColor(WHITE);
   display.setCursor(20, 10); // position to display
  display.println("Frequency:     HZ                       Pulse width:     us");
  
    display.setCursor(80, 10); // position to display
    display.println(freq1); // text to display
   
    display.setCursor(86, 10); // position to display
    display.println(freq2); // text to display
   
    display.setCursor(86, 13); // position to display
    display.println("_"); // text to display
    
    display.setCursor(92, 10); // position to display
    display.println(freq3); // text to display
   
    display.setCursor(80, 26); // position to display
    display.println(pulsed); // text to display
    display.display(); // show on OLED


    if (digitalRead(selectionselectpin) == HIGH) //Detection button interface to=HIGH
    {
      delay(5); //Delay 10ms for the elimination of key leading-edge jitter
      if (digitalRead(selectionselectpin) == HIGH) //Confirm button is pressed
      {
        while (digitalRead(selectionselectpin) == HIGH); //Wait for key
        delay(5); //delay 10ms for the elimination of key trailing-edge jitter
        while (digitalRead(selectionselectpin) == HIGH); //Confirm button release
        selectionselectcounter = false;
      }
    }
  }
  selectionselectcounter = true;
  selectioncounter=0;
  while (selectionselectcounter == true)
  {
    if (digitalRead(selectionpin) == HIGH)
    {
      delay(5);
      if (digitalRead(selectionpin) == HIGH)
      {
        while (digitalRead(selectionpin) == HIGH);
        delay(5);
        while (digitalRead(selectionpin) == HIGH);
        
        selectioncounter = selectioncounter + 1;
        if (selectioncounter == 10)
        {
          selectioncounter = 0;
        }
        freq3 = selectioncounter;
      }
    }
    display.clearDisplay();
   display.setTextSize(1);
    display.setTextColor(WHITE);
   display.setCursor(20,10); // position to display
   display.println("Frequency:     HZ                       Pulse width:     us");
  
    display.setCursor(80, 10); // position to display
    display.println(freq1); // text to display
   
    display.setCursor(86, 10); // position to display
    display.println(freq2); // text to display
   
    display.setCursor(92, 10); // position to display
    display.println(freq3); // text to display
   
    display.setCursor(92, 13); // position to display
    display.println("_"); // text to display
    
    display.setCursor(80, 26); // position to display
    display.println(pulsed); // text to display
    display.display(); // show on OLED


    if (digitalRead(selectionselectpin) == HIGH) //Detection button interface to=HIGH
    {
      delay(5); //Delay 10ms for the elimination of key leading-edge jitter
      if (digitalRead(selectionselectpin) == HIGH) //Confirm button is pressed
      {
        while (digitalRead(selectionselectpin) == HIGH); //Wait for key
        delay(5); //delay 10ms for the elimination of key trailing-edge jitter
        while (digitalRead(selectionselectpin) == HIGH); //Confirm button release
        selectionselectcounter = false;
      }
    }
  }
  selectionselectcounter = true;
  selectioncounter=0;
  while (selectionselectcounter == true)
  {
    if (digitalRead(selectionpin) == HIGH)
    {
      delay(5);
      if (digitalRead(selectionpin) == HIGH)
      {
        while (digitalRead(selectionpin) == HIGH);
        delay(5);
        while (digitalRead(selectionpin) == HIGH);
        selectioncounter = selectioncounter + 1;
        if (selectioncounter == 10)
        {
          selectioncounter = 0;
        }
        pulsed3 = selectioncounter;
      }
    }
    freq = (100 * freq1) + (10 * freq2) + freq3;
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(20, 10); // position to display
    display.println("Frequency:     HZ                       Pulse width:     us");
    
    display.setCursor(80, 10); // position to display
    display.println(freq); // text to display
    
    display.setCursor(82, 26); // position to display
    display.println(pulsed3); // text to display
    
    display.setCursor(88, 26); // position to display
    display.println(pulsed2); // text to display
    
    display.setCursor(94, 26); // position to display
    display.println(pulsed1); // text to display
   
    display.setCursor(82, 29); // position to display
    display.println("_"); // text to display
    display.display(); // show on OLED


    if (digitalRead(selectionselectpin) == HIGH) //Detection button interface to=HIGH
    {
      delay(5); //Delay 10ms for the elimination of key leading-edge jitter
      if (digitalRead(selectionselectpin) == HIGH) //Confirm button is pressed
      {
        while (digitalRead(selectionselectpin) == HIGH); //Wait for key
        delay(5); //delay 10ms for the elimination of key trailing-edge jitter
        while (digitalRead(selectionselectpin) == HIGH); //Confirm button release
        selectionselectcounter = false;
      }
    }
  }
  selectionselectcounter = true;
  selectioncounter=0;
  while (selectionselectcounter == true)
  {
    if (digitalRead(selectionpin) == HIGH)
    {
      delay(5);
      if (digitalRead(selectionpin) == HIGH)
      {
        while (digitalRead(selectionpin) == HIGH);
        delay(5);
        while (digitalRead(selectionpin) == HIGH);
        selectioncounter = selectioncounter + 1;
        if (selectioncounter == 10)
        {
          selectioncounter = 0;
        }
        pulsed2 = selectioncounter;
      }
    }
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(20, 10); // position to display
   display.println("Frequency:     HZ                       Pulse width:     us");
    
    display.setCursor(80, 10); // position to display
    display.println(freq); // text to display
    
    display.setCursor(82, 26); // position to display
    display.println(pulsed3); // text to display
    
    display.setCursor(88, 26); // position to display
    display.println(pulsed2); // text to display
    
    display.setCursor(88, 29); // position to display
    display.println("_"); // text to display
    
    display.setCursor(94, 26); // position to display
    display.println(pulsed1); // text to display
    display.display(); // show on OLED


    if (digitalRead(selectionselectpin) == HIGH) //Detection button interface to=HIGH
    {
      delay(5); //Delay 10ms for the elimination of key leading-edge jitter
      if (digitalRead(selectionselectpin) == HIGH) //Confirm button is pressed
      {
        while (digitalRead(selectionselectpin) == HIGH); //Wait for key
        delay(5); //delay 10ms for the elimination of key trailing-edge jitter
        while (digitalRead(selectionselectpin) == HIGH); //Confirm button release
        selectionselectcounter = false;
      }
    }
  }

  selectionselectcounter = true;
  selectioncounter=0;
  while (selectionselectcounter == true)
  {
    if (digitalRead(selectionpin) == HIGH)
    {
      delay(5);
      if (digitalRead(selectionpin) == HIGH)
      {
        while (digitalRead(selectionpin) == HIGH);
        delay(5);
        while (digitalRead(selectionpin) == HIGH);
        selectioncounter = selectioncounter + 1;
        if (selectioncounter == 10)
        {
          selectioncounter = 0;
        }
        pulsed1 = selectioncounter;
      }
    }
    pulsed = (100 * pulsed3) + (10 * pulsed2) + pulsed1;
   display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(20, 10); // position to display
    display.println("Frequency:     HZ                       Pulse width:     us");
    
    display.setCursor(80, 10); // position to display
    display.println(freq); // text to display
    
    display.setCursor(82, 26); // position to display
    display.println(pulsed3); // text to display
    
    display.setCursor(88, 26); // position to display
    display.println(pulsed2); // text to display
    
    display.setCursor(94, 26); // position to display
    display.println(pulsed1); // text to display
    
    display.setCursor(94, 29); // position to display
    display.println("_"); // text to display
    display.display(); // show on OLED


    if (digitalRead(selectionselectpin) == HIGH) //Detection button interface to=HIGH
    {
      delay(5); //Delay 10ms for the elimination of key leading-edge jitter
      if (digitalRead(selectionselectpin) == HIGH) //Confirm button is pressed
      {
        while (digitalRead(selectionselectpin) == HIGH); //Wait for key
        delay(5); //delay 10ms for the elimination of key trailing-edge jitter
        while (digitalRead(selectionselectpin) == HIGH); //Confirm button release
        selectionselectcounter = false;
      }
    }
  }
  freq = (100 * freq1) + (10 * freq2) + freq3;
  pulsed = (100 * pulsed3) + (10 * pulsed2) + pulsed1;
}


 
