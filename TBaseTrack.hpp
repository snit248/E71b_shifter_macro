#pragma once
#include <TObject.h>
#include <TMath.h>
class TBaseTrack : public TObject
{
public:
	TBaseTrack() {}
	TBaseTrack( Int_t pl, Int_t id, Double_t ax, Double_t ay, Double_t x, Double_t y, Int_t id1, Int_t ph1, Double_t ax1, Double_t ay1, Int_t id2, Int_t ph2, Double_t ax2, Double_t ay2 )
	: pl(pl), id(id), ax(ax), ay(ay), x(x), y(y), id1(id1), ph1(ph1), ax1(ax1), ay1(ay1), id2(id2), ph2(ph2), ax2(ax2), ay2(ay2) {}
	Int_t pl, id ;
	Double_t ax, ay, x, y ;
	Int_t id1, ph1 ;
	Double_t ax1, ay1 ;
	Int_t id2, ph2 ;
	Double_t ax2, ay2 ;
	Int_t ph_sum() const { return ( ph1 + ph2 ) / 10000 ; }
	Int_t phv_sum() const { return ( ph1 + ph2 ) % 10000 ; }
	Double_t angle() const { return TMath::Hypot( ax, ay ) ; }
	static bool compare_size_id( const TBaseTrack &l, const TBaseTrack &r ) { return ( l.id < r.id ) ; }
	static bool compare_equal_id( const TBaseTrack &l, const TBaseTrack &r ) { return ( l.id == r.id ) ; }
	static bool compare_size_x( const TBaseTrack &l, const TBaseTrack &r ) { return ( l.x < r.x ) ; }
	ClassDef( TBaseTrack, 11 ) ;
};
#ifdef __MAKECINT__
#pragma link C++ class TBaseTrack+;
#endif