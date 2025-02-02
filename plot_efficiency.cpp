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
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphErrors.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TError.h>
#include <TLatex.h>
#include "NTBMSummary.hh"
#include "TMatchingTrack.hpp"
ClassImp( TMatchingTrack ) ;
ClassImp( NTBMSummary ) ;

class Sharing_file {
public:
	Sharing_file()
	{
		pl=-1; ecc_id=-1; fixedwall_id=-1; trackerwall_id=-1; spotid=-1;
		zone[0]=-1; zone[1]=-1; rawid[0]=-1; rawid[1]=-1; unix_time=-1; tracker_track_id=-1; }
	int32_t pl, ecc_id, fixedwall_id, trackerwall_id, spotid, zone[2], rawid[2], unix_time, tracker_track_id;
};

class Cut_NTBMSummary
{
public:
	Double_t centerY, centerX ;
	Double_t widthY, widthX ;
	Int_t min_noh, max_noh ;

	Cut_NTBMSummary()
	{
		centerY = +66. ;
		centerX = +343. ;
		widthY = 340. ;
		widthX = 250. ;
		min_noh = 2 ;
		max_noh = 4 ;
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
	bool isNumberOfHit( const vector<int> noh ) const
	{
        if ( noh.size() != 2 ) { cout << "  Error : noh " << endl ; return false ; }
		if ( noh.at(0) >= min_noh && noh.at(0) <= max_noh && noh.at(1) >= min_noh && noh.at(1) <= max_noh ) { return true ; }
		return false ;
	}
};

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
		if ( spotA == restart_spotA - 1 && spotB == 24 ) {  return GetTimeMin( spotA, spotB ) + time_5hour + time_buf * 2 + 400 ; }
        return GetTimeMin( spotA, spotB ) + time_5hour + time_buf * 2 ;
    }
	Int_t GetSpotId( Int_t unix_time )
	{
		bool flag = false ;
		Int_t spotA = -1 , spotB = -1 ;
		for ( Int_t i_spotA = 1 ; i_spotA <= 25 ; ++i_spotA ) {
		for ( Int_t i_spotB = 1 ; i_spotB <= 24 ; ++i_spotB ) {
			if ( unix_time > GetTimeMin( i_spotA, i_spotB ) && unix_time < GetTimeMax( i_spotA, i_spotB ) ) {
				spotA = i_spotA ;
				spotB = i_spotB ;
				flag = true ;
				break ;
			}
		}
		if ( flag ) { break ; }
		}
		if ( !flag ) { cout << "Error : TimeOfSpot::GetSpotId() : unix_time = " << unix_time << endl; }
		return 100*spotA + spotB ;
	}
};

void SetStyle() ;
void Write_sharing_file_txt(std::string filename, std::vector<Sharing_file>&sharing_file_v) ;
void OutputSharingFile( string ofname, vector<TMatchingTrack> v_mt ) ;

