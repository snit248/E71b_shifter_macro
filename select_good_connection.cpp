#include <iostream>
#include <iomanip>
#include <sstream>
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TError.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TLine.h>
#include <TLegend.h>
#include "TMatchingTrack.hpp"
ClassImp( TMatchingTrack ) ;

//parameters for fixed functions
Double_t pos_angle_border = 0.4 ;
Double_t pos_x_0 = 2.5 ;
Double_t pos_x_1 = 6.5 ;
Double_t pos_const_x = TMath::Hypot( pos_x_0, pos_x_1 * pos_angle_border ) ;
Double_t pos_y_0 = 2.5 ;
Double_t pos_y_1 = 6.5 ;
Double_t pos_const_y = TMath::Hypot( pos_y_0, pos_y_1 * pos_angle_border ) ;

Double_t ang_x_0 = 0.040 ;
Double_t ang_x_1 = 0.050 ;

Double_t ang_y_0 = 0.015 ;
Double_t ang_y_1 = 0.050 ;

//parameters for fitting
Double_t nbins_da = 50. ;
Double_t width_da = 0.5 ;
Double_t nbins_dp = 50. ;
Double_t width_dp = 50. ;
Double_t dax_center = 0. ;
Double_t day_center = 0. ;
Double_t dx_center = 0. ;
Double_t dy_center = 0. ;
//Double_t da_cut = 0.15 ;
//Double_t dp_cut = 10.0 ;
Double_t da_cut = 0.30 ;
Double_t dp_cut = 30.0 ;
Double_t nbins_ang = 100. ;
Double_t width_ang = 1.0 ;
TString str_cut_da = Form( "TMath::Abs(dax)<%.3lf&&TMath::Abs(day)<%.3lf", da_cut, da_cut ) ;
TCut cut_da = str_cut_da.Data() ;
TString str_cut_dp = Form( "TMath::Abs(dx)<%.1lf&&TMath::Abs(dy)<%.1lf", dp_cut, dp_cut ) ;
TCut cut_dp = str_cut_dp.Data() ;

class params_2
{
public:
    params_2(){}
    Double_t xA, xB;
    Double_t yA, yB;
};

class point_2
{
public:
	point_2(){}
	Double_t ax ,ay ,ax_error, ay_error;
	Double_t sdx, sdy, sdx_error, sdy_error; 
};

void SetStyle() ;
point_2 Draw_ang_dp_detail( TString psname, TCanvas* c1, TTree* t, Double_t ang_min, Double_t ang_max ) ;
point_2 Draw_ang_da_detail( TString psname, TCanvas* c1, TTree* t, Double_t ang_min, Double_t ang_max ) ;
params_2 plot_dp_detail( TTree* t ) ;
params_2 plot_da_detail( TTree* t ) ;

