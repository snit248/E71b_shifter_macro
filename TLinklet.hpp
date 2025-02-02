#pragma once
#include <TObject.h>
#include <TMath.h>

class TLinklet : public TObject
{
public:
	TLinklet(){}
	Int_t id0, id1, id00, id01, id10, id11 ;
	Float_t pos0, pos1, ph0, ax0, ay0, x0, y0, ph1, ax1, ay1, x1, y1, z0, z1, zproj, dx, dy ;
	Float_t ph00, ax00, ay00, ph01, ax01, ay01, ph10, ax10, ay10, ph11, ax11, ay11 ;
	Double_t angle_difference() const { return TMath::Hypot( ax0 - ax1, ay0 - ay1 ) ; }
	//add
	//Double_t chi_square_4() const {
    //    return chis_da_lat + chis_da_rad + chis_dp_lat + chis_dp_rad;
    //}
	//add
	static bool compare_size_id0(const TLinklet &a, const TLinklet &b) { return ( a.id0 < b.id0 ) ; }
	static bool compare_size_id1(const TLinklet &a, const TLinklet &b) { return ( a.id1 < b.id1 ) ; }
	static bool compare_size_da(const TLinklet &a, const TLinklet &b) { return ( a.angle_difference() < b.angle_difference() ) ; }
	ClassDef( TLinklet, 3 ) ;
};
#ifdef __MAKECINT__
#pragma link C++ class TLinklet+;
#endif