void plot_efficiency( TString ifname = "efficiencybt063_file.root")
{
	cout << fixed << " == Plot Efficiency == " << endl;
	vector<TMatchingTrack> v_mt ;
	//Double_t cut_chi2 = 60. ;
	cout << "\n* Input matching data\n  " << ifname << endl ;
	TFile *f = TFile::Open( ifname , "read" ) ;
	if ( !f ) { exit(1) ; }
	TTree *t = (TTree*)f->Get("t") ;
	TMatchingTrack* mt = 0 ;
	t->SetBranchAddress( "match", &mt ) ;
	cout << "  Number of Entries : " << setw(10) << t->GetEntries() << endl;
	//cout << "  Cut chi2 : " << setprecision(1) << cut_chi2 << endl;
	for ( Int_t i = 0 ; i < t->GetEntries() ; ++i ) {
    	t->GetEntry(i) ;
		//if ( mt->chi_square > cut_chi2 ) { continue ; }
		v_mt.push_back( *mt ) ;
    	if ( i % 1000 == 0 ) { cout << "  Selected : " << setw(6) << v_mt.size() << " , Progress : " << setw(9) << i << " / " << t->GetEntries() << "\r" ; }
    }
    cout << "  Selected : " << setw(6) << v_mt.size() << " , Progress : " << setw(9) << t->GetEntries() << " / " << t->GetEntries() << endl;
	
	Double_t time_start = 1700553600. ; // 0:00, 5th,Nov.,2019 (JST)
	Double_t width_time = 107 * 24 * 60 * 60 ; 
	Double_t nbins_time = 107 ;
	Double_t time_half = time_start + width_time / 2. ;
	TimeOfSpot time ;

	Cut_NTBMSummary cut ;
	Int_t max_cluster = 5 ;
	Double_t min_mom = 500. ;
	//TString str_cut_chi2 = Form( " #chi^{2} < %.1lf  ,", cut_chi2 ) ;
	TString str_select_area = "*Area selection : center of the NINJA detecter 30cm square ." ;
	TString str_select_sand_muon = "*Sand muon selection : track_type == 1 ." ;
	TString str_select_nhit = Form( "*Number of hit selection : %d <= NoH <= %d for both side .", cut.min_noh, cut.max_noh ) ;
	TString str_select_ncluster = Form( "*Number of cluster selection : NoC <= %d .", max_cluster ) ;
	TString str_select_momentum = Form( "*Momentum selection : Muon momentum >= %.0lf  .", min_mom ) ;
	TString str_select_time = "*Timing of event selection : half latter of the beam time ." ;
	TCanvas *c1 = new TCanvas("c1","c1",900,800) ;
	ofstream ofs("eff.txt") ;

	for ( Int_t mode = 0 ; mode <= 5 ; ++mode ){
		TString str_mode = Form("%d", mode) ;
		SetStyle() ;
		TString psname = "Plot_efficiencybt063_mode_" + str_mode + ".pdf" ;
		cout << "\n* Create Canvas \n  FileName : " << psname << endl ;
		
		c1->Clear() ;
		c1->cd() ;
		c1->Print( psname + "[" , "pdf" ) ;

		TH1D* h1d_time_eff = new TH1D( "h1d_time_eff"+str_mode, "Number of events per day ( connected )", nbins_time, time_start, time_start + width_time ) ;
		TH1D* h1d_time_ineff = new TH1D( "h1d_time_ineff"+str_mode, "Number of events per day  ( unconnected )", nbins_time, time_start, time_start + width_time ) ;
		TH1D* h1d_time_eff_seg = new TH1D( "h1d_time_eff_seg"+str_mode, "time of day ( connected );time of day", 24*6 , 0, 24 ) ;
		TH1D* h1d_time_ineff_seg = new TH1D( "h1d_time_ineff_seg"+str_mode, "time of day ( unconnected );time of day", 24*6, 0 , 24 ) ;

		TH2D* h2d_ang_eff = new TH2D( "h2d_ang_eff"+str_mode, "angle ( connected );ax;ay", 300, -1.5, 1.5, 300, -1.5, 1.5 ) ;
		TH2D* h2d_ang_ineff = new TH2D( "h2d_ang_ineff"+str_mode, "angle ( unconnected);ax;ay", 300, -1.5, 1.5, 300, -1.5, 1.5 ) ;
		//TH2D* h2d_pos_eff = new TH2D( "h2d_pos_eff"+str_mode, "position ( connected );x;y", 265, -560., 500., 255, -460., 560. ) ;
		TH2D* h2d_pos_eff = new TH2D( "h2d_pos_eff"+str_mode, "position ( connected );x;y", 40, 265., 385., 55, -40., 180. ) ;
		//TH2D* h2d_pos_ineff = new TH2D( "h2d_pos_ineff"+str_mode, "position ( unconnected );x;y", 265, -560., 500., 255, -460., 560. ) ;
		TH2D* h2d_pos_ineff = new TH2D( "h2d_pos_ineff"+str_mode, "position ( unconnected );x;y", 40, 265., 385., 55, -40., 180. ) ;

		TH1D* h1d_nohx_eff = new TH1D( "h1d_nohx_eff"+str_mode, "Number of Hits x-projection ( connected );number of hits", 20, 0., 20. ) ;
		TH1D* h1d_nohx_ineff = new TH1D( "h1d_nohx_ineff"+str_mode, "Number of Hits x-projection ( unconnected );number of hits", 20, 0., 20. ) ;
		TH1D* h1d_nohy_eff = new TH1D( "h1d_nohy_eff"+str_mode, "Number of Hits y-projection ( connected );number of hits", 20, 0., 20. ) ;
		TH1D* h1d_nohy_ineff = new TH1D( "h1d_nohy_ineff"+str_mode, "Number of Hits y-projection ( unconnected );number of hits", 20, 0., 20. ) ;

		TH1D* h1d_noc_eff = new TH1D( "h1d_noc_eff"+str_mode, "Number of Clusters ( connected );number of clusters", 20, 0., 20. ) ;
		TH1D* h1d_noc_ineff = new TH1D( "h1d_noc_ineff"+str_mode, "Number of Clusters ( unconnected );number of clusters", 20, 0., 20. ) ;
		TH1D* h1d_noh_tot_eff = new TH1D( "h1d_noh_tot_eff"+str_mode, "Number of Total Hits ( connected );number of hits", 40, 0., 40. ) ;
		TH1D* h1d_noh_tot_ineff = new TH1D( "h1d_noh_tot_ineff"+str_mode, "Number of Total Hits ( unconnected );number of hits", 40, 0., 40. ) ;

		TH1D* h1d_not_eff = new TH1D( "h1d_not_eff"+str_mode, "Number of BabyMIND tracks ( connected );number of tracks", 20, 0., 20. ) ;
		TH1D* h1d_not_ineff = new TH1D( "h1d_not_ineff"+str_mode, "Number of BabyMIND tracks ( unconnected );number of tracks", 20, 0., 20. ) ;

		TH1D* h1d_pe_bd0_eff = new TH1D( "h1d_pe_bd0_eff"+str_mode, "Sum of p.e. ( ADC trigger ) ( connected );number of p.e.", 100, 0., 200. ) ;
		TH1D* h1d_pe_bd0_mean_eff = new TH1D( "h1d_pe_bd0_mean_eff"+str_mode, "Mean of p.e. ( ADC trigger ) ( connected );number of p.e.", 100, 0., 100. ) ;
		TH1D* h1d_pe_bd1_eff = new TH1D( "h1d_pe_bd1_eff"+str_mode, "Sum of Time over Threshold ( TDC trigger ) ( connected );number of ToT", 100, 0., 1000. ) ;
		TH1D* h1d_pe_bd1_mean_eff = new TH1D( "h1d_pe_bd1_mean_eff"+str_mode, "Mean of Time over Threshold ( TDC trigger ) ( connected );number of ToT", 100, 0., 500. ) ;

		TH1D* h1d_pe_bd0_ineff = new TH1D( "h1d_pe_bd0_ineff"+str_mode, "Sum of p.e. ( ADC trigger ) ( unconnected );number of p.e.", 100, 0., 200. ) ;
		TH1D* h1d_pe_bd0_mean_ineff = new TH1D( "h1d_pe_bd0_mean_ineff"+str_mode, "Mean of p.e. ( ADC trigger ) ( unconnected );number of p.e.", 100, 0., 100. ) ;
		TH1D* h1d_pe_bd1_ineff = new TH1D( "h1d_pe_bd1_ineff"+str_mode, "Sum of Time over Theshold ( TDC trigger ) ( unconnected );number of ToT", 100, 0., 1000. ) ;
		TH1D* h1d_pe_bd1_mean_ineff = new TH1D( "h1d_pe_bd1_mean_ineff"+str_mode, "Mean of Time over Theshold ( TDC trigger ) ( unconnected );number of ToT", 100, 0., 500. ) ;

		TH2D* h2d_ang_map_pred = new TH2D( "h2d_ang_map_pred"+str_mode, ";ax;ay", 10, -1., 1., 10, -1., 1. ) ;
		TH2D* h2d_ang_map_eff = new TH2D( "h2d_ang_map_eff"+str_mode, "angle - efficiency map;ax;ay", 10, -1., 1., 10, -1., 1. ) ;
		TH1D* h1d_ang_map_pred = new TH1D( "h1d_ang_map_pred"+str_mode, ";angle", 10, 0, 1.) ;
		TH1D* h1d_ang_map_eff = new TH1D( "h1d_ang_map_eff"+str_mode, "angle - efficiency ;angle", 10, 0, 1.) ;
		TH1D* h1d_ang_map_deg_pred = new TH1D( "h1d_ang_map_deg_pred"+str_mode, ";angle", 9, 0, 45.) ;
		TH1D* h1d_ang_map_deg_eff = new TH1D( "h1d_ang_map_deg_eff"+str_mode, "angle - efficiency ;angle", 9, 0, 45.) ;

		TH2D* h2d_spot_map_pred = new TH2D( "h2d_spot_map_pred"+str_mode, ";slow;fast", 27, -0.5, 26.5, 27, -0.5, 26.5 ) ;
		TH2D* h2d_spot_map_eff = new TH2D( "h2d_spot_map_eff"+str_mode, "efficiency of each spot;slow;fast", 27, -0.5, 26.5, 27, -0.5, 26.5 ) ;
		TH1D* h1d_spot_map_pred = new TH1D( "h1d_spot_map_pred"+str_mode, ";slow", 27, -0.5, 26.5 ) ;
		TH1D* h1d_spot_map_eff = new TH1D( "h1d_spot_map_eff"+str_mode, "efficiency of slow spot;slow", 27, -0.5, 26.5 ) ;

		TH1D* h1d_mom_eff = new TH1D( "h1d_mom_eff"+str_mode, "Muon momentum ( connected events );Momentum [MeV]", 12, -200., 2200. ) ;
		TH1D* h1d_mom_ineff = new TH1D( "h1d_mom_ineff"+str_mode, "Muon momentum ( inefficiency events );Momentum [MeV]", 12, -200., 2200. ) ;
		TH1D* h1d_mom_map_pred = new TH1D( "h1d_mom_map_pred"+str_mode, ";Momentum [MeV]", 12, -200., 2200. ) ;
		TH1D* h1d_mom_map_eff = new TH1D( "h1d_mom_map_eff"+str_mode, "momentum dependence of efficiency;Momentum [MeV]", 12, -200., 2200. ) ;

		vector<TMatchingTrack> v_mt_eff, v_mt_ineff ;
		for ( auto m = v_mt.begin() ; m != v_mt.end() ; ++m )
		{
			Double_t momentum = -1000. ;
			//if ( m->baby_mind_momentum() > 0 ) { momentum = m->baby_mind_momentum() ; }
			if ( m->momentum() > 0 ) { momentum = m->momentum() ; }
			else if ( m->baby_mind_maximum_plane() > 15 ) { momentum = 2000. ; }
			else { momentum = -1 ; }

			if ( mode >= 1 && m->track_type() != 1 ) { continue ; }
			if ( mode >= 2 && !cut.isNumberOfHit( m->ntbm_summary.GetNumberOfHits( m->nt.icluster ) ) ) { continue ; }
			if ( mode >= 3 && m->ntbm_summary.GetNumberOfNinjaClusters() > max_cluster ) { continue ; }
			if ( mode >= 4 && momentum < min_mom ) { continue ; }
			if ( mode >= 5 && m->unix_time() < time_half ) { continue ; }

			h2d_ang_map_pred->Fill( m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 1), m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 0) ) ;
			h1d_ang_map_pred->Fill( TMath::Hypot( m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 1), m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 0) ) ) ;
			h1d_ang_map_deg_pred->Fill( TMath::ATan( TMath::Hypot( m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 1), m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 0) ) ) * 180. / TMath::Pi() ) ;
			Int_t spot_id = time.GetSpotId( m->unix_time() ) ;
			h2d_spot_map_pred->Fill( spot_id/100, spot_id%100 ) ;
			h1d_spot_map_pred->Fill( spot_id/100 ) ;
			h1d_mom_map_pred->Fill( momentum ) ;
			if ( m->bt.id >= 0 ) {
				h1d_time_eff->Fill( m->unix_time() ) ;
				h1d_time_eff_seg->Fill( (Double_t)( ( m->unix_time() - (Int_t)time_start ) % ( 24 * 60 * 60 ) ) / ( 60. * 60. ) ) ;
				h2d_ang_eff->Fill( m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 1), m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 0) ) ;
				h2d_ang_map_eff->Fill( m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 1), m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 0) ) ;
				h1d_ang_map_eff->Fill( TMath::Hypot( m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 1), m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 0) ) ) ;
				h1d_ang_map_deg_eff->Fill( TMath::ATan( TMath::Hypot( m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 1), m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 0) ) ) * 180. / TMath::Pi() ) ;
				h2d_spot_map_eff->Fill( spot_id/100, spot_id%100 ) ;
				h1d_spot_map_eff->Fill( spot_id/100 ) ;
				h1d_mom_map_eff->Fill( momentum ) ; 
				h2d_pos_eff->Fill( m->ntbm_summary.GetNinjaPosition(m->nt.icluster, 1), m->ntbm_summary.GetNinjaPosition( m->nt.icluster, 0 ) ) ;
				h1d_nohx_eff->Fill( m->ntbm_summary.GetNumberOfHits( m->nt.icluster, 1 ) ) ;
				h1d_nohy_eff->Fill( m->ntbm_summary.GetNumberOfHits( m->nt.icluster, 0 ) ) ;
				h1d_noc_eff->Fill( m->ntbm_summary.GetNumberOfNinjaClusters() ) ;
				h1d_not_eff->Fill( m->ntbm_summary.GetNumberOfTracks() ) ;
				h1d_mom_eff->Fill( momentum ) ;
				if ( m->ntbm_summary.GetBunchDifference( m->nt.icluster ) == 0 ) {
					Int_t count_pe = 0 ;
					for ( Int_t ihit = 0 ; ihit < m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 1) ; ++ihit ) { count_pe += m->ntbm_summary.GetPe( m->nt.icluster, 1, ihit ) ; }
					for ( Int_t ihit = 0 ; ihit < m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 0) ; ++ihit ) { count_pe += m->ntbm_summary.GetPe( m->nt.icluster, 0, ihit ) ; }
					h1d_pe_bd0_eff->Fill( count_pe ) ;
					Double_t sum_of_hits = m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 1) + m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 0) ;
					h1d_pe_bd0_mean_eff->Fill( (Double_t)count_pe / sum_of_hits ) ;
				} else {
					Int_t count_pe = 0 ;
					for ( Int_t ihit = 0 ; ihit < m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 1) ; ++ihit ) { count_pe += m->ntbm_summary.GetPe( m->nt.icluster, 1, ihit ) ; }
					for ( Int_t ihit = 0 ; ihit < m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 0) ; ++ihit ) { count_pe += m->ntbm_summary.GetPe( m->nt.icluster, 0, ihit ) ; }
					h1d_pe_bd1_eff->Fill( count_pe ) ;
					Double_t sum_of_hits = m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 1) + m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 0) ;
					h1d_pe_bd1_mean_eff->Fill( (Double_t)count_pe / sum_of_hits ) ;
				}
				h1d_noh_tot_eff->Fill( m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 0) + m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 1) ) ;
				v_mt_eff.push_back( *m ) ;
			}
			else {
				h1d_time_ineff->Fill( m->unix_time() ) ;
				h1d_time_ineff_seg->Fill( (Double_t)( ( m->unix_time() - (Int_t)time_start ) % ( 24 * 60 * 60 ) ) / ( 60. * 60. ) ) ;
				h2d_ang_ineff->Fill( m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 1), m->ntbm_summary.GetNinjaTangent(m->nt.icluster, 0) ) ;
				h2d_pos_ineff->Fill( m->ntbm_summary.GetNinjaPosition(m->nt.icluster, 1), m->ntbm_summary.GetNinjaPosition( m->nt.icluster, 0 ) ) ;
				h1d_nohx_ineff->Fill( m->ntbm_summary.GetNumberOfHits( m->nt.icluster, 1 ) ) ;
				h1d_nohy_ineff->Fill( m->ntbm_summary.GetNumberOfHits( m->nt.icluster, 0 ) ) ;
				h1d_noc_ineff->Fill( m->ntbm_summary.GetNumberOfNinjaClusters() ) ;
				h1d_not_ineff->Fill( m->ntbm_summary.GetNumberOfTracks() ) ;
				h1d_mom_ineff->Fill( momentum ) ;
				if ( m->ntbm_summary.GetBunchDifference( m->nt.icluster ) == 0 ) {
					Int_t count_pe = 0 ;
					for ( Int_t ihit = 0 ; ihit < m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 1) ; ++ihit ) { count_pe += m->ntbm_summary.GetPe( m->nt.icluster, 1, ihit ) ; }
					for ( Int_t ihit = 0 ; ihit < m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 0) ; ++ihit ) { count_pe += m->ntbm_summary.GetPe( m->nt.icluster, 0, ihit ) ; }
					h1d_pe_bd0_ineff->Fill( count_pe ) ;
					Double_t sum_of_hits = m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 1) + m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 0) ;
					h1d_pe_bd0_mean_ineff->Fill( (Double_t)count_pe / sum_of_hits ) ;
				} else {
					Int_t count_pe = 0 ;
					for ( Int_t ihit = 0 ; ihit < m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 1) ; ++ihit ) { count_pe += m->ntbm_summary.GetPe( m->nt.icluster, 1, ihit ) ; }
					for ( Int_t ihit = 0 ; ihit < m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 0) ; ++ihit ) { count_pe += m->ntbm_summary.GetPe( m->nt.icluster, 0, ihit ) ; }
					h1d_pe_bd1_ineff->Fill( count_pe ) ;
					Double_t sum_of_hits = m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 1) + m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 0) ;
					h1d_pe_bd1_mean_ineff->Fill( (Double_t)count_pe / sum_of_hits ) ;
				}
				h1d_noh_tot_ineff->Fill( m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 0) + m->ntbm_summary.GetNumberOfHits(m->nt.icluster, 1) ) ;
				v_mt_ineff.push_back( *m ) ;
			}
		}
		cout << "  tracks : " << v_mt_eff.size() + v_mt_ineff.size() << endl ;
		cout << "  eff events : " << v_mt_eff.size() << "  , ineff events : " << v_mt_ineff.size() << endl;
		Double_t eff = (Double_t)v_mt_eff.size() / (Double_t)(v_mt_eff.size() + v_mt_ineff.size()) ;
		cout << "  Connection Efficiency : " << setw(8) << setprecision(4) << eff << " ( " << eff*100 << " % )" << endl;

		c1->Clear() ;
		TLatex latex ;
		latex.SetTextSize(0.04) ;
		latex.DrawLatex( 0.1, 0.8, Form( "Mode %d : ", mode ) ) ;
		latex.DrawLatex( 0.4, 0.8, Form( "Efficiency : %.4lf ( %d / %d ) ", eff, (Int_t)v_mt_eff.size(), (Int_t)(v_mt_eff.size() + v_mt_ineff.size()) ) ) ;
		//latex.DrawLatex( 0.5, 0.88, str_cut_chi2.Data() ) ;
		if ( mode >= 0 ) { latex.DrawLatex(0.05,0.7, str_select_area.Data() ) ; }
		if ( mode >= 1 ) { latex.DrawLatex(0.05,0.6, str_select_sand_muon.Data() ) ; }
		if ( mode >= 2 ) { latex.DrawLatex(0.05,0.5, str_select_nhit.Data() ) ; }
		if ( mode >= 3 ) { latex.DrawLatex(0.05,0.4, str_select_ncluster.Data() ) ; }
		if ( mode >= 4 ) { latex.DrawLatex(0.05,0.3, str_select_momentum.Data() ) ; }
		if ( mode >= 5 ) { latex.DrawLatex(0.05,0.2, str_select_time.Data() ) ; }
		c1->Print(psname) ;

		c1->Clear() ;
		c1->Divide(1,2);
		c1->cd(1);
		h1d_time_eff->SetFillColor( kYellow ) ;
		h1d_time_eff->GetXaxis()->SetTimeDisplay(1) ;
		h1d_time_eff->GetXaxis()->SetTimeFormat("#splitline{%d/%m}{/%Y}");
		h1d_time_eff->SetLabelSize(0.045,"xy");
		h1d_time_eff->GetXaxis()->SetLabelOffset(0.03);
		h1d_time_eff->Draw();
		c1->cd(2);
		h1d_time_ineff->SetFillColor( kYellow+1 ) ;
		h1d_time_ineff->GetXaxis()->SetTimeDisplay(1) ;
		h1d_time_ineff->GetXaxis()->SetTimeFormat("#splitline{%d/%m}{/%Y}");
		h1d_time_ineff->SetLabelSize(0.045,"xy");
		h1d_time_ineff->GetXaxis()->SetLabelOffset(0.03);
		h1d_time_ineff->Draw();
		c1->Update() ;
		c1->Print( psname ) ;

		c1->Clear() ;
		c1->Divide(1,2);
		c1->cd(1);
		h1d_time_eff_seg->SetFillColor( kOrange ) ;
		h1d_time_eff_seg->SetLabelSize(0.045,"xy");
		h1d_time_eff_seg->SetTitleSize(0.045,"xy");
		h1d_time_eff_seg->SetMinimum(0);
		h1d_time_eff_seg->Draw();
		c1->cd(2);
		h1d_time_ineff_seg->SetFillColor( kOrange+1 ) ;
		h1d_time_ineff_seg->SetLabelSize(0.045,"xy");
		h1d_time_ineff_seg->SetTitleSize(0.045,"xy");
		h1d_time_ineff_seg->SetMinimum(0);
		h1d_time_ineff_seg->Draw();
		c1->Update() ;
		c1->Print( psname ) ;

		c1->Clear() ;
		c1->Divide(2,2);
		c1->cd(1);
		h2d_ang_eff->Draw("colz") ;
		c1->cd(2);
		h2d_ang_eff->SetFillColor( kCyan ) ;
		h2d_ang_eff->ProjectionY()->Draw() ;
		c1->cd(3);
		h2d_ang_ineff->Draw("colz") ;
		c1->cd(4);
		h2d_ang_ineff->SetFillColor( kCyan+1 ) ;
		h2d_ang_ineff->ProjectionY()->Draw() ;
		c1->Update() ;
		c1->Print( psname ) ;

		c1->Clear();
		c1->Divide(2,2);
		c1->cd(1);
		h2d_pos_eff->Draw("colz") ;
		c1->cd(2);
		h2d_pos_eff->SetFillColor( kGreen ) ;
		h2d_pos_eff->ProjectionX()->Draw() ;
		c1->cd(3);
		h2d_pos_ineff->Draw("colz") ;
		c1->cd(4);
		h2d_pos_ineff->SetFillColor( kGreen+1 ) ;
		h2d_pos_ineff->ProjectionX()->Draw();
		c1->Update() ;
		c1->Print( psname ) ;

		gStyle->SetOptStat("eo") ;
		c1->Clear();
		c1->Divide(2,2);
		c1->cd(1);
		h1d_noc_eff->SetFillColor(kYellow);
		h1d_noc_eff->Draw();
		c1->cd(2);
		h1d_not_eff->SetFillColor(kCyan);
		h1d_not_eff->Draw();
		c1->cd(3);
		h1d_noc_ineff->SetFillColor(kYellow+1);
		h1d_noc_ineff->Draw();
		c1->cd(4);
		h1d_not_ineff->SetFillColor(kCyan+1);
		h1d_not_ineff->Draw();
		c1->Update() ;
		c1->Print( psname ) ;

		c1->Clear();
		c1->Divide(2,2);
		c1->cd(1);
		h1d_nohx_eff->SetFillColor(kGreen);
		h1d_nohx_eff->Draw();
		c1->cd(2);
		h1d_nohy_eff->SetFillColor(kCyan);
		h1d_nohy_eff->Draw();
		c1->cd(3);
		h1d_nohx_ineff->SetFillColor(kGreen+1);
		h1d_nohx_ineff->Draw();
		c1->cd(4);
		h1d_nohy_ineff->SetFillColor(kCyan+1);
		h1d_nohy_ineff->Draw();
		c1->Update() ;
		c1->Print( psname ) ;
