#ifndef GM_PINPOLY_H_H_H
#define GM_PINPOLY_H_H_H


enum MAPTYPE
{
    MAPTYPE_GOOGLE,
    MAPTYPE_BAIDU,
    MAPTYPE_GAODE,
    MAPTYPE_ANY = 99999
};


int pnpoly(int nvert, double *vertx, double *verty, double testx, double testy);

bool needTrans(double lng, double lat, MAPTYPE maptype);

#endif
