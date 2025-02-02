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
#include <TLine.h>
#include "TMatchingTrack.hpp"
ClassImp( TMatchingTrack ) ;

Double_t nbins_da = 50. ;
Double_t width_da = 0.5 ;
Double_t nbins_dp = 50. ;
Double_t width_dp = 50. ;
Double_t dax_center = 0.0 ;
Double_t day_center = 0.0 ;
Double_t dx_center = 0.0 ;
Double_t dy_center = 0.0 ;
//Double_t da_cut = 0.30 ;//original
//Double_t dp_cut = 30.0 ;//original
Double_t da_cut = 0.15 ;
Double_t dp_cut = 50.0 ;
Double_t nbins_x = 100 ;
Double_t x_min = -10. ;
Double_t x_max = 260. ;
Double_t nbins_y = 70. ;
Double_t y_min = -10. ;
Double_t y_max = 350. ;
Double_t nbins_ang = 100. ;
Double_t width_ang = 1.0 ;
Double_t time_start = 1700553600. ;
Double_t width_time = 107 * 24 * 60 * 60 ; 
Double_t nbins_time = 107 ;
TString str_cut_da = Form( "TMath::Hypot(dax-(%.3lf),day-(%.3lf))<%.3lf", dax_center, day_center,  da_cut ) ;
TCut cut_da = str_cut_da.Data() ;
TString str_cut_dp = Form( "TMath::Hypot(dx-(%.1lf),dy-(%.1lf))<%.1lf", dx_center, dy_center, dp_cut ) ;
TCut cut_dp = str_cut_dp.Data() ;

void SetStyle() ;
void Draw_dp( TString psname, TCanvas* c1, TTree* t ) ;
void Draw_da( TString psname, TCanvas* c1, TTree* t ) ;
void Draw_pos( TString psname, TCanvas* c1, TTree* t ) ;
void Draw_ang( TString psname, TCanvas* c1, TTree* t ) ;
void Draw_time( TString psname, TCanvas* c1, TTree* t ) ;
void Draw_ang_dp( TString psname, TCanvas* c1, TTree* t ) ;
void Draw_ang_da( TString psname, TCanvas* c1, TTree* t ) ;
void Draw_pos_dp( TString psname, TCanvas* c1, TTree* t ) ;
void Draw_pos_da( TString psname, TCanvas* c1, TTree* t ) ;

void cut_signal_roughly( TString ifname = "ninja_shifter_tracker_matching_cutOriginal.root" )
{
	cout << "\n* Input matching data" << endl ;
	TFile *f = TFile::Open( ifname , "read" ) ;
	if ( !f ) { exit(1) ; }
	TTree *t = (TTree*)f->Get("t") ;
	cout << "  FileName : " << ifname << "\n  TreeName : " << t->GetName() << "\n  Entries : " << t->GetEntries() << endl ;
	SetStyle() ;
	TString psname = "Plot_" + ifname + "hardAng.pdf" ;
	cout << "\n* Create Canvas \n  FileName : " << psname << endl ;
	TCanvas *c1 = new TCanvas("c1","c1",800,800) ;
	c1->cd() ;
	c1->Print( psname + "[" , "pdf" ) ;
	cout << "\n* Drawing " << endl;
	Draw_dp( psname, c1, t ) ; cout << "  draw_dp" << endl ;
	Draw_da( psname, c1, t ) ; cout << "  draw_da" << endl ;
	Draw_pos( psname, c1, t ) ; cout << "  draw_pos" << endl ;
	Draw_ang( psname, c1, t ) ; cout << "  draw_ang" << endl ;
	Draw_time( psname, c1, t ) ; cout << "  draw_time" << endl ;
	//Draw_ang_dp( psname, c1, t ) ; cout << "  draw_ang_dp" << endl ;
	//Draw_ang_da( psname, c1, t ) ; cout << "  draw_ang_da" << endl ;
	//Draw_pos_dp( psname, c1, t ) ; cout << "  draw_pos_dp" << endl ;
	//Draw_pos_da( psname, c1, t ) ; cout << "  draw_pos_da" << endl ;
	c1->Print( psname + "]" ) ;
	cout << endl ;
	TString ofname = ifname.ReplaceAll(".root", ".sig.root") ;
	cout << "\n* Output signal data" << endl ;
	TFile *of = TFile::Open( ofname, "recreate" ) ;
	TTree* t_save = (TTree*)t->CopyTree( cut_da&&cut_dp ) ;
	cout << "  FileName : " << ofname << "\n  TreeName : " << t_save->GetName() << "\n  Entries : " << t_save->GetEntries() << endl ;
	t_save->Write() ;
	of->Close() ;
}

