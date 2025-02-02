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
#include <TFile.h>
#include <TTree.h>
#include <TCut.h>
#include "TBaseTrackSpot.hpp"
#include "TMatchingTrack.hpp"
#include "NTBMSummary.hh"
ClassImp( TBaseTrackSpot ) ;
ClassImp( TMatchingTrack ) ;
ClassImp( NTBMSummary ) ;

class TimeOfSpot
{
public:
	TimeOfSpot()
	{
		time_start = 1700553600 ;  // 2023/11/21/17:00:00 (JST)
		time_5hour = 5 * 60 * 60 ; //E71b is 5hour Shift
		time_shift = -65.0 / 24 ;  // shifter time is 65sec. every 4 days. ( -65sec. / 24 spots )
		restart_spotA = 8 ;  // Shifter reset because of second beam exposure
		time_restart = 1707015600 ; //shifter restart 2024-02-04 12:00:00 (JST)
		time_buf = 300 ; // buffer time for matching
	}
	Int_t time_start ;
	Int_t time_5hour ;
	Double_t time_shift ;
	Int_t restart_spotA ;
	Int_t time_restart ;
	Int_t time_buf ;
    Int_t GetTimeStandard( Int_t spotA, Int_t spotB )
	//spotB = fast, spotA = slow
    {
		if ( spotA < restart_spotA ){
			return time_start + ( ( spotA - 1 ) * 24 + ( spotB - 1 ) ) * ( time_5hour ) ; //
		}else{
			return time_restart + ( ( spotA - 8 ) * 24 + ( spotB - 1 ) ) * ( time_5hour ) ; //after restart
		}
 
    }
	Int_t GetTimeMin( Int_t spotA, Int_t spotB )
	{
		if ( spotA < restart_spotA ) {
            Int_t total_shift = ((spotA-1.)*24.+(spotB-1.))*time_shift ; 
			return GetTimeStandard(spotA, spotB) + total_shift - time_buf ; 
		} else {
            Int_t total_shift = (((spotA-1.)*24.+(spotB-1.)) - 7.*24. )*time_shift ;
			return  GetTimeStandard(spotA, spotB) + total_shift - time_buf ;
		}
	}
	Int_t GetTimeMax( Int_t spotA, Int_t spotB )
    {
        return GetTimeMin( spotA, spotB ) + time_5hour + time_buf * 2 ;
    }
};

