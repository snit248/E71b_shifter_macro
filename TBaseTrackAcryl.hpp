#pragma once
#include <TObject.h>
#include "TBaseTrack.hpp"
#include "TLinkletAcryl.hpp"

class TMiniBaseTrack : public TObject
{
public:
	TMiniBaseTrack(){ id = -1 ; }
	Int_t pl, id ;
	void Set( Int_t pl_new, Int_t id_new ) { pl = pl_new ; id = id_new ; }
	bool IsFound() { if ( id >= 0 ) { return true ; } else { return false ; } }
	ClassDef( TMiniBaseTrack, 2 ) ;
};

class TBaseTrackAcryl : public TBaseTrack
{
public:
	TBaseTrackAcryl() :v_mbt(4) {}
	TBaseTrackAcryl( const Int_t new_id, const TLinkletAcryl& l ) :v_mbt(4)
	{
		id = new_id ;
		pl = (Int_t)l.pos0 /10 ;
		ax = l.ax_acryl ;
		ay = l.ay_acryl ;
		x = l.x0 ;
		y = l.y0 ;
		id1 = l.id00 ;
		ph1 = l.ph00 ;
		ax1 = l.ax00 ;
		ay1 = l.ay00 ;
		id2 = l.id01 ;
		ph2 = l.ph01 ;
		ax2 = l.ax01 ;
		ay2 = l.ay01 ;
		SetMiniBaseTrack( (Int_t)l.pos0 /10, l.id0 ) ;
		SetMiniBaseTrack( (Int_t)l.pos1 /10, l.id1 ) ;
		da = l.angle_difference() ;
	}
	vector<TMiniBaseTrack> v_mbt ;
	Double_t da ;
	TMiniBaseTrack mbt( Int_t i_pl )
	{
		if ( i_pl > v_mbt.size() || i_pl < 1 ) { cerr << "TBaseTrackAcryl::mbt()::Error " << endl ; exit(1) ; }
		return v_mbt.at( i_pl - 1 ) ;
	}
	void SetMiniBaseTrack( Int_t pl_new, Int_t id_new )
	{
		Int_t i ;
		if ( pl_new /10 != 0 ) { i = pl_new %10 ; } else { i = 5 - pl_new ; }
		if ( i < 1 || i > v_mbt.size() ) { cerr << "TBaseTrackAcryl::SetMiniBaseTrack()::Error " << endl ; exit(1) ; }
		v_mbt.at( i - 1 ).Set( pl_new, id_new ) ;
	}
	void SetAngle( Double_t ax_new, Double_t ay_new, Double_t da_new )
	{
		ax = ax_new ;
		ay = ay_new ;
		da = da_new ;
	}
	ClassDef( TBaseTrackAcryl, 5 );
};
#ifdef __MAKECINT__
#pragma link C++ class TMiniBaseTrack+;
#pragma link C++ class TBaseTrackAcryl+;
#endif