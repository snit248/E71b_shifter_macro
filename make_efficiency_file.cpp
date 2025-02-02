#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <iterator>
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
#include <TCut.h>
#include "NTBMSummary.hh"
#include "TMatchingTrack.hpp"
ClassImp( TMatchingTrack ) ;
ClassImp( NTBMSummary ) ;

class Cut_NTBMSummary
{
private:
	Double_t centerY, centerX ;
	Double_t widthY, widthX ;
public:
	Cut_NTBMSummary()
	{
		/*original
		centerY = +50. +370.;
		centerX = -50. ;
		widthY = 300. ;
		widthX = 300. ;
		*/

		//Just in range 1/10
		//centerY = +66. ;
		//centerX = +343. ;
		//widthY = 340. ;
		//widthX = 250. ;
		

		
		centerY = +66. ;
		centerX = +323. ;
		widthY = 190. ;
		widthX = 100. ;
		

	}
	double y_min() const { return centerY - widthY / 2. ; }
	double y_max() const { return centerY + widthY / 2. ; }
	double x_min() const { return centerX - widthX / 2. ; }
	double x_max() const { return centerX + widthX / 2. ; }
	bool isArea( const vector<double> position ) const
	{
        if ( position.size() != 2 ) { cout << "  Error : position " << endl ; return false ; }
		if ( position.at(0) > y_min() && position.at(0) < y_max() && position.at(1) > x_min() && position.at(1) < x_max() ) { return true ; }
		return false ;
	}
};


vector<TMatchingTrack> InputMatchingTrack( TString ifname ) ;
vector<NTBMSummary> InputNTBMSummary( TString ifname ) ;
vector<TMatchingTrack> MakeMatchingEfficiency( vector<NTBMSummary>& v_nt, vector<TMatchingTrack>& v_mt ) ;
void OutputMatchingTrack( string ofname, vector<TMatchingTrack>& v_match ) ;

void make_efficiency_file()
{
	vector<TMatchingTrack> v_mt = InputMatchingTrack( "ninja_shifter_bt063_tracker_matching.sig.cll.root" ) ;
	vector<NTBMSummary> v_nt = InputNTBMSummary( "/home/nu/itoshun/data/ninja/nt_all_matched/nt_all_matched.root" ) ;

	vector<TMatchingTrack> v_mt_eff = MakeMatchingEfficiency( v_nt, v_mt )  ;
	OutputMatchingTrack( "efficiencybt063_file.root", v_mt_eff ) ;
}

vector<TMatchingTrack> MakeMatchingEfficiency( vector<NTBMSummary>& v_nt, vector<TMatchingTrack>& v_mt )
{
	vector<TMatchingTrack> v_mt_eff ;
	Cut_NTBMSummary cut ;
	cout << "\n* Make efficiency data" << endl ;
	for ( auto n = v_nt.begin() ; n != v_nt.end() ; ++n )
	{
		for ( Int_t icluster = 0 ; icluster < n->GetNumberOfNinjaClusters() ; ++icluster )
		{
			if ( n->GetNumberOfHits(icluster, 0) == 0 || n->GetNumberOfHits(icluster, 1) == 0 ) { continue; }
			if ( !cut.isArea( n->GetNinjaPosition(icluster) ) ) { continue; }
			bool flag = false ;
			Double_t min_chi_square = 1.E6 ;
			auto it_mt = v_mt.begin() ;
			for ( auto m = v_mt.begin() ; m != v_mt.end() ; ++m )
			{
				if ( m->unix_time() == (Int_t)n->GetTimestamp() && m->nt.icluster == icluster )
				{
					if ( m->chi_square < min_chi_square )
					{
						it_mt = m ;
						min_chi_square = m->chi_square ;
						flag = true ;
					}
				}
			}
			if ( flag ) { v_mt_eff.push_back( *it_mt ) ; }
			else { v_mt_eff.push_back( TMatchingTrack( NTBMTrackInfo(icluster,0.,0.,0.,0.), BaseTrackInfo(), TBaseTrackTrackerWall(), *n ) ) ; }
		}
		if ( distance( v_nt.begin(), n )%100 == 0 ) { cout << "  number of clusters : " << setw(6) << v_mt_eff.size() << " , number of events : " << v_nt.size() << "\r" ; }
	}
	cout << "  number of clusters : " << setw(6) << v_mt_eff.size() << " , number of events : " << v_nt.size() << endl ;
	return v_mt_eff ;
}

