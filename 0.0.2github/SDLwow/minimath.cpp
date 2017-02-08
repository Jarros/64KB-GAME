////#include "minimath.h"
////
////float sqrt(float x)
////{
////  _asm
////  {
////    fld x
////    fsqrt
////    fst x
////  }
////  return x;
////}
////
//float sinm(float x)
//{
//  _asm
//  {
//    fld x
//    fsin
//    fst x
//  }
//  return x;
//}
////
//float cosm(float x)
//{
//  _asm
//  {
//    fld x
//    fcos
//    fst x
//  }
//  return x;
//}
////
////float atan(float x,float y)
////{
////  float v;
////  _asm
////  {
////    fld x
////    fld y
////    fpatan
////    fwait
////    fst v
////  }
////  return v;
////}
////
////float asin(float x)
////{
////  return atan(x,sqrt(1-x*x));
////}
////
////float acos(float x)
////{
////  return atan(sqrt(1-x*x),x);
////}
////
////float abss(float x)
////{
////	if(x<0)return -x;
////	return x;
////}
////
////float tan(float x)
////{
////	return sin(x)/cos(x);
////}
////
////float pow(float x, float y)
////{
////	int a;
////	for(a=1;a<(int)(y+1);a++)
////	{
////		x*=a;
////	}
////	return x;
////}