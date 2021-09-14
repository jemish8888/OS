#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "loglib.h"

static int T = 0;
char * log_filename = NULL;
char * err_string = NULL;

int main(const int argc, char * const argv[]){

	log_filename = strdup("messages.log");
	err_string = malloc(sizeof(char)*100);
	if(err_string == NULL){
		perror("malloc");
		return EXIT_FAILURE;
	}
	snprintf(err_string, 100, "%s: Error:", argv[0]);

	int opt;
	while((opt=getopt(argc, argv, "ht:")) != -1){
		switch(opt){
			case 'h':
				printf("driver [-h] [-t sec] [logfile]\n");
				return 0;

			case 't':
				T = atoi(optarg);
				break;

			default:
				printf("driver [-h] [-t sec] [logfile]\n");
				break;
		}
	}

	if(optind == (argc-1)){
		free(log_filename);
		log_filename = strdup(argv[argc-1]);
	}

	char buf[20];
	sprintf(buf, "T=%i", T);

	addmsg('I', buf);
	if(T != 0){	sleep(rand() % (2*T)); }

	addmsg('I', "Nobody here");
	if(T != 0){	sleep(rand() % (2*T)); }

	addmsg('W', "Sombody here");
	if(T != 0){	sleep(rand() % (2*T)); }

	addmsg('E', "Too many people here");
	if(T != 0){	sleep(rand() % (2*T)); }

	addmsg('F', "Fatality...");
	if(T != 0){	sleep(rand() % (2*T)); }


	//show log
	char * log = getlog();
	puts(log);
	free(log);

	savelog(log_filename);
	clearlog();

	addmsg('E', "Error 1");
	if(T != 0){	sleep(rand() % (2*T)); }


	//show log
	log = getlog();
	puts(log);
	free(log);

	savelog(log_filename);
	clearlog();

	free(err_string);
	free(log_filename);

	return 0;
}