void select_good_connection( TString ifname = "ninja_shifter_bt063_tracker_matching.sig.root" )
{
    cout << fixed << "\n* Input matching data" << endl ;
	TFile *f = TFile::Open( ifname , "read" ) ;
	if ( !f ) { exit(1) ; }
	TTree *t = (TTree*)f->Get("t") ;
	cout << "  FileName : " << ifname << "\n  TreeName : " << t->GetName() << "\n  Entries : " << t->GetEntries() << endl ;
    params_2 param_pos = plot_dp_detail( t );
    params_2 param_ang = plot_da_detail( t );

    TMatchingTrack *match = 0 ;
	t->SetBranchAddress( "match", &match ) ;

	TString ofname = ifname.ReplaceAll(".root",".cll.root") ;
	TFile* of = TFile::Open(ofname, "recreate");
	TTree *t_save = new TTree("t","t") ;
	t_save->Branch( "match","TMatchingTrack", match ) ;

	Double_t cut_chi_square = 80. ;
	Double_t cut_lnk_chis = 20. ;
	cout << "*  Select Good Connection\n  cut_chi_square : "<< setprecision(1) << cut_chi_square << endl ; 
	TString psname = Form("Plot_bt063_matching_chi_square_cut%.0lf.pdf", cut_lnk_chis ) ;
	cout << "\n* Create Canvas \n  FileName : " << psname << endl ;
	TCanvas *c1 = new TCanvas("c1","c1",800,1200) ;
	c1->cd() ;
	c1->Print( psname + "[" , "pdf" ) ;
	TH2D *h_ang_chis = new TH2D("h_ang_chis",";angle;chi_square",200,0,2.0,200,0,60) ;
	TH2D *h_ang_chis_2 = new TH2D("h_ang_chis_2",";angle;chi_square",200,0,2.0,200,0,30) ;
	TH2D *h_dx_dax = new TH2D("h_dx_dax","dx - dax;dx/#sigma_{dx};dax/#sigma_{dax}",60,-6,6,60,-6,6) ;
	TH2D *h_dx_day = new TH2D("h_dx_day","dx - day;dx/#sigma_{dx};day/#sigma_{day}",60,-6,6,60,-6,6) ;
	TH2D *h_dy_day = new TH2D("h_dy_day","dy - day;dy/#sigma_{dy};day/#sigma_{day}",60,-6,6,60,-6,6) ;
	TH2D *h_dy_dax = new TH2D("h_dy_dax","dy - dax;dy/#sigma_{dy};dax/#sigma_{dax}",60,-6,6,60,-6,6) ;
	TH2D *h_dx_dy = new TH2D("h_dx_dy","dx - dy;dx/#sigma_{dx};dy/#sigma_{dy}",60,-6,6,60,-6,6) ;
	TH2D *h_dax_day = new TH2D("h_dax_day","dax - day;dax/#sigma_{dax};day/#sigma_{day}",60,-6,6,60,-6,6) ;
    for( Int_t i = 0 ; i < t->GetEntries() ; ++i )
    {
        t->GetEntry(i);
        Double_t sigma_dx ; 
        Double_t sigma_dy ; 
        Double_t sigma_dax ; 
        Double_t sigma_day ; 

		if ( TMath::Abs(match->bt.ax) < pos_angle_border ) { sigma_dx = TMath::Hypot( pos_x_0, pos_x_1 * match->bt.ax ) ; }
		else { sigma_dx = pos_const_x ; }
		if ( TMath::Abs(match->bt.ay) < pos_angle_border ) { sigma_dy = TMath::Hypot( pos_y_0, pos_y_1 * match->bt.ay ) ; }
		else { sigma_dy = pos_const_y ; }

		sigma_dax = TMath::Hypot( ang_x_0, ang_x_1 * match->bt.ax ) ;
		sigma_day = TMath::Hypot( ang_y_0, ang_y_1 * match->bt.ay ) ;
		printf("%lf, %lf, %lf, %lf\n",sigma_dx, sigma_dy, sigma_dax, sigma_day);

		Double_t chis_dx = (match->dx * match->dx) / (sigma_dx * sigma_dx);
		Double_t chis_dy = (match->dy * match->dy) / (sigma_dy * sigma_dy);
		Double_t chis_dax = (match->dax * match->dax) / (sigma_dax * sigma_dax);
		Double_t chis_day = (match->day * match->day) / (sigma_day * sigma_day);
		match->chi_square = chis_dx + chis_dy + chis_dax + chis_day ;

        if( match->chi_square > cut_chi_square ) { continue; } 
		t_save->Fill();
		//if( match->bt_tracker_wall.lnk.chi_square_4() > cut_lnk_chis ) { continue; }//original
		if( match->bt_tracker_wall.fast.chi_square_4() > cut_lnk_chis ) { continue; }
		h_ang_chis->Fill( match->bt_tracker_wall.angle(), match->chi_square ) ;
		h_ang_chis_2->Fill( match->bt_tracker_wall.angle(), match->chi_square ) ;
		h_dx_dax->Fill( match->dx / sigma_dx, match->dax / sigma_dax ) ;
		h_dx_day->Fill( match->dx / sigma_dx, match->day / sigma_day ) ;
		h_dy_day->Fill( match->dy / sigma_dy, match->day / sigma_day ) ;
		h_dy_dax->Fill( match->dy / sigma_dy, match->dax / sigma_dax ) ;
		h_dx_dy->Fill( match->dx / sigma_dx, match->dy / sigma_dy ) ;
		h_dax_day->Fill( match->dax / sigma_dax, match->day / sigma_day ) ;
    }

	c1->Clear();
	c1->Divide(1,2) ;
	c1->cd(1);
	h_ang_chis->Draw("colz");
	c1->Update();
	c1->cd(2);
	h_ang_chis_2->Draw("colz");
	c1->Update() ;
	c1->Print( psname ) ;

	c1->Clear();
	c1->Divide(2,3) ;
	c1->cd(1);
	c1->GetPad(1)->SetLeftMargin(0.15);
	c1->GetPad(1)->SetRightMargin(0.15);
	h_dx_dax->Draw("colz") ;
	c1->cd(2);
	c1->GetPad(2)->SetLeftMargin(0.15);
	c1->GetPad(2)->SetRightMargin(0.15);
	h_dx_day->Draw("colz") ;
	c1->cd(3);
	c1->GetPad(3)->SetLeftMargin(0.15);
	c1->GetPad(3)->SetRightMargin(0.15);
	h_dy_dax->Draw("colz") ;
	c1->cd(4);
	c1->GetPad(4)->SetLeftMargin(0.15);
	c1->GetPad(4)->SetRightMargin(0.15);
	h_dy_day->Draw("colz") ;
	c1->cd(5);
	c1->GetPad(5)->SetLeftMargin(0.15);
	c1->GetPad(5)->SetRightMargin(0.15);
	h_dx_dy->Draw("colz") ;
	c1->cd(6);
	c1->GetPad(6)->SetLeftMargin(0.15);
	c1->GetPad(6)->SetRightMargin(0.15);
	h_dax_day->Draw("colz") ;
	c1->Update() ;
	c1->Print( psname ) ;

	c1->Print( psname + "]" ) ;
	cout << endl ;

	cout << "  Saved : " << t_save->GetEntries() << endl;
	t_save->Write() ;
	of->Close() ;
}

