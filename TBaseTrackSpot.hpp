#pragma once
#include <TObject.h>
#include "TBaseTrack.hpp"
#include "TLinklet.hpp"

class TSpotInfo : public TObject
{
public:
	TSpotInfo(){ spot=-1; id=-1; chis_da_lat=1E+6; chis_da_rad=1E+6; chis_dp_lat=1E+6; chis_dp_rad=1E+6; }
	void SetSpotInfo( Int_t spot_, const TLinklet& l, Int_t mode )
	{
		if ( mode == 0 ) { spot = spot_; id = l.id0; ax = l.ax0; ay = l.ay0; ph1 = l.ph00; ph2 = l.ph01; }
		else { spot = spot_; id = l.id1; ax = l.ax1; ay = l.ay1; ph1 = l.ph10; ph2 = l.ph11; }
		dx = l.dx; dy = l.dy;
		dax = l.ax0-l.ax1; day = l.ay0-l.ay1;
		dz = l.z0-l.z1;
		chis_da_lat=1E+6; chis_da_rad=1E+6; chis_dp_lat=1E+6; chis_dp_rad=1E+6;
	}
	Int_t spot;
	Int_t id;
	Double_t ax, ay;
	Double_t dx, dy;
	Double_t dax, day;
	Double_t dz;
	Double_t ph1, ph2;
	Double_t chis_da_lat, chis_da_rad ;
	Double_t chis_dp_lat, chis_dp_rad ;

	Double_t angle() const { return TMath::Hypot(ax, ay); }
	Double_t da() const { return TMath::Hypot(dax, day); }
	Double_t da_rad() const { return ( dax*ax + day*ay ) / angle(); }
	Double_t da_lat() const { return ( dax*ay - day*ax ) / angle(); }
	Double_t dp() const { return TMath::Hypot(dx, dy); }
	Double_t dp_rad() const { return ( dx*ax + dy*ay ) / angle(); }
	Double_t dp_lat() const { return ( dx*ay - dy*ax ) / angle(); }
	Double_t chi_square_4() const { return chis_da_lat + chis_da_rad + chis_dp_lat + chis_dp_rad ; }
	Double_t chi_square_3() const { return chis_da_lat + chis_da_rad + chis_dp_lat ; }
	static bool compare_size_da( const TSpotInfo &l, const TSpotInfo &r ) { return ( l.da() < r.da() ) ; }
	static bool compare_size_da_lat( const TSpotInfo &l, const TSpotInfo &r ) { return ( TMath::Abs(l.da_lat()) < TMath::Abs(r.da_lat()) ) ; }
	static bool compare_size_chi_square_4( const TSpotInfo &l, const TSpotInfo &r ) { return ( l.chi_square_4() < r.chi_square_4() ) ; }
	static bool compare_size_chi_square_3( const TSpotInfo &l, const TSpotInfo &r ) { return ( l.chi_square_3() < r.chi_square_3() ) ; }
	ClassDef( TSpotInfo, 6 );
};

class TBaseTrackSpot : public TBaseTrack
{
public:
	TBaseTrackSpot(){}
	TBaseTrackSpot( Int_t spot_slow, Int_t spot_fast, const TBaseTrack& bt, const TLinklet& l )
	: TBaseTrack(bt)
	{
		if ( spot_slow != -1 && spot_fast == -1 ) { slow.SetSpotInfo(spot_slow, l, 0); }
		else if ( spot_slow == -1 && spot_fast != -1 ) { fast.SetSpotInfo(spot_fast, l, 1); }
		else { cerr << "Error : TBaseTrack() : spot id is not given correctly." << endl; }
	}
	TBaseTrackSpot( const TBaseTrack& bt, const TSpotInfo& slow, const TSpotInfo& fast )
	: TBaseTrack(bt), slow(slow), fast(fast) {}
	void SetVectorSlow( vector<TBaseTrackSpot>& v_list )
	{
		for ( auto p_bt = v_list.cbegin() ; p_bt != v_list.cend() ; ++p_bt )
		{
			v_slow.push_back( p_bt->slow );
		}
	}
	void SetVectorFast( vector<TBaseTrackSpot>& v_list )
	{
		for ( auto p_bt = v_list.cbegin() ; p_bt != v_list.cend() ; ++p_bt )
		{
			v_fast.push_back( p_bt->fast );
		}
	}
	TSpotInfo slow, fast;
	vector<TSpotInfo> v_slow, v_fast;

	static bool compare_size_slow_da( const TBaseTrackSpot &l, const TBaseTrackSpot &r ) { return ( l.slow.da() < r.slow.da() ); }
	static bool compare_size_slow_da_lat( const TBaseTrackSpot &l, const TBaseTrackSpot &r ) { return ( TMath::Abs(l.slow.da_lat()) < TMath::Abs(r.slow.da_lat()) ) ; }
	static bool compare_size_fast_da( const TBaseTrackSpot &l, const TBaseTrackSpot &r ) { return ( l.fast.da() < r.fast.da() ); }
	static bool compare_size_fast_da_lat( const TBaseTrackSpot &l, const TBaseTrackSpot &r ) { return ( TMath::Abs(l.fast.da_lat()) < TMath::Abs(r.fast.da_lat()) ) ; }
	ClassDef( TBaseTrackSpot, 6 );
};

class TBaseTrackTrackerWall : public TBaseTrack
{
public:
	TBaseTrackTrackerWall(){}
	TBaseTrackTrackerWall( const TBaseTrack& bt, const TSpotInfo& slow, const TSpotInfo& fast, const TLinklet& lnk )
	: TBaseTrack(bt), slow(slow), fast(fast), lnk(lnk) {}
	TSpotInfo slow, fast;
	TLinklet lnk ;
	ClassDef( TBaseTrackTrackerWall, 6 );
};

#ifdef __MAKECINT__
#pragma link C++ class TBaseTrackSpot+;
#pragma link C++ class TBaseTrackTrackerWall+;
#pragma link C++ class TSpotInfo+;
#endif