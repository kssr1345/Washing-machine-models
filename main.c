/*
 * File:   main.c
 * Author: K SOMA SEKHAR REDDY
 *
 * Created on 12 October, 2021, 10:23 AM
 */


#include "main.h"
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)

//global variables
unsigned char reset_mode,program_no=0,level_no=0,operation_mode; 

//washing program
char *washing_prog[]={"Daily","Heavy","Delicate","White","StainWash","EcoCotton","Woollen","BedSheets","Rinse+Dry","Dry Only","Wash Only","Aqua Store"};

//water levels
char *water_levels[]={"Auto","Low","Medium","High","Very high"};

//time check variables
int wash_time,rinse_time,spin_time,total_time,time;


//minutes and seconds
unsigned char min,sec;

static void init_config(void) {
    //Write your initialization code here
    init_clcd(); //CLCD module
    init_digital_keypad(); //Digital keypad
    init_timer2(); //Timer2
    
    FAN_DDR=0; //Output of fan
    
    PEIE =1;  //Peripheral Interrupt enable
    GIE = 1; //Global Interrupt Enable
    
    BUZZER_DDR=0; //Output(Buzzer)
    
    
   
}




//main program
void main(void) {
       init_config();
       unsigned char key;//Switch variable
       operation_mode=WASHING_PROGRAM_DISPLAY; 
        //Write application code here
       int flag=1; //Condition
       
       //starting screen
       clcd_print(" Press Key5 To",LINE1(0)); 
       clcd_print("   Power ON  ",LINE2(0));
       clcd_print("Washing  Machine",LINE3(0));
       
     
       while(read_digital_keypad(STATE)!=SW5)
        {
            ;
        }
        clear_scr();
        power_on(); //Powering screen
        reset_mode=WASHING_PROGRAM_DISPLAY_RESET; //Initialization of washing programs display screen
        while(1)
        {   key=read_digital_keypad(STATE); 
            if(key==LSW4 && operation_mode==WASHING_PROGRAM_DISPLAY)
            {
                operation_mode=WATER_LEVEL;
                reset_mode=WATER_LEVEL_RESET;
            }
        
            else if(key==LSW4 && operation_mode==WATER_LEVEL)
            {
                operation_mode=START_STOP;
                reset_mode=START_STOP_RESET;
            }
        
            if(key==SW5 && operation_mode==PAUSE)
            {
                operation_mode=START_PROGRAM;
                TMR2ON=1;
                FAN=1;
            }
            switch(operation_mode)
            {   case WASHING_PROGRAM_DISPLAY:
                    washing_program_display(key);
                    break;
                case WATER_LEVEL:
                    wash_level_display(key);
                    break;
                case START_STOP:
                    operation_mode=start_stop_display(key);
                    if(operation_mode==WASHING_PROGRAM_DISPLAY)
                        reset_mode=WASHING_PROGRAM_DISPLAY_RESET;
                    else if(operation_mode==START_PROGRAM)
                        flag=1;
                    break;
                case START_PROGRAM:
                    if(flag==1)
                    {
                        reset_mode=START_PROGRAM_RESET;
                    }
                    flag=0;
                    run_program(key);
                    break; 
          }
          reset_mode=RESET_NOTHING;  
        }

}



void clear_scr(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(100);
}



void power_on(void)
{   unsigned char i;
    for(i=0;i<16;i++)  //printing block characters
    { clcd_putch(BLOCK,LINE1(i));
    }
    clcd_print("  Powering On",LINE2(0));
    clcd_print("Washing Machine",LINE3(0));
    for(i=0;i<16;i++)
    { clcd_putch(BLOCK,LINE4(i));
    }
    __delay_ms(2000);  
   
    clear_scr();
    //imagination company
    clcd_print("     LOOP",LINE2(0));
    clcd_print("    Company ",LINE3(0));
   __delay_ms(2000);
}



//Starting and stopping  screen display
unsigned char start_stop_display(unsigned char key)
{ if(reset_mode==START_STOP_RESET) //initialization
  {  clear_scr();}
  set_time_for_program();
  clcd_print("Press Switch:",LINE1(0));
  clcd_print(" SW5:START",LINE2(0));
  clcd_print(" SW6:STOP",LINE3(0));
  if(key==SW6)
  {   clear_scr();
      return WASHING_PROGRAM_DISPLAY;
  }
  else if(key==SW5)
  {   
      return START_PROGRAM;
  }
  return START_STOP;
}