params_2 plot_dp_detail( TTree* t )
{
	SetStyle() ;
	TString psname = "Plot_bt063_signal_dp_detail.pdf" ;
	cout << "\n* Create Canvas \n  FileName : " << psname << endl ;
	TCanvas *c1 = new TCanvas("c1","c1",800,800) ;
	c1->cd() ;
	c1->Print( psname + "[" , "pdf" ) ;
	cout << "* Drawing dp detail " << endl;

	gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.15);
	vector<point_2> v_point ;
	Draw_ang_dp_detail( psname, c1, t, 0.0, 1.0 );
	v_point.push_back( Draw_ang_dp_detail( psname, c1, t, 0.0, 0.1 ) );
	v_point.push_back( Draw_ang_dp_detail( psname, c1, t, 0.1, 0.2 ) );
	v_point.push_back( Draw_ang_dp_detail( psname, c1, t, 0.2, 0.3 ) );
	v_point.push_back( Draw_ang_dp_detail( psname, c1, t, 0.3, 0.4 ) );
	v_point.push_back( Draw_ang_dp_detail( psname, c1, t, 0.4, 0.5 ) );
	v_point.push_back( Draw_ang_dp_detail( psname, c1, t, 0.5, 0.6 ) );
	v_point.push_back( Draw_ang_dp_detail( psname, c1, t, 0.6, 0.8 ) );
	v_point.push_back( Draw_ang_dp_detail( psname, c1, t, 0.8, 1.2 ) );
	
	c1->Clear();
	//c1->SetLeftMargin(0.15);
	//c1->SetRightMargin(0.15);
	TGraphErrors* g_x = new TGraphErrors() ; 
	TGraphErrors* g_y = new TGraphErrors() ;
	for ( auto p = v_point.begin() ; p != v_point.end() ; ++p ) {
		Int_t i = distance( v_point.begin(), p ) ;
		g_x->SetPoint( i, p->ax, p->sdx ) ;
		g_x->SetPointError( i, p->ax_error, p->sdx_error ) ;
		g_y->SetPoint( i, p->ay, p->sdy ) ;
		g_y->SetPointError( i, p->ay_error, p->sdy_error ) ;
	}
	g_x->SetTitle(";tan#theta_{X/Y};#sigma(#delta(X/Y)) [mm]");
	g_x->SetMarkerStyle(20);
	g_y->SetMarkerStyle(20);
	g_x->SetMarkerColor(kRed);
	g_y->SetMarkerColor(kBlue);

	g_x->GetXaxis()->SetRangeUser(0.0,1.2);
	g_x->GetYaxis()->SetRangeUser(0.0, 20);
	g_x->Draw("ap");
	g_y->Draw("p");

    TF1* f_x = new TF1("f_x", "TMath::Hypot([0], [1]*x)", 0.0, 0.4 );
    f_x->SetParameter(0, pos_x_0);
    f_x->SetParameter(1, pos_x_1);
    f_x->SetLineColor(kRed);
	//f_x->Draw("same");
	TF1* func_fixed_x = new TF1("func_fixed_x", "[0]", 0.4, 1.2 );
	func_fixed_x->SetParameter(0, pos_const_x);
    func_fixed_x->SetLineColor(kRed+1);
	//func_fixed_x->Draw("same");
    //g_x->Fit(f_x);

    TF1* f_y = new TF1("f_y", "TMath::Hypot([0], [1]*x)", 0.0, 0.4 );
    f_y->SetParameter(0, pos_y_0);
    f_y->SetParameter(1, pos_y_1);
    f_y->SetLineColor(kBlue);
	//f_y->Draw("same");
	TF1* func_fixed_y = new TF1("func_fixed_y", "[0]", 0.4, 1.2 );
	func_fixed_y->SetParameter(0, pos_const_y);
    func_fixed_y->SetLineColor(kBlue+1);
	//func_fixed_y->Draw("same");
    //g_y->Fit(f_y);

	TLegend* leg = new TLegend(0.2 ,0.75 ,0.5,0.88);
	leg->SetFillColor(0);
	leg->SetBorderSize(1);
	leg->SetTextFont();
	leg->SetShadowColor(0);
	leg->SetTextAlign(22);
	leg->AddEntry(g_x,"X projection","P");
	leg->AddEntry(g_y,"Y projection","P");
	leg->Draw();
	c1->Update() ;
	c1->Print( psname ) ;

	c1->Print( psname + "]" ) ;
	cout << endl ;
    delete c1;

    params_2 p ;
    p.xA = f_x->GetParameter(0);
    p.xB = f_x->GetParameter(1);
    p.yA = f_y->GetParameter(0);
    p.yB = f_y->GetParameter(1);
    return p ;
}

