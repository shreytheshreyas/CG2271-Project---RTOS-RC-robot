//Include guard
#ifndef MUSIC_H   
#define MUSIC_H

#define MARIO_NOTE_COUNT	78
#define PIRATES_OF_THE_CARIBBEAN_NOTE_COUNT 203

void initMusicPWM(void);
void playStartUpMusic(void);
void playPiratesOfTheCaribbeanMusic(int);
void playMarioMusic(int);

#endif