void SetStyle()
{
	gErrorIgnoreLevel = 2000 ;
	gStyle->SetGridColor(17) ;
	gStyle->SetPadGridX(1) ;
	gStyle->SetPadGridY(1) ;
	gStyle->SetOptStat("emou") ;
	gStyle->SetStatX(0.9) ;
	gStyle->SetStatY(0.9) ;
	gStyle->SetStatH(0.5) ;
	gStyle->SetStatW(0.2) ;
	gStyle->SetStatFontSize(0.03) ;
	gStyle->SetOptFit(1) ;
}

void Draw_dp( TString psname, TCanvas* c1, TTree* t )
{
	c1->Clear() ;
	c1->Divide(2,2) ;
	c1->cd(1) ;
	TH2D* h2d_dp = new TH2D( "h2d_dp", ";dx;dy", nbins_dp, dx_center - width_dp, dx_center + width_dp, nbins_dp, dy_center - width_dp, dy_center + width_dp ) ;
	h2d_dp->SetTitle( "dp histogram {" + str_cut_da +"}" ) ;
	t->Draw( "dy:dx>>h2d_dp", cut_da , "colz" ) ;

	c1->cd(2) ;
	TH1D* h1d_dy = new TH1D( "h1d_dy", ";dy", nbins_dp, dy_center - width_dp, dy_center + width_dp ) ;
	h1d_dy->SetFillColor(kBlue) ;
	TString str_cut_proj_dy = Form( "TMath::Abs(dx-(%0.1lf))<%.1lf", dx_center, dp_cut ) ;
	TCut cut_proj_dy = str_cut_proj_dy.Data() ;
	h1d_dy->SetTitle( "dy : " + str_cut_proj_dy ) ;
	t->Draw( "dy>>h1d_dy", cut_da&&cut_proj_dy , "colz" ) ;
	h1d_dy->Fit("gaus", "Q", "", dy_center - dp_cut, dy_center + dp_cut ) ;

	c1->cd(3) ;
	TH1D* h1d_dx = new TH1D( "h1d_dx", ";dx", nbins_dp, dx_center - width_dp, dx_center + width_dp ) ;
	h1d_dx->SetFillColor(kRed) ;
	TString str_cut_proj_dx = Form( "TMath::Abs(dy-(%0.1lf))<%.1lf", dy_center, dp_cut ) ;
	TCut cut_proj_dx = str_cut_proj_dx.Data() ;
	h1d_dx->SetTitle( "dx : " + str_cut_proj_dx ) ;
	t->Draw( "dx>>h1d_dx", cut_da&&cut_proj_dx , "colz" ) ;
	h1d_dx->Fit("gaus", "Q", "", dx_center - dp_cut, dx_center + dp_cut ) ;

	c1->cd(4) ;
	TH1D* h1d_dp = new TH1D( "h1d_dp", ";d_pos", nbins_dp, 0, width_dp ) ;
	TString str_h1d_dp = Form("TMath::Hypot(dx-(%.1lf),dy-(%.1lf))", dx_center, dy_center ) ;
	h1d_dp->SetTitle( str_h1d_dp ) ;
	h1d_dp->SetFillColor(kYellow) ;
	h1d_dp->SetMinimum(0);
	t->Draw( str_h1d_dp + ">>h1d_dp", cut_da , "" ) ;
	TLine* line = new TLine( dp_cut, 0, dp_cut, h1d_dp->GetMaximum()+3 ) ;
	line->SetLineColor( kRed ) ;
	line->Draw() ;
	c1->Update() ;
	c1->Print( psname ) ;
}

