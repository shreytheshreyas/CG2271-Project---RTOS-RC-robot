#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
 
#define PTB0_Pin	0         
#define START_UP_MUSIC_NOTES 10
#define FREQ_2_MOD(x)	(375000 / x)

int mario_musical_notes[] = {
	2637, 2637, 0, 2637,
  0, 2093, 2637, 0,
  3136, 0, 0,  0,
  1568, 0, 0, 0,
 
  2093, 0, 0, 1568,
  0, 0, 1319, 0,
  0, 1319, 0, 1976,
  0, 1865, 1319, 0,
 
  1568, 2637, 3136,
  3520, 0, 2794, 3136,
  0, 2637, 0, 2093,
  2349, 1976, 0, 0,
 
  2093, 0, 0, 1568,
  0, 0, 1319, 0,
  0, 1319, 0, 1976,
  0, 1865, 1319, 0,
 
  1568, 2637, 3136,
  3520, 0, 2794, 3136,
  0, 2637, 0, 2093,
  2349, 1976, 0, 0
};

int start_up_music_notes [] = {
	0, 200, 400, 600, 800,
	1000, 1200, 1400, 1600,0
};

int caribbean_music_notes[] = {
   330, 392, 440, 440, 0, 
   440, 494, 523, 523, 0, 
   523, 587, 494, 494, 0,
   440, 392, 440, 0,
   
   330, 392, 440, 440, 0, 
   440, 494, 523, 523, 0, 
   523, 587, 494, 494, 0,
   440, 392, 440, 0,
   
   330, 392, 440, 440, 0, 
   440, 523, 587, 587, 0, 
   587, 659, 698, 698, 0,
   659, 587, 659, 440, 0,
   
   440, 494, 523, 523, 0, 
   587, 659, 440, 0, 
   440, 523, 494, 494, 0,
   523, 440, 494, 0,

   440, 440, 
   //Repeat of first part
   440, 494, 523, 523, 0, 
   523, 587, 494, 494, 0,
   440, 392, 440, 0,

   330, 392, 440, 440, 0, 
   440, 494, 523, 523, 0, 
   523, 587, 494, 494, 0,
   440, 392, 440, 0,
   
   330, 392, 440, 440, 0, 
   440, 523, 587, 587, 0, 
   587, 659, 698, 698, 0,
   659, 587, 659, 440, 0,
   
   440, 494, 523, 523, 0, 
   587, 659, 440, 0, 
   440, 523, 494, 494, 0,
   523, 440, 494, 0,
   //End of Repeat

   659, 0, 0, 698, 0, 0,
   659, 659, 0, 784, 0, 659, 587, 0, 0,
   587, 0, 0, 523, 0, 0,
   494, 523, 0, 494, 0, 440,

   659, 0, 0, 698, 0, 0,
   659, 659, 0, 784, 0, 659, 587, 0, 0,
   587, 0, 0, 523, 0, 0,
   494, 523, 0, 494, 0, 440
};
int caribbean_note_duration [] = {
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125, 
  250, 125, 250, 125, 
  125, 125, 250, 125, 125,
  125, 125, 375, 375,

  250, 125,
  //Rpeat of First Part
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125, 
  250, 125, 250, 125, 
  125, 125, 250, 125, 125,
  125, 125, 375, 375,
  //End of Repeat
  
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500,

  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500
};

void initMusicPWM(void) {
	//Enable Clock Gating for PORTB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	//Configure Mode 3 for the PWM pin operation 
	PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);

	// Enable clock gating for Timer 1
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	//Select clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	TPM1->MOD = 7500;
	TPM1_C0V = 0;
	
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK)|(TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1)| TPM_CnSC_MSB(1));
}

void playStartUpMusic() {		
	for(int i = 0; i < START_UP_MUSIC_NOTES; i++) {	
		TPM1->MOD	= FREQ_2_MOD(start_up_music_notes[i]); 
		TPM1_C0V = (FREQ_2_MOD(start_up_music_notes[i])) / 6;
		osDelay(150);
	}	
}
void playPiratesOfTheCaribbeanMusic(int i) {
	TPM1->MOD = FREQ_2_MOD(caribbean_music_notes[i] / 3);
	TPM1_C0V = (FREQ_2_MOD(caribbean_music_notes[i])) / 6;
	osDelay(caribbean_note_duration[i]);
}

void playMarioMusic(int i) {
	TPM1->MOD = FREQ_2_MOD(mario_musical_notes[i]);
	TPM1_C0V = (FREQ_2_MOD(mario_musical_notes[i])) / 6;
	if(i == 33 || i == 34 || i == 35 || i == 63 || i == 64 || i ==65) {
		osDelay(180);
	} else {
		osDelay(240);
	}
}