class Cut_NTBMSummary
{
private:
	Double_t centerY, centerX ;
	Double_t widthY, widthX ;
	Double_t time_min, time_max ;
public:
	Cut_NTBMSummary()
	{
		centerY = +50. ;
		centerX = -50. ;
		widthY = 1200. ;
		widthX = 1200. ;
		time_min = 1700553600 ;  // 2023/11/21/17:00:00 (JST) 
		time_max = 1709715600 ; //2024-03-06 18:00:00 (JST)
	}
    Cut_NTBMSummary( Int_t ecc )
	{
        if ( ecc == 5 ) {
		    centerY = +50. ;
		    centerX = -50. ;
		    widthY = 500. ;
		    widthX = 600. ;
		} else if ( ecc == 6 ) {
			centerY = +50. ;
		    centerX = -50. + 300. ;
		    widthY = 1000. ;
		    widthX = 600. ;
		} else if ( ecc == 4 ) {
			centerY = +50. ;
		    centerX = -50. - 300. ;
		    widthY = 500. ;
		    widthX = 600. ;
		} else if ( ecc == 2 ) {
			centerY = +50. + 340. ;
		    centerX = -50. ;
		    widthY = 500. ;
		    widthX = 600. ;
		} else if ( ecc == 3 ) {
			centerY = +50. + 340. ;
		    centerX = -50. + 300. ;
		    widthY = 500. ;
		    widthX = 600. ;
		} else if ( ecc == 1 ) {
			centerY = +50. + 340. ;
		    centerX = -50. - 300. ;
		    widthY = 500. ;
		    widthX = 600. ;
        } else {
            centerY = +50. ;
    		centerX = -50. ;
    		widthY = 1500. ;  // covering all area
    		widthX = 1500. ;
        }
		time_min = 1700553600 ;  // 2023/11/21/17:00:00 (JST) 
		time_max = 1709715600 ;//2024-03-06 18:00:00
	}
	void SetAreaCenter( Double_t centerY_new, Double_t centerX_new )
	{
		centerY = centerY_new;
		centerX = centerX_new ;
	}
	void SetAreaWidth( Double_t widthY_new, Double_t widthX_new )
	{
		widthY = widthY_new ;
		widthX = widthX_new ;
	}
	void SetTime( Double_t time_min_new, Double_t time_max_new )
	{
		time_min = time_min_new ;
		time_max = time_max_new ;
	}
	Double_t y_min() const { return centerY - widthY / 2. ; }
	Double_t y_max() const { return centerY + widthY / 2. ; }
	Double_t x_min() const { return centerX - widthX / 2. ; }
	Double_t x_max() const { return centerX + widthX / 2. ; }
	bool isArea( const vector<double> position ) const
	{
        if ( position.size() != 2 ) { cout << "  Error : position " << endl ; return false ; }
		if ( position.at(0) > y_min() && position.at(0) < y_max() && position.at(1) > x_min() && position.at(1) < x_max() ) { return true ; }
		return false ;
	}
	bool isTime( const double unixtime ) const
	{
		if ( unixtime > time_min && unixtime < time_max ) { return true ; }
		return false ;
	}
};

class PartitionOfNTBMSummary
{
public:
    PartitionOfNTBMSummary();
	PartitionOfNTBMSummary( Int_t n_spotA, Int_t n_spotB ) ;
	const Int_t n_spotA ;
	const Int_t n_spotB ;
	vector<vector<NTBMSummary>> v_v_ntbm ;
	vector<NTBMSummary>& GetSpotPartition( Int_t i_spotA, Int_t i_spotB ) {
        return v_v_ntbm.at( i_spotA * n_spotB + i_spotB );
    }
    Int_t SumOfEntries() const {
        Int_t sum = 0;
        for ( auto p = v_v_ntbm.cbegin() ; p != v_v_ntbm.cend() ; ++p ) {
            sum += p->size();
        }
        return sum ;
    }
};
PartitionOfNTBMSummary::PartitionOfNTBMSummary( Int_t n_spotA, Int_t n_spotB ) : n_spotA(n_spotA), n_spotB(n_spotB) { v_v_ntbm.resize( n_spotA * n_spotB ) ; }

class Shift_dxdy
{
public:
	Shift_dxdy()
	{
		dax = 0.000 ;//original
		day = 0.000 ;//original
		dx = 315. ;
		dy = 95. ;
	}
	Shift_dxdy( Int_t ecc )
	{
		dax = -0.0118 ;//original
		day = -0.0069 ;//original
		//dax = 0.100 ;
		//day = -0.300 ;
		//dax = 0.200 ;
		//day = -0.600 ;
		//dax = 0.078 ;//peak fix dax+=0.078
		//day = 0.072 ;//peak fix day+=0.072
		if ( ecc == 5 )      { dx = 315.;      dy = 95.; }
		//else if ( ecc == 6 ) { dx =  80.;      dy = 95.; } //original
		//else if ( ecc == 6 ) { dx =  65.;      dy = 110.; }
		//else if ( ecc == 6 ) { dx =  0.;      dy = 110.; }
		//else if ( ecc == 6 ) { dx =  87.;      dy = 85.; } //peak fix dx+=7, dy+=-10
		else if ( ecc == 6 ) { dx =  -217.;      dy = 103.; } //fix
		else if ( ecc == 4 ) { dx = 315.+270.; dy = 95.; }
		else if ( ecc == 2 ) { dx = 315.;      dy = 95. - 350.; }
		else if ( ecc == 3 ) { dx =  80.;      dy = 95. - 350.; }
		else if ( ecc == 1 ) { dx = 315.+270.; dy = 95. - 350.; }
		else if ( ecc == 8 ) { dx = 315.;      dy = 95. + 350.; }
		else if ( ecc == 9 ) { dx =  80.;      dy = 95. + 350.; }
		else if ( ecc == 7 ) { dx = 315.+270.; dy = 95. + 350.; }
		else { dx = 315.; dy = 95.; }
	}
	Double_t dx, dy ;
	Double_t dax, day ;
};