void Draw_da( TString psname, TCanvas* c1, TTree* t )
{
	c1->Clear() ;
	c1->Divide(2,2) ;
	c1->cd(1) ;
	TH2D* h2d_da = new TH2D( "h2d_da", ";dax;day", nbins_da, -width_da, width_da, nbins_da, -width_da, width_da ) ;
	h2d_da->SetTitle( "da histogram {" + str_cut_dp + "}" ) ;
	t->Draw( "day:dax>>h2d_da", cut_dp, "colz" ) ;

	c1->cd(2) ;
	TH1D* h1d_day = new TH1D( "h1d_day", ";day", nbins_da, -width_da, width_da ) ;
	h1d_day->SetFillColor(kBlue) ;
	TString str_cut_proj_day = Form( "TMath::Abs(dax)<%.3lf", da_cut ) ;
	TCut cut_proj_day = str_cut_proj_day.Data() ;
	h1d_day->SetTitle( "day : " + str_cut_proj_day ) ;
	t->Draw( "day>>h1d_day", cut_dp&&cut_proj_day , "colz" ) ;
	h1d_day->Fit("gaus", "Q", "", -da_cut, +da_cut ) ;

	c1->cd(3) ;
	TH1D* h1d_dax = new TH1D( "h1d_dax", ";dax", nbins_da, -width_da, width_da ) ;
	h1d_dax->SetFillColor(kRed) ;
	TString str_cut_proj_dax = Form( "TMath::Abs(day)<%.3lf", da_cut ) ;
	TCut cut_proj_dax = str_cut_proj_dax.Data() ;
	h1d_dax->SetTitle( "dax : " + str_cut_proj_dax ) ;
	t->Draw( "dax>>h1d_dax", cut_dp&&cut_proj_dax , "colz" ) ;
	h1d_dax->Fit("gaus", "Q", "", -da_cut, +da_cut ) ;

	c1->cd(4) ;
	TH1D* h1d_da = new TH1D( "h1d_da", ";d_ang", nbins_da, 0, width_da ) ;
	TString str_h1d_da = "TMath::Hypot(dax,day)" ;
	h1d_da->SetTitle( str_h1d_da ) ;
	h1d_da->SetFillColor(kYellow) ;
	t->Draw( str_h1d_da + ">>h1d_da", cut_dp , "" ) ;
	TLine* line = new TLine( da_cut, 0, da_cut, h1d_da->GetMaximum()+3 ) ;
	line->SetLineColor( kRed ) ;
	line->Draw() ;
	c1->Update() ;
	c1->Print( psname ) ;
}

void Draw_pos( TString psname, TCanvas* c1, TTree* t )
{
	c1->Clear() ;
	TH2D* h2d_pos = new TH2D( "h2d_pos", ";x;y", nbins_x, x_min, x_max, nbins_y, y_min, y_max ) ;
	h2d_pos->SetTitle(" position  {" + str_cut_dp + "&&" + str_cut_da + "} ") ;
	h2d_pos->SetMarkerStyle(20) ;
	t->Draw( "bt.y:bt.x>>h2d_pos", cut_dp&&cut_da, "colz" ) ;
	c1->Update() ;
	c1->Print( psname ) ;
}

