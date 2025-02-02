#pragma once
#include <TObject.h>
#include "TLinklet.hpp"

class TLinkletAcryl : public TLinklet
{
public:
	TLinkletAcryl(){}
	TLinkletAcryl( const TLinklet& l ) :TLinklet(l) {}
	Double_t ax_acryl, ay_acryl ;
	void SetAcrylAngle()
	{
		ax_acryl = CalcAngleAx() ;
		ay_acryl = CalcAngleAy() ;
	}
	Double_t CalcAngleAx() { return ( x0 - x1 ) / ( z0 - z1 ) ; }
	Double_t CalcAngleAy() { return ( y0 - y1 ) / ( z0 - z1 ) ; }
	ClassDef( TLinkletAcryl, 4 ) ;
};
#ifdef __MAKECINT__
#pragma link C++ class TLinkletAcryl+;
#endif