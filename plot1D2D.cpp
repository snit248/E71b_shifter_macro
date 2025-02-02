#include <iostream>
#include <fstream>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>

//// 関数宣言 ////
TF1 *gaus_Fit(TH1D *hist, Double_t min, Double_t max)
{
   //// TF1の定義 ////
   // TF1 *関数名 = new TF1("関数名", "関数の定義", 関数の範囲の最小値, 関数の範囲の最大値)
   TF1 *fit = new TF1("fit", "gaus", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
   hist->Fit(fit, "", "", min, max); // fitの範囲指定
   return fit;
}

void plot1D2D()
{

   // gROOT->Reset();
   gStyle->SetOptStat(000011011); // 統計ボックスの設定
                                  //  ksiourmenの順番
                                  //  0は非表示,1は表示

   gStyle->SetOptFit(1111); // Fit情報の表示
                            // pcevの順番

   // gStyle->SetStatX(0.2); //統計boxの右上角のX座標（左下を(0,0),右上を(1,1)）
   // gStyle->SetStatY(0.9); //統計boxの右上角のY座標（左下を(0,0),右上を(1,1)）
   gStyle->SetStatH(0.05);         // 高さを変える
   gStyle->SetStatW(0.15);         // 幅を変える (Canvas?全体の長さを１とする)
   gStyle->SetStatFontSize(0.007); // フォントのサイズを変える

   double tmp;
   double X, Y;

   //---------------------------------------------------------------------------------------------------------------
   //// X layer ////

   TCanvas *c1 = new TCanvas("c1");
   // TH1D *histX = new TH1D("X layer", "Position Difference in X layer", 400, -20, 20); // 1次元用のプロット
   TH1D *histX = new TH1D("X layer", "Angle Difference in X layer", 500, -5, 5); // 1次元用のプロット

   // std::ifstream ifs1("/Users/user/Desktop/Shifter/dat/Matching_Sandmuon_bt041_2021_04_15.dat");
   // std::ifstream ifs1("/Users/user/Desktop/Shifter/dat/Shifter_matching_track/Shifter_matching_track_Sandmuon_bt041_da_dp_2021_04_15.dat");
   std::ifstream ifs1("/Users/user/Desktop/Shifter/dat/ninja_trackmatch_2021_04_15.dat");

   // while (ifs1 >> tmp >> tmp >> tmp >> X)
   // while (ifs1 >> tmp >> X)
   while (ifs1 >> tmp >> tmp >> tmp >> tmp >> tmp >> Y >> X)
   {
      if (std::abs(X) > 0.1 || std::abs(Y) > 0.1)
      {
         continue;
      }

      histX->Fill(X);
      histX->SetTitleSize(0.05, "xy"); // X, Y軸のタイトルのサイズを変える
      histX->SetLabelSize(0.05, "xy");
      // histX->GetXaxis()->SetTitle("X_exp - X_trk [mm]");
      histX->GetXaxis()->SetTitle("dax");
      histX->GetYaxis()->SetTitle("Entries/0.02");
      // histX->Fit("gaus");
      histX->Draw();
   }

   // TF1 *f1 = gaus_Fit(histX, -100, 100);
   // f1->Draw("same");

   //---------------------------------------------------------------------------------------------------------------
   //// Y layer ////

   TCanvas *c2 = new TCanvas("c2");
   // TH1D *histY = new TH1D("Y layer", "Position Difference in Y layer", 400, -20, 20); // 1次元用のプロット
   TH1D *histY = new TH1D("Y layer", "Angle Difference in Y layer", 500, -5, 5); // 1次元用のプロット

   // std::ifstream ifs2("/Users/user/Desktop/Shifter/dat/alignment_search_bt041_2021_04_15.dat");
   // std::ifstream ifs2("/Users/user/Desktop/Shifter/dat/Shifter_matching_track/Shifter_matching_track_Sandmuon_bt041_da_dp_2021_04_15.dat");
   std::ifstream ifs2("/Users/user/Desktop/Shifter/dat/ninja_trackmatch_2021_04_15.dat");

   // while (ifs2 >> tmp >> tmp >> Y >> tmp)
   // while (ifs2 >> tmp >> Y)
   while (ifs2 >> tmp >> tmp >> tmp >> tmp >> tmp >> Y >> X)
   {
      if (std::abs(X) > 0.1 || std::abs(Y) > 0.1)
      {
         continue;
      }

      histY->Fill(Y);
      histY->SetTitleSize(0.05, "xy"); // X, Y軸のタイトルのサイズを変える
      histY->SetLabelSize(0.05, "xy");
      // histY->GetXaxis()->SetTitle("Y_exp - Y_trk [mm]");
      histY->GetXaxis()->SetTitle("day");
      histY->GetYaxis()->SetTitle("Entries/0.02");
      // histY->Fit("gaus");
      histY->Draw();
   }

   // TF1 *f2 = gaus_Fit(histY, -100, 100);
   // f2->Draw("same");

   //---------------------------------------------------------------------------------------------------------------
   //// 2次元plot用 ////

   double x, y;

   TCanvas *c3 = new TCanvas("c3");
   TH2S *hist = new TH2S("h1", "", 50, -1000, 1000, 50, -1000, 1000);

   std::ifstream ifs3("/Users/user/Desktop/Shifter/dat/Shifter_bt061_2021_04_15.dat");

   while (ifs3 >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> y >> x >> Y >> X)
   // while (ifs3 >> Y >> X)
   {
      if (std::abs(X) > 0.1 || std::abs(Y) > 0.1)
      {
         continue;
      }

      Double_t exp_posY = (y / 1000.0 - 94) + Y * 17;
      Double_t exp_posX = (x / 1000.0 - 308) + X * 37;

      hist->Fill(exp_posX,exp_posY); // 2次元用のプロット
      // hist->SetMarkerStyle(8);  // プロットの形
      // hist->SetMarkerColor(2);  // プロットの色
      // hist->SetMarkerSize(0.5); // プロットのサイズ
      hist->GetXaxis()->SetTitle("x [mm]");
      hist->GetYaxis()->SetTitle("y [mm]");
      hist->Draw("colz");
   }
}