void Draw_ang( TString psname, TCanvas* c1, TTree* t )
{
	c1->Clear() ;
	TH2D* h2d_ang = new TH2D( "h2d_ang", ";ax;ay", nbins_ang, -width_ang, width_ang, nbins_ang, -width_ang, width_ang ) ;
	h2d_ang->SetTitle(" angle  {" + str_cut_dp + "&&" + str_cut_da + "} ") ;
	h2d_ang->SetMarkerStyle(20) ;
	t->Draw( "bt.ay:bt.ax>>h2d_ang", cut_dp&&cut_da, "colz" ) ;
	c1->Update() ;
	c1->Print( psname ) ;
}

void Draw_time( TString psname, TCanvas* c1, TTree* t )
{
	c1->Clear() ;
	gStyle->SetOptStat(0);
	TH1D* h1d_time = new TH1D( "h1d_time", "", nbins_time, time_start, time_start + width_time ) ;
	h1d_time->SetTitle(" unixtime  {" + str_cut_dp + "&&" + str_cut_da + "} ") ;
	gStyle->SetTimeOffset(0) ;
	//gStyle->SetTimeOffset(-788918400) ;
	h1d_time->GetXaxis()->SetTimeDisplay(1) ;
	h1d_time->GetXaxis()->SetTimeFormat("#splitline{%d/%m}{/%Y}");
	h1d_time->SetFillColor( kYellow ) ;
	h1d_time->SetLabelSize(0.02);
	t->Draw( "match.unix_time()>>h1d_time", "", "" ) ;
	c1->Update() ;
	c1->Print( psname ) ;
}

