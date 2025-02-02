#pragma once
#include <TObject.h>
#include <TMath.h>
#include "TLinklet.hpp"

class TSpot : public TObject
{
public:
	TSpot(){}
	TSpot( Int_t id, Double_t x_center, Double_t y_center, Double_t radius ) : id(id), x_center(x_center), y_center(y_center), radius(radius) {}
	Int_t id ;
	Double_t x_center, y_center ;
	Double_t radius ;
	Double_t phsum_mean, ph0_mean, ph1_mean ;
	Double_t vphsum_mean, vph0_mean, vph1_mean ;
	vector<TLinklet> v_lnk ;
	static bool compare_size_id(const TSpot &a, const TSpot &b) { return ( a.id < b.id ) ; }
	ClassDef( TSpot, 3 ) ;
};
#ifdef __MAKECINT__
#pragma link C++ class TSpot+;
#endif