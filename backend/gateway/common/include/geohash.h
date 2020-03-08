/*
 *  geohash.h
 *  libgeohash
 *
 *  Created by Derek Smith on 10/6/09.
 *  Copyright (c) 2010, SimpleGeo
 *	All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met:

 *  Redistributions of source code must retain the above copyright notice, this list
 *  of conditions and the following disclaimer. Redistributions in binary form must 
 *  reproduce the above copyright notice, this list of conditions and the following 
 *  disclaimer in the documentation and/or other materials provided with the distribution.
 *  Neither the name of the SimpleGeo nor the names of its contributors may be used
 *  to endorse or promote products derived from this software without specific prior 
 *  written permission. 
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
 *  THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __H_GEOHASH
#define __H_GEOHASH


// ���򣬻�ȡ�ھӷ���ʱ��
#define GEOHASH_NORTH               0
#define GEOHASH_EAST                1
#define GEOHASH_SOUTH               2
#define GEOHASH_WEST                3

// ����ĳ�/���ף�
typedef struct GeoBoxDimensionStruct {
	
	double height;
	double width;

} GeoBoxDimension;

typedef struct GeoCoordStruct {
    
    double longitude;
    double latitude;
    
    double north;
    double east;
    double south;
    double west;

	GeoBoxDimension dimension;
    
    GeoCoordStruct()
    {
    }

    GeoCoordStruct(double lon, double lat):longitude(lon),latitude(lat)
    {
    }

    GeoCoordStruct(double lon, double lat, double no, double ea, double so, double we)
        :longitude(lon), latitude(lat), north(no), east(ea), south(so), west(we)
    {
    }
} GeoCoord;

// Geohash���뺯�������Ż��İ汾�������7%���ҵ��ٶ� by Ouyibao
// ����γ�ȱ����Geohashֵ
// Input:   buf_length  �������������
// Input:   lat         γ��
// Input:   lng         ����
// Input:   precision   Geohash���뾫�ȡ���1��12֮��
// Output:  buf         �����������ž�γ�ȱ�����hashֵ
// Return:  ������geohashֵ
extern char* geohash_encode(char * buf, int buf_length, double lat, double lng, int precision);

// ��Geohashֵ����Ϊ��Ӧ�ķ������꣨��γ�ȣ�
// Input:   hash    ĳ�����geohashֵ
// Return:  ��hashֵ��Ӧ�ķ���γ������
extern GeoCoord geohash_decode(const char* hash);

// �޸ĺ�Ļ�ȡ�ھӷ����������ڴ�й¶���� by Ouyibao
// Input:   hash        �÷����geohashֵ
// Input:   direction   ��Ҫ��ȡ�÷����ĸ�������ھ�
// Input:   buf_length  pBuf���������ȣ���ӦС��hash�ַ����ĳ���
// Output:   buf         �����ھӷ���geohashֵ
// Return:  ��ȡ�ھ��Ƿ�ɹ��� true���ɹ���false��ʧ��
extern bool get_neighbor(const char *hash, int direction, char * buf, int buf_length) ;



// ��ȡָ�������µķ����С����/��ȣ�
// Input:   precision        Ŀ�꾫��
// Return:  �þ����·����С
extern GeoBoxDimension geohash_dimensions_for_precision(int precision);



// ����Ϊ��Դģ�������ʵ�֣��ɹ��ο�����������ʹ��
#ifdef USE_ORIGIN_GEOHASH
/*
* Creates a the hash at the specified precision. If precision is set to 0.
* or less than it defaults to 12.
*/
// ��Դ�ļ������Geohash���뺯��
extern char* geohash_encode_origin(char * hash, int dwBufLen, double lat, double lng, int precision);

// ��Դ�ļ�����Ļ�ȡ�ھӺ��������ڴ�й¶���⣬����ʹ��
extern char* get_neighbor_origin(const char *hash, int direction);

/* 
* Return an array of geohashes that represent the neighbors of the passed
* in value. The neighbors are indexed as followed:
*
*                  N, NE, E, SE, S, SW, W, NW
* 					0, 1,  2,  3, 4,  5, 6, 7
*/ 
// ��Դ�ļ�����Ļ�ȡ�ھӺ��������ڴ�й¶���⣬����ʹ��
extern char** geohash_neighbors(const char* hash);

#endif  // USE_ORIGIN_GEOHASH

#endif  // #ifndef __H_GEOHASH