void Draw_ang_dp( TString psname, TCanvas* c1, TTree* t )
{
	c1->Clear() ;
	c1->Divide(2,2) ;
	c1->cd(1) ;
	gStyle->SetStatH(0.2) ;
	gStyle->SetStatW(0.15) ;
	gStyle->SetStatFontSize(0.02) ;
	TH2D* h2d_ax_dx = new TH2D( "h2d_ax_dx", ";ax;dx", nbins_ang, -width_ang, width_ang, nbins_dp, dx_center - width_dp, dx_center + width_dp ) ;
	h2d_ax_dx->SetTitle( " ax - dx  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_ax_dx->SetMarkerStyle(20) ;
	t->Draw( "dx:bt.ax>>h2d_ax_dx", cut_dp&&cut_da, "colz" ) ;
	h2d_ax_dx->Fit("pol1", "Q", "", -width_ang, width_ang ) ;

	c1->cd(2) ;
	TH2D* h2d_ax_dy = new TH2D( "h2d_ax_dy", ";ax;dy", nbins_ang, -width_ang, width_ang, nbins_dp, dy_center - width_dp, dy_center + width_dp ) ;
	h2d_ax_dy->SetTitle( " ax - dy  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_ax_dy->SetMarkerStyle(20) ;
	t->Draw( "dy:bt.ax>>h2d_ax_dy", cut_dp&&cut_da, "colz" ) ;
	h2d_ax_dy->Fit("pol1", "Q", "", -width_ang, width_ang ) ;

	c1->cd(3) ;
	TH2D* h2d_ay_dx = new TH2D( "h2d_ay_dx", ";ay;dx", nbins_ang, -width_ang, width_ang, nbins_dp, dx_center - width_dp, dx_center + width_dp ) ;
	h2d_ay_dx->SetTitle( " ay - dx  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_ay_dx->SetMarkerStyle(20) ;
	t->Draw( "dx:bt.ay>>h2d_ay_dx", cut_dp&&cut_da, "colz" ) ;
	h2d_ay_dx->Fit("pol1", "Q", "", -width_ang, width_ang ) ;

	c1->cd(4) ;
	TH2D* h2d_ay_dy = new TH2D( "h2d_ay_dy", ";ay;dy", nbins_ang, -width_ang, width_ang, nbins_dp, dy_center - width_dp, dy_center + width_dp ) ;
	h2d_ay_dy->SetTitle( " ay - dy  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_ay_dy->SetMarkerStyle(20) ;
	t->Draw( "dy:bt.ay>>h2d_ay_dy", cut_dp&&cut_da, "colz" ) ;
	h2d_ay_dy->Fit("pol1", "Q", "", -width_ang, width_ang ) ;
	c1->Update() ;
	c1->Print( psname ) ;
}

void Draw_ang_da( TString psname, TCanvas* c1, TTree* t )
{
	c1->Clear() ;
	c1->Divide(2,2) ;
	c1->cd(1) ;
	gStyle->SetStatH(0.2) ;
	gStyle->SetStatW(0.15) ;
	gStyle->SetStatFontSize(0.02) ;
	TH2D* h2d_ax_dax = new TH2D( "h2d_ax_dax", ";ax;dax", nbins_ang, -width_ang, width_ang, nbins_da, -width_da, width_da ) ;
	h2d_ax_dax->SetTitle( " ax - dax  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_ax_dax->SetMarkerStyle(20) ;
	t->Draw( "dax:bt.ax>>h2d_ax_dax", cut_dp&&cut_da, "colz" ) ;
	h2d_ax_dax->Fit("pol1", "Q", "", -width_ang, width_ang ) ;

	c1->cd(2) ;
	TH2D* h2d_ax_day = new TH2D( "h2d_ax_day", ";ax;day", nbins_ang, -width_ang, width_ang, nbins_da, -width_da, width_da ) ;
	h2d_ax_day->SetTitle( " ax - day  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_ax_day->SetMarkerStyle(20) ;
	t->Draw( "day:bt.ax>>h2d_ax_day", cut_dp&&cut_da, "colz" ) ;
	h2d_ax_day->Fit("pol1", "Q", "", -width_ang, width_ang ) ;

	c1->cd(3) ;
	TH2D* h2d_ay_dax = new TH2D( "h2d_ay_dax", ";ay;dax", nbins_ang, -width_ang, width_ang, nbins_da, -width_da, width_da ) ;
	h2d_ay_dax->SetTitle( " ay - dax  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_ay_dax->SetMarkerStyle(20) ;
	t->Draw( "dax:bt.ay>>h2d_ay_dax", cut_dp&&cut_da, "colz" ) ;
	h2d_ay_dax->Fit("pol1", "Q", "", -width_ang, width_ang ) ;

	c1->cd(4) ;
	TH2D* h2d_ay_day = new TH2D( "h2d_ay_day", ";ay;day", nbins_ang, -width_ang, width_ang, nbins_da, -width_da, width_da ) ;
	h2d_ay_day->SetTitle( " ay - day  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_ay_day->SetMarkerStyle(20) ;
	t->Draw( "day:bt.ay>>h2d_ay_day", cut_dp&&cut_da, "colz" ) ;
	h2d_ay_day->Fit("pol1", "Q", "", -width_ang, width_ang ) ;
	c1->Update() ;
	c1->Print( psname ) ;
}

void Draw_pos_dp( TString psname, TCanvas* c1, TTree* t )
{
	c1->Clear() ;
	c1->Divide(2,2) ;
	c1->cd(1) ;
	gStyle->SetStatH(0.2) ;
	gStyle->SetStatW(0.15) ;
	gStyle->SetStatFontSize(0.02) ;
	TH2D* h2d_x_dx = new TH2D( "h2d_x_dx", ";x;dx", nbins_x, x_min, x_max, nbins_dp, dx_center - width_dp, dx_center + width_dp ) ;
	h2d_x_dx->SetTitle( " x - dx  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_x_dx->SetMarkerStyle(20) ;
	t->Draw( "dx:bt.x>>h2d_x_dx", cut_dp&&cut_da, "colz" ) ;
	h2d_x_dx->Fit("pol1", "Q", "", x_min, x_max ) ;

	c1->cd(2) ;
	TH2D* h2d_x_dy = new TH2D( "h2d_x_dy", ";x;dy", nbins_x, x_min, x_max, nbins_dp, dy_center - width_dp, dy_center + width_dp ) ;
	h2d_x_dy->SetTitle( " x - dy  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_x_dy->SetMarkerStyle(20) ;
	t->Draw( "dy:bt.x>>h2d_x_dy", cut_dp&&cut_da, "colz" ) ;
	h2d_x_dy->Fit("pol1", "Q", "", x_min, x_max ) ;

	c1->cd(3) ;
	TH2D* h2d_y_dx = new TH2D( "h2d_y_dx", ";y;dx", nbins_y, y_min, y_max, nbins_dp, dx_center - width_dp, dx_center + width_dp ) ;
	h2d_y_dx->SetTitle( " y - dx  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_y_dx->SetMarkerStyle(20) ;
	t->Draw( "dx:bt.y>>h2d_y_dx", cut_dp&&cut_da, "colz" ) ;
	h2d_y_dx->Fit("pol1", "Q", "", y_min, y_max ) ;

	c1->cd(4) ;
	TH2D* h2d_y_dy = new TH2D( "h2d_y_dy", ";y;dy", nbins_y, y_min, y_max, nbins_dp, dy_center - width_dp, dy_center + width_dp ) ;
	h2d_y_dy->SetTitle( " y - dy  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_y_dy->SetMarkerStyle(20) ;
	t->Draw( "dy:bt.y>>h2d_y_dy", cut_dp&&cut_da, "colz" ) ;
	h2d_y_dy->Fit("pol1", "Q", "", y_min, y_max ) ;
	c1->Update() ;
	c1->Print( psname ) ;
}

void Draw_pos_da( TString psname, TCanvas* c1, TTree* t )
{
	c1->Clear() ;
	c1->Divide(2,2) ;
	c1->cd(1) ;
	gStyle->SetStatH(0.2) ;
	gStyle->SetStatW(0.15) ;
	gStyle->SetStatFontSize(0.02) ;
	TH2D* h2d_x_dax = new TH2D( "h2d_x_dax", ";x;dax", nbins_x, x_min, x_max, nbins_da, -width_da, width_da ) ;
	h2d_x_dax->SetTitle( " x - dax  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_x_dax->SetMarkerStyle(20) ;
	t->Draw( "dax:bt.x>>h2d_x_dax", cut_dp&&cut_da, "colz" ) ;
	h2d_x_dax->Fit("pol1", "Q", "", x_min, x_max ) ;

	c1->cd(2) ;
	TH2D* h2d_x_day = new TH2D( "h2d_x_day", ";x;day", nbins_x, x_min, x_max, nbins_da, -width_da, width_da ) ;
	h2d_x_day->SetTitle( " x - day  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_x_day->SetMarkerStyle(20) ;
	t->Draw( "day:bt.x>>h2d_x_day", cut_dp&&cut_da, "colz" ) ;
	h2d_x_day->Fit("pol1", "Q", "", x_min, x_max ) ;

	c1->cd(3) ;
	TH2D* h2d_y_dax = new TH2D( "h2d_y_dax", ";y;dax", nbins_y, y_min, y_max, nbins_da, -width_da, width_da ) ;
	h2d_y_dax->SetTitle( " y - dax  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_y_dax->SetMarkerStyle(20) ;
	t->Draw( "dax:bt.y>>h2d_y_dax", cut_dp&&cut_da, "colz" ) ;
	h2d_y_dax->Fit("pol1", "Q", "", y_min, y_max ) ;

	c1->cd(4) ;
	TH2D* h2d_y_day = new TH2D( "h2d_y_day", ";y;day", nbins_y, y_min, y_max, nbins_da, -width_da, width_da ) ;
	h2d_y_day->SetTitle( " y - day  {" + str_cut_dp + "&&" + str_cut_da + "} " ) ;
	h2d_y_day->SetMarkerStyle(20) ;
	t->Draw( "day:bt.y>>h2d_y_day", cut_dp&&cut_da, "colz" ) ;
	h2d_y_day->Fit("pol1", "Q", "", y_min, y_max ) ;
	c1->Update() ;
	c1->Print( psname ) ;
}