void shifterNT_matching( Int_t ecc, string shifter_file_name, string tracker_file_name, string ofname );
vector<TBaseTrackTrackerWall> InputBaseTrackTracerWall( string ifname );
PartitionOfNTBMSummary InputNTBMSummary( string ifname, Int_t ecc );
vector<TMatchingTrack> Matching( vector<TBaseTrackTrackerWall>& v_bt, PartitionOfNTBMSummary& part_ntbm, Int_t ecc ) ;
void OutputMatchingTrack( string ofname, vector<TMatchingTrack>& v_match ) ;

void shifterNT_matching( Int_t ecc )
{
    string shifter_file_name = "../data/bt061_spot.root";
    string tracker_file_name = "/home/nu/itoshun/data/ninja/nt_all_matched/nt_all_matched.root";
    string ofname = "ninja_shifter_tracker_matching_cutOriginal.root";
    shifterNT_matching( ecc, shifter_file_name, tracker_file_name, ofname );
}

void shifterNT_matching( Int_t ecc, string shifter_file_name, string tracker_file_name, string ofname )
{
	clock_t start_time = clock() ;
    cout << fixed << "\n === Matching Shifter & Tracker ===" << endl;

    //Input data in the Emulsion Film on the Tracker
    vector<TBaseTrackTrackerWall> v_bt = InputBaseTrackTracerWall( shifter_file_name ) ;
	clock_t file1_time = clock();
	cout << "  Processing Time : "<< setprecision(2) << (Double_t)( file1_time - start_time )/CLOCKS_PER_SEC << " sec." << endl ;

    //Input data of the NINJA Tracker & Partitioning by time
    PartitionOfNTBMSummary part_ntbm = InputNTBMSummary( tracker_file_name, ecc ) ;
	clock_t file2_time = clock();
	cout << "  Processing Time : "<< setprecision(2) << (Double_t)( file2_time - file1_time )/CLOCKS_PER_SEC << " sec." << endl ;

    //Matching
	vector<TMatchingTrack> v_match = Matching( v_bt, part_ntbm, ecc ) ;
	clock_t matching_time = clock();
	cout << "  Processing Time : "<< setprecision(2) << (Double_t)( matching_time - file2_time )/CLOCKS_PER_SEC << " sec." << endl ;

	//Output
	OutputMatchingTrack( ofname, v_match ) ;
	clock_t end_time = clock();
	cout << "  Processing Time : "<< setprecision(2) << (Double_t)( end_time - matching_time )/CLOCKS_PER_SEC << " sec." << endl ;
	cout << "\n* End\n  Total Processing Time : "<< setprecision(2) << (Double_t)( end_time - start_time )/CLOCKS_PER_SEC << " sec.\n" << endl ;
	
}

