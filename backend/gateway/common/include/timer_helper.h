#ifndef TIMER_HELPER_H
#define TIMER_HELPER_H

class timer_helper
{
public:

		timer_helper(unsigned long long *time_diff) : diff(time_diff)
		{
			struct timeval start;
			gettimeofday(&start, NULL);
			start_utc = (unsigned long long) start.tv_sec *  1000000 + start.tv_usec;
		}

		~timer_helper()
		{
			struct timeval end;
			gettimeofday(&end, NULL);
			end_utc = (unsigned long long) end.tv_sec * 1000000 + end.tv_usec;
			*diff = end_utc - start_utc;
		}

private:
		unsigned long long start_utc, end_utc;
		unsigned long long *diff;
};

#endif