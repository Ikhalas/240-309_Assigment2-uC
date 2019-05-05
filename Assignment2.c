#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#define num_led 12
#define on 1
#define SLEEP_POWER_DOWN 2
#define F_CPU 16000000UL

signed int i=0;
unsigned int j,inc,dec;

void SLEEP_DISABLE (void){
	SMCR &= 0xFE;
}

void SLEEP_INITIALIZE (uint8_t m){
	SMCR = (m << 1)|0x01;
}

void display_each_led(int led_num, char status_on){
	DDRB &= 0b11110000;
	switch(led_num){
		case 0:  DDRB |= 0b00011; if(status_on) PORTB=0b0001; else PORTB=0b0000; break;
		case 1:  DDRB |= 0b00011; if(status_on) PORTB=0b0010; else PORTB=0b0000; break;
		case 2:  DDRB |= 0b00110; if(status_on) PORTB=0b0010; else PORTB=0b0000; break;
		case 3:  DDRB |= 0b00110; if(status_on) PORTB=0b0100; else PORTB=0b0000; break;
		case 4:  DDRB |= 0b01100; if(status_on) PORTB=0b0100; else PORTB=0b0000; break;
		case 5:  DDRB |= 0b01100; if(status_on) PORTB=0b1000; else PORTB=0b0000; break;
		case 6:  DDRB |= 0b00101; if(status_on) PORTB=0b0001; else PORTB=0b0000; break;
		case 7:  DDRB |= 0b00101; if(status_on) PORTB=0b0100; else PORTB=0b0000; break;
		case 8:  DDRB |= 0b01010; if(status_on) PORTB=0b0010; else PORTB=0b0000; break;
		case 9:  DDRB |= 0b01010; if(status_on) PORTB=0b1000; else PORTB=0b0000; break;
		case 10: DDRB |= 0b01001; if(status_on) PORTB=0b0001; else PORTB=0b0000; break;
		case 11: DDRB |= 0b01001; if(status_on) PORTB=0b1000; else PORTB=0b0000; break;
		
	}
}

ISR (INT1_vect){
	uint8_t tmp;
	uint8_t state=0;
	_delay_ms(15);
	tmp = PIND>>3;
	tmp &= 0x01;
	if(tmp==0){
		for(j=0;j<30;j++){
			tmp = PIND>>3;
			tmp &= 0x01;
			_delay_ms(1000);
		
			if(tmp==0){
				state++;
				if(inc==1){
					inc=0;
					dec=0;
					i--;
					i--;
				}
				if(dec==1){
					inc=0;
					dec=0;
				}
										 			
			}	
			else{
				state=0;	
				i++;
				if(i>=num_led) 
					i=0;
				break;
			}
				  
		}
		if(state>=30)
			inc=1;
	 		
		state=0;
	}

	EIFR = (1<<INTF1);
}

ISR(INT0_vect){
	uint8_t tmp;
	uint8_t state=0;
	_delay_ms(15);
	tmp = PIND>>2;
	tmp &= 0x01;
	if(tmp==0){
		for(j=0;j<30;j++){
			tmp = PIND>>2;
			tmp &= 0x01;
			_delay_ms(1000);
		
			if(tmp==0){
				state++;
					if(inc==1){
					inc=0;
					dec=0;
				}
				if(dec==1){
					inc=0;
					dec=0;
					i++;
					i++;
		
				}						
			}
			
			else{
				state=0;	
				i--;
				if(i<0) 
					i=11;
				break;
			}
			
				  
		}
		if(state>=30)
			dec=1;
			
		state=0;	
	}
	
	EIFR = (1<<INTF0);
}

int main(void)
{
	DDRD  = 0xF3;
	EICRA = 0b1010;
	EIMSK = 0b0011;
	display_each_led(i,on);
	sei();
    while(1){
			display_each_led(i,on);
			SLEEP_INITIALIZE(SLEEP_POWER_DOWN);
			sleep_cpu();
			SLEEP_DISABLE();
			if(inc==1){
				while(inc){
					display_each_led(i,on);
					 i++;				
					_delay_ms(20000);			
					if(i>=num_led)
						 i=0;		
				}
			}
			if(dec==1){
				while(dec){
					display_each_led(i,on);
					 i--;				
					_delay_ms(20000);
					if(i<0) 
						i=11;	
				}
			}
	}		
}