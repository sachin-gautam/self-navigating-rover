#include <reg51.h>
#include<math.h>

#define sound_velocity 34300      /* sound velocity in cm per second */
#define period_in_us pow(10,-6)
#define Clock_period 1.085*period_in_us        /* period for clock cycle of 8051*/

sbit Trigger_pin=P1^7;            /* Trigger pin */
sbit Echo_pin=P1^5;             /* Echo pin */

sbit in1 = P1^0;
sbit in2 =P1^1;
sbit in3 =P1^2;
sbit in4 =P1^3;



void Delay_us()
{
    TL0=0xF5;
    TH0=0xFF;
    TR0=1;
    while (TF0==0);
    TR0=0;
    TF0=0;
}


void send_trigger_pulse()
{
    Trigger_pin= 1;               /* pull trigger pin HIGH */
    Delay_us();                   /* provide 10uS Delay*/
    Trigger_pin = 0;              /* pull trigger pin LOW*/
}

void stop(){
    in1 =0;
    in2=0;
    in3 =0;
    in4=0;
}


void forward(){
    in1 =0;
    in2 =1;
    in3=0;
    in4=1;    
}

void backward(){
    in1 =1;
    in2=0;
    in3 =1;
    in4=0;

}

void left(){
    in1 =0;
    in2=1;
    in3 =1;
    in4=0;

}

void right(){

    in1 =1;
    in2=0;
    in3 =0;
    in4=1;

}
void init_timer()
{
    TMOD=0x21;                                        /*initialize Timer*/
    TF0=0;
    TR0 = 0;
}
/*void check_distance()
{
    if(distance_measurement>20.0)
        stop();
}*/

void main(void){
    
        float distance_measurement, value;
    unsigned char input;
      init_timer();
    //TMOD=0x21;
    TH1=0xFD;
    SCON=0x50;
    TR1=1;
    RI = 0;
    TI = 0;
    while(1){
            
                send_trigger_pulse();            /* send trigger pulse of 10us */
    
                while(!Echo_pin);                   /* Waiting for Echo */
                TR0 = 1;                            /* Timer Starts */
            while(Echo_pin && !TF0);            /* Waiting for Echo goes LOW */
            TR0 = 0;                            /* Stop the timer */
      
                value = Clock_period * sound_velocity; 
                distance_measurement = (TL0|(TH0<<8));    
                distance_measurement = (distance_measurement*value)/2.0;  /* find distance(in cm) */            
                Delay_us();
            
              //if (distance_measurement>10.0)
                //{
                    while (RI == 0);
                    RI = 0;
                    input = SBUF;
                    if(input == 'F' && distance_measurement>10.0){
                        forward();
                    }
                    else if(input == 'B'){
                        backward();
                    }
                    else if(input == 'R'&& distance_measurement>10.0){
           left();
                    }
                    else if(input == 'L'&& distance_measurement>10.0){
           right();
                    }
                    else{
                        stop();
                    }
                
                
              }
        
        
    }