/*
		c1->Clear();
		c1->Divide(2,2);
		c1->cd(1);
		h1d_noh_tot_eff->SetFillColor(kYellow);
		h1d_noh_tot_eff->Draw();
		c1->cd(2);
		c1->cd(3);
		h1d_noh_tot_ineff->SetFillColor(kYellow+1);
		h1d_noh_tot_ineff->Draw();
		c1->cd(4);
		c1->Update() ;
		c1->Print( psname ) ;
*/
		c1->Clear();
		c1->Divide(2,2);
		c1->cd(1);
		h1d_pe_bd0_eff->SetFillColor(kGreen);
		h1d_pe_bd0_eff->Draw();
		c1->cd(2);
		h1d_pe_bd1_eff->SetFillColor(kCyan);
		h1d_pe_bd1_eff->Draw();
		c1->cd(3);
		h1d_pe_bd0_mean_eff->SetFillColor(kGreen);
		h1d_pe_bd0_mean_eff->Draw();
		c1->cd(4);
		h1d_pe_bd1_mean_eff->SetFillColor(kCyan);
		h1d_pe_bd1_mean_eff->Draw();
		c1->Update() ;
		c1->Print( psname ) ;

		c1->Clear();
		c1->Divide(2,2);
		c1->cd(1);
		h1d_pe_bd0_ineff->SetFillColor(kGreen+1);
		h1d_pe_bd0_ineff->Draw();
		c1->cd(2);
		h1d_pe_bd1_ineff->SetFillColor(kCyan+1);
		h1d_pe_bd1_ineff->Draw();
		c1->cd(3);
		h1d_pe_bd0_mean_ineff->SetFillColor(kGreen+1);
		h1d_pe_bd0_mean_ineff->Draw();
		c1->cd(4);
		h1d_pe_bd1_mean_ineff->SetFillColor(kCyan+1);
		h1d_pe_bd1_mean_ineff->Draw();
		c1->Update() ;
		c1->Print( psname ) ;

		c1->Clear();
		gStyle->SetOptStat(0) ;
		c1->Divide(2,2);
		c1->cd(1);
		h2d_ang_map_eff->Divide(h2d_ang_map_pred) ;
		h2d_ang_map_eff->SetMinimum(0.0) ;
		h2d_ang_map_eff->SetMaximum(1.0) ;
		h2d_ang_map_eff->Draw("colz") ;
		c1->cd(2);
		h1d_ang_map_eff->Divide(h1d_ang_map_pred)  ;
		h1d_ang_map_eff->SetMinimum(0.0) ;
		h1d_ang_map_eff->SetMaximum(1.0) ;
		//h1d_ang_map_eff->SetLineColor(kRed);
		//h1d_ang_map_eff->Draw() ;
		TGraphErrors* g_ang = new TGraphErrors() ;
		for( Int_t i = 0 ; i < h1d_ang_map_eff->GetNbinsX() ; ++i ) {
			Double_t eff_i = h1d_ang_map_eff->GetBinContent( i+1 ) ;
			g_ang->SetPoint( i, h1d_ang_map_eff->GetBinCenter( i+1 ), eff_i ) ;
			g_ang->SetPointError( i, h1d_ang_map_eff->GetBinWidth( i+1 )/2., TMath::Sqrt( eff_i*(1.0-eff_i) / h1d_ang_map_pred->GetBinContent( i+1 ) ) ) ;
			if ( mode == 1 ) {
				ofs << std::right << std::fixed
					<< std::setw(8) << std::setprecision(3) << h1d_ang_map_eff->GetBinCenter( i+1 )
					<< std::setw(8) << std::setprecision(5) << eff_i
					<< std::setw(8) << std::setprecision(5) << TMath::Sqrt( eff_i*(1.0-eff_i) / h1d_ang_map_pred->GetBinContent( i+1 ) )
					<< endl;
			}
		}
		g_ang->SetTitle(";angle; efficiency") ;
		g_ang->SetMarkerStyle(20);
		g_ang->SetMarkerColor(kRed+1);
		g_ang->GetXaxis()->SetRangeUser(0.0, 1.0);
		g_ang->GetYaxis()->SetRangeUser(0.0, 1.0);
		g_ang->Draw("ap");
		c1->cd(4);
		h1d_ang_map_deg_eff->Divide(h1d_ang_map_deg_pred) ;
		h1d_ang_map_deg_eff->SetMinimum(0.0) ;
		h1d_ang_map_deg_eff->SetMaximum(1.0) ;
		TGraphErrors* g_ang_deg = new TGraphErrors() ;
		for( Int_t i = 0 ; i < h1d_ang_map_deg_eff->GetNbinsX() ; ++i ) {
			Double_t eff_i = h1d_ang_map_deg_eff->GetBinContent( i+1 ) ;
			g_ang_deg->SetPoint( i, h1d_ang_map_deg_eff->GetBinCenter( i+1 ), eff_i ) ;
			g_ang_deg->SetPointError( i, h1d_ang_map_deg_eff->GetBinWidth( i+1 )/2., TMath::Sqrt( eff_i*(1.0-eff_i) / h1d_ang_map_deg_pred->GetBinContent( i+1 ) ) ) ;
		}
		g_ang_deg->SetTitle(";Angle [deg]; efficiency") ;
		g_ang_deg->SetMarkerStyle(20);
		g_ang_deg->SetMarkerColor(kRed+1);
		g_ang_deg->GetXaxis()->SetRangeUser(0.0, 45.0);
		g_ang_deg->GetYaxis()->SetRangeUser(0.0, 1.0);
		g_ang_deg->Draw("ap");
		c1->Update() ;
		c1->Print( psname ) ;

		c1->Clear();
		c1->Divide(2,2);
		c1->cd(1);
		h2d_spot_map_eff->Divide(h2d_spot_map_pred) ;
		h2d_spot_map_eff->SetMinimum(0.0) ;
		h2d_spot_map_eff->SetMaximum(1.0) ;
		h2d_spot_map_eff->Draw("colz");
		c1->cd(2);
		h1d_spot_map_eff->Divide(h1d_spot_map_pred) ;
		h1d_spot_map_eff->SetMinimum(0.0) ;
		h1d_spot_map_eff->SetMaximum(1.0) ;
		h1d_spot_map_eff->SetLineColor(kRed);
		h1d_spot_map_eff->Draw() ;
		c1->cd(3);
		TGraphErrors* g_first_half = new TGraphErrors() ;
		{
			Int_t count_point = 0 ;
			for( Int_t i = 3 ; i <= 9 ; ++i ) {
				Double_t eff_i = h1d_spot_map_eff->GetBinContent( i ) ;
				g_first_half->SetPoint( count_point, h1d_spot_map_eff->GetBinCenter( i ), eff_i ) ;
				g_first_half->SetPointError( count_point++, h1d_spot_map_eff->GetBinWidth( i )/2., TMath::Sqrt( eff_i*(1.0-eff_i) / h1d_spot_map_pred->GetBinContent( i ) ) ) ;
			}
		}
		g_first_half->SetTitle("first half;slow;efficiency") ;
		g_first_half->SetMarkerStyle(21);
		g_first_half->SetMarkerColor(kYellow+1);
		g_first_half->GetXaxis()->SetRangeUser(0.0, 12.0);
		g_first_half->GetYaxis()->SetRangeUser(0.0, 1.0);
		g_first_half->Draw("ap");
		c1->cd(4);
		TGraphErrors* g_latter_half = new TGraphErrors() ;
		{
			Int_t count_point = 0 ;
			for( Int_t i = 20 ; i <= 26 ; ++i ) {
				Double_t eff_i = h1d_spot_map_eff->GetBinContent( i ) ;
				g_latter_half->SetPoint( count_point, h1d_spot_map_eff->GetBinCenter( i ), eff_i ) ;
				g_latter_half->SetPointError( count_point++, h1d_spot_map_eff->GetBinWidth( i )/2., TMath::Sqrt( eff_i*(1.0-eff_i) / h1d_spot_map_pred->GetBinContent( i ) ) ) ;
			}
		}
		g_latter_half->SetTitle("latter half;slow;efficiency") ;
		g_latter_half->SetMarkerStyle(21);
		g_latter_half->SetMarkerColor(kYellow+1);
		g_latter_half->GetXaxis()->SetRangeUser(15.0, 27.0);
		g_latter_half->GetYaxis()->SetRangeUser(0.0, 1.0);
		g_latter_half->Draw("ap");
		c1->Update() ;
		c1->Print( psname ) ;

		gStyle->SetOptStat("eou") ;
		gStyle->SetStatX(0.4) ;
		c1->Clear();
		c1->Divide(2,2);
		c1->cd(1);
		h1d_mom_eff->SetFillColor( kCyan ) ;
		h1d_mom_eff->Draw() ;
		c1->cd(3);
		h1d_mom_ineff->SetFillColor( kCyan+1 ) ;
		h1d_mom_ineff->Draw() ;
		c1->Update();
		c1->cd(2);
		h1d_mom_map_eff->Divide(h1d_mom_map_pred) ;
		h1d_mom_map_eff->SetMinimum(0.0) ;
		h1d_mom_map_eff->SetMaximum(1.0) ;
		h1d_mom_map_eff->SetLineColor(kRed);
		h1d_mom_map_eff->Draw() ;
		gStyle->SetOptStat(0) ;
		c1->cd(4);
		TGraphErrors* g_mom = new TGraphErrors() ;
		for( Int_t i = 0 ; i < h1d_mom_map_eff->GetNbinsX() ; ++i ) {
			Double_t eff_i = h1d_mom_map_eff->GetBinContent( i+1 ) ;
			g_mom->SetPoint( i, h1d_mom_map_eff->GetBinCenter( i+1 ), eff_i ) ;
			g_mom->SetPointError( i, h1d_mom_map_eff->GetBinWidth( i+1 )/2., TMath::Sqrt( eff_i*(1.0-eff_i) / h1d_mom_map_pred->GetBinContent( i+1 ) ) ) ;
		}
		g_mom->SetTitle(";Muon momentum [MeV]; efficiency") ;
		g_mom->SetMarkerStyle(20);
		g_mom->SetMarkerColor(kCyan+1);
		g_mom->GetXaxis()->SetLimits(-200., 2200.0);
		g_mom->GetYaxis()->SetRangeUser(0.0, 1.0);
		g_mom->Draw("ap");
		c1->Update();
		c1->Print( psname ) ;

		c1->Print( psname + "]" ) ;
		f->Close();
		if ( mode == 0 )
		{
			OutputSharingFile( "sharing_file_connected.txt", v_mt_eff ) ;
			OutputSharingFile( "sharing_file_unconnected.txt", v_mt_ineff ) ;
		}
	}
}