vector<TMatchingTrack> Matching( vector<TBaseTrackTrackerWall>& v_bt, PartitionOfNTBMSummary& part_ntbm, Int_t ecc )
{
	Double_t cut_da = 0.3 ;//original
	Double_t cut_dp = 60. ;//original
	//Double_t cut_da = 0.05 ;//smaller
	//Double_t cut_dp = 200. ;//larger
	//Double_t cut_dp = 10. ;
	Double_t dz_x = 37.0 ;
	Double_t dz_y = 17.0 ;
	//why dz_x and dz_y are different? 20241218

    cout << "\n* Matching" << endl;
	vector<TMatchingTrack> v_match ;
	Shift_dxdy shift( ecc ) ;
    for ( auto b = v_bt.begin() ; b != v_bt.end() ; ++b ) {
        if ( distance(v_bt.begin(), b) % 1000 == 0 ) {
            cout << "  Number of matching pairs : " << setw(9) << v_match.size() << " , Progress : " << setw(9) << distance(v_bt.begin(), b) << " / " << v_bt.size() << "\r";
        }
        if ( b->slow.spot < 1 || b->fast.spot < 1 ) { continue ; }
		BaseTrackInfo bt( b->slow.spot*100+b->fast.spot, b->pl, b->id, b->ax, b->ay, b->x/1000., b->y/1000., b->ph1, b->ph2 ) ;
        for ( auto n = part_ntbm.GetSpotPartition(b->slow.spot, b->fast.spot).begin() ; n != part_ntbm.GetSpotPartition(b->slow.spot, b->fast.spot).end() ; ++n ) {
            for ( Int_t icluster = 0 ; icluster < n->GetNumberOfNinjaClusters(); ++icluster ) {
                if ( n->GetNumberOfHits(icluster, 0) == 0 || n->GetNumberOfHits(icluster, 1) == 0 ) { continue; }
				if ( n->GetNinjaPosition(icluster, 1) - bt.x + shift.dx > 90. || n->GetNinjaPosition(icluster, 1) - bt.x + shift.dx < -90. ) { continue; }  //Rough cut to speed up.
				if ( n->GetNinjaPosition(icluster, 0) - bt.y + shift.dy > 90. || n->GetNinjaPosition(icluster, 0) - bt.y + shift.dy < -90. ) { continue; }
				if ( TMath::Abs( n->GetNinjaTangent(icluster, 1) - bt.ax + shift.dax ) > cut_da ) { continue; }
				if ( TMath::Abs( n->GetNinjaTangent(icluster, 0) - bt.ay + shift.day )  > cut_da ) { continue; }
				if ( TMath::Abs( n->GetNinjaPosition(icluster, 1) - ( bt.x + ( dz_x * bt.ax ) ) + shift.dx ) > cut_dp ) { continue; }
				if ( TMath::Abs( n->GetNinjaPosition(icluster, 0) - ( bt.y + ( dz_y * bt.ay ) ) + shift.dy ) > cut_dp ) { continue; }
                v_match.push_back(
					TMatchingTrack(
						NTBMTrackInfo(
							icluster,
							n->GetNinjaTangent(icluster, 1) + shift.dax,  //ax
							n->GetNinjaTangent(icluster, 0) + shift.day,  //ay
							n->GetNinjaPosition(icluster, 1) + shift.dx,  //x
							n->GetNinjaPosition(icluster, 0) + shift.dy   //y
						),
						bt,
						*b,
						*n
					)
				);
				// extrapolation is calculated in TMatchingTrack's constructor.
            }
        }
    }
    cout << "  Number of matching pairs : " << setw(9) << v_match.size() << " , Progress : " << setw(9) << v_bt.size() << " / " << v_bt.size() << endl;
	return v_match ;
}

vector<TBaseTrackTrackerWall> InputBaseTrackTracerWall( string ifname )
{
	cout << "\n* Input tracks in emulsion film on NINJA tracker\n  " << ifname << endl ;
	TFile *f = TFile::Open( ifname.c_str(), "read" ) ;
	if ( !f ) { exit(1) ; }
	TTree *t = (TTree*)f->Get("t") ;
	cout << "  Number of Entries : " << setw(10) << t->GetEntries() << endl ;
	TBaseTrackTrackerWall* p_bt = 0 ;
	t->SetBranchAddress( "bt", &p_bt ) ;
    vector<TBaseTrackTrackerWall> v_bt ; 
	for ( Int_t i = 0 ; i < t->GetEntries() ; ++i ) {
		t->GetEntry(i) ;
		v_bt.push_back( *p_bt ) ;
		if ( i % 1000 == 0 ) { cout << "  Progress : " << setw(9) << i << " / " << t->GetEntries() << "\r" ; }
	}
	cout << "  Progress : " << setw(9) << v_bt.size() << " / " << t->GetEntries() << endl ;
	f->Close() ;
    return v_bt ;
}

