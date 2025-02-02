#pragma once
#include <TObject.h>
class TCorrection : public TObject
{
public:
	TCorrection(){}
	Int_t id, pos0, pos1 ;
	Double_t xmin, xmax, ymin, ymax ;
	Double_t pos_a, pos_b, pos_c, pos_d, pos_p, pos_q ;
	Double_t ang_a, ang_b, ang_c, ang_d, ang_p, ang_q ;
	Double_t dz, sig, bg, sn, rms_x, rms_y, rms_ax, rms_ay ;
	Int_t ix, iy ;
	static const Double_t dz_nominal ;
	Double_t Ax( Double_t ax, Double_t ay ) const { return ang_a*ax+ang_b*ay+ang_p ; }
	Double_t Ay( Double_t ax, Double_t ay ) const { return ang_c*ax+ang_d*ay+ang_q ; }
	Double_t Px( Double_t x, Double_t y ) const { return pos_a*x+pos_b*y+pos_p ; }
	Double_t Py( Double_t x, Double_t y ) const { return pos_c*x+pos_d*y+pos_q ; }
	static bool compare_size_iyix ( const TCorrection &l, const TCorrection &r ) { return ( l.iy != r.iy ? l.iy < r.iy : l.ix < r.ix ) ; }
	static bool compare_equal_iyix ( const TCorrection &l, const TCorrection &r ) { return ( l.iy == r.iy && l.ix == r.ix ) ; }
	void Flush() ;
	ClassDef( TCorrection, 9 ) ;
};
const Double_t TCorrection::dz_nominal = -310. * 3. ;   //get dz_nominal from emulsion structure, if that is essential.
void TCorrection::Flush()
{
		id = -1 ; pos0 = -1 ; pos1 = -1 ;
		xmin = 0. ; xmax = 0. ; ymin = 0. ; ymax = 0. ;
		pos_a = 1. ; pos_b = 0. ; pos_c = 0. ; pos_d = 1. ; pos_p = 0. ; pos_q = 0. ;
		ang_a = 1. ; ang_b = 0. ; ang_c = 0. ; ang_d = 1. ; ang_p = 0. ; ang_q = 0. ;
		dz = 0. ; sig = 0. ; bg = 0. ; sn = 0. ; rms_x = 0. ; rms_y = 0. ; rms_ax = 0. ; rms_ay = 0. ;
		ix = -1 ; iy = -1 ;
}
#ifdef __MAKECINT__
#pragma link C++ class TCorrection+;
#endif