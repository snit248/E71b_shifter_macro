#pragma once
#include <TObject.h>

class TMiniLinklet : public TObject
{
public:
	TMiniLinklet(){}
	Int_t id0, id1 ;
	Float_t pos0, pos1, ph0, ax0, ay0, x0, y0, ph1, ax1, ay1, x1, y1, z0, z1, zproj, dx, dy ;
	Double_t angle_difference() const { return TMath::Hypot( ax0 - ax1, ay0 - ay1 ) ; }
	static bool compare_size_id0(const TMiniLinklet &a, const TMiniLinklet &b) { return ( a.id0 < b.id0 ) ; }
	static bool compare_size_id1(const TMiniLinklet &a, const TMiniLinklet &b) { return ( a.id1 < b.id1 ) ; }
	static bool compare_size_da(const TMiniLinklet &a, const TMiniLinklet &b) { return ( a.angle_difference() < b.angle_difference() ) ; }
	ClassDef( TMiniLinklet, 1 ) ;
};
#ifdef __MAKECINT__
#pragma link C++ class TMiniLinklet+;
#endif