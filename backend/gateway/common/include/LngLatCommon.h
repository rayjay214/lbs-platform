// ��γ����س��ú���
#ifndef __H_LNG_LAT_COMMON_
#define __H_LNG_LAT_COMMON_

#ifndef PI_DOUBLE
#define PI_DOUBLE 3.14159265358979323846
#endif

#ifndef EARTH_RADIUS
#define EARTH_RADIUS 6378.137   // ����뾶(KM)
#endif

// ��ȷ����������A��B����֮��ľ���
// Input:   lat1        ��A��γ��
// Input:   lng1        ��A�ľ���
// Input:   lat2        ��B��γ��
// Input:   lng2        ��B�ľ���
// Return:  A��B����֮���������루��λ���ף�
double CalcDistancePrecise(double lat1, double lng1, double lat2, double lng2);

// ���Լ���������A��B����֮��ľ��루�ٶȽ�CalcDistancePrecise�죩
// Input:   lat1        ��A��γ��
// Input:   lng1        ��A�ľ���
// Input:   lat2        ��B��γ��
// Input:   lng2        ��B�ľ���
// Return:  A��B����֮����Ե�������루��λ���ף�
double CalcDistanceRough(double lat1, double lng1, double lat2, double lng2);


// �Ƕ�תΪ����
// Input:   deg     �Ƕ�ֵ
// Return:  �Ƕȶ�Ӧ�Ļ���
inline double Deg2Rad(double deg)
{
    return (deg * PI_DOUBLE / 180);
}

// ����תΪ�Ƕ�
// Input:   rad     ����ֵ
// Return:  ���ȶ�Ӧ�ĽǶ�
inline double Rad2Deg(double rad)
{
    return (rad * 180 / PI_DOUBLE);
}

#endif  // __H_LNG_LAT_COMMON_
