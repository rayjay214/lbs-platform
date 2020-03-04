/********************************************************************
 * Copyright (C), 2009-2012, Goome Tech. Co., Ltd.
 * File name:   my_time.h
 * Author:      Cen Jie
 * Version:     1.0
 * Date:        2012-11-23
 * Description: �Լ�д��ʱ�䴦���һЩ���ú�����
 * Function List:  ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
*********************************************************************/

#ifndef _MY_TIME_H_
#define _MY_TIME_H_

namespace MYTIME
{

/* 	convert GSM time format to UTC time format
 * 	e.g. 2012-11-23 18:08:33  ==>  1353665313
 */
unsigned long mktime (unsigned int year, unsigned int mon, unsigned int day, 
					  unsigned int hour, unsigned int min, unsigned int sec)
{
	/* 1..12 -> 11,12,1..10 */
    if (0 >= (int) (mon -= 2)) 
    {    
         mon += 12;      /* Puts Feb last since it has leap day */
         year -= 1;
    }

    unsigned long utcTime = ((((unsigned long)
              (year/4 - year/100 + year/400 + 367*mon/12 + day) + year*365 - 719499
          )*24 + hour /* now have hours */
       )*60 + min /* now have minutes */
    )*60 + sec; /* finally seconds */

	/* time zone is 8 hour early, subtract it here */
   	return utcTime - 8 * 60 * 60;
}

}	// namespace MYTIME


#endif	// _MY_TIME_H_