vector<TMatchingTrack> InputMatchingTrack( TString ifname )
{
	Double_t cut = 60.;
	vector<TMatchingTrack> v_mt ;
	cout << "\n* Input matching data\n  " << ifname << endl ;
	TFile *f = TFile::Open( ifname , "read" ) ;
	if ( !f ) { exit(1) ; }
	TTree *t = (TTree*)f->Get("t") ;
	TMatchingTrack* mt = 0 ;
    t->SetBranchAddress( "match", &mt ) ;
	cout << "  Number of Entries : " << setw(10) << t->GetEntries() << endl;
	for ( Int_t i = 0 ; i < t->GetEntries() ; ++i ) {
    	t->GetEntry(i) ;
		if ( mt->chi_square > cut ) { continue; }
    	v_mt.push_back( *mt ) ;
    	if ( i % 1000 == 0 ) { cout << "  Selected : " << setw(6) << v_mt.size() << " , Progress : " << setw(9) << i << " / " << t->GetEntries() << "\r" ; }
    }
    cout << "  Selected : " << setw(6) << v_mt.size() << " , Progress : " << setw(9) << t->GetEntries() << " / " << t->GetEntries() << endl;
    f->Close();
	return v_mt ;
}

vector<NTBMSummary> InputNTBMSummary( TString ifname )
{
	Cut_NTBMSummary cut ;
	vector<NTBMSummary> v_nt ;
    cout << "\n* Input NTBMSummary \n  " << ifname << endl ;
    TFile* f = TFile::Open( ifname, "read" );
 	if ( !f ) { exit(1) ; }
  	TTree* tree = (TTree*) f->Get("tree") ;
    NTBMSummary* n = 0 ;
    tree->SetBranchAddress( "NTBMSummary", &n );
    cout << "  Number of Entries : " << setw(10) << tree->GetEntries() << endl;
    for ( Int_t i = 0 ; i < tree->GetEntries() ; ++i ) {
    	tree->GetEntry(i) ;
		bool flag = false ;
		for ( Int_t icluster = 0 ; icluster < n->GetNumberOfNinjaClusters() ; ++icluster ) {
			if ( n->GetNumberOfHits(icluster, 0) == 0 || n->GetNumberOfHits(icluster, 1) == 0 ) { continue; }
			if ( !cut.isArea( n->GetNinjaPosition(icluster) ) ) { continue; }
            flag = true ;
		}
		if ( flag ) { v_nt.push_back( *n ) ; }
    	if ( i % 1000 == 0 ) { cout << "  Selected : " << setw(6) << v_nt.size() << " , Progress : " << setw(6) << i << " / " << tree->GetEntries() << "\r" ; }
    }
    cout << "  Selected : " << setw(6) << v_nt.size() << " , Progress : " << setw(9) << tree->GetEntries() << " / " << tree->GetEntries() << endl;
    f->Close();
	return v_nt ;
}

void OutputMatchingTrack( string ofname, vector<TMatchingTrack>& v_match )
{
	cout << "\n* Output\n  " << ofname << endl;
    TFile* of = TFile::Open(ofname.c_str(), "recreate");
	TTree *t_save = new TTree("t","t") ;
	TMatchingTrack match ;
    t_save->Branch( "match", "TMatchingTrack", &match ) ;
	for( auto p = v_match.cbegin() ; p != v_match.cend() ; ++p ) {
		match = *p ;
		t_save->Fill() ;
		if ( distance( v_match.cbegin(), p ) %1000 == 0 ) { cerr << "  " << setw(8) << distance( v_match.cbegin(), p ) << " / " << v_match.size() << "\r" ; }
	}
	cerr << "  " << setw(8) << t_save->GetEntries() << " / " << v_match.size() << endl ;
	t_save->Write() ;
	of->Close() ;
}