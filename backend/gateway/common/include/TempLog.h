#include <stdlib.h>
#ifndef DEBUG_TEMP_LOG_H
#define DEBUG_TEMP_LOG_H

#include <stdio.h>
#include <string>
#include <time.h>
#include <stdarg.h>

class DEBUG_TEMP_LOG
{
public:
	
	DEBUG_TEMP_LOG(char *logfile):fp_m(NULL), logfile_m(NULL)
	{
		if(logfile)
		{
			fp_m = fopen(logfile, "a");
		}

		logfile_m = logfile;
	}

	~DEBUG_TEMP_LOG()
	{
		if(fp_m) fclose(fp_m);
	}

	void log(const char *fmt, ...)
	{		
		if((!fp_m) && logfile_m)
		{
			fp_m = fopen(logfile_m, "a");
		}

		if(fp_m)
		{
			char formatted[2048] = {0};
			{
				va_list va;
				va_start(va, fmt);
				vsnprintf(formatted, sizeof(formatted), fmt, va);
				va_end(va);
			}
		
			char buffer [64] = {0};
			{
				time_t rawtime;
				struct tm *timeinfo;
				time (&rawtime);
				timeinfo = localtime (&rawtime);

				strftime (buffer, sizeof(buffer), "%c", timeinfo);
			}
		
			fprintf(fp_m, "[%s] %s\n", buffer, formatted);
		}
	}

private:
	FILE *fp_m;
	char *logfile_m;
};

#endif
