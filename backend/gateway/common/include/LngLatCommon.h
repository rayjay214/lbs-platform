// 经纬度相关常用函数
#ifndef __H_LNG_LAT_COMMON_
#define __H_LNG_LAT_COMMON_

#ifndef PI_DOUBLE
#define PI_DOUBLE 3.14159265358979323846
#endif

#ifndef EARTH_RADIUS
#define EARTH_RADIUS 6378.137   // 地球半径(KM)
#endif

// 精确计算地球表面A、B两点之间的距离
// Input:   lat1        点A的纬度
// Input:   lng1        点A的经度
// Input:   lat2        点B的纬度
// Input:   lng2        点B的经度
// Return:  A、B两点之间的球面距离（单位：米）
double CalcDistancePrecise(double lat1, double lng1, double lat2, double lng2);

// 粗略计算地球表面A、B两点之间的距离（速度较CalcDistancePrecise快）
// Input:   lat1        点A的纬度
// Input:   lng1        点A的经度
// Input:   lat2        点B的纬度
// Input:   lng2        点B的经度
// Return:  A、B两点之间粗略的球面距离（单位：米）
double CalcDistanceRough(double lat1, double lng1, double lat2, double lng2);


// 角度转为弧度
// Input:   deg     角度值
// Return:  角度对应的弧度
inline double Deg2Rad(double deg)
{
    return (deg * PI_DOUBLE / 180);
}

// 弧度转为角度
// Input:   rad     弧度值
// Return:  弧度对应的角度
inline double Rad2Deg(double rad)
{
    return (rad * 180 / PI_DOUBLE);
}

#endif  // __H_LNG_LAT_COMMON_