//Water levels display
void wash_level_display(unsigned char key)
{  if(reset_mode==WATER_LEVEL_RESET)
   { level_no=0;
     clear_scr();
     //clcd_print("Washing Programs",LINE1(0));
  }
  if(key==SW4)
  { clear_scr();
    if((level_no+1)>=5)
    {  level_no=0; }    //clcd_print("Washing Programs",LINE1(0));}  
    else
        level_no+=1;   
  }
  clcd_print("WATER LEVEL",LINE1(0));
  clcd_putch('>',LINE2(0));
  if(level_no<=2)
  { clcd_print(water_levels[level_no],LINE2(1));
    clcd_print(water_levels[level_no+1],LINE3(1));
    clcd_print(water_levels[level_no+2],LINE4(1));
  }
  else if(level_no==3)
  { clcd_print(water_levels[level_no],LINE2(1));
    clcd_print(water_levels[level_no+1],LINE3(1));
    clcd_print(water_levels[0],LINE4(1));
  }
  else  
  { clcd_print(water_levels[level_no],LINE2(1));
    clcd_print(water_levels[0],LINE3(1));
    clcd_print(water_levels[1],LINE4(1));
  }

    
}











//washing programs screen
void washing_program_display(unsigned char key)
{   if(reset_mode==WASHING_PROGRAM_DISPLAY_RESET)
    { program_no=0;
      clear_scr();
     //clcd_print("Washing Programs",LINE1(0));
    }
    if(key==SW4)
    { clear_scr();
      if((program_no+1)>=12)
      {  program_no=0; }    //clcd_print("Washing Programs",LINE1(0));}  
      else
        program_no+=1;   
    }
    clcd_print("Washing Programs",LINE1(0));
    clcd_putch('>',LINE2(0));
    if(program_no<=9)
    {   clcd_print(washing_prog[program_no],LINE2(1));
        clcd_print(washing_prog[program_no+1],LINE3(1));    
        clcd_print(washing_prog[program_no+2],LINE4(1));
    }
    else if(program_no==10)
    {
    //clcd_print("Washing Programs",LINE1(0));
        clcd_print(washing_prog[program_no],LINE2(1));
        clcd_print(washing_prog[program_no+1],LINE3(1));
        clcd_print(washing_prog[0],LINE4(1));
    }
    else 
    {  //clcd_print("Washing Programs",LINE1(0));
        clcd_print(washing_prog[program_no],LINE2(1));
        clcd_print(washing_prog[0],LINE3(1));
        clcd_print(washing_prog[1],LINE4(1));
    }

}








void set_time_for_program(void)
{
    switch(program_no)
    {
        case 0: //Daily program
            switch(level_no)
            {
                case 1:
                    sec=33;
                    min=0;
                    break;
                case 0:   
                case 2:
                    sec=41;
                    min=0;
                    break;
                case 3:
                case 4:
                    sec=45;
                    min=0;
            }
          break;
          
        case 1://Heavy
            switch(level_no)
            {
                case 1:
                    sec=43;
                    min=0;
                    break;
                case 0:   
                case 2:
                    sec=50;
                    min=0;
                    break;
                case 3:
//                    sec=57
                case 4:
                    sec=57;
                    min=0;
            }
            break;
        case 2://Delicates
            switch(level_no)
            {
                case 1:
                case 0:   
                case 2:
                    sec=26;
                    min=0;
                    break;
                case 3:
//                    sec=57
                case 4:
                    sec=31;
                    min=0;
            }
            break;
        case 3://whites
            sec=16;
            min=1;
            break;
        case 4: //Stainwash
            sec=36;
            min=1;
            break;
        case 5: //Eco cottons
            sec=28;
            min=0;
            break;
        case 6: //Woolens
            sec=29;
            break;
        case 7://Bed sheets
             switch(level_no)
            {
                case 1: 
                    sec=46;
                    min=0;
                    break;
                case 0:   
                case 2:
                    sec=53;
                    min=0;
                    break;
                case 3:
//                    sec=57
                case 4:
                    sec=60;
                    min=0;
            }
            break;
        case 8://Rinse +Dry
            switch(level_no)
            {
                case 1: 
                    sec=18;
                    min=0;
                    break;
                case 0:   
                case 2:
                case 3:
//                    sec=57
                case 4:
                    sec=20;
                    min=0;
            }
            break;
        case 9: //Dry only
            sec=6;
            min=0;
            break;
            
            break;
        case 10:  //wash only
        case 11:  //Aqua store
           switch(level_no)
            {
                case 1: 
                    sec=16;
                    min=0;
                    break;
                case 0:   
                case 2:
                    sec=21;
                    min=0;
                    break;
                case 3:
//                    sec=57
                case 4:
                    sec=26;
                    min=0;
            }
            break;
        
            
    }
} 

