#ifndef _GOOME_PUBLIC_MD5_H_
#define _GOOME_PUBLIC_MD5_H_

#ifdef __cplusplus  
extern "C" {  
#endif 

typedef struct {
    unsigned int state[4];     
    unsigned int count[2];     
    unsigned char buffer[64];     
} MD5Context;

// If you have large data to gen md5, please use these 3 function(How to use? see GenMD5())
void Goome_MD5_Init(MD5Context * context);
void Goome_MD5_Update(MD5Context * context, const unsigned char * buf, int len);
void Goome_MD5_Final(MD5Context * context, unsigned char digest[16]);

// If you don't have much data to gen md5, use this function for simply
char * Goome_GenMD5(const unsigned char * src, int len, char * dest);

#ifdef __cplusplus  
}  
#endif  

#endif