PartitionOfNTBMSummary InputNTBMSummary( string ifname, Int_t ecc )
{
    vector<NTBMSummary> ntbm_all ;
    {
        cout << "\n* Input \n  " << ifname << endl ;
        TFile* f = TFile::Open( ifname.c_str(), "read" );
    	if ( !f ) { exit(1) ; }
    	TTree* tree = (TTree*) f->Get("tree") ;
        NTBMSummary* n = 0 ;
	    tree->SetBranchAddress( "NTBMSummary", &n );
        cout << "  Number of Entries : " << setw(10) << tree->GetEntries() << endl;
        for ( Int_t i = 0 ; i < tree->GetEntries() ; ++i ) {
	    	tree->GetEntry(i) ;
	    	ntbm_all.push_back( *n ) ;
	    	if ( i % 1000 == 0 ) { cout << "  Progress : " << setw(9) << i << " / " << tree->GetEntries() << "\r" ; }
	    }
        cout << "  Progress : " << setw(9) << ntbm_all.size() << " / " << tree->GetEntries() << endl;
	    f->Close();
    }
    cout << "\n* Partitioning for shifter matching" << endl;
    TimeOfSpot time;
    PartitionOfNTBMSummary part_ntbm(26, 25);
    Cut_NTBMSummary cut(ecc);
	Int_t count = 0 ;
    for( Int_t i_spotA = 1 ; i_spotA < part_ntbm.n_spotA ; ++i_spotA ) {
	for( Int_t i_spotB = 1 ; i_spotB < part_ntbm.n_spotB ; ++i_spotB ) {
        cut.SetTime(time.GetTimeMin(i_spotA, i_spotB), time.GetTimeMax(i_spotA, i_spotB));
        for ( auto n = ntbm_all.begin() ; n != ntbm_all.end() ; ++n ) {
            if ( !cut.isTime( n->GetTimestamp() ) ) { continue ;}
            //if ( !n->GetBsdGoodSpillFlag() ) { continue; }
            //if ( !n->GetWagasciGoodSpillFlag() ) { continue; }
            bool flag = false ;
            for ( Int_t icluster = 0 ; icluster < n->GetNumberOfNinjaClusters() ; ++icluster ) {
                if ( n->GetNumberOfHits(icluster, 0) == 0 || n->GetNumberOfHits(icluster, 1) == 0 ) { continue; }
                if ( !cut.isArea( n->GetNinjaPosition(icluster) ) ) { continue; }
                flag = true ;
            }
            if ( flag ) { part_ntbm.GetSpotPartition(i_spotA, i_spotB).push_back( *n ); }
        }
        time_t time_start = time.GetTimeStandard( i_spotA, i_spotB );
		tm *tm_start = localtime( &time_start );
		//cout << "  spot(" << setw(2) << i_spotA << " , " << setw(2) << i_spotB << ") : "
		//     << setw(9) << part_ntbm.GetSpotPartition(i_spotA, i_spotB).size()
		//     << ", time : " << asctime(tm_start) ;
		++count ;
		cout << "  Partition : " << setw(3) << count  << " / " << 25*24 << "\r";
    }
    }
    cout << "  Partition : " << setw(3) << count << " / " << 25*24 << " --> Sum of filled : " << setw(6) <<part_ntbm.SumOfEntries() << endl ;
    return part_ntbm;
}

void OutputMatchingTrack( string ofname, vector<TMatchingTrack>& v_match )
{
	cout << "\n* Output" << endl;
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