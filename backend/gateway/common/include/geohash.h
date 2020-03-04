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


// 方向，获取邻居方块时用
#define GEOHASH_NORTH               0
#define GEOHASH_EAST                1
#define GEOHASH_SOUTH               2
#define GEOHASH_WEST                3

// 方块的长/宽（米）
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

// Geohash编码函数经过优化的版本，能提高7%左右的速度 by Ouyibao
// 将经纬度编码成Geohash值
// Input:   buf_length  输出缓冲区长度
// Input:   lat         纬度
// Input:   lng         经度
// Input:   precision   Geohash编码精度。在1和12之间
// Output:  buf         输出缓冲区存放经纬度编码后的hash值
// Return:  编码后的geohash值
extern char* geohash_encode(char * buf, int buf_length, double lat, double lng, int precision);

// 将Geohash值解码为对应的方格坐标（经纬度）
// Input:   hash    某方格的geohash值
// Return:  该hash值对应的方格经纬度坐标
extern GeoCoord geohash_decode(const char* hash);

// 修改后的获取邻居方格函数，无内存泄露问题 by Ouyibao
// Input:   hash        该方格的geohash值
// Input:   direction   需要获取该方格哪个方向的邻居
// Input:   buf_length  pBuf缓冲区长度，不应小于hash字符串的长度
// Output:   buf         保存邻居方块geohash值
// Return:  获取邻居是否成功。 true：成功；false：失败
extern bool get_neighbor(const char *hash, int direction, char * buf, int buf_length) ;



// 获取指定精度下的方格大小（长/宽等）
// Input:   precision        目标精度
// Return:  该精度下方格大小
extern GeoBoxDimension geohash_dimensions_for_precision(int precision);



// 以下为开源模块最初的实现，可供参考，但不建议使用
#ifdef USE_ORIGIN_GEOHASH
/*
* Creates a the hash at the specified precision. If precision is set to 0.
* or less than it defaults to 12.
*/
// 开源文件最初的Geohash编码函数
extern char* geohash_encode_origin(char * hash, int dwBufLen, double lat, double lng, int precision);

// 开源文件最初的获取邻居函数，有内存泄露问题，请勿使用
extern char* get_neighbor_origin(const char *hash, int direction);

/* 
* Return an array of geohashes that represent the neighbors of the passed
* in value. The neighbors are indexed as followed:
*
*                  N, NE, E, SE, S, SW, W, NW
* 					0, 1,  2,  3, 4,  5, 6, 7
*/ 
// 开源文件最初的获取邻居函数，有内存泄露问题，请勿使用
extern char** geohash_neighbors(const char* hash);

#endif  // USE_ORIGIN_GEOHASH

#endif  // #ifndef __H_GEOHASH
