#include <time.h>

typedef struct data_struct {
	time_t	time;		//Time stamp
	char		type;		//Message Type (I/W/E/F)
	char * string;	//Message String
} data_t;

int addmsg( const char type, const char * msg );
void clearlog();
char * getlog();
int savelog( char * filename );

int make_msg(const char * prog, const char * str, data_t *data);

extern char * log_filename;
extern char * err_string;
