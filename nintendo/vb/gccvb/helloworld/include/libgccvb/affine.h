// affine.h
// v0.1 beta (preliminary scaling functions added)
//  functions and macros to make affine transformations easier
//  for use with gccVB
//  written by Parasyte (parasytic_i[at]yahoo.com)


#ifndef __AFFINE_H
#define __AFFINE_H

#include "types.h"
#include "video.h"


#define fixed_7_9(n)		(f32)(n * (1<<9))			//convert from float\int\etc to 7.9 fixed
#define fixed_13_3(n)		(f16)(n * (1<<3))			//convert from float\int\etc to 13.3 fixed
#define inverse_fixed(n)	(f16)((1<<18)/fixed_7_9(n))	//convert from float\int\etc to 7.9 fixed (with inversion)

//clear a world's affine param table based on world height
void affine_clr_param(u8 world) {
	int i,tmp;
	s16 *param;

	tmp = (world<<4);
	param = (s16*)((WAM[tmp+9]<<1)+0x00020000);
	for (i = 0; i < (WAM[tmp+8]<<3); i++) param[i]=0;
}

//scale an affine background
//world: number of the world to apply scaling to, must be using affine BGM
//centerX/centerY: center point (relative to the world) to scale around.
//imageW/imageH: original image width and height
//scaleX/scaleY: scale factor per axis.
//  lowest positive:  0.0156254
//  highest positive: 512
//  lowest negative:  -512
//  highest negative: -0.015625
//'1' is 100% scale, '2' is 200%, '0.05' is 5%, etc
void affine_scale(u8 world, s16 centerX, s16 centerY, u16 imageW, u16 imageH, float scaleX, float scaleY) {
	int i,tmp;
	s16 *param;
	f16 XSrc,XScl,YScl;
	f32 YSrc;

	tmp = (world<<4);
	param = (s16*)((WAM[tmp+9]<<1)+0x00020000);

	XScl = inverse_fixed(scaleX);
	YScl = inverse_fixed(scaleY);

	XSrc = -fixed_13_3((centerX-((imageW*scaleX)/2))/scaleX); //keep image centered
	YSrc = -fixed_7_9((centerY-((imageH*scaleY)/2))/scaleY);

	i=0;
	while (i < (int)(WAM[tmp+8]<<3)) {
		param[i++] = XSrc;		//XSrc
		i++;					//Prlx
		param[i++] = (YSrc>>6);	//YSrc -- convert from 7.9 to 13.3 fixed
		param[i++] = XScl; 		//XScl
		i+=4; //skip remaining entries

		YSrc += YScl; //grab value of next scanline
	}
}

//scale an affine background
//world: number of the world to apply scaling to, must be using affine BGM
//scale: scale factor.
//  lowest positive:  0.0156254
//  highest positive: 512
//  lowest negative:  -512
//  highest negative: -0.015625
//'1' is 100% scale, '2' is 200%, '0.05' is 5%, etc
void affine_fast_scale(u8 world, float scale) {
	int i,tmp;
	s16 *param;
	f16 XScl,YScl;
	f32 YSrc;

	tmp = (world<<4);
	param = (s16*)((WAM[tmp+9]<<1)+0x00020000);

	XScl = YScl = inverse_fixed(scale);
	YSrc = 0;

	i=0;
	while (i < (int)(WAM[tmp+8]<<3)) {
		param[i++] = 0;			//XSrc
		i++;					//Prlx
		param[i++] = (YSrc>>6);	//YSrc -- convert from 7.9 to 13.3 fixed
		param[i++] = XScl; 		//XScl
		i+=4; //skip remaining entries

		YSrc += YScl; //grab value of next scanline
	}
}

#endif