params_2 plot_da_detail( TTree* t )
{
	SetStyle() ;
	TString psname = "Plot_bt063_signal_da_detail.pdf" ;
	cout << "\n* Create Canvas \n  FileName : " << psname << endl ;
	TCanvas *c1 = new TCanvas("c1","c1",800,800) ;
	c1->cd() ;
	c1->Print( psname + "[" , "pdf" ) ;
	cout << "* Drawing da detail" << endl;

	gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.15);
	vector<point_2> v_point ;
	Draw_ang_da_detail( psname, c1, t, 0.0, 1.0 );
	v_point.push_back( Draw_ang_da_detail( psname, c1, t, 0.0, 0.1 ) );
	v_point.push_back( Draw_ang_da_detail( psname, c1, t, 0.1, 0.2 ) );
	v_point.push_back( Draw_ang_da_detail( psname, c1, t, 0.2, 0.3 ) );
	v_point.push_back( Draw_ang_da_detail( psname, c1, t, 0.3, 0.4 ) );
	v_point.push_back( Draw_ang_da_detail( psname, c1, t, 0.4, 0.5 ) );
	v_point.push_back( Draw_ang_da_detail( psname, c1, t, 0.5, 0.6 ) );
	v_point.push_back( Draw_ang_da_detail( psname, c1, t, 0.6, 0.8 ) );
	v_point.push_back( Draw_ang_da_detail( psname, c1, t, 0.8, 1.2 ) );
	
	c1->Clear();
	c1->SetLeftMargin(0.15);
	c1->SetRightMargin(0.15);
	TGraphErrors* g_x = new TGraphErrors() ; 
	TGraphErrors* g_y = new TGraphErrors() ;
	for ( auto p = v_point.begin() ; p != v_point.end() ; ++p ) {
		Int_t i = distance( v_point.begin(), p ) ;
		g_x->SetPoint( i, p->ax, p->sdx ) ;
		g_x->SetPointError( i, p->ax_error, p->sdx_error ) ;
		g_y->SetPoint( i, p->ay, p->sdy ) ;
		g_y->SetPointError( i, p->ay_error, p->sdy_error ) ;
	}
	g_x->SetTitle(";tan#theta_{X/Y};#sigma(#delta(tan#theta_{X/Y}))");
	g_x->SetMarkerStyle(20);
	g_y->SetMarkerStyle(20);
	g_x->SetMarkerColor(kRed);
	g_y->SetMarkerColor(kBlue);

	g_x->GetXaxis()->SetRangeUser(0.0,1.2);
	g_x->GetYaxis()->SetRangeUser(0.0,0.2);
	g_x->Draw("ap");
	g_y->Draw("p");

    TF1* f_y = new TF1("f_y", "TMath::Hypot([0], [1]*x)", 0.0, 1.2 );
    f_y->SetParameter(0, ang_y_0);
    f_y->SetParameter(1, ang_y_1);
    f_y->SetLineColor(kBlue);
    //f_y->Draw("same");
	//g_y->Fit(f_y);

	TF1* f_x = new TF1("f_x", "TMath::Hypot([0], [1]*x)", 0.0, 1.2 );
    f_x->SetParameter(0, ang_x_0);
    f_x->SetParameter(1, ang_x_1);
    f_x->SetLineColor(kRed);
	//f_x->Draw("same");
	//g_x->Fit(f_x);

	TLegend* leg = new TLegend(0.2 ,0.75 ,0.5,0.88);
	leg->SetFillColor(0);
	leg->SetBorderSize(1);
	leg->SetTextFont();
	leg->SetShadowColor(0);
	leg->SetTextAlign(22);
	leg->AddEntry(g_x,"X projection","P");
	leg->AddEntry(g_y,"Y projection","P");
	leg->Draw();
	c1->Update() ;
	c1->Print( psname ) ;

	c1->Print( psname + "]" ) ;
	cout << endl ;
    delete c1;

    params_2 p ;
    p.xA = f_x->GetParameter(0);
    p.xB = f_x->GetParameter(1);
    p.yA = f_y->GetParameter(0);
    p.yB = f_y->GetParameter(1);
    return p ;
}

