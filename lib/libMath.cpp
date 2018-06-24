#include "libMath.h"

/*
static const unsigned int g_arSinCos[512]={//TODO
0x0, 0x6, 0xc, 0x12, 0x19, 0x1f, 0x25, 0x2b, 0x31, 0x38, 0x3e, 0x44, 0x4a, 0x50, 0x56, 0x5c, 0x61, 0x67, 0x6d, 0x73, 0x78,
0x7e, 0x83, 0x88, 0x8e, 0x93, 0x98, 0x9d, 0xa2, 0xa7, 0xab, 0xb0, 0xb5, 0xb9, 0xbd, 0xc1, 0xc5, 0xc9, 0xcd, 0xd1, 0xd4, //245
0xd8, 0xdb, 0xde, 0xe1, 0xe4, 0xe7, 0xea, 0xec, 0xee, 0xf1, 0xf3, 0xf4, 0xf6, 0xf8, 0xf9, 0xfb, 0xfc, 0xfd, 0xfe, 0xfe,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfd, 0xfc, 0xfb, 0xf9, 0xf8, 0xf6, 0xf4, 0xf3, 0xf1, 0xee, 0xec,
0xea, 0xe7, 0xe4, 0xe1, 0xde, 0xdb, 0xd8, 0xd4, 0xd1, 0xcd, 0xc9, 0xc5, 0xc1, 0xbd, 0xb9, 0xb5, 0xb0, 0xab, 0xa7, 0xa2,
0x9d, 0x98, 0x93, 0x8e, 0x88, 0x83, 0x7e, 0x78, 0x73, 0x6d, 0x67, 0x61, 0x5c, 0x56, 0x50, 0x4a, 0x44, 0x3e, 0x38, 0x31,
0x2b, 0x25, 0x1f, 0x19, 0x12, 0xc, 0x6, 0x0, 0xfffffffa, 0xfffffff4, 0xffffffee, 0xffffffe7, 0xffffffe1, 0xffffffdb, // 145
0xffffffd5, 0xffffffcf, 0xffffffc8, 0xffffffc2, 0xffffffbc, 0xffffffb6, 0xffffffb0, 0xffffffaa, 0xffffffa4, 0xffffff9f,
0xffffff99, 0xffffff93, 0xffffff8d, 0xffffff88, 0xffffff82, 0xffffff7d, 0xffffff78, 0xffffff72, 0xffffff6d, 0xffffff68,
0xffffff63, 0xffffff5e, 0xffffff59, 0xffffff55, 0xffffff50, 0xffffff4b, 0xffffff47, 0xffffff43, 0xffffff3f, 0xffffff3b,
0xffffff37, 0xffffff33, 0xffffff2f, 0xffffff2c, 0xffffff28, 0xffffff25, 0xffffff22, 0xffffff1f, 0xffffff1c, 0xffffff19,
0xffffff16, 0xffffff14, 0xffffff12, 0xffffff0f, 0xffffff0d, 0xffffff0c, 0xffffff0a, 0xffffff08, 0xffffff07, 0xffffff05,
0xffffff04, 0xffffff03, 0xffffff02, 0xffffff02, 0xffffff01, 0xffffff01, 0xffffff01, 0xffffff01, 0xffffff01, 0xffffff01,
0xffffff01, 0xffffff02, 0xffffff02, 0xffffff03, 0xffffff04, 0xffffff05, 0xffffff07, 0xffffff08, 0xffffff0a, 0xffffff0c,
0xffffff0d, 0xffffff0f, 0xffffff12, 0xffffff14, 0xffffff16, 0xffffff19, 0xffffff1c, 0xffffff1f, 0xffffff22, 0xffffff25,
0xffffff28, 0xffffff2c, 0xffffff2f, 0xffffff33, 0xffffff37, 0xffffff3b, 0xffffff3f, 0xffffff43, 0xffffff47, 0xffffff4b,
0xffffff50, 0xffffff55, 0xffffff59, 0xffffff5e, 0xffffff63, 0xffffff68, 0xffffff6d, 0xffffff72, 0xffffff78, 0xffffff7d,
0xffffff82, 0xffffff88, 0xffffff8d, 0xffffff93, 0xffffff99, 0xffffff9f, 0xffffffa4, 0xffffffaa, 0xffffffb0, 0xffffffb6,
0xffffff82, 0xffffff88, 0xffffff8d, 0xffffff93, 0xffffff99, 0xffffff9f, 0xffffffa4, 0xffffffaa, 0xffffffb0, 0xffffffb6,
0xffffffbc, 0xffffffc2, 0xffffffc8, 0xffffffcf, 0xffffffd5, 0xffffffdb, 0xffffffe1, 0xffffffe7, 0xffffffee, 0xfffffff4,
0xfffffffa
};

const unsigned int&
sin(u32 p_nAngle)
{
	return g_arSinCos[p_nAngle & 0xFF];
}


const unsigned int&
cos(u32 p_nAngle)
{
	return g_arSinCos[(p_nAngle+64) & 0xFF];
}
*/
