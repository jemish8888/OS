#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loglib.h"

typedef struct list_struct{
	data_t item;
	struct list_struct *next;
} log_t;

static log_t * headptr = NULL;
static log_t * tailptr = NULL;


int addmsg(const char type, const char * msg){

	int fail_exit = 0;

	log_t * node = malloc(sizeof(log_t));
	if(node == NULL){
		perror(err_string);
		return -1;
	}

	//check message type
	switch(type){
		case 'I': case 'W': case 'E':
			break;
		case 'F':
			fail_exit = 1;
			break;
		default:	//invalid type
			return -1;
	}

	node->item.type = type;
	node->item.time	= time(NULL);

	node->item.string	= strdup(msg);
	if(node->item.string == NULL){
		perror(err_string);
		return -1;
	}
	node->next = NULL;

	if(headptr == NULL){	//if log is empty
		headptr = tailptr = node;
	}else{
		tailptr->next = node;
		tailptr = node;
	}

	if(fail_exit){
		savelog(log_filename);
		clearlog();
		exit(EXIT_FAILURE);
	}

	return 0;
}

void clearlog(void){
	log_t * iter = headptr;
	while(iter){

		log_t * next = iter->next;

		//free the log node
		free(iter->item.string);
		free(iter);

		iter = next;
	}
	headptr = tailptr = NULL;
}

char * getlog(void){
	int size = 1000;
	char buf[50];

	log_t * iter = headptr;

	int offset = 0;
	char * log_str = malloc(sizeof(char) * size);
	if(log_str == NULL){
		perror(err_string);
		return NULL;
	}

	while(iter){

		//convert time to string
		struct tm *tmp = localtime(&iter->item.time);
		if (tmp == NULL) {
		  break;
		}
		strftime(buf, sizeof(buf), "%Y%m%d %T", tmp);

		int len = 0;

		switch(iter->item.type){
			case 'I':	len = snprintf(NULL, 0, "[%s] INFO: %s\n",  buf, iter->item.string); break;
			case 'W': len = snprintf(NULL, 0, "[%s] WARN: %s\n",  buf, iter->item.string); break;
			case 'E': len = snprintf(NULL, 0, "[%s] ERROR: %s\n", buf, iter->item.string); break;
			case 'F':	len = snprintf(NULL, 0, "[%s] FAIL: %s\n",  buf, iter->item.string); break;
			default:	//invalid type
				free(log_str);
				return NULL;
		}

		if((offset + len) >= size){
			size += 100;
			log_str = (char*) realloc(log_str, size);
		}

		snprintf(&log_str[offset], size - offset, "[%s] INFO: %s\n",  buf, iter->item.string);

		offset += len + 1;
		log_str[offset] = '\n';

		iter = iter->next;
	}
	log_str[offset] = '\0';

	return log_str;
}

int savelog(char * filename){
	char buf[100];

	int rv = 0;

	FILE * fp = fopen(filename, "w");
	if(fp == NULL){
		perror(err_string);
		return -1;
	}

	log_t * iter = headptr;
	while(iter){

		struct tm *tmp = localtime(&iter->item.time);
		if (tmp == NULL) {
			rv = -1;


			perror(err_string);
		  break;
		}

		//convert time to string
		strftime(buf, sizeof(buf), "%Y%m%d %T", tmp);

		switch(iter->item.type){
			case 'I':	fprintf(fp, "[%s] INFO: %s\n",  buf, iter->item.string); break;
			case 'W': fprintf(fp, "[%s] WARN: %s\n",  buf, iter->item.string); break;
			case 'E': fprintf(fp, "[%s] ERROR: %s\n", buf, iter->item.string); break;
			case 'F':	fprintf(fp, "[%s] FAIL: %s\n",  buf, iter->item.string); break;
			default:	//invalid type
				rv = -1;
				break;
		}

		iter = iter->next;
	}
	fclose(fp);

	return rv;
}