//door status check
void door_open(void)
{
    if(RB0==0)
    {   TMR2ON=0;
        BUZZER=1;
        FAN=0;
        clear_scr();
        clcd_print(" DOOR : OPEN",LINE1(0));
        clcd_print(" Please close",LINE3(0));
        while(RB0==0)
        {
            ;
        }
        FAN=1;
        TMR2ON=1;
        BUZZER=0;
        clear_scr();
         clcd_print(" Function: ",LINE1(0));
         clcd_print(" Time:  ",LINE2(0));
         clcd_print("5-START  6-PAUSE",LINE4(0));
        
     }
}


//Program running
void run_program(unsigned char key)
{
    
    if(reset_mode==START_PROGRAM_RESET)
    {   clear_scr();
        clcd_print("Prog:",LINE1(0));
        clcd_print(washing_prog[program_no],LINE1(6));
        clcd_print("Time:",LINE2(0));
        
        //min
        
        clcd_putch((min/10)+'0',LINE2(6));
        clcd_putch((min%10)+'0',LINE2(7));
        clcd_putch(':',LINE2(8));
        clcd_putch((sec/10) +'0',LINE2(9));
        clcd_putch((sec%10)+'0',LINE2(10)); 
        
        
        clcd_print("(MM:SS)",LINE3(5));
        
        __delay_ms(3000);
        
        
         clear_scr();
        //display screen
         clcd_print(" Function - ",LINE1(0));
         clcd_print(" TIME:  ",LINE2(0));
         clcd_print("5-START  6-PAUSE",LINE4(0));
    
        
          
          time=total_time=(min*60+sec);
          
          wash_time=(int)total_time*0.46;
          rinse_time=(int)total_time*0.12;
          spin_time=(int)(total_time-wash_time-rinse_time);
         
           //Start the timer
          TMR2ON=1;
          //starting the fan
          FAN = 1;
          
           
    }
    door_open();
    if(key==SW6)
    {
        TMR2ON=0;
        FAN=0;
        operation_mode=PAUSE;
    }
    else{
     
     total_time=(min*60+sec);
     if(program_no<=7)
     {  if(total_time>=(time-wash_time))
        {
            clcd_print("Wash",LINE1(10));
        }
        else if(total_time>=(time-wash_time-rinse_time))
        {
            clcd_print("Rinse",LINE1(10));
        }
        else
             clcd_print("Spin ",LINE1(10));
     }
     else if(program_no==8)//
     {
         if(total_time>=(time-(0.40*time)))
             clcd_print("Rinse",LINE1(10));
         else 
             clcd_print("Spin ",LINE1(10));
     }
     else if(program_no==9)
     {
         clcd_print("Rinse ",LINE1(10));
     }
     else if(program_no==10)
     {
         clcd_print("Wash",LINE1(10));
         
     }
    //Display screen
     clcd_putch((min/10)+'0',LINE2(6));
     clcd_putch((min%10)+'0',LINE2(7));
     clcd_putch(':',LINE2(8));
     clcd_putch((sec/10) +'0',LINE2(9));
     clcd_putch((sec%10)+'0',LINE2(10));
   
    
     if(sec==0 && min ==0)
     {
         FAN=0;
         //stop the fan
         TMR2ON=0;
         //stop the timer
         BUZZER = 1;
         clear_scr();
         clcd_print("  Completed  ",LINE1(0));
         clcd_print(" Remove cloths",LINE2(0));
         __delay_ms(5000);
         BUZZER = 0;
         operation_mode=WASHING_PROGRAM_DISPLAY; 
         reset_mode=WASHING_PROGRAM_DISPLAY_RESET;
         clear_scr();
         
     }
    }
}