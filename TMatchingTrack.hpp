#pragma once
#include <TObject.h>
#include <TMath.h>
#include "TBaseTrackSpot.hpp"
#include "NTBMSummary.hh"

class NTBMTrackInfo : public TObject
{
public:
	NTBMTrackInfo() { icluster = -1 ; }
	NTBMTrackInfo( Int_t icluster, Double_t ax, Double_t ay, Double_t x, Double_t y )
	:icluster(icluster), ax(ax), ay(ay), x(x), y(y) {}
	Int_t icluster ;
	Double_t ax, ay, x, y ;
};

class BaseTrackInfo : public TObject
{
public:
	BaseTrackInfo() { spot_id = -1 ; pl = -1 ; id = -1 ; }
	BaseTrackInfo( Int_t spot_id, Int_t pl, Int_t id, Double_t ax, Double_t ay, Double_t x, Double_t y, Int_t ph1, Int_t ph2 )
	:spot_id(spot_id), pl(pl), id(id), ax(ax), ay(ay), x(x), y(y), ph1(ph1), ph2(ph2) {}
	Int_t spot_id ;
	Int_t pl, id ;
	Double_t ax, ay ; 
	Double_t x, y ;			//[mm]
	Int_t ph1, ph2 ;
};

class TMatchingTrack : public TObject
{
public:
	TMatchingTrack() { dz_x = 37.0 ; dz_y = 17.0 ; dx = 0. ; dy = 0. ; dax = 0. ; day = 0. ; chi_square = 1.E10 ; }
	TMatchingTrack( const NTBMTrackInfo& nt, const BaseTrackInfo& bt, const TBaseTrackTrackerWall& bt_tracker_wall, const NTBMSummary& ntbm_summary )
	:nt(nt), bt(bt), bt_tracker_wall(bt_tracker_wall), ntbm_summary(ntbm_summary)
	{
		dz_x = 37.0;
		dz_y = 17.0;
		dx = nt.x - ( bt.x + ( dz_x * bt.ax ) ) ;
		dy = nt.y - ( bt.y + ( dz_y * bt.ay ) ) ;
		dax = nt.ax - bt.ax ;
		day = nt.ay - bt.ay ;
		chi_square = 1.E10;
	}
	Double_t dx, dy ;
	Double_t dax, day ;
	Double_t dz_x, dz_y ;
	Double_t chi_square ;
	NTBMTrackInfo nt ;
	BaseTrackInfo bt ;
	TBaseTrackTrackerWall bt_tracker_wall ;
	NTBMSummary ntbm_summary ;

	Int_t BabyMIND_track_id() const { return ntbm_summary.GetBabyMindTrackId( nt.icluster ) ; }
	Int_t unix_time() const  { return ntbm_summary.GetTimestamp(); }
	Int_t wagasci_flag() const { return ntbm_summary.GetWagasciGoodSpillFlag(); }
	Int_t bsd_flag() const { return ntbm_summary.GetBsdGoodSpillFlag(); }
	Int_t track_type() const { return ntbm_summary.GetNinjaTrackType( ntbm_summary.GetBabyMindTrackId( nt.icluster ) ) ; }
	Int_t direction() const { return ntbm_summary.GetDirection( ntbm_summary.GetBabyMindTrackId( nt.icluster ) ) ; }
	Int_t charge() const { return ntbm_summary.GetCharge( ntbm_summary.GetBabyMindTrackId( nt.icluster ) ) ; }
	Double_t momentum() const { return ntbm_summary.GetMomentum( ntbm_summary.GetBabyMindTrackId( nt.icluster ) ) ; }
	Int_t momentum_type() const { return ntbm_summary.GetMomentumType( ntbm_summary.GetBabyMindTrackId( nt.icluster ) ) ; }
	Int_t baby_mind_maximum_plane() const { return ntbm_summary.GetBabyMindMaximumPlane( ntbm_summary.GetBabyMindTrackId( nt.icluster ) ) ;  }
	Double_t angle() const { return TMath::Hypot( ntbm_summary.GetNinjaTangent(nt.icluster, 1) , ntbm_summary.GetNinjaTangent(nt.icluster, 0) ) ; }

	static bool compare_size_bt_id( const TMatchingTrack &l, const TMatchingTrack &r ) { return ( l.bt.id < r.bt.id ) ; }
	static bool compare_size_unix_time( const TMatchingTrack &l, const TMatchingTrack &r ) {
		return ( l.unix_time() != r.unix_time() ? l.unix_time() < r.unix_time() : l.BabyMIND_track_id() < r.BabyMIND_track_id() ) ;
	}
	ClassDef( TMatchingTrack, 2 ) ;
};
#ifdef __MAKECINT__
#pragma link C++ class TMatchingTrack+;
#endif