point_2 Draw_ang_dp_detail( TString psname, TCanvas* c1, TTree* t, Double_t ang_min, Double_t ang_max )
{
	TCut cut_slice_ax = Form( "(%lf)<TMath::Abs(bt.ax)&&TMath::Abs(bt.ax)<(%lf)", ang_min, ang_max );
	TCut cut_slice_ay = Form( "(%lf)<TMath::Abs(bt.ay)&&TMath::Abs(bt.ay)<(%lf)", ang_min, ang_max );
	TCut cut_day = Form( "TMath::Abs(day)<(%lf)", 0.06 );
	TCut cut_dax = Form( "TMath::Abs(dax)<(%lf)", 0.06 );
	TCut cut_dy = Form( "TMath::Abs(dy)<(%lf)", 6. );
	TCut cut_dx = Form( "TMath::Abs(dx)<(%lf)", 6. );
	//TCut cut_lnk_chis = Form( "bt_tracker_wall.lnk.chis_da_lat+bt_tracker_wall.lnk.chis_da_rad+bt_tracker_wall.lnk.chis_dp_lat < %.1lf", 30. ) ;//original
	TCut cut_lnk_chis = Form( "bt_tracker_wall.fast.chis_da_lat+bt_tracker_wall.fast.chis_da_rad+bt_tracker_wall.fast.chis_dp_lat < %.1lf", 30. ) ;
	TString title_ax = Form( "%.1lf < Abs(bt.ax) < %.1lf", ang_min, ang_max ) ;
	TString title_ay = Form( "%.1lf < Abs(bt.ay) < %.1lf", ang_min, ang_max ) ;

	c1->Clear() ;
	c1->Divide(2,2) ;
	c1->cd(1) ;
	gStyle->SetStatH(0.2) ;
	gStyle->SetStatW(0.15) ;
	gStyle->SetStatFontSize(0.02) ;
	c1->GetPad(1)->SetLeftMargin(0.15);
	c1->GetPad(1)->SetRightMargin(0.15);
	TH2D* h2d_ax_dx = new TH2D( "h2d_ax_dx", ";ax;dx", nbins_ang, -width_ang, width_ang, nbins_dp, dx_center - width_dp, dx_center + width_dp ) ;
	h2d_ax_dx->SetTitle( " ax - dx :  " + title_ax ) ;
	h2d_ax_dx->SetMarkerStyle(20) ;
	t->Draw( "dx:bt.ax>>h2d_ax_dx", cut_slice_ax&&cut_day&&cut_dy&&cut_da&&cut_dp&&cut_lnk_chis, "colz" ) ;
	//h2d_ax_dx->Fit("pol1", "Q", "", -width_ang, width_ang ) ;

	c1->cd(3) ;
	c1->GetPad(3)->SetLeftMargin(0.15);
	c1->GetPad(3)->SetRightMargin(0.15);
	TH1D* h1d_dx = new TH1D( "h1d_dx", ";dx", nbins_dp, dx_center - width_dp, dx_center + width_dp ) ;
	h1d_dx->SetFillColor(kRed);
	t->Draw( "dx>>h1d_dx", cut_slice_ax&&cut_day&&cut_dy&&cut_da&&cut_dp&&cut_lnk_chis, "" ) ;
	h1d_dx->Fit("gaus", "Q", "", dx_center - width_dp, dx_center + width_dp ) ;
    h1d_dx->GetFunction("gaus")->SetParameter(0, h1d_dx->GetBinContent(h1d_dx->GetMaximumBin()) ) ;
    h1d_dx->GetFunction("gaus")->FixParameter(1,0) ;
    h1d_dx->GetFunction("gaus")->SetParameter(2, h1d_dx->GetRMS() ) ;
	h1d_dx->Fit("gaus", "Q", "", dx_center - 3*h1d_dx->GetRMS(), dx_center + 3*h1d_dx->GetRMS() ) ;

	c1->cd(2) ;
	c1->GetPad(2)->SetLeftMargin(0.15);
	c1->GetPad(2)->SetRightMargin(0.15);
	TH2D* h2d_ay_dy = new TH2D( "h2d_ay_dy", ";ay;dy", nbins_ang, -width_ang, width_ang, nbins_dp, dy_center - width_dp, dy_center + width_dp ) ;
	h2d_ay_dy->SetTitle( " ay - dy :  " + title_ay ) ;
	h2d_ay_dy->SetMarkerStyle(20) ;
	t->Draw( "dy:bt.ay>>h2d_ay_dy", cut_slice_ay&&cut_dax&&cut_dx&&cut_da&&cut_dp&&cut_lnk_chis, "colz" ) ;

	c1->cd(4) ;
	c1->GetPad(4)->SetLeftMargin(0.15);
	c1->GetPad(4)->SetRightMargin(0.15);
	TH1D* h1d_dy = new TH1D( "h1d_dy", ";dy", nbins_dp, dy_center - width_dp, dy_center + width_dp ) ;
	h1d_dy->SetFillColor(kBlue);
	t->Draw( "dy>>h1d_dy", cut_slice_ay&&cut_dax&&cut_dx&&cut_da&&cut_dp&&cut_lnk_chis, "" ) ;
    h1d_dy->Fit("gaus", "Q", "", dy_center - width_dp, dy_center + width_dp ) ;
    h1d_dy->GetFunction("gaus")->SetParameter(0, h1d_dy->GetBinContent(h1d_dy->GetMaximumBin()) ) ;
    h1d_dy->GetFunction("gaus")->FixParameter(1,0) ;
    h1d_dy->GetFunction("gaus")->SetParameter(2, h1d_dy->GetRMS() ) ;
	h1d_dy->Fit("gaus", "Q", "", dy_center - 3*h1d_dy->GetRMS(), dy_center + 3*h1d_dy->GetRMS() ) ;

	c1->Update() ;
	c1->Print( psname ) ;

	point_2 p ;
	p.ax = p.ay = ( ang_min + ang_max ) / 2. ;
	p.ax_error = p.ay_error = ( ang_max - ang_min ) / 2. ;
	p.sdx = h1d_dx->GetFunction("gaus")->GetParameter(2) ;
	p.sdx_error = h1d_dx->GetFunction("gaus")->GetParError(2) ;
	p.sdy = h1d_dy->GetFunction("gaus")->GetParameter(2) ;
	p.sdy_error = h1d_dy->GetFunction("gaus")->GetParError(2) ;

	delete h2d_ax_dx;
	delete h1d_dx;
	delete h2d_ay_dy;
	delete h1d_dy;

	return p ;
}

