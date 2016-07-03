#include <commands.h>
#include <stdio.h>
#include <console.h>
#include <songs.h>
#include <string.h>


static uint32_t notes[26] = {11492, 10847, 9121, 8609, 8126, 7670, 7239, 6833, 6449, 6087, 5743, 5423, 5119, 4831, 4560, 4304, 4063, 3834, 3619, 3416, 3224, 3043, 2873, 2711, 2559, 2416};


COMMAND_FUNCTION(songs);
COMMAND_HELP(songs);

COMMAND_FUNCTION(songs) {
	if(argc == 0 || argc >= 2){
		printf("Please enter a valid song number\n$>");
		return ERROR;
		
	}
	
	if(strcmp(argv[0], "1") == 0){
		play_song(RICK);
	}else if(strcmp(argv[0], "2") == 0){
		play_song(BECAUSE);
	}
	return OK;
}

COMMAND_HELP(songs) {
	if(argc >= 1) {
		printf("<ps> Error: too many arguments.\n");
		return ERROR;
	}

	printf("Put the song number you would like to play.\n");
	printf("\t1. Never Gonna Give You Up - Rick Astley\n");
	printf("\t2. Because - The Beatles\n");
	printf("Usage: 'songs X' being X the song number\n");

	return OK;
}

void play_song(char* songsDirections){
	int i=0, j=0;
	while(j<10){
		uint32_t tempo = 200;
		while(songsDirections[i] != '0'){
			uint32_t notePos = songsDirections[i] - 'A';
			uint32_t note = notes[notePos];
			uint8_t len = songsDirections[i+1] - '0';
			uint32_t aux = tempo * len;
			sys_play_note(note, aux);
			i+=2;
		} 
		j++;
	}
}