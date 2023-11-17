#include <stdlib.h>
#include "keypad.h"
#include "LCD.h"

#define KEY_LENGTH 16
#define INPUT_DELAY 6000
#define TRANSITION_DELAY 250000


void otp_encrypt(unsigned char *plaintext, unsigned char *key, unsigned char *ciphertext, unsigned int length) {
		int i;
    for (i = 0; i < length; i++) {
        ciphertext[i] = ((plaintext[i] - '0') + (key[i] - '0')) % 10 + '0';
    }
}

void otp_decrypt(unsigned char *plaintext, unsigned char *key, unsigned char *ciphertext, unsigned int length) {
		int i;
    for (i = 0; i < length; i++) {
        int temp = ((ciphertext[i] - '0') - (key[i] - '0'));
        if (temp < 0) temp += 10;
        plaintext[i] = (unsigned char) temp + '0';
    }
}

void generate_key(unsigned char *key) {
		int i;
    for (i = 0; i < KEY_LENGTH; i++) {
        key[i] = (unsigned char) (rand() % 10) + '0';
    }
}

void stall_for_input() {
	unsigned char temp;
	while (1) {
		temp = GetKey();
		if (temp == '#') break;
	}
	LCD_clear();
}

int main()
{
	unsigned char temp;
	unsigned char plaintext[KEY_LENGTH], key[KEY_LENGTH], ciphertext[KEY_LENGTH];
	int idx = 0;
	
	kbdInit();
	LCD_init();		
	LCD_display(1,1,"OTP Encryption");
	
	while(1)
	{
		temp = GetKey();
		if (temp == '#') break;
		delay(INPUT_DELAY);
		if(temp != 0 && idx < KEY_LENGTH)
		{
			plaintext[idx++] = temp;
			LCD_display(2, 1, plaintext);
		}
	}
	LCD_clear();
	
	while (1) {
		generate_key(key);
		otp_encrypt(plaintext, key, ciphertext, idx);
		otp_decrypt(plaintext, key, ciphertext, idx);
		
		LCD_clear();
		LCD_display(1, 1, "Generating Key..");
		delay(TRANSITION_DELAY);
		
		
		LCD_clear();
		LCD_display(1, 1, "Generated Key");
		LCD_display(2, 1, key);
		
		stall_for_input();
		
		LCD_clear();
		LCD_display(1, 1, "Encrypting...");
		delay(TRANSITION_DELAY);
		
		LCD_display(1, 1, "Encrypted Text");
		LCD_display(2, 1, ciphertext);
		
		stall_for_input();
		
		LCD_clear();
		LCD_display(1, 1, "Decrypting...");
		delay(TRANSITION_DELAY);
		
		LCD_clear();
		LCD_display(1, 1, "Plain Text");
		LCD_display(2, 1, plaintext);
		
		stall_for_input();
	}
}