point_2 Draw_ang_da_detail( TString psname, TCanvas* c1, TTree* t, Double_t ang_min, Double_t ang_max )
{
	TCut cut_slice_ax = Form( "(%lf)<TMath::Abs(bt.ax)&&TMath::Abs(bt.ax)<(%lf)", ang_min, ang_max );
	TCut cut_slice_ay = Form( "(%lf)<TMath::Abs(bt.ay)&&TMath::Abs(bt.ay)<(%lf)", ang_min, ang_max );
	TCut cut_day = Form( "TMath::Abs(day)<(%lf)", 0.06 );
	TCut cut_dax = Form( "TMath::Abs(dax)<(%lf)", 0.06 );
	TCut cut_dy = Form( "TMath::Abs(dy)<(%lf)", 6. );
	TCut cut_dx = Form( "TMath::Abs(dx)<(%lf)", 6. );
	//TCut cut_lnk_chis = Form( "bt_tracker_wall.lnk.chis_da_lat+bt_tracker_wall.lnk.chis_da_rad+bt_tracker_wall.lnk.chis_dp_lat < %.1lf", 30. ) ;//original
	TCut cut_lnk_chis = Form( "bt_tracker_wall.fast.chis_da_lat+bt_tracker_wall.fast.chis_da_rad+bt_tracker_wall.fast.chis_dp_lat < %.1lf", 30. ) ;
	TString title_ax = Form( "%.1lf < Abs(bt.ax) < %.1lf", ang_min, ang_max ) ;
	TString title_ay = Form( "%.1lf < Abs(bt.ay) < %.1lf", ang_min, ang_max ) ;

	c1->Clear() ;
	c1->Divide(2,2) ;
	c1->cd(1) ;
	gStyle->SetStatH(0.2) ;
	gStyle->SetStatW(0.15) ;
	gStyle->SetStatFontSize(0.02) ;
	c1->GetPad(1)->SetLeftMargin(0.15);
	c1->GetPad(1)->SetRightMargin(0.15);
	TH2D* h2d_ax_dx = new TH2D( "h2d_ax_dx", ";ax;dax", nbins_ang, -width_ang, width_ang, nbins_da, dax_center - width_da, dax_center + width_da ) ;
	h2d_ax_dx->SetTitle( " ax - dax :  " + title_ax ) ;
	h2d_ax_dx->SetMarkerStyle(20) ;
	t->Draw( "dax:bt.ax>>h2d_ax_dx", cut_slice_ax&&cut_day&&cut_dy&&cut_da&&cut_dp&&cut_lnk_chis, "colz" ) ;

	c1->cd(3) ;
	c1->GetPad(3)->SetLeftMargin(0.15);
	c1->GetPad(3)->SetRightMargin(0.15);
	TH1D* h1d_dx = new TH1D( "h1d_dx", ";dax", nbins_da, dax_center - width_da, dax_center + width_da ) ;
	h1d_dx->SetFillColor(kRed);
	t->Draw( "dax>>h1d_dx", cut_slice_ax&&cut_day&&cut_dy&&cut_da&&cut_dp&&cut_lnk_chis, "" ) ;
	h1d_dx->Fit("gaus", "Q", "", dax_center - width_da, dax_center + width_da ) ;
    h1d_dx->GetFunction("gaus")->SetParameter(0, h1d_dx->GetBinContent(h1d_dx->GetMaximumBin()) ) ;
    h1d_dx->GetFunction("gaus")->FixParameter(1,0) ;
    h1d_dx->GetFunction("gaus")->SetParameter(2, h1d_dx->GetRMS() ) ;
	h1d_dx->Fit("gaus", "Q", "", dax_center - 3*h1d_dx->GetRMS(), dax_center + 3*h1d_dx->GetRMS() ) ;

	c1->cd(2) ;
	c1->GetPad(2)->SetLeftMargin(0.15);
	c1->GetPad(2)->SetRightMargin(0.15);
	TH2D* h2d_ay_dy = new TH2D( "h2d_ay_dy", ";ay;day", nbins_ang, -width_ang, width_ang, nbins_da, day_center - width_da, day_center + width_da ) ;
	h2d_ay_dy->SetTitle( " ay - day :  " + title_ay ) ;
	h2d_ay_dy->SetMarkerStyle(20) ;
	t->Draw( "day:bt.ay>>h2d_ay_dy", cut_slice_ay&&cut_dax&&cut_dx&&cut_da&&cut_dp&&cut_lnk_chis, "colz" ) ;
	
	c1->cd(4) ;
	c1->GetPad(4)->SetLeftMargin(0.15);
	c1->GetPad(4)->SetRightMargin(0.15);
	TH1D* h1d_dy = new TH1D( "h1d_dy", ";day", nbins_da, day_center - width_da, day_center + width_da ) ;
	h1d_dy->SetFillColor(kBlue);
	t->Draw( "day>>h1d_dy", cut_slice_ay&&cut_dax&&cut_dx&&cut_da&&cut_dp&&cut_lnk_chis, "" ) ;
	h1d_dy->Fit("gaus", "Q", "", day_center - width_da, day_center + width_da ) ;
    h1d_dy->GetFunction("gaus")->SetParameter(0, h1d_dy->GetBinContent(h1d_dy->GetMaximumBin()) ) ;
    h1d_dy->GetFunction("gaus")->FixParameter(1,0) ;
    h1d_dy->GetFunction("gaus")->SetParameter(2, h1d_dy->GetRMS() ) ;
	h1d_dy->Fit("gaus", "Q", "", day_center - 3*h1d_dy->GetRMS(), day_center + 3*h1d_dy->GetRMS() ) ;

	c1->Update() ;
	c1->Print( psname ) ;

	point_2 p ;
	p.ax = p.ay = ( ang_min + ang_max ) / 2. ;
	p.ax_error = p.ay_error = ( ang_max - ang_min ) / 2. ;
	p.sdx = h1d_dx->GetFunction("gaus")->GetParameter(2) ;
	p.sdx_error = h1d_dx->GetFunction("gaus")->GetParError(2) ;
	p.sdy = h1d_dy->GetFunction("gaus")->GetParameter(2) ;
	p.sdy_error = h1d_dy->GetFunction("gaus")->GetParError(2) ;

	delete h2d_ax_dx;
	delete h1d_dx;
	delete h2d_ay_dy;
	delete h1d_dy;

	return p ;
}

void SetStyle()
{
	gErrorIgnoreLevel = 2000 ;
	gStyle->SetGridColor(17) ;
	gStyle->SetPadGridX(1) ;
	gStyle->SetPadGridY(1) ;
	gStyle->SetOptStat("emrou") ;
	gStyle->SetStatX(0.85) ;
	gStyle->SetStatY(0.90) ;
	gStyle->SetStatH(0.5) ;
	gStyle->SetStatW(0.2) ;
	gStyle->SetStatFontSize(0.03) ;
	gStyle->SetOptFit(1) ;
}