#ifdef __CLING__
#pragma cling optimize(0)
#endif
void c()
{
//=========Macro generated from canvas: c/the canvas
//=========  (Mon May  6 22:07:39 2024) by ROOT version 6.30/03
   TCanvas *c = new TCanvas("c", "the canvas",200,31,650,500);
   gStyle->SetOptStat(0);
   c->Range(-0.128266,0.07692306,1.059382,1.102564);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   //ci = TColor::GetColor("#f0f0f0");
   c->SetFillColor(kWhite);
   c->SetBorderMode(0);
   c->SetBorderSize(2);
   c->SetGridx();
   c->SetGridy();
   c->SetTickx(1);
   c->SetTicky(1);
   c->SetRightMargin(0.05);
   c->SetBottomMargin(0.12);

   //ci = TColor::GetColor("#fffffd");
   c->SetFrameFillColor(kWhite);
   c->SetFrameBorderMode(0);

   //ci = TColor::GetColor("#fffffd");
   c->SetFrameFillColor(kWhite);
   c->SetFrameBorderMode(0);
   
   TH2F *frame__1 = new TH2F("frame__1","Background rejection versus Signal efficiency",500,0,1,500,0.2,1);
   frame__1->SetStats(0);
   frame__1->SetLineWidth(2);
   frame__1->SetMarkerStyle(21);
   frame__1->SetMarkerSize(0.3);
   frame__1->SetTitleSize(0);
   frame__1->GetXaxis()->SetTitle("Signal efficiency");
   frame__1->GetXaxis()->SetLabelOffset(0.012);
   frame__1->GetXaxis()->SetLabelSize(0.04);
   frame__1->GetXaxis()->SetTitleSize(0.045);
   frame__1->GetXaxis()->SetTitleOffset(1.25);
   frame__1->GetYaxis()->SetTitle("Background rejection");
   frame__1->GetYaxis()->SetLabelOffset(0.012);
   frame__1->GetYaxis()->SetLabelSize(0.04);
   frame__1->GetYaxis()->SetTitleSize(0.045);
   frame__1->GetYaxis()->SetTitleOffset(1.22);
   frame__1->GetZaxis()->SetLabelSize(0.04);
   frame__1->GetZaxis()->SetTitleSize(0.04);
   frame__1->GetZaxis()->SetTitleOffset(1);
   frame__1->Draw("");
   
   TH1D *MVA_BDT_rejBvsS__2 = new TH1D("MVA_BDT_rejBvsS__2","MVA_BDT",100,0,1);
   MVA_BDT_rejBvsS__2->SetBinContent(1,1);
   MVA_BDT_rejBvsS__2->SetBinContent(2,1);
   MVA_BDT_rejBvsS__2->SetBinContent(3,1);
   MVA_BDT_rejBvsS__2->SetBinContent(4,1);
   MVA_BDT_rejBvsS__2->SetBinContent(5,1);
   MVA_BDT_rejBvsS__2->SetBinContent(6,1);
   MVA_BDT_rejBvsS__2->SetBinContent(7,1);
   MVA_BDT_rejBvsS__2->SetBinContent(8,1);
   MVA_BDT_rejBvsS__2->SetBinContent(9,1);
   MVA_BDT_rejBvsS__2->SetBinContent(10,1);
   MVA_BDT_rejBvsS__2->SetBinContent(11,1);
   MVA_BDT_rejBvsS__2->SetBinContent(12,1);
   MVA_BDT_rejBvsS__2->SetBinContent(13,1);
   MVA_BDT_rejBvsS__2->SetBinContent(14,1);
   MVA_BDT_rejBvsS__2->SetBinContent(15,1);
   MVA_BDT_rejBvsS__2->SetBinContent(16,1);
   MVA_BDT_rejBvsS__2->SetBinContent(17,1);
   MVA_BDT_rejBvsS__2->SetBinContent(18,1);
   MVA_BDT_rejBvsS__2->SetBinContent(19,1);
   MVA_BDT_rejBvsS__2->SetBinContent(20,1);
   MVA_BDT_rejBvsS__2->SetBinContent(21,1);
   MVA_BDT_rejBvsS__2->SetBinContent(22,1);
   MVA_BDT_rejBvsS__2->SetBinContent(23,1);
   MVA_BDT_rejBvsS__2->SetBinContent(24,1);
   MVA_BDT_rejBvsS__2->SetBinContent(25,1);
   MVA_BDT_rejBvsS__2->SetBinContent(26,1);
   MVA_BDT_rejBvsS__2->SetBinContent(27,1);
   MVA_BDT_rejBvsS__2->SetBinContent(28,1);
   MVA_BDT_rejBvsS__2->SetBinContent(29,1);
   MVA_BDT_rejBvsS__2->SetBinContent(30,1);
   MVA_BDT_rejBvsS__2->SetBinContent(31,1);
   MVA_BDT_rejBvsS__2->SetBinContent(32,1);
   MVA_BDT_rejBvsS__2->SetBinContent(33,1);
   MVA_BDT_rejBvsS__2->SetBinContent(34,1);
   MVA_BDT_rejBvsS__2->SetBinContent(35,1);
   MVA_BDT_rejBvsS__2->SetBinContent(36,1);
   MVA_BDT_rejBvsS__2->SetBinContent(37,1);
   MVA_BDT_rejBvsS__2->SetBinContent(38,1);
   MVA_BDT_rejBvsS__2->SetBinContent(39,1);
   MVA_BDT_rejBvsS__2->SetBinContent(40,1);
   MVA_BDT_rejBvsS__2->SetBinContent(41,1);
   MVA_BDT_rejBvsS__2->SetBinContent(42,1);
   MVA_BDT_rejBvsS__2->SetBinContent(43,1);
   MVA_BDT_rejBvsS__2->SetBinContent(44,1);
   MVA_BDT_rejBvsS__2->SetBinContent(45,1);
   MVA_BDT_rejBvsS__2->SetBinContent(46,1);
   MVA_BDT_rejBvsS__2->SetBinContent(47,1);
   MVA_BDT_rejBvsS__2->SetBinContent(48,1);
   MVA_BDT_rejBvsS__2->SetBinContent(49,1);
   MVA_BDT_rejBvsS__2->SetBinContent(50,1);
   MVA_BDT_rejBvsS__2->SetBinContent(51,1);
   MVA_BDT_rejBvsS__2->SetBinContent(52,1);
   MVA_BDT_rejBvsS__2->SetBinContent(53,1);
   MVA_BDT_rejBvsS__2->SetBinContent(54,1);
   MVA_BDT_rejBvsS__2->SetBinContent(55,1);
   MVA_BDT_rejBvsS__2->SetBinContent(56,1);
   MVA_BDT_rejBvsS__2->SetBinContent(57,1);
   MVA_BDT_rejBvsS__2->SetBinContent(58,1);
   MVA_BDT_rejBvsS__2->SetBinContent(59,1);
   MVA_BDT_rejBvsS__2->SetBinContent(60,1);
   MVA_BDT_rejBvsS__2->SetBinContent(61,1);
   MVA_BDT_rejBvsS__2->SetBinContent(62,1);
   MVA_BDT_rejBvsS__2->SetBinContent(63,1);
   MVA_BDT_rejBvsS__2->SetBinContent(64,1);
   MVA_BDT_rejBvsS__2->SetBinContent(65,1);
   MVA_BDT_rejBvsS__2->SetBinContent(66,1);
   MVA_BDT_rejBvsS__2->SetBinContent(67,1);
   MVA_BDT_rejBvsS__2->SetBinContent(68,1);
   MVA_BDT_rejBvsS__2->SetBinContent(69,1);
   MVA_BDT_rejBvsS__2->SetBinContent(70,1);
   MVA_BDT_rejBvsS__2->SetBinContent(71,1);
   MVA_BDT_rejBvsS__2->SetBinContent(72,1);
   MVA_BDT_rejBvsS__2->SetBinContent(73,1);
   MVA_BDT_rejBvsS__2->SetBinContent(74,1);
   MVA_BDT_rejBvsS__2->SetBinContent(75,1);
   MVA_BDT_rejBvsS__2->SetBinContent(76,1);
   MVA_BDT_rejBvsS__2->SetBinContent(77,0.9994112);
   MVA_BDT_rejBvsS__2->SetBinContent(78,0.9989643);
   MVA_BDT_rejBvsS__2->SetBinContent(79,0.9989643);
   MVA_BDT_rejBvsS__2->SetBinContent(80,0.9986518);
   MVA_BDT_rejBvsS__2->SetBinContent(81,0.9976834);
   MVA_BDT_rejBvsS__2->SetBinContent(82,0.9971763);
   MVA_BDT_rejBvsS__2->SetBinContent(83,0.9956023);
   MVA_BDT_rejBvsS__2->SetBinContent(84,0.993723);
   MVA_BDT_rejBvsS__2->SetBinContent(85,0.9930237);
   MVA_BDT_rejBvsS__2->SetBinContent(86,0.9930237);
   MVA_BDT_rejBvsS__2->SetBinContent(87,0.9883208);
   MVA_BDT_rejBvsS__2->SetBinContent(88,0.9852592);
   MVA_BDT_rejBvsS__2->SetBinContent(89,0.982176);
   MVA_BDT_rejBvsS__2->SetBinContent(90,0.9735343);
   MVA_BDT_rejBvsS__2->SetBinContent(91,0.9675145);
   MVA_BDT_rejBvsS__2->SetBinContent(92,0.9531245);
   MVA_BDT_rejBvsS__2->SetBinContent(93,0.9281969);
   MVA_BDT_rejBvsS__2->SetBinContent(94,0.9010079);
   MVA_BDT_rejBvsS__2->SetBinContent(95,0.8783992);
   MVA_BDT_rejBvsS__2->SetBinContent(96,0.8306514);
   MVA_BDT_rejBvsS__2->SetBinContent(97,0.7740305);
   MVA_BDT_rejBvsS__2->SetBinContent(98,0.6896454);
   MVA_BDT_rejBvsS__2->SetBinContent(99,0.6065019);
   MVA_BDT_rejBvsS__2->SetBinContent(100,0.3574992);
   MVA_BDT_rejBvsS__2->SetEntries(100);
   MVA_BDT_rejBvsS__2->SetLineWidth(3);
   MVA_BDT_rejBvsS__2->GetXaxis()->SetTitle("Signal eff");
   MVA_BDT_rejBvsS__2->GetXaxis()->SetLabelFont(42);
   MVA_BDT_rejBvsS__2->GetXaxis()->SetTitleOffset(1);
   MVA_BDT_rejBvsS__2->GetXaxis()->SetTitleFont(42);
   MVA_BDT_rejBvsS__2->GetYaxis()->SetTitle("Backgr rejection (1-eff)");
   MVA_BDT_rejBvsS__2->GetYaxis()->SetLabelFont(42);
   MVA_BDT_rejBvsS__2->GetYaxis()->SetTitleFont(42);
   MVA_BDT_rejBvsS__2->GetZaxis()->SetLabelFont(42);
   MVA_BDT_rejBvsS__2->GetZaxis()->SetTitleOffset(1);
   MVA_BDT_rejBvsS__2->GetZaxis()->SetTitleFont(42);
   MVA_BDT_rejBvsS__2->Draw("csame");
   
   TH2F *frame__3 = new TH2F("frame__3","Background rejection versus Signal efficiency",500,0,1,500,0.2,1);
   frame__3->SetStats(0);
   frame__3->SetLineWidth(2);
   frame__3->SetMarkerStyle(21);
   frame__3->SetMarkerSize(0.3);
   frame__3->SetTitleSize(0);
   frame__3->GetXaxis()->SetTitle("Signal efficiency");
   frame__3->GetXaxis()->SetLabelOffset(0.012);
   frame__3->GetXaxis()->SetLabelSize(0.04);
   frame__3->GetXaxis()->SetTitleSize(0.045);
   frame__3->GetXaxis()->SetTitleOffset(1.25);
   frame__3->GetYaxis()->SetTitle("Background rejection");
   frame__3->GetYaxis()->SetLabelOffset(0.012);
   frame__3->GetYaxis()->SetLabelSize(0.04);
   frame__3->GetYaxis()->SetTitleSize(0.045);
   frame__3->GetYaxis()->SetTitleOffset(1.22);
   frame__3->GetZaxis()->SetLabelSize(0.04);
   frame__3->GetZaxis()->SetTitleSize(0.04);
   frame__3->GetZaxis()->SetTitleOffset(1);
   frame__3->Draw("sameaxis");
   
   TLegend *leg = new TLegend(0.15,0.171,0.5,0.281,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62);

   ci = TColor::GetColor("#7d8b9d");
   leg->SetLineColor(ci);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("NULL","MVA Method:","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   entry=leg->AddEntry("MVA_BDT_rejBvsS","BDT","l");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(3);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   leg->Draw();
   
   // TPaveText *pt = new TPaveText(0.01,0.9344958,0.71,0.995,"blNDC");
   // pt->SetName("title");
   // pt->SetBorderSize(1);

   // ci = TColor::GetColor("#5d6b7d");
   // pt->SetFillColor(ci);

   // ci = TColor::GetColor("#7d8b9d");
   // pt->SetLineColor(ci);
   // pt->SetTextColor(0);
   // TText *pt_LaTex = pt->AddText("Background rejection versus Signal efficiency");
   // pt->Draw();
  
    int w = 800;
    int h = 600;
 
    float t = 0.08*h;
    float b = 0.12*h;
    float l = 0.12*w;
    float r = 0.04*w;

    int align_ = 13;
    TString lumiText = "13 TeV";
    TLatex latex;
    latex.SetNDC();
    latex.SetTextAngle(0);
    latex.SetTextColor(kBlack);    
    latex.SetTextFont(42);
    latex.SetTextAlign(31);
    latex.SetTextSize(0.045);
    latex.DrawLatex(1-(r/w), 1-(t/h) + 0.025, lumiText);
 
    float posX_ = (l/w) + 0.05;
    float posY_ = 0.95 - (t/h);

        latex.SetTextFont(61);
        latex.SetTextSize(0.055);
        latex.SetTextAlign(align_);
        latex.DrawLatex(posX_, posY_, "CMS");
    

        latex.SetTextFont(52);
        latex.SetTextAlign(align_);
        latex.SetTextSize(0.04);
        latex.DrawLatex(posX_, posY_ - 0.05, "Simulation Preliminary");
    

// ------------>Primitives in pad: imgpad
   TPad *imgpad__0 = new TPad("imgpad", "imgpad",0.788395,0.91,0.95,0.965);
   imgpad__0->Draw();
   imgpad__0->cd();
   imgpad__0->Range(0,0,1,1);
   imgpad__0->SetFillColor(0);
   imgpad__0->SetBorderMode(0);
   imgpad__0->SetBorderSize(2);
   imgpad__0->SetTickx(1);
   imgpad__0->SetTicky(1);
   imgpad__0->SetLeftMargin(0);
   imgpad__0->SetRightMargin(0);
   imgpad__0->SetTopMargin(0);
   imgpad__0->SetBottomMargin(0);

   ci = TColor::GetColor("#fffffd");
   imgpad__0->SetFrameFillColor(kWhite);
   imgpad__0->SetFrameBorderMode(0);
   imgpad__0->SetFrameLineColor(0);
   imgpad__0->SetFrameBorderMode(0);

/* XPM */

   imgpad__0->Modified();
   c->cd();
   c->Modified();
   c->SetSelected(c);
}