void OutputSharingFile( string ofname, vector<TMatchingTrack> v_mt )
{
	vector<Sharing_file> v_sf ;
	for ( auto m = v_mt.begin() ; m != v_mt.end() ; ++m )
	{
		Sharing_file sf;
		sf.trackerwall_id = m->bt.id ;
		sf.spotid = m->bt.spot_id ;
		sf.unix_time = m->unix_time() ;
		sf.tracker_track_id = m->ntbm_summary.GetBabyMindTrackId( m->nt.icluster ) ;
		v_sf.push_back( sf ) ;
	}
	Write_sharing_file_txt( ofname, v_sf ) ;
}

void Write_sharing_file_txt(std::string filename, std::vector<Sharing_file>&sharing_file_v) {
	std::ofstream ofs(filename);
	if (!ofs) {

		fprintf(stderr, "File[%s] is not exist!!\n", filename.c_str());
		exit(1);
	}
	if (sharing_file_v.size() == 0) {
		fprintf(stderr, "target data ... null\n");
		fprintf(stderr, "File[%s] has no text\n", filename.c_str());
	}
	else {
		int64_t count = 0;
		int64_t max = sharing_file_v.size();

		for (int i = 0; i < max; i++) {
			if (count % 10000 == 0) {
				std::cerr << std::right << std::fixed << "\r now writing ..." << std::setw(4) << std::setprecision(1) << count * 100. / sharing_file_v.size() << "%%";
			}
			count++;
			ofs << std::right << std::fixed
				<< std::setw(3) << std::setprecision(0) << sharing_file_v[i].pl << " "
				<< std::setw(10) << std::setprecision(0) << sharing_file_v[i].ecc_id << " "
				<< std::setw(10) << std::setprecision(0) << sharing_file_v[i].fixedwall_id << " "
				<< std::setw(10) << std::setprecision(0) << sharing_file_v[i].trackerwall_id << " "
				<< std::setw(4) << std::setprecision(0) << sharing_file_v[i].spotid << " "
				<< std::setw(2) << std::setprecision(0) << sharing_file_v[i].zone[0] << " "
				<< std::setw(10) << std::setprecision(0) << sharing_file_v[i].rawid[0] << " "
				<< std::setw(2) << std::setprecision(0) << sharing_file_v[i].zone[1] << " "
				<< std::setw(10) << std::setprecision(0) << sharing_file_v[i].rawid[1] << " "
				<< std::setw(10) << std::setprecision(0) << sharing_file_v[i].unix_time << " "
				<< std::setw(3) << std::setprecision(0) << sharing_file_v[i].tracker_track_id << std::endl;
		}
		std::cerr << std::right << std::fixed << "\r now writing ..." << std::setw(4) << std::setprecision(1) << count * 100. / sharing_file_v.size() << "%%" << std::endl;
	}
}

void SetStyle()
{
	gErrorIgnoreLevel = 2000 ;
	gStyle->SetGridColor(17) ;
	gStyle->SetPadGridX(1) ;
	gStyle->SetPadGridY(1) ;
	gStyle->SetOptStat("e") ;
	gStyle->SetStatX(0.9) ;
	gStyle->SetStatY(0.9) ;
	gStyle->SetStatH(0.5) ;
	gStyle->SetStatW(0.2) ;
	gStyle->SetStatFontSize(0.04) ;
	gStyle->SetOptFit(1) ;
	gStyle->SetTimeOffset(0) ;
}
