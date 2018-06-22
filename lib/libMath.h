#ifndef __MATH_H__
#define __MATH_H__

#include <stdio.h>
#include <memory.h>


#include "libTypes.h"
#include "math.h"


class CMngMath
{
public:
	static void MatrixConcat(float *p_pMatrix1, float *p_pMatrix2)
	{
		float dest[16];
		for(int i= 0; i < 16; i+= 4){
			for(int j= 0; j < 4; ++j)
				dest[i + j]= (p_pMatrix1[i] * p_pMatrix2[j] +
							 p_pMatrix1[i + 1] * p_pMatrix2[j + 4] +
							 p_pMatrix1[i + 2] * p_pMatrix2[j + 8] +
							 p_pMatrix1[i + 3] * p_pMatrix2[j + 12]);
		}

		memcpy(p_pMatrix1, dest, sizeof(float)*16);
	};
	static void MatrixRotate(float *p_pMatrix, u32 p_RX, u32 p_RY, u32 p_RZ)
	{
	float matrixTemp[16];

		CMngMath::MatrixRotateX(matrixTemp, p_RX);
		CMngMath::MatrixRotateY(p_pMatrix, p_RY);
		CMngMath::MatrixConcat(p_pMatrix, matrixTemp);
		CMngMath::MatrixRotateZ(matrixTemp, p_RZ);
		CMngMath::MatrixConcat(p_pMatrix, matrixTemp);

	};
	static void MatrixIdentity(float *p_pMatrix)
	{
		memset(p_pMatrix, 0, sizeof(float)*16);
		p_pMatrix[0]= p_pMatrix[5]= p_pMatrix[10]= p_pMatrix[15]= 1.0f;
	};
	static void MatrixRotateX(float *p_pMatrix, u32 p_RX)
	{
	float sinx= sin(p_RX);
	float cosx= cos(p_RX);

		CMngMath::MatrixIdentity(p_pMatrix);
		p_pMatrix[5] = cosx;
		p_pMatrix[6] = sinx;
		p_pMatrix[9] = -sinx;
		p_pMatrix[10]= cosx;
	};
	static void MatrixRotateY(float *p_pMatrix, u32 p_RY)
	{
	float siny= sin(p_RY);
	float cosy= cos(p_RY);

		CMngMath::MatrixIdentity(p_pMatrix);
		p_pMatrix[0] = cosy;
		p_pMatrix[2] = -siny;
		p_pMatrix[8] = siny;
		p_pMatrix[10]= cosy;
	};
	static void MatrixRotateZ(float *p_pMatrix, u32 p_RZ)
	{
	float sinz = sin(p_RZ);
	float cosz = cos(p_RZ);

		CMngMath::MatrixIdentity(p_pMatrix);
		p_pMatrix[0] = cosz;
		p_pMatrix[1] = sinz;
		p_pMatrix[4] = -sinz;
		p_pMatrix[5] = cosz;
	};
	static void MatrixTranslate(float *p_pMatrix, u32 p_TX, u32 p_TY, u32 p_TZ)
	{
		CMngMath::MatrixIdentity(p_pMatrix);
		p_pMatrix[12]+= p_TX;
		p_pMatrix[13]+= p_TY;
		p_pMatrix[14]+= p_TZ;
	};
	static void VectorAdd(float* p_dst, float* p_vec1, float* p_vec2)
	{
		p_dst[0]= p_vec1[0] + p_vec2[0];
		p_dst[1]= p_vec1[1] + p_vec2[1];
		p_dst[2]= p_vec1[2] + p_vec2[2];
	}
	static void VectorMin(float* p_dst, float* p_vec1, float* p_vec2)
	{
		p_dst[0]= p_vec1[0] - p_vec2[0];
		p_dst[1]= p_vec1[1] - p_vec2[1];
		p_dst[2]= p_vec1[2] - p_vec2[2];
	}
	static void VectorScale(float* p_dst, float* p_vec1, float p_scale)
	{
		p_dst[0]= p_vec1[0] * p_scale;
		p_dst[1]= p_vec1[1] * p_scale;
		p_dst[2]= p_vec1[2] * p_scale;
	}
	static float VectorDot(float* p_vec1, float* p_vec2)
	{
		return (p_vec1[0] * p_vec2[0]) + (p_vec1[1] * p_vec2[1]) + (p_vec1[2] * p_vec2[2]);
	}
	static float VectorGetLenght(float *p_vec)
	{
		float xyLen= float(sqrt((p_vec[0]*p_vec[0])+(p_vec[1]*p_vec[1])));
		return float(sqrt((xyLen*xyLen)+(p_vec[2]*p_vec[2])));
	}
};

#pragma pack(push, forTVec, 1)
typedef struct tagTVec
{
	float	x;
	float	y;
	float	z;
}TVec;
#pragma pack(pop, forTVec)

#endif /* __MATH_H__ */
