//-------------------------------------------------------------------------------------------------------->>
// UNIVERSIDADE DO ESTADO DO RIO DE JANEIRO - CMS/Brasil
//-------------------------------------------------------------------------------------------------------->>
// Project: Exclusive Dijets Analysis
//-------------------------------------------------------------------------------------------------------->>
//
// Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/FwdPhysicsExclusiveDijetsAnalysis#Example_Analysis_Macro
//

//#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TChain.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TMath.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

#include "ExclDijetsComp.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "KKousour/QCDAnalysis/interface/QCDEvent.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

using namespace diffractiveAnalysis;
using namespace exclusiveDijetsAnalysis;
using namespace eventInfo;
using namespace reweight;

void ExclDijetsComp::LoadFile(std::string fileinput, std::string processinput){

  inf = NULL;
  tr  = NULL;
  inf = TFile::Open(fileinput.c_str(),"read");
  tr = (TTree*)inf->Get(processinput.c_str());
  eventdiff = new DiffractiveEvent();
  eventexcl = new ExclusiveDijetsEvent();
  eventinfo = new EventInfoEvent();
  diff = tr->GetBranch("DiffractiveAnalysis");
  excl = tr->GetBranch("ExclusiveDijetsAnalysis");
  info = tr->GetBranch("EventInfo");
  diff->SetAddress(&eventdiff);
  excl->SetAddress(&eventexcl);
  info->SetAddress(&eventinfo);

}

void ExclDijetsComp::Run(std::string filein_, std::string savehistofile_, std::string pudatafile_, std::string pumcfile_, std::string processname_, std::string filecor_, std::string filetrigger_, double jet1PT_, double jet2PT_, std::string JetUn_, int optnVertex_, int optTrigger_, bool switchWeightPU_, bool switchWeightLumi_, bool switchWeightEff_, bool switchTriggerEff_, bool switchWeightePw_, bool switchMultiple_, bool switchPreSel_, bool switchTrigger_, double weightlumipass_){

  filein = filein_;
  pudatafile = pudatafile_;
  pumcfile = pumcfile_;
  savehistofile = savehistofile_;
  processname = processname_;
  filecor = filecor_;
  filein = filein_;
  filetrigger = filetrigger_;
  jet1PT = jet1PT_;
  jet2PT = jet2PT_;
  JetUn = JetUn_;
  optnVertex = optnVertex_;
  optTrigger = optTrigger_;
  switchWeightPU = switchWeightPU_;
  switchWeightLumi = switchWeightLumi_;
  switchWeightEff = switchWeightEff_;
  switchTriggerEff = switchTriggerEff_;
  switchWeightePw = switchWeightePw_;
  switchPreSel = switchPreSel_;
  switchTrigger = switchTrigger_;
  weightlumipass = weightlumipass_;
  switchMultiple = switchMultiple_;

  std::cout << "" << std::endl;
  std::cout << "Running..." << std::endl;
  std::cout << "" << std::endl;
  std::cout << "<< INPUTS >>" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "Input file: " << filein << std::endl;
  std::cout << "Output file: " << savehistofile << std::endl;
  std::cout << " " << std::cout; 
  std::cout << "Input PU Data file: " << pudatafile << std::endl;
  std::cout << "Output PU MC file: " << pumcfile << std::endl;
  std::cout << " " << std::cout;
  std::cout << "Input Efficiency File: " << filecor << std::endl;
  std::cout << " " << std::cout;
  std::cout << "pT(Jet1): " << jet1PT << "GeV" << std::endl;
  std::cout << "pT(Jet2): " << jet2PT << "GeV" << std::endl;
  std::cout << "Jet Scale: " << JetUn << std::endl;
  std::cout << "# Vertex: " << optnVertex << std::endl;
  std::cout << "Trigger Option: " << optTrigger << std::endl;
  std::cout << " " << std::endl;
  std::cout << "--> TRUE = 1 FALSE = 0" << std::endl;
  std::cout << "PU Weight: " << switchWeightPU << std::endl;
  std::cout << "Lumi. Weight: " << switchWeightLumi << std::endl;
  std::cout << "Eff. Corr.: " << switchWeightEff << std::endl;
  std::cout << "Trigger Efficiency: "<< switchTriggerEff << std::endl; 
  std::cout << "Evt. - Evt. Weight: " << switchWeightePw << std::endl;
  std::cout << "Multiple PU Histograms: " << switchMultiple << std::endl;
  std::cout << "Trigger Switch: " << switchTrigger << std::endl;
  std::cout << "Pre-Selection Switch: " << switchPreSel << std::endl;
  std::cout << " " << std::endl;
  std::cout << "--> Factors" << std::endl;
  std::cout << "Lumi. Weight: " << weightlumipass << std::endl;
  std::cout << "" << std::endl;

  if (optnVertex == 0){

    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << "Please, restart your setup. Respect the Condition # Vertex > 0)" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;
    return;

  }

  TFile check1(pumcfile.c_str());
  TFile check2(pudatafile.c_str());
  TFile check3(filein.c_str());
  TFile check4(filecor.c_str());
  TFile check5(filetrigger.c_str());



  if (check1.IsZombie()){

    std::cout << "\n----------------------------------------------" << std::endl;
    std::cout << " There is no Pile-Up Monte Carlo file or the"   << std::endl;
    std::cout << " path is not correct." << std::endl;
    std::cout << " Edit the source and recompile." << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    return;

  }

  if (check2.IsZombie()){

    std::cout << "\n----------------------------------------------" << std::endl;
    std::cout << " There is no Pile-Up Estimated file or the"   << std::endl;
    std::cout << " path is not correct." << std::endl;
    std::cout << " Edit the source and recompile." << std::endl;
    std::cout << "----------------------------------------------" << std::endl; 
    return;

  }

  if (check3.IsZombie()){

    std::cout << "\n----------------------------------------------" << std::endl;
    std::cout << " There is no PatTuple file or the"   << std::endl;
    std::cout << " path is not correct." << std::endl;
    std::cout << " Edit the source and recompile." << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    return;

  }

  if (check4.IsZombie()){

    std::cout << "\n-----------------------------------------------" << std::endl;
    std::cout << " There is no Correction Eff Histogram file or the"   << std::endl;
    std::cout << " path is not correct." << std::endl;
    std::cout << " Edit the source and recompile." << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    return;

  }

  if (check5.IsZombie()){

    std::cout << "\n---------------------------------------------------" << std::endl;
    std::cout << " There is no trigger efficiency histogram file or the"   << std::endl;
    std::cout << " path is not correct." << std::endl;
    std::cout << " Edit the source and recompile." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    return;

  }

  TFile *l1  = TFile::Open(filecor.c_str());
  TH1F* h_eff_excl = (TH1F*)l1->Get("RatioPreSel");
  TH1F* h_eff_vertex = (TH1F*)l1->Get("RatioVertex");
  TH1F* h_eff_step_4_4 = (TH1F*)l1->Get("RatioStep4_4");
  TH1F* h_eff_step_4_3 = (TH1F*)l1->Get("RatioStep4_3");
  TH1F* h_eff_step_4_2 = (TH1F*)l1->Get("RatioStep4_2");
  TH1F* h_eff_step_4_1 = (TH1F*)l1->Get("RatioStep4_1");

  TFile *l2  = TFile::Open(filetrigger.c_str());
  TH1F* h_eff_trigger_eta1 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta1");
  TH1F* h_eff_trigger_eta2 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta2");
  TH1F* h_eff_trigger_eta3 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta3");
  TH1F* h_eff_trigger_eta4 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta4");

  float lumimaxl4=0.;
  float lumiminl4=0.;

  float lumimaxl3=0.;
  float lumiminl3=0.;

  float lumimaxl2=0.;
  float lumiminl2=0.;

  float lumimaxl1=0.;
  float lumiminl1=0.;

  lumiminl4 = h_eff_trigger_eta4->GetXaxis()->GetBinCenter(h_eff_trigger_eta4->FindFirstBinAbove(0,1));
  lumimaxl4 = h_eff_trigger_eta4->GetXaxis()->GetBinCenter(h_eff_trigger_eta4->FindLastBinAbove(0,1));

  lumiminl3 = h_eff_trigger_eta3->GetXaxis()->GetBinCenter(h_eff_trigger_eta3->FindFirstBinAbove(0,1));
  lumimaxl3 = h_eff_trigger_eta3->GetXaxis()->GetBinCenter(h_eff_trigger_eta3->FindLastBinAbove(0,1));

  lumiminl2 = h_eff_trigger_eta2->GetXaxis()->GetBinCenter(h_eff_trigger_eta2->FindFirstBinAbove(0,1));
  lumimaxl2 = h_eff_trigger_eta2->GetXaxis()->GetBinCenter(h_eff_trigger_eta2->FindLastBinAbove(0,1));

  lumiminl1 = h_eff_trigger_eta1->GetXaxis()->GetBinCenter(h_eff_trigger_eta1->FindFirstBinAbove(0,1));
  lumimaxl1 = h_eff_trigger_eta1->GetXaxis()->GetBinCenter(h_eff_trigger_eta1->FindLastBinAbove(0,1));

  std::cout << "\nLumi. min1. : " << lumiminl1 << "  | Lumi. max1. : "<< lumimaxl1 << std::endl;
  std::cout << "Lumi. min2. : " << lumiminl2 << "  | Lumi. max2. : "<< lumimaxl2 << std::endl;
  std::cout << "Lumi. min3. : " << lumiminl3 << "  | Lumi. max3. : "<< lumimaxl3 << std::endl;
  std::cout << "Lumi. min4. : " << lumiminl4 << "  | Lumi. max4. : "<< lumimaxl4 << std::endl;

  LoadFile(filein,processname);
  edm::LumiReWeighting LumiWeights_(pumcfile.c_str(),pudatafile.c_str(),"pileUpBx0_complete_without_cuts","pileup");

  std::cout << " " << std::endl;
  std::cout << "pT(jet1) > " << jet1PT << std::endl;
  std::cout << "pT(jet2) > " << jet2PT << std::endl;
  std::cout << " " << std::endl;

  TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");

  TString outtxt = savehistofile;
  outtxt.ReplaceAll("root","txt");  
  std::ofstream outstring(outtxt); 

  outstring << "" << std::endl;
  outstring << "<< Gold Events >>" << std::endl;
  outstring << "" << std::endl;
  outstring << "Please, insert this events in another text file to be used by PickEvent Tool. " << std::endl;
  outstring << "Twiki: https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookPickEvents " << std::endl;
  outstring << ">>---------------------------------------------------------------------- " << std::endl;
  outstring << "Selected Events: Jets At Tracker ( |Eta| < 2) & step4_2. " << std::endl; 

  //int NEVENTS = tr->GetEntriesFast();
  int NEVENTS = tr->GetEntries();

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  unsigned NEntries = tr->GetEntries();
  std::cout << "" << std::endl;
  std::cout<< "Reading Tree: "<< NEntries << " events"<<std::endl;
  std::cout << "" << std::endl;

  int decade = 0;

  for(int m=0;m<2;m++) {

    tr->GetEntry(m);

    if ( switchMultiple && (eventexcl->GetNPileUpBx0()==-1 && eventexcl->GetNPileUpBxm1()==-1 && eventexcl->GetNPileUpBxp1()==-1 )){
      std::cout << "--------------------------------------------------------------" << std::endl;
      std::cout << " There is no Pile Up TTree information in your PATTuplefile."   << std::endl;
      std::cout << " Please, use another PATTuple with PU information to run mul- " << std::endl;
      std::cout << " tiple PU option." << std::endl;
      std::cout << "--------------------------------------------------------------" << std::endl;
      return;
    }

  }

  int TotalE = 0;
  double counterlumi = 0.;
  double counterTrigger = 0.;
  double counterJetsstep1 = 0.;
  double counterJetsstep2 = 0.;
  double counterJetsAllstep3 = 0.;
  double counterJetsAllstep4_4 = 0.;
  double counterJetsAllstep4_3 = 0.;
  double counterJetsAllstep4_2 = 0.;
  double counterJetsAllstep4_1 = 0.;
  double counterJetsTrackerstep3 = 0.;
  double counterJetsTrackerstep4_4 = 0.;
  double counterJetsTrackerstep4_3 = 0.;
  double counterJetsTrackerstep4_2 = 0.;
  double counterJetsTrackerstep4_1 = 0.;
  double counterJetsEta2step3 = 0.;
  double counterJetsEta2step4_4 = 0.;
  double counterJetsEta2step4_3 = 0.;
  double counterJetsEta2step4_2 = 0.;
  double counterJetsEta2step4_1 = 0.;
  double deltaphi_ = 0.;
  double aSumE_ = 0.;
  double absdeltaetapf_ = 0.;
  double deltaetapf_ = 0.;
  double ptJet1_ = 0.;
  double ptJet2_ = 0.;  
  std::string jetstatus;  

  std::vector <std::string> Folders;
  Folders.push_back("without_cuts");
  Folders.push_back("with_trigger");
  Folders.push_back("step1");
  Folders.push_back("step2");
  Folders.push_back("All_step3");
  Folders.push_back("All_step4_4");
  Folders.push_back("All_step4_3");
  Folders.push_back("All_step4_2");
  Folders.push_back("All_step4_1");
  Folders.push_back("Tracker_step3");
  Folders.push_back("Tracker_step4_4");
  Folders.push_back("Tracker_step4_3");
  Folders.push_back("Tracker_step4_2");
  Folders.push_back("Tracker_step4_1");
  Folders.push_back("JetsAtEta2_step3");
  Folders.push_back("JetsAtEta2_step4_4");
  Folders.push_back("JetsAtEta2_step4_3");
  Folders.push_back("JetsAtEta2_step4_2");
  Folders.push_back("JetsAtEta2_step4_1");

  int nloop;
  int indexV;

  if (switchMultiple){
    nloop = 21;
  }
  else {
    nloop = 1;
  }

  for (int j=0; j<19; j++){

    m_hVector_lumi.push_back( std::vector<TH1D*>() );
    m_hVector_rjj.push_back( std::vector<TH1D*>() );
    m_hVector_detagen.push_back( std::vector<TH1D*>() );
    m_hVector_mxGen.push_back( std::vector<TH1D*>() );
    m_hVector_multhf.push_back( std::vector<TH2F*>() );
    m_hVector_etcalos.push_back( std::vector<TH2F*>() );
    m_hVector_tracks.push_back( std::vector<TH1D*>() );
    m_hVector_pfetamax.push_back( std::vector<TH1D*>() );
    m_hVector_pfetamin.push_back( std::vector<TH1D*>() );
    m_hVector_asumE.push_back( std::vector<TH1D*>() );
    m_hVector_deltaetajets.push_back( std::vector<TH1D*>() );
    m_hVector_deltaphijets.push_back( std::vector<TH1D*>() );
    m_hVector_deltaptjets.push_back( std::vector<TH1D*>() );
    m_hVector_dijetmass.push_back( std::vector<TH1D*>() );
    m_hVector_ptjet1.push_back( std::vector<TH1D*>() );
    m_hVector_ptjet2.push_back( std::vector<TH1D*>() );
    m_hVector_etajet1.push_back( std::vector<TH1D*>() );
    m_hVector_etajet2.push_back( std::vector<TH1D*>() );
    m_hVector_phijet1.push_back( std::vector<TH1D*>() );
    m_hVector_phijet2.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHFplus.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHFminus.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHEplus.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHEminus.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHFpfplus.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHFpfminus.push_back( std::vector<TH1D*>() );
    m_hVector_sumECastor.push_back( std::vector<TH1D*>() );
    m_hVector_deltaEtaPF.push_back( std::vector<TH1D*>() );
    m_hVector_absdeltaEtaPF.push_back( std::vector<TH1D*>() );
    m_hVector_vertex.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHFplusVsetaMax.push_back( std::vector<TH2D*>() );
    m_hVector_sumEHFminusVsetaMin.push_back( std::vector<TH2D*>() );
    m_hVector_sumEHFpfplusVsetaMax.push_back( std::vector<TH2D*>() );
    m_hVector_sumEHFpfminusVsetaMin.push_back( std::vector<TH2D*>() );
    m_hVector_sumEHFplusVsiEta.push_back( std::vector<TH1D*>() );
    m_hVector_sumEHFminusVsiEta.push_back( std::vector<TH1D*>() );

    /*
       char name_ieta[300];
       for(int ieta = 29; ieta <= 41; ++ieta){
       sprintf(name_ieta,"sumEHFplus_iEta_%d_PU_%s_%s",ieta,"TT",Folders.at(j).c_str());
       TH1D *histo_sumEHFplus_ieta = new TH1D(name_ieta,"HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",500,0,500);
       m_hVector_sumEHFplusVsiEta[j].push_back(histo_sumEHFplus_ieta);

       sprintf(name_ieta,"sumEHFminus_iEta_%d_PU_%s_%s",ieta,"TT",Folders.at(j).c_str());
       TH1D *histo_sumEHFminus_ieta = new TH1D(name_ieta,"HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",500,0,500);
       m_hVector_sumEHFminusVsiEta[j].push_back(histo_sumEHFminus_ieta);
       }
     */  

    for (int k=0;k<nloop;k++){

      char tag[300];

      if (!switchMultiple){
	sprintf(tag,"Complete");
      }
      else {
	sprintf(tag,"%i",k);
      }

      char name1[300];
      sprintf(name1,"RJJ_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_rjj = new TH1D(name1,"R_{jj} Distribution; R_{jj}; N events",20,0,1.2);
      m_hVector_rjj[j].push_back(histo_rjj);

      char name2[300];
      sprintf(name2,"DeltaEtaGen_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_detagen = new TH1D(name2,"#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
      m_hVector_detagen[j].push_back(histo_detagen);

      char name3[300];
      sprintf(name3,"MxGenRange_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_MxGenRange = new TH1D(name3,"Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
      m_hVector_mxGen[j].push_back(histo_MxGenRange);

      char name4[300];
      sprintf(name4,"mHF_PU_%s_%s",tag,Folders.at(j).c_str());
      TH2F *histo_MultHF = new TH2F(name4,"HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
      m_hVector_multhf[j].push_back(histo_MultHF);

      char name5[300];
      sprintf(name5,"ETCalos_PU_%s_%s",tag,Folders.at(j).c_str());
      TH2F *histo_ET_Calos = new TH2F(name5,"HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
      m_hVector_etcalos[j].push_back(histo_ET_Calos);

      char name6[300];
      sprintf(name6,"Tracks_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_Tracks = new TH1D(name6,"Tracks Multiplicity; n Tracks; N events",50,0,150);
      m_hVector_tracks[j].push_back(histo_Tracks);

      char name7[300];
      sprintf(name7,"pfetamax_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_PFEtamax = new TH1D(name7,"Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
      m_hVector_pfetamax[j].push_back(histo_PFEtamax);

      char name8[300];
      sprintf(name8,"pfetamin_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_PFEtamin = new TH1D(name8,"Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
      m_hVector_pfetamin[j].push_back(histo_PFEtamin);

      char name9[300];
      sprintf(name9,"aEnergy_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_aSumE = new TH1D(name9,"Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
      m_hVector_asumE[j].push_back(histo_aSumE);

      char name10[300];
      sprintf(name10,"deltaEtaJets_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltaetajets = new TH1D(name10,"#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
      m_hVector_deltaetajets[j].push_back(histo_deltaetajets);

      char name11[300];
      sprintf(name11,"deltaPhiJets_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltaphijets = new TH1D(name11,"#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
      m_hVector_deltaphijets[j].push_back(histo_deltaphijets);

      char name12[300];
      sprintf(name12,"deltaPtJets_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltaptjets = new TH1D(name12,"#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
      m_hVector_deltaptjets[j].push_back(histo_deltaptjets);

      char name13[300];
      sprintf(name13,"DijetMass_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_dijetmass = new TH1D(name13,"Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",80,0,800);
      m_hVector_dijetmass[j].push_back(histo_dijetmass);

      char name14[300];
      sprintf(name14,"pTJet1_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_ptjet1 = new TH1D(name14,"Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
      m_hVector_ptjet1[j].push_back(histo_ptjet1);

      char name15[300];
      sprintf(name15,"pTJet2_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_ptjet2 = new TH1D(name15,"Second Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
      m_hVector_ptjet2[j].push_back(histo_ptjet2);

      char name16[300];
      sprintf(name16,"etaJet1_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_etajet1 = new TH1D(name16,"Leading Jet - #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_etajet1[j].push_back(histo_etajet1);

      char name17[300];
      sprintf(name17,"etaJet2_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_etajet2 = new TH1D(name17,"Second Jet - #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_etajet2[j].push_back(histo_etajet2);

      char name18[300];
      sprintf(name18,"phiJet1_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_phijet1 = new TH1D(name18,"Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
      m_hVector_phijet1[j].push_back(histo_phijet1);

      char name19[300];
      sprintf(name19,"phiJet2_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_phijet2 = new TH1D(name19,"Second Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
      m_hVector_phijet2[j].push_back(histo_phijet2);

      char name20[300];
      sprintf(name20,"sumEHFplus_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHFplus = new TH1D(name20,"HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
      m_hVector_sumEHFplus[j].push_back(histo_sumEHFplus);

      char name21[300];
      sprintf(name21,"sumEHFminus_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHFminus = new TH1D(name21,"HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
      m_hVector_sumEHFminus[j].push_back(histo_sumEHFminus);

      char name22[300];
      sprintf(name22,"sumEHEplus_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHEplus = new TH1D(name22,"HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
      m_hVector_sumEHEplus[j].push_back(histo_sumEHEplus);

      char name23[300];
      sprintf(name23,"sumEHEminus_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHEminus = new TH1D(name23,"HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
      m_hVector_sumEHEminus[j].push_back(histo_sumEHEminus);

      char name24[300];
      sprintf(name24,"sumEHFpfplus_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHFpfplus = new TH1D(name24,"HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
      m_hVector_sumEHFpfplus[j].push_back(histo_sumEHFpfplus);

      char name25[300];
      sprintf(name25,"sumEHFpfminus_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumEHFpfminus = new TH1D(name25,"HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);
      m_hVector_sumEHFpfminus[j].push_back(histo_sumEHFpfminus);

      char name26[300];
      sprintf(name26,"deltaEtamaxminPF_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_deltaEtaPF = new TH1D(name26,"#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
      m_hVector_deltaEtaPF[j].push_back(histo_deltaEtaPF);

      char name27[300];
      sprintf(name27,"absdeltaEtamaxminPF_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_absdeltaEtaPF = new TH1D(name27,"|#Delta#eta_{PF}| Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
      m_hVector_absdeltaEtaPF[j].push_back(histo_absdeltaEtaPF);

      char name28[300];
      sprintf(name28,"vertex_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_vertex = new TH1D(name28,"Number of Vertex; # Vertex; N events",25,0,25);
      m_hVector_vertex[j].push_back(histo_vertex);

      char name29[300];
      sprintf(name29,"lumi_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_lumi = new TH1D(name29,"Luminosity per Bunch; L_{Bunch} [#mub^{-1}s^{-1}]; N events",25,0,2);
      m_hVector_lumi[j].push_back(histo_lumi);

      char name30[300];
      sprintf(name30,"sumEHFpfplusVspfetamax_PU_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_sumEHFpfplusVsPFEtamax = new TH2D(name30,"#sum E_{HF^{+}} Vs #eta_{max} Distribution; #eta_{Max};#sum E_{HF^{+}} [GeV]; N events",20,0,5.5,100, 0., 2000.);
      m_hVector_sumEHFpfplusVsetaMax[j].push_back(histo_sumEHFpfplusVsPFEtamax);

      char name31[300];
      sprintf(name31,"sumEHFpfminusVspfetamin_PU_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_sumEHFpfminusVsPFEtamin = new TH2D(name31,"#sum E_{HF^{-}} Vs #eta_{min} Distribution; #eta_{Min};#sum E_{HF^{-}} [GeV]; N events",20,-5.5,0,100, 0., 2000);
      m_hVector_sumEHFpfminusVsetaMin[j].push_back(histo_sumEHFpfminusVsPFEtamin);

      char name32[300];
      sprintf(name32,"sumEHFplusVspfetamax_PU_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_sumEHFplusVsPFEtamax = new TH2D(name32,"#sum E_{HF^{+}} Vs #eta_{max} Distribution; #eta_{Max};#sum E_{HF^{+}} [GeV]; N events",20,0,5.5,100, 0., 2000.);
      m_hVector_sumEHFplusVsetaMax[j].push_back(histo_sumEHFplusVsPFEtamax);

      char name33[300];
      sprintf(name33,"sumEHFminusVspfetamin_PU_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_sumEHFminusVsPFEtamin = new TH2D(name33,"#sum E_{HF^{-}} Vs #eta_{min} Distribution; #eta_{Min};#sum E_{HF^{-}} [GeV]; N events",20,-5.5,0,100, 0., 2000);
      m_hVector_sumEHFminusVsetaMin[j].push_back(histo_sumEHFminusVsPFEtamin);

      char name34[300];
      sprintf(name34,"sumECastor_PU_%s_%s",tag,Folders.at(j).c_str());
      TH1D *histo_sumECastor = new TH1D(name34,"Castor - Total Energy; #sum E_{Castor} [GeV]; N events",100,0,2000);
      m_hVector_sumECastor[j].push_back(histo_sumECastor);


      char name_ieta[300];
      for(int ieta = 29; ieta <= 41; ++ieta){
	sprintf(name_ieta,"sumEHFplus_iEta_%d_PU_%s_%s",ieta,tag,Folders.at(j).c_str());
	TH1D *histo_sumEHFplus_ieta = new TH1D(name_ieta,"HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",500,0,500);
	m_hVector_sumEHFplusVsiEta[j].push_back(histo_sumEHFplus_ieta);

	sprintf(name_ieta,"sumEHFminus_iEta_%d_PU_%s_%s",ieta,tag,Folders.at(j).c_str());
	TH1D *histo_sumEHFminus_ieta = new TH1D(name_ieta,"HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",500,0,500);
	m_hVector_sumEHFminusVsiEta[j].push_back(histo_sumEHFminus_ieta);
      }
    }
  }

  for(int i=0;i<NEVENTS;i++){

    double totalweight = -999.;
    double totalweightbxm1 = -999.;
    double totalweightbxp1 = -999.;
    ++TotalE;

    double progress = 10.0*i/(1.0*NEVENTS);
    int l = TMath::FloorNint(progress); 

    if (l > decade){
      std::cout <<"\n<<<<<< STATUS >>>>>>" << std::endl; 
      std::cout<<10*l<<" % completed." << std::endl;
      std::cout <<"<<<<<<<<<<>>>>>>>>>>\n" << std::endl;
    }
    decade = l;          

    tr->GetEntry(i);

    if (switchMultiple){
      indexV = eventexcl->GetNPileUpBx0();
    }
    else {
      indexV = 0;
    }

    if (!switchMultiple || (switchMultiple && eventexcl->GetNPileUpBx0() < 21)){

      deltaphi_ = fabs(eventexcl->GetLeadingJetPhi()-eventexcl->GetSecondJetPhi());
      aSumE_ = (eventdiff->GetSumEnergyHFPlus() - eventdiff->GetSumEnergyHFMinus())/(eventdiff->GetSumEnergyHFPlus() + eventdiff->GetSumEnergyHFMinus());
      absdeltaetapf_ = fabs(eventdiff->GetEtaMaxFromPFCands()-eventdiff->GetEtaMinFromPFCands());
      deltaetapf_ = (eventdiff->GetEtaMaxFromPFCands()-eventdiff->GetEtaMinFromPFCands());

      if (JetUn == "plus"){
	ptJet1_ = eventexcl->GetLeadingJetPt() + eventexcl->GetUnc1()*eventexcl->GetLeadingJetPt(); 
	ptJet2_ = eventexcl->GetSecondJetPt() + eventexcl->GetUnc2()*eventexcl->GetSecondJetPt();
	jetstatus = "Jets Unc. Plus";
      }

      else if (JetUn == "minus"){
	ptJet1_ = eventexcl->GetLeadingJetPt() - eventexcl->GetUnc1()*eventexcl->GetLeadingJetPt();             
	ptJet2_ = eventexcl->GetSecondJetPt() - eventexcl->GetUnc2()*eventexcl->GetSecondJetPt();
	jetstatus = "Jets Unc. Minus";
      }

      else if (JetUn == "none"){
	ptJet1_ = eventexcl->GetLeadingJetPt();
	ptJet2_ = eventexcl->GetSecondJetPt();
	jetstatus = "Normal";
      } 

      else {
	ptJet1_ = eventexcl->GetLeadingJetPt();
	ptJet2_ = eventexcl->GetSecondJetPt();
	jetstatus = "Unrecognized jet energy scale correction. Defined without it.";
      }


      // SET PILE-UP REWEIGHT METHOD!!!!
      // NOTE: you can use eventexcl->GetNPileUpBxm1() or eventexcl->GetNPileUpBxp1()

      double weight;
      double weightbxp1;
      double weightbxm1;

      if (switchWeightPU) { 
	//weight = LumiWeights_.weightOOT(eventexcl->GetNPileUpBx0(),eventexcl->GetNPileUpBxm1()); 
	weight = LumiWeights_.weight( eventexcl->GetNPileUpBx0()); 
	weightbxp1 = LumiWeights_.weight( eventexcl->GetNPileUpBxp1()); 
	weightbxm1 = LumiWeights_.weight( eventexcl->GetNPileUpBxm1()); 
      }
      else {
	weight = 1.0;
	weightbxp1 = 1.0;
	weightbxm1 = 1.0;
      }

      double triggereff4;
      double triggereff3;
      double triggereff2;
      double triggereff1;

      if (switchTriggerEff){

	if (eventinfo->GetInstLumiBunch()<lumiminl1){
	  triggereff1 = 1./h_eff_trigger_eta1->GetBinContent(h_eff_trigger_eta1->GetXaxis()->FindBin(lumiminl1));
	}

	if(eventinfo->GetInstLumiBunch()>lumimaxl1){
	  triggereff1 = 1./h_eff_trigger_eta1->GetBinContent(h_eff_trigger_eta1->GetXaxis()->FindBin(lumimaxl1));
	}

	if (eventinfo->GetInstLumiBunch()<lumiminl2){
	  triggereff2 = 1./h_eff_trigger_eta2->GetBinContent(h_eff_trigger_eta2->GetXaxis()->FindBin(lumiminl2));
	}

	if(eventinfo->GetInstLumiBunch()>lumimaxl2){
	  triggereff2 = 1./h_eff_trigger_eta2->GetBinContent(h_eff_trigger_eta2->GetXaxis()->FindBin(lumimaxl2));
	}

	if (eventinfo->GetInstLumiBunch()<lumiminl3){
	  triggereff3 = 1./h_eff_trigger_eta3->GetBinContent(h_eff_trigger_eta3->GetXaxis()->FindBin(lumiminl3));
	}

	if(eventinfo->GetInstLumiBunch()>lumimaxl3){
	  triggereff3 = 1./h_eff_trigger_eta3->GetBinContent(h_eff_trigger_eta3->GetXaxis()->FindBin(lumimaxl3));
	}

	if (eventinfo->GetInstLumiBunch()<lumiminl4){
	  triggereff4 = 1./h_eff_trigger_eta4->GetBinContent(h_eff_trigger_eta4->GetXaxis()->FindBin(lumiminl4));
	}

	if(eventinfo->GetInstLumiBunch()>lumimaxl4){
	  triggereff4 = 1./h_eff_trigger_eta4->GetBinContent(h_eff_trigger_eta4->GetXaxis()->FindBin(lumimaxl4));
	}

	if (eventinfo->GetInstLumiBunch()>lumiminl1 && eventinfo->GetInstLumiBunch()<lumimaxl1) {
	  if (h_eff_trigger_eta1->GetBinContent(h_eff_trigger_eta1->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch()))== 0.){
	    triggereff1 = 1.;
	  }
	  else triggereff1 = 1./h_eff_trigger_eta1->GetBinContent(h_eff_trigger_eta1->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch()));
	}

	if (eventinfo->GetInstLumiBunch()>lumiminl2 && eventinfo->GetInstLumiBunch()<lumimaxl2) {

	  if (h_eff_trigger_eta2->GetBinContent(h_eff_trigger_eta2->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch()))==0.){
	    triggereff2 = 1.;
	  }

	  else triggereff2 = 1./h_eff_trigger_eta2->GetBinContent(h_eff_trigger_eta2->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch()));

	}

	if (eventinfo->GetInstLumiBunch()>lumiminl3 && eventinfo->GetInstLumiBunch()<lumimaxl3) {

	  if (h_eff_trigger_eta3->GetBinContent(h_eff_trigger_eta3->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch())) == 0.){
	    triggereff3 = 1.;
	  }

	  else triggereff3 = 1./h_eff_trigger_eta3->GetBinContent(h_eff_trigger_eta3->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch()));

	}   

	if (eventinfo->GetInstLumiBunch()>lumiminl4 && eventinfo->GetInstLumiBunch()<lumimaxl4) {

	  if (h_eff_trigger_eta4->GetBinContent(h_eff_trigger_eta4->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch())) == 0.){
	    triggereff4 = 1.;
	  }

	  else triggereff4 = 1./h_eff_trigger_eta4->GetBinContent(h_eff_trigger_eta4->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch()));
	}


      }

      else {
	triggereff1 = 1.0;
	triggereff2 = 1.0;
	triggereff3 = 1.0;
	triggereff4 = 1.0;
      }


      // Each Cut Efficiency Corrections
      double triggereff_excl; // presel
      double triggereff_vertex; // presel + vertex
      double triggereff_step4_4; // presel + vertex + etamax 4
      double triggereff_step4_3; // presel + vertex + etamax 3
      double triggereff_step4_2; // presel + vertex + etamax 2
      double triggereff_step4_1; // presel + vertex + etamax 1

      if (switchWeightEff){
	if (switchPreSel) {
	  triggereff_excl = 1./h_eff_excl->GetBinContent(h_eff_excl->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch()));
	} else {
	  triggereff_excl = 1.0;
	}       
	triggereff_vertex = 1./h_eff_vertex->GetBinContent(h_eff_vertex->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch()));
	triggereff_step4_4 = triggereff4*1./h_eff_step_4_4->GetBinContent(h_eff_step_4_4->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch()));
	triggereff_step4_3 = triggereff3*1./h_eff_step_4_3->GetBinContent(h_eff_step_4_3->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch()));
	triggereff_step4_2 = triggereff2*1./h_eff_step_4_2->GetBinContent(h_eff_step_4_2->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch()));
	triggereff_step4_1 = triggereff1*1./h_eff_step_4_1->GetBinContent(h_eff_step_4_1->GetXaxis()->FindBin(eventinfo->GetInstLumiBunch()));  
      } 

      else { 
	triggereff_excl = 1.0;
	triggereff_vertex = 1.0;
	triggereff_step4_4 = triggereff4*1.0;
	triggereff_step4_3 = triggereff3*1.0;
	triggereff_step4_2 = triggereff2*1.0;
	triggereff_step4_1 = triggereff1*1.0;
      }

      double weightlumi;
      double weightepw;

      if (switchWeightLumi) { weightlumi = weightlumipass; }
      else { weightlumi = 1.0;}

      if (switchWeightePw) { weightepw = eventinfo->GetGeneratorWeight();}
      else { weightepw = 1.0;}

      //---------->>
      /*
	 std::cout << "BEFORE TRIGGER" << std::endl;
	 std::cout << "PileUp-1: " << eventexcl->GetNPileUpBxm1() << std::endl; 
	 std::cout << "PileUp0: " << eventexcl->GetNPileUpBx0() << std::endl;
	 std::cout << "PileUp+1: " << eventexcl->GetNPileUpBxp1() << std::endl;
	 std::cout << "==============" << std::endl;   
       */
      //---------->>

      totalweight = weight*weightlumi*weightepw;
      totalweightbxm1 = weightbxm1*weightlumi*weightepw;
      totalweightbxp1 = weightbxp1*weightlumi*weightepw;


      if( i % 1000 == 0 ){
	std::cout << "\nEvent " << i << std::endl
	  << "Nr. events Bx 0   : " << eventexcl->GetNPileUpBx0() << std::endl
	  << "Nr. events Bxm1   : " << eventexcl->GetNPileUpBxm1() << std::endl
	  << "Nr. events Bxp1   : " << eventexcl->GetNPileUpBxp1() << std::endl
	  << "Pile-up weight    : " << weight << std::endl
	  << "Weight Lumi       : " << weightlumi << std::endl
	  << "Lumi per Bunch    : " << eventinfo->GetInstLumiBunch() << std::endl
	  << "Trigger Eff. per Lumi. (Eta4): " << triggereff4 << std::endl
	  << "Trigger Eff. per Lumi. (Eta3): " << triggereff3 << std::endl
	  << "Trigger Eff. per Lumi. (Eta2): " << triggereff2 << std::endl
	  << "Trigger Eff. per Lumi. (Eta1): " << triggereff1 << std::endl
 	  << "Lumi eff corr. Pre Sel. : " << triggereff_excl << std::endl
	  << "Lumi eff corr. Pre Sel. + Vertex: " << triggereff_vertex << std::endl
	  << "Lumi eff corr. Pre Sel. + Vertex + Eta_PF(max,min) < 4: " << triggereff_step4_4 << std::endl
	  << "Lumi eff corr. Pre Sel. + Vertex + Eta_PF(max,min) < 3: " << triggereff_step4_3 << std::endl
	  << "Lumi eff corr. Pre Sel. + Vertex + Eta_PF(max,min) < 2: " << triggereff_step4_2 << std::endl
	  << "Lumi eff corr. Pre Sel. + Vertex + Eta_PF(max,min) < 1: " << triggereff_step4_1 << std::endl
	  << "MC event weight   : " << weightepw << std::endl
	  << "Total weight BX0  : " << totalweight << std::endl
	  << "Total weight BXm1 : " << totalweightbxm1 << std::endl
	  << "Total weight BXp1 : " << totalweightbxp1 << std::endl
	  << "Leading Jet: " << eventexcl->GetLeadingJetPt() << " | Unc: " << eventexcl->GetUnc1() << " | Unc*pT: " << eventexcl->GetLeadingJetPt()*eventexcl->GetUnc1() << " | Eta: " << eventexcl->GetLeadingJetEta() << std::endl
	  << "Second Jet: " << eventexcl->GetSecondJetPt() << " | Unc: " << eventexcl->GetUnc2() << " | Unc*pT: " << eventexcl->GetSecondJetPt()*eventexcl->GetUnc2() << " | Eta: " << eventexcl->GetSecondJetEta() << std::endl;
      }

      counterlumi++;

      m_hVector_rjj[0].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight);
      m_hVector_detagen[0].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight);
      m_hVector_mxGen[0].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight);
      m_hVector_multhf[0].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
      m_hVector_etcalos[0].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
      m_hVector_tracks[0].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
      m_hVector_pfetamax[0].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
      m_hVector_pfetamin[0].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
      m_hVector_asumE[0].at(indexV)->Fill(aSumE_,totalweight);
      m_hVector_deltaetajets[0].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
      m_hVector_deltaphijets[0].at(indexV)->Fill(deltaphi_,totalweight);
      m_hVector_deltaptjets[0].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
      m_hVector_dijetmass[0].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight);
      m_hVector_ptjet1[0].at(indexV)->Fill(ptJet1_,totalweight);
      m_hVector_ptjet2[0].at(indexV)->Fill(ptJet2_,totalweight);
      m_hVector_etajet1[0].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight);
      m_hVector_etajet2[0].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight);
      m_hVector_phijet1[0].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
      m_hVector_phijet2[0].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight);
      m_hVector_sumEHFplus[0].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight);
      m_hVector_sumEHFminus[0].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight);
      m_hVector_sumEHEplus[0].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight);
      m_hVector_sumEHEminus[0].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight);
      m_hVector_sumEHFpfplus[0].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight);
      m_hVector_sumEHFpfminus[0].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight);
      m_hVector_deltaEtaPF[0].at(indexV)->Fill(deltaetapf_,totalweight);
      m_hVector_absdeltaEtaPF[0].at(indexV)->Fill(absdeltaetapf_,totalweight);
      m_hVector_vertex[0].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight);
      m_hVector_lumi[0].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
      m_hVector_sumEHFpfplusVsetaMax[0].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
      m_hVector_sumEHFpfminusVsetaMin[0].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
      m_hVector_sumEHFplusVsetaMax[0].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
      m_hVector_sumEHFminusVsetaMin[0].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);     
      m_hVector_sumECastor[0].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);


      for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
	m_hVector_sumEHFplusVsiEta[0][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight);
	m_hVector_sumEHFminusVsiEta[0][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight);
      }

      //------------------------------------------------------------------------------------------
      //
      // SET TRIGGER OR TRIGGER EMULATOR!!!!
      //
      //

      if (!switchTrigger || (switchTrigger && eventexcl->GetHLTPath(optTrigger)) ){
	if (!switchPreSel || (switchPreSel && eventdiff->GetSumEnergyHFPlus() < 30 && eventdiff->GetSumEnergyHFMinus() < 30)  ){

	  counterTrigger+=totalweight*triggereff_excl;

	  // With Trigger: online or emulate          
	  ////////////////////////////////////////////////
	  m_hVector_rjj[1].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_excl);
	  m_hVector_detagen[1].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_excl);
	  m_hVector_mxGen[1].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_excl);
	  m_hVector_multhf[1].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_excl);
	  m_hVector_etcalos[1].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_excl);
	  m_hVector_tracks[1].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_excl);
	  m_hVector_pfetamax[1].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_excl);
	  m_hVector_pfetamin[1].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_excl);
	  m_hVector_asumE[1].at(indexV)->Fill(aSumE_,totalweight*triggereff_excl);
	  m_hVector_deltaetajets[1].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_excl);
	  m_hVector_deltaphijets[1].at(indexV)->Fill(deltaphi_,totalweight*triggereff_excl);
	  m_hVector_deltaptjets[1].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_excl);
	  m_hVector_dijetmass[1].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_excl);
	  m_hVector_ptjet1[1].at(indexV)->Fill(ptJet1_,totalweight*triggereff_excl);
	  m_hVector_ptjet2[1].at(indexV)->Fill(ptJet2_,totalweight*triggereff_excl);
	  m_hVector_etajet1[1].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_excl);
	  m_hVector_etajet2[1].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_excl);
	  m_hVector_phijet1[1].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_excl);
	  m_hVector_phijet2[1].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_excl);
	  m_hVector_sumEHFplus[1].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_excl);
	  m_hVector_sumEHFminus[1].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_excl);
	  m_hVector_sumEHEplus[1].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_excl);
	  m_hVector_sumEHEminus[1].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_excl);
	  m_hVector_sumEHFpfplus[1].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_excl);
	  m_hVector_sumEHFpfminus[1].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_excl);
	  m_hVector_deltaEtaPF[1].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_excl);
	  m_hVector_absdeltaEtaPF[1].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_excl);
	  m_hVector_vertex[1].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_excl);
	  m_hVector_lumi[1].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
	  m_hVector_sumEHFpfplusVsetaMax[1].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
	  m_hVector_sumEHFpfminusVsetaMin[1].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
	  m_hVector_sumEHFplusVsetaMax[1].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
	  m_hVector_sumEHFminusVsetaMin[1].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
	  m_hVector_sumECastor[1].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

	  for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
	    m_hVector_sumEHFplusVsiEta[1][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_excl);
	    m_hVector_sumEHFminusVsiEta[1][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_excl);
	  }

	  //////////////////////////////////////////////////

	  if(eventexcl->GetNVertex() > 0 && eventexcl->GetNVertex()<= optnVertex){

	    counterJetsstep1+=totalweight*triggereff_vertex; 

	    //---------->>

	    /*
	       std::cout << "" << std::endl;
	       std::cout << "PileUp-1: " << eventexcl->GetNPileUpBxm1() << std::endl;    
	       std::cout << "PileUp0: " << eventexcl->GetNPileUpBx0() << std::endl;
	       std::cout << "PileUp+1: " << eventexcl->GetNPileUpBxp1() << std::endl;
	       std::cout << "Weigh["<<i<<"]: " << LumiWeights_.ITweight(eventexcl->GetNPileUpBx0()) << std::endl;
	       std::cout << "" << std::endl;
	     */

	    //std::cout << "" << std::endl;
	    //std::cout << "Weight["<<i<<"]: " << weight << std::endl;
	    //std::cout << "" << std::endl;

	    //---------->

	    // STEP1        
	    //////////////////////////////////////////////////
	    m_hVector_rjj[2].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_vertex);
	    m_hVector_detagen[2].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_vertex);
	    m_hVector_mxGen[2].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_vertex);
	    m_hVector_multhf[2].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_vertex);
	    m_hVector_etcalos[2].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_vertex);
	    m_hVector_tracks[2].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_vertex);
	    m_hVector_pfetamax[2].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_vertex);
	    m_hVector_pfetamin[2].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_vertex);
	    m_hVector_asumE[2].at(indexV)->Fill(aSumE_,totalweight*triggereff_vertex);
	    m_hVector_deltaetajets[2].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_vertex);
	    m_hVector_deltaphijets[2].at(indexV)->Fill(deltaphi_,totalweight*triggereff_vertex);
	    m_hVector_deltaptjets[2].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_vertex);
	    m_hVector_dijetmass[2].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_vertex);
	    m_hVector_ptjet1[2].at(indexV)->Fill(ptJet1_,totalweight*triggereff_vertex);
	    m_hVector_ptjet2[2].at(indexV)->Fill(ptJet2_,totalweight*triggereff_vertex);
	    m_hVector_etajet1[2].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_vertex);
	    m_hVector_etajet2[2].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_vertex);
	    m_hVector_phijet1[2].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_vertex);
	    m_hVector_phijet2[2].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_vertex);
	    m_hVector_sumEHFplus[2].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_vertex);
	    m_hVector_sumEHFminus[2].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_vertex);
	    m_hVector_sumEHEplus[2].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_vertex);
	    m_hVector_sumEHEminus[2].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_vertex);
	    m_hVector_sumEHFpfplus[2].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_vertex);
	    m_hVector_sumEHFpfminus[2].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_vertex);
	    m_hVector_deltaEtaPF[2].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_vertex);
	    m_hVector_absdeltaEtaPF[2].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_vertex);
	    m_hVector_vertex[2].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_vertex);
	    m_hVector_lumi[2].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
	    m_hVector_sumEHFpfplusVsetaMax[2].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
	    m_hVector_sumEHFpfminusVsetaMin[2].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
	    m_hVector_sumEHFplusVsetaMax[2].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
	    m_hVector_sumEHFminusVsetaMin[2].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
	    m_hVector_sumECastor[2].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

	    for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
	      m_hVector_sumEHFplusVsiEta[2][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_vertex);
	      m_hVector_sumEHFminusVsiEta[2][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_vertex);
	    }

	    //////////////////////////////////////////////////


	    // Setting Analysis Cut
	    if ((eventexcl->GetLeadingJetPt() > jet1PT && eventexcl->GetSecondJetPt() > jet2PT) && (eventexcl->GetLeadingJetPt() < 400 && eventexcl->GetSecondJetPt() < 400) ){

	      if(deltaphi_>M_PI) deltaphi_=2.0*M_PI-deltaphi_;
	      if(deltaphi_>0.5*M_PI) {

		counterJetsstep2+=totalweight*triggereff_vertex;

		// STEP2         
		////////////////////////////////////////////////
		m_hVector_rjj[3].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_vertex);
		m_hVector_detagen[3].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_vertex);
		m_hVector_mxGen[3].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_vertex);
		m_hVector_multhf[3].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_vertex);
		m_hVector_etcalos[3].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_vertex);
		m_hVector_tracks[3].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_vertex);
		m_hVector_pfetamax[3].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_vertex);
		m_hVector_pfetamin[3].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_vertex);
		m_hVector_asumE[3].at(indexV)->Fill(aSumE_,totalweight*triggereff_vertex);
		m_hVector_deltaetajets[3].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_vertex);
		m_hVector_deltaphijets[3].at(indexV)->Fill(deltaphi_,totalweight*triggereff_vertex);
		m_hVector_deltaptjets[3].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_vertex);
		m_hVector_dijetmass[3].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_vertex);
		m_hVector_ptjet1[3].at(indexV)->Fill(ptJet1_,totalweight*triggereff_vertex);
		m_hVector_ptjet2[3].at(indexV)->Fill(ptJet2_,totalweight*triggereff_vertex);
		m_hVector_etajet1[3].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_vertex);
		m_hVector_etajet2[3].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_vertex);
		m_hVector_phijet1[3].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_vertex);
		m_hVector_phijet2[3].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_vertex);
		m_hVector_sumEHFplus[3].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_vertex);
		m_hVector_sumEHFminus[3].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_vertex);
		m_hVector_sumEHEplus[3].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_vertex);
		m_hVector_sumEHEminus[3].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_vertex);
		m_hVector_sumEHFpfplus[3].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_vertex);
		m_hVector_sumEHFpfminus[3].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_vertex);
		m_hVector_deltaEtaPF[3].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_vertex);
		m_hVector_absdeltaEtaPF[3].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_vertex);
		m_hVector_vertex[3].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_vertex);
		m_hVector_lumi[3].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		m_hVector_sumEHFpfplusVsetaMax[3].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		m_hVector_sumEHFpfminusVsetaMin[3].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		m_hVector_sumEHFplusVsetaMax[3].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		m_hVector_sumEHFminusVsetaMin[3].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		m_hVector_sumECastor[3].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);


		for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		  m_hVector_sumEHFplusVsiEta[3][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_vertex);
		  m_hVector_sumEHFminusVsiEta[3][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_vertex);
		}

		////////////////////////////////////////////////

		if (eventexcl->GetLeadingJetEta() < 5.2 && eventexcl->GetSecondJetEta() < 5.2 && eventexcl->GetLeadingJetEta() > -5.2 && eventexcl->GetSecondJetEta() > -5.2){

		  counterJetsAllstep3+=totalweight*triggereff_vertex;

		  // ALL - STEP3         
		  ////////////////////////////////////////////////
		  m_hVector_rjj[4].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_vertex);
		  m_hVector_detagen[4].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_vertex);
		  m_hVector_mxGen[4].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_vertex);
		  m_hVector_multhf[4].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_vertex);
		  m_hVector_etcalos[4].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_vertex);
		  m_hVector_tracks[4].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_vertex);
		  m_hVector_pfetamax[4].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_vertex);
		  m_hVector_pfetamin[4].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_vertex);
		  m_hVector_asumE[4].at(indexV)->Fill(aSumE_,totalweight*triggereff_vertex);
		  m_hVector_deltaetajets[4].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_vertex);
		  m_hVector_deltaphijets[4].at(indexV)->Fill(deltaphi_,totalweight*triggereff_vertex);
		  m_hVector_deltaptjets[4].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_vertex);
		  m_hVector_dijetmass[4].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_vertex);
		  m_hVector_ptjet1[4].at(indexV)->Fill(ptJet1_,totalweight*triggereff_vertex);
		  m_hVector_ptjet2[4].at(indexV)->Fill(ptJet2_,totalweight*triggereff_vertex);
		  m_hVector_etajet1[4].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_vertex);
		  m_hVector_etajet2[4].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_vertex);
		  m_hVector_phijet1[4].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_vertex);
		  m_hVector_phijet2[4].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_vertex);
		  m_hVector_sumEHFplus[4].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHFminus[4].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHEplus[4].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHEminus[4].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHFpfplus[4].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHFpfminus[4].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_vertex);
		  m_hVector_deltaEtaPF[4].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_vertex);
		  m_hVector_absdeltaEtaPF[4].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_vertex);
		  m_hVector_vertex[4].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_vertex);
		  m_hVector_lumi[4].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		  m_hVector_sumEHFpfplusVsetaMax[4].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		  m_hVector_sumEHFpfminusVsetaMin[4].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		  m_hVector_sumEHFplusVsetaMax[4].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		  m_hVector_sumEHFminusVsetaMin[4].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		  m_hVector_sumECastor[4].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		  for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		    m_hVector_sumEHFplusVsiEta[4][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_vertex);
		    m_hVector_sumEHFminusVsiEta[4][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_vertex);
		  }

		  //////////////////////////////////////////////////////////////////////////////////////////

		  // Eta max and Eta min cut
		  if ((eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.)|| (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ){

		    counterJetsAllstep4_4+=totalweight*triggereff_step4_4;
		    m_hVector_rjj[5].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_step4_4);
		    m_hVector_detagen[5].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_step4_4);
		    m_hVector_mxGen[5].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_step4_4);
		    m_hVector_multhf[5].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_step4_4);
		    m_hVector_etcalos[5].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_step4_4);
		    m_hVector_tracks[5].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_step4_4);
		    m_hVector_pfetamax[5].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_step4_4);
		    m_hVector_pfetamin[5].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_step4_4);
		    m_hVector_asumE[5].at(indexV)->Fill(aSumE_,totalweight*triggereff_step4_4);
		    m_hVector_deltaetajets[5].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_step4_4);
		    m_hVector_deltaphijets[5].at(indexV)->Fill(deltaphi_,totalweight*triggereff_step4_4);
		    m_hVector_deltaptjets[5].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_step4_4);
		    m_hVector_dijetmass[5].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_step4_4);
		    m_hVector_ptjet1[5].at(indexV)->Fill(ptJet1_,totalweight*triggereff_step4_4);
		    m_hVector_ptjet2[5].at(indexV)->Fill(ptJet2_,totalweight*triggereff_step4_4);
		    m_hVector_etajet1[5].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_step4_4);
		    m_hVector_etajet2[5].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_step4_4);
		    m_hVector_phijet1[5].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_step4_4);
		    m_hVector_phijet2[5].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHFplus[5].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHFminus[5].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHEplus[5].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHEminus[5].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHFpfplus[5].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHFpfminus[5].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_step4_4);
		    m_hVector_deltaEtaPF[5].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_step4_4);
		    m_hVector_absdeltaEtaPF[5].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_step4_4);
		    m_hVector_vertex[5].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_step4_4);
		    m_hVector_lumi[5].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		    m_hVector_sumEHFpfplusVsetaMax[5].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		    m_hVector_sumEHFpfminusVsetaMin[5].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		    m_hVector_sumEHFplusVsetaMax[5].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		    m_hVector_sumEHFminusVsetaMin[5].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		    m_hVector_sumECastor[5].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		    for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		      m_hVector_sumEHFplusVsiEta[5][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_step4_4);
		      m_hVector_sumEHFminusVsiEta[5][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_step4_4);
		    }

		  }

		  if ((eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.)|| (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)){

		    counterJetsAllstep4_3+=totalweight*triggereff_step4_3;
		    m_hVector_rjj[6].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_step4_3);
		    m_hVector_detagen[6].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_step4_3);
		    m_hVector_mxGen[6].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_step4_3);
		    m_hVector_multhf[6].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_step4_3);
		    m_hVector_etcalos[6].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_step4_3);
		    m_hVector_tracks[6].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_step4_3);
		    m_hVector_pfetamax[6].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_step4_3);
		    m_hVector_pfetamin[6].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_step4_3);
		    m_hVector_asumE[6].at(indexV)->Fill(aSumE_,totalweight*triggereff_step4_3);
		    m_hVector_deltaetajets[6].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_step4_3);
		    m_hVector_deltaphijets[6].at(indexV)->Fill(deltaphi_,totalweight*triggereff_step4_3);
		    m_hVector_deltaptjets[6].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_step4_3);
		    m_hVector_dijetmass[6].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_step4_3);
		    m_hVector_ptjet1[6].at(indexV)->Fill(ptJet1_,totalweight*triggereff_step4_3);
		    m_hVector_ptjet2[6].at(indexV)->Fill(ptJet2_,totalweight*triggereff_step4_3);
		    m_hVector_etajet1[6].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_step4_3);
		    m_hVector_etajet2[6].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_step4_3);
		    m_hVector_phijet1[6].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_step4_3);
		    m_hVector_phijet2[6].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHFplus[6].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHFminus[6].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHEplus[6].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHEminus[6].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHFpfplus[6].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHFpfminus[6].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_step4_3);
		    m_hVector_deltaEtaPF[6].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_step4_3);
		    m_hVector_absdeltaEtaPF[6].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_step4_3);
		    m_hVector_vertex[6].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_step4_3);
		    m_hVector_lumi[6].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		    m_hVector_sumEHFpfplusVsetaMax[6].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		    m_hVector_sumEHFpfminusVsetaMin[6].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		    m_hVector_sumEHFplusVsetaMax[6].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		    m_hVector_sumEHFminusVsetaMin[6].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		    m_hVector_sumECastor[6].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		    for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		      m_hVector_sumEHFplusVsiEta[6][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_step4_3);
		      m_hVector_sumEHFminusVsiEta[6][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_step4_3);
		    }

		  }

		  if ((eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)){

		    counterJetsAllstep4_2+=totalweight*triggereff_step4_2;
		    m_hVector_rjj[7].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_step4_2);
		    m_hVector_detagen[7].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_step4_2);
		    m_hVector_mxGen[7].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_step4_2);
		    m_hVector_multhf[7].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_step4_2);
		    m_hVector_etcalos[7].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_step4_2);
		    m_hVector_tracks[7].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_step4_2);
		    m_hVector_pfetamax[7].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_step4_2);
		    m_hVector_pfetamin[7].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_step4_2);
		    m_hVector_asumE[7].at(indexV)->Fill(aSumE_,totalweight*triggereff_step4_2);
		    m_hVector_deltaetajets[7].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_step4_2);
		    m_hVector_deltaphijets[7].at(indexV)->Fill(deltaphi_,totalweight*triggereff_step4_2);
		    m_hVector_deltaptjets[7].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_step4_2);
		    m_hVector_dijetmass[7].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_step4_2);
		    m_hVector_ptjet1[7].at(indexV)->Fill(ptJet1_,totalweight*triggereff_step4_2);
		    m_hVector_ptjet2[7].at(indexV)->Fill(ptJet2_,totalweight*triggereff_step4_2);
		    m_hVector_etajet1[7].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_step4_2);
		    m_hVector_etajet2[7].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_step4_2);
		    m_hVector_phijet1[7].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_step4_2);
		    m_hVector_phijet2[7].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHFplus[7].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHFminus[7].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHEplus[7].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHEminus[7].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHFpfplus[7].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHFpfminus[7].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_step4_2);
		    m_hVector_deltaEtaPF[7].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_step4_2);
		    m_hVector_absdeltaEtaPF[7].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_step4_2);
		    m_hVector_vertex[7].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_step4_2);
		    m_hVector_lumi[7].at(indexV)->Fill(eventinfo->GetInstLumiBunch()); 
		    m_hVector_sumEHFpfplusVsetaMax[7].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		    m_hVector_sumEHFpfminusVsetaMin[7].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		    m_hVector_sumEHFplusVsetaMax[7].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		    m_hVector_sumEHFminusVsetaMin[7].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);                              m_hVector_sumECastor[7].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		    for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		      m_hVector_sumEHFplusVsiEta[7][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_step4_2);
		      m_hVector_sumEHFminusVsiEta[7][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_step4_2);
		    }

		  }

		  if ((eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)){

		    counterJetsAllstep4_1+=totalweight*triggereff_step4_1;
		    m_hVector_rjj[8].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_step4_1);
		    m_hVector_detagen[8].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_step4_1);
		    m_hVector_mxGen[8].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_step4_1);
		    m_hVector_multhf[8].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_step4_1);
		    m_hVector_etcalos[8].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_step4_1);
		    m_hVector_tracks[8].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_step4_1);
		    m_hVector_pfetamax[8].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_step4_1);
		    m_hVector_pfetamin[8].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_step4_1);
		    m_hVector_asumE[8].at(indexV)->Fill(aSumE_,totalweight*triggereff_step4_1);
		    m_hVector_deltaetajets[8].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_step4_1);
		    m_hVector_deltaphijets[8].at(indexV)->Fill(deltaphi_,totalweight*triggereff_step4_1);
		    m_hVector_deltaptjets[8].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_step4_1);
		    m_hVector_dijetmass[8].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_step4_1);
		    m_hVector_ptjet1[8].at(indexV)->Fill(ptJet1_,totalweight*triggereff_step4_1);
		    m_hVector_ptjet2[8].at(indexV)->Fill(ptJet2_,totalweight*triggereff_step4_1);
		    m_hVector_etajet1[8].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_step4_1);
		    m_hVector_etajet2[8].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_step4_1);
		    m_hVector_phijet1[8].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_step4_1);
		    m_hVector_phijet2[8].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHFplus[8].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHFminus[8].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHEplus[8].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHEminus[8].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHFpfplus[8].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHFpfminus[8].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_step4_1);
		    m_hVector_deltaEtaPF[8].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_step4_1);
		    m_hVector_absdeltaEtaPF[8].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_step4_1);
		    m_hVector_vertex[8].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_step4_1);
		    m_hVector_lumi[8].at(indexV)->Fill(eventinfo->GetInstLumiBunch());      
		    m_hVector_sumEHFpfplusVsetaMax[8].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		    m_hVector_sumEHFpfminusVsetaMin[8].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		    m_hVector_sumEHFplusVsetaMax[8].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		    m_hVector_sumEHFminusVsetaMin[8].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);                     m_hVector_sumECastor[8].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		    for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		      m_hVector_sumEHFplusVsiEta[8][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_step4_1);
		      m_hVector_sumEHFminusVsiEta[8][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_step4_1);
		    }

		  }

		}  
		// end Jets at all CMS Acceptance

		if (eventexcl->GetLeadingJetEta() < 2.9 && eventexcl->GetSecondJetEta() < 2.9 && eventexcl->GetLeadingJetEta() > -2.9 && eventexcl->GetSecondJetEta() > -2.9){

		  counterJetsTrackerstep3+=totalweight*triggereff_vertex;

		  // Tracker - STEP3         
		  ////////////////////////////////////////////////
		  m_hVector_rjj[9].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_vertex);
		  m_hVector_detagen[9].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_vertex);
		  m_hVector_mxGen[9].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_vertex);
		  m_hVector_multhf[9].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_vertex);
		  m_hVector_etcalos[9].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_vertex);
		  m_hVector_tracks[9].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_vertex);
		  m_hVector_pfetamax[9].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_vertex);
		  m_hVector_pfetamin[9].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_vertex);
		  m_hVector_asumE[9].at(indexV)->Fill(aSumE_,totalweight*triggereff_vertex);
		  m_hVector_deltaetajets[9].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_vertex);
		  m_hVector_deltaphijets[9].at(indexV)->Fill(deltaphi_,totalweight*triggereff_vertex);
		  m_hVector_deltaptjets[9].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_vertex);
		  m_hVector_dijetmass[9].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_vertex);
		  m_hVector_ptjet1[9].at(indexV)->Fill(ptJet1_,totalweight*triggereff_vertex);
		  m_hVector_ptjet2[9].at(indexV)->Fill(ptJet2_,totalweight*triggereff_vertex);
		  m_hVector_etajet1[9].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_vertex);
		  m_hVector_etajet2[9].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_vertex);
		  m_hVector_phijet1[9].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_vertex);
		  m_hVector_phijet2[9].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_vertex);
		  m_hVector_sumEHFplus[9].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHFminus[9].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHEplus[9].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHEminus[9].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHFpfplus[9].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHFpfminus[9].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_vertex);
		  m_hVector_deltaEtaPF[9].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_vertex);
		  m_hVector_absdeltaEtaPF[9].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_vertex);
		  m_hVector_vertex[9].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_vertex);
		  m_hVector_lumi[9].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		  m_hVector_sumEHFpfplusVsetaMax[9].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		  m_hVector_sumEHFpfminusVsetaMin[9].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		  m_hVector_sumEHFplusVsetaMax[9].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		  m_hVector_sumEHFminusVsetaMin[9].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		  m_hVector_sumECastor[9].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		  for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		    m_hVector_sumEHFplusVsiEta[9][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_vertex);
		    m_hVector_sumEHFminusVsiEta[9][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_vertex);
		  }


		  // Eta max and nt ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		  if ((eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.)|| (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ){

		    counterJetsTrackerstep4_4+=totalweight*triggereff_step4_4;
		    m_hVector_rjj[10].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_step4_4);
		    m_hVector_detagen[10].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_step4_4);
		    m_hVector_mxGen[10].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_step4_4);
		    m_hVector_multhf[10].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_step4_4);
		    m_hVector_etcalos[10].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_step4_4);
		    m_hVector_tracks[10].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_step4_4);
		    m_hVector_pfetamax[10].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_step4_4);
		    m_hVector_pfetamin[10].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_step4_4);
		    m_hVector_asumE[10].at(indexV)->Fill(aSumE_,totalweight*triggereff_step4_4);
		    m_hVector_deltaetajets[10].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_step4_4);
		    m_hVector_deltaphijets[10].at(indexV)->Fill(deltaphi_,totalweight*triggereff_step4_4);
		    m_hVector_deltaptjets[10].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_step4_4);
		    m_hVector_dijetmass[10].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_step4_4);
		    m_hVector_ptjet1[10].at(indexV)->Fill(ptJet1_,totalweight*triggereff_step4_4);
		    m_hVector_ptjet2[10].at(indexV)->Fill(ptJet2_,totalweight*triggereff_step4_4);
		    m_hVector_etajet1[10].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_step4_4);
		    m_hVector_etajet2[10].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_step4_4);
		    m_hVector_phijet1[10].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_step4_4);
		    m_hVector_phijet2[10].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHFplus[10].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHFminus[10].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHEplus[10].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHEminus[10].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHFpfplus[10].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHFpfminus[10].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_step4_4);
		    m_hVector_deltaEtaPF[10].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_step4_4);
		    m_hVector_absdeltaEtaPF[10].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_step4_4);
		    m_hVector_vertex[10].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_step4_4);
		    m_hVector_lumi[10].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		    m_hVector_sumEHFpfplusVsetaMax[10].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		    m_hVector_sumEHFpfminusVsetaMin[10].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		    m_hVector_sumEHFplusVsetaMax[10].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		    m_hVector_sumEHFminusVsetaMin[10].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		    m_hVector_sumECastor[10].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		    for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		      m_hVector_sumEHFplusVsiEta[10][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_step4_4);
		      m_hVector_sumEHFminusVsiEta[10][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_step4_4);
		    }

		  }

		  if ((eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.)|| (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ){

		    counterJetsTrackerstep4_3+=totalweight*triggereff_step4_3;
		    m_hVector_rjj[11].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_step4_3);
		    m_hVector_detagen[11].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_step4_3);
		    m_hVector_mxGen[11].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_step4_3);
		    m_hVector_multhf[11].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_step4_3);
		    m_hVector_etcalos[11].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_step4_3);
		    m_hVector_tracks[11].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_step4_3);
		    m_hVector_pfetamax[11].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_step4_3);
		    m_hVector_pfetamin[11].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_step4_3);
		    m_hVector_asumE[11].at(indexV)->Fill(aSumE_,totalweight*triggereff_step4_3);
		    m_hVector_deltaetajets[11].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_step4_3);
		    m_hVector_deltaphijets[11].at(indexV)->Fill(deltaphi_,totalweight*triggereff_step4_3);
		    m_hVector_deltaptjets[11].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_step4_3);
		    m_hVector_dijetmass[11].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_step4_3);
		    m_hVector_ptjet1[11].at(indexV)->Fill(ptJet1_,totalweight*triggereff_step4_3);
		    m_hVector_ptjet2[11].at(indexV)->Fill(ptJet2_,totalweight*triggereff_step4_3);
		    m_hVector_etajet1[11].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_step4_3);
		    m_hVector_etajet2[11].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_step4_3);
		    m_hVector_phijet1[11].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_step4_3);
		    m_hVector_phijet2[11].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHFplus[11].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHFminus[11].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHEplus[11].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHEminus[11].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHFpfplus[11].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHFpfminus[11].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_step4_3);
		    m_hVector_deltaEtaPF[11].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_step4_3);
		    m_hVector_absdeltaEtaPF[11].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_step4_3);
		    m_hVector_vertex[11].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_step4_3);
		    m_hVector_lumi[11].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		    m_hVector_sumEHFpfplusVsetaMax[11].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		    m_hVector_sumEHFpfminusVsetaMin[11].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		    m_hVector_sumEHFplusVsetaMax[11].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		    m_hVector_sumEHFminusVsetaMin[11].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		    m_hVector_sumECastor[11].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		    for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		      m_hVector_sumEHFplusVsiEta[11][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_step4_3);
		      m_hVector_sumEHFminusVsiEta[11][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_step4_3);
		    }

		  }

		  if( (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.)|| (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ){

		    counterJetsTrackerstep4_2+=totalweight*triggereff_step4_2;
		    m_hVector_rjj[12].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_step4_2);
		    m_hVector_detagen[12].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_step4_2);
		    m_hVector_mxGen[12].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_step4_2);
		    m_hVector_multhf[12].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_step4_2);
		    m_hVector_etcalos[12].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_step4_2);
		    m_hVector_tracks[12].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_step4_2);
		    m_hVector_pfetamax[12].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_step4_2);
		    m_hVector_pfetamin[12].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_step4_2);
		    m_hVector_asumE[12].at(indexV)->Fill(aSumE_,totalweight*triggereff_step4_2);
		    m_hVector_deltaetajets[12].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_step4_2);
		    m_hVector_deltaphijets[12].at(indexV)->Fill(deltaphi_,totalweight*triggereff_step4_2);
		    m_hVector_deltaptjets[12].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_step4_2);
		    m_hVector_dijetmass[12].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_step4_2);
		    m_hVector_ptjet1[12].at(indexV)->Fill(ptJet1_,totalweight*triggereff_step4_2);
		    m_hVector_ptjet2[12].at(indexV)->Fill(ptJet2_,totalweight*triggereff_step4_2);
		    m_hVector_etajet1[12].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_step4_2);
		    m_hVector_etajet2[12].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_step4_2);
		    m_hVector_phijet1[12].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_step4_2);
		    m_hVector_phijet2[12].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHFplus[12].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHFminus[12].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHEplus[12].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHEminus[12].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHFpfplus[12].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHFpfminus[12].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_step4_2);
		    m_hVector_deltaEtaPF[12].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_step4_2);
		    m_hVector_absdeltaEtaPF[12].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_step4_2);
		    m_hVector_vertex[12].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_step4_2);
		    m_hVector_lumi[12].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		    m_hVector_sumEHFpfplusVsetaMax[12].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		    m_hVector_sumEHFpfminusVsetaMin[12].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		    m_hVector_sumEHFplusVsetaMax[12].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		    m_hVector_sumEHFminusVsetaMin[12].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		    m_hVector_sumECastor[12].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		    for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		      m_hVector_sumEHFplusVsiEta[12][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_step4_2);
		      m_hVector_sumEHFminusVsiEta[12][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_step4_2);
		    }

		  }

		  if ((eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)){

		    counterJetsTrackerstep4_1+=totalweight*triggereff_step4_1;
		    m_hVector_rjj[13].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_step4_1);
		    m_hVector_detagen[13].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_step4_1);
		    m_hVector_mxGen[13].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_step4_1);
		    m_hVector_multhf[13].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_step4_1);
		    m_hVector_etcalos[13].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_step4_1);
		    m_hVector_tracks[13].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_step4_1);
		    m_hVector_pfetamax[13].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_step4_1);
		    m_hVector_pfetamin[13].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_step4_1);
		    m_hVector_asumE[13].at(indexV)->Fill(aSumE_,totalweight*triggereff_step4_1);
		    m_hVector_deltaetajets[13].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_step4_1);
		    m_hVector_deltaphijets[13].at(indexV)->Fill(deltaphi_,totalweight*triggereff_step4_1);
		    m_hVector_deltaptjets[13].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_step4_1);
		    m_hVector_dijetmass[13].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_step4_1);
		    m_hVector_ptjet1[13].at(indexV)->Fill(ptJet1_,totalweight*triggereff_step4_1);
		    m_hVector_ptjet2[13].at(indexV)->Fill(ptJet2_,totalweight*triggereff_step4_1);
		    m_hVector_etajet1[13].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_step4_1);
		    m_hVector_etajet2[13].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_step4_1);
		    m_hVector_phijet1[13].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_step4_1);
		    m_hVector_phijet2[13].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHFplus[13].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHFminus[13].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHEplus[13].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHEminus[13].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHFpfplus[13].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHFpfminus[13].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_step4_1);
		    m_hVector_deltaEtaPF[13].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_step4_1);
		    m_hVector_absdeltaEtaPF[13].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_step4_1);
		    m_hVector_vertex[13].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_step4_1);
		    m_hVector_lumi[13].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		    m_hVector_sumEHFpfplusVsetaMax[13].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		    m_hVector_sumEHFpfminusVsetaMin[13].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		    m_hVector_sumEHFplusVsetaMax[13].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		    m_hVector_sumEHFminusVsetaMin[13].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		    m_hVector_sumECastor[13].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		    for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		      m_hVector_sumEHFplusVsiEta[13][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_step4_1);
		      m_hVector_sumEHFminusVsiEta[13][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_step4_1);
		    }

		  }

		} // end jets at tracker


		if (eventexcl->GetLeadingJetEta() < 2. && eventexcl->GetSecondJetEta() < 2. && eventexcl->GetLeadingJetEta() > -2. && eventexcl->GetSecondJetEta() > -2.){

		  counterJetsEta2step3+=totalweight*triggereff_vertex;

		  // JetsEta2 - STEP3         
		  ////////////////////////////////////////////////
		  m_hVector_rjj[14].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_vertex);
		  m_hVector_detagen[14].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_vertex);
		  m_hVector_mxGen[14].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_vertex);
		  m_hVector_multhf[14].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_vertex);
		  m_hVector_etcalos[14].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_vertex);
		  m_hVector_tracks[14].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_vertex);
		  m_hVector_pfetamax[14].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_vertex);
		  m_hVector_pfetamin[14].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_vertex);
		  m_hVector_asumE[14].at(indexV)->Fill(aSumE_,totalweight*triggereff_vertex);
		  m_hVector_deltaetajets[14].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_vertex);
		  m_hVector_deltaphijets[14].at(indexV)->Fill(deltaphi_,totalweight*triggereff_vertex);
		  m_hVector_deltaptjets[14].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_vertex);
		  m_hVector_dijetmass[14].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_vertex);
		  m_hVector_ptjet1[14].at(indexV)->Fill(ptJet1_,totalweight*triggereff_vertex);
		  m_hVector_ptjet2[14].at(indexV)->Fill(ptJet2_,totalweight*triggereff_vertex);
		  m_hVector_etajet1[14].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_vertex);
		  m_hVector_etajet2[14].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_vertex);
		  m_hVector_phijet1[14].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_vertex);
		  m_hVector_phijet2[14].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_vertex);
		  m_hVector_sumEHFplus[14].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHFminus[14].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHEplus[14].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHEminus[14].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHFpfplus[14].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_vertex);
		  m_hVector_sumEHFpfminus[14].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_vertex);
		  m_hVector_deltaEtaPF[14].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_vertex);
		  m_hVector_absdeltaEtaPF[14].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_vertex);
		  m_hVector_vertex[14].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_vertex);
		  m_hVector_lumi[14].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		  m_hVector_sumEHFpfplusVsetaMax[14].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		  m_hVector_sumEHFpfminusVsetaMin[14].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		  m_hVector_sumEHFplusVsetaMax[14].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		  m_hVector_sumEHFminusVsetaMin[14].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		  m_hVector_sumECastor[14].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);  

		  for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		    m_hVector_sumEHFplusVsiEta[14][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_vertex);
		    m_hVector_sumEHFminusVsiEta[14][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_vertex);
		  }

		  // Eta max and Eta min cut
		  if ((eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)){

		    counterJetsEta2step4_4+=totalweight*triggereff_step4_4;
		    m_hVector_rjj[15].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_step4_4);
		    m_hVector_detagen[15].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_step4_4);
		    m_hVector_mxGen[15].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_step4_4);
		    m_hVector_multhf[15].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_step4_4);
		    m_hVector_etcalos[15].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_step4_4);
		    m_hVector_tracks[15].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_step4_4);
		    m_hVector_pfetamax[15].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_step4_4);
		    m_hVector_pfetamin[15].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_step4_4);
		    m_hVector_asumE[15].at(indexV)->Fill(aSumE_,totalweight*triggereff_step4_4);
		    m_hVector_deltaetajets[15].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_step4_4);
		    m_hVector_deltaphijets[15].at(indexV)->Fill(deltaphi_,totalweight*triggereff_step4_4);
		    m_hVector_deltaptjets[15].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_step4_4);
		    m_hVector_dijetmass[15].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_step4_4);
		    m_hVector_ptjet1[15].at(indexV)->Fill(ptJet1_,totalweight*triggereff_step4_4);
		    m_hVector_ptjet2[15].at(indexV)->Fill(ptJet2_,totalweight*triggereff_step4_4);
		    m_hVector_etajet1[15].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_step4_4);
		    m_hVector_etajet2[15].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_step4_4);
		    m_hVector_phijet1[15].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_step4_4);
		    m_hVector_phijet2[15].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHFplus[15].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHFminus[15].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHEplus[15].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHEminus[15].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHFpfplus[15].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_step4_4);
		    m_hVector_sumEHFpfminus[15].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_step4_4);
		    m_hVector_deltaEtaPF[15].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_step4_4);
		    m_hVector_absdeltaEtaPF[15].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_step4_4);
		    m_hVector_vertex[15].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_step4_4);
		    m_hVector_lumi[15].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		    m_hVector_sumEHFpfplusVsetaMax[15].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		    m_hVector_sumEHFpfminusVsetaMin[15].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		    m_hVector_sumEHFplusVsetaMax[15].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		    m_hVector_sumEHFminusVsetaMin[15].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		    m_hVector_sumECastor[15].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		    for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		      m_hVector_sumEHFplusVsiEta[15][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_step4_4);
		      m_hVector_sumEHFminusVsiEta[15][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_step4_4);
		    }

		  }

		  if ((eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)){

		    counterJetsEta2step4_3+=totalweight*triggereff_step4_3;
		    m_hVector_rjj[16].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_step4_3);
		    m_hVector_detagen[16].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_step4_3);
		    m_hVector_mxGen[16].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_step4_3);
		    m_hVector_multhf[16].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_step4_3);
		    m_hVector_etcalos[16].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_step4_3);
		    m_hVector_tracks[16].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_step4_3);
		    m_hVector_pfetamax[16].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_step4_3);
		    m_hVector_pfetamin[16].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_step4_3);
		    m_hVector_asumE[16].at(indexV)->Fill(aSumE_,totalweight*triggereff_step4_3);
		    m_hVector_deltaetajets[16].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_step4_3);
		    m_hVector_deltaphijets[16].at(indexV)->Fill(deltaphi_,totalweight*triggereff_step4_3);
		    m_hVector_deltaptjets[16].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_step4_3);
		    m_hVector_dijetmass[16].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_step4_3);
		    m_hVector_ptjet1[16].at(indexV)->Fill(ptJet1_,totalweight*triggereff_step4_3);
		    m_hVector_ptjet2[16].at(indexV)->Fill(ptJet2_,totalweight*triggereff_step4_3);
		    m_hVector_etajet1[16].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_step4_3);
		    m_hVector_etajet2[16].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_step4_3);
		    m_hVector_phijet1[16].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_step4_3);
		    m_hVector_phijet2[16].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHFplus[16].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHFminus[16].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHEplus[16].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHEminus[16].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHFpfplus[16].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_step4_3);
		    m_hVector_sumEHFpfminus[16].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_step4_3);
		    m_hVector_deltaEtaPF[16].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_step4_3);
		    m_hVector_absdeltaEtaPF[16].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_step4_3);
		    m_hVector_vertex[16].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_step4_3);
		    m_hVector_lumi[16].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		    m_hVector_sumEHFpfplusVsetaMax[16].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		    m_hVector_sumEHFpfminusVsetaMin[16].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		    m_hVector_sumEHFplusVsetaMax[16].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		    m_hVector_sumEHFminusVsetaMin[16].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		    m_hVector_sumECastor[16].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		    for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		      m_hVector_sumEHFplusVsiEta[16][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_step4_3);
		      m_hVector_sumEHFminusVsiEta[16][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_step4_3);
		    }

		  }


		  if ((eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)){
		    outstring << eventdiff->GetRunNumber() << ":" << eventdiff->GetLumiSection() << ":" << eventdiff->GetEventNumber() << std::endl;
		    counterJetsEta2step4_2+=totalweight*triggereff_step4_2;
		    m_hVector_rjj[17].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_step4_2);
		    m_hVector_detagen[17].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_step4_2);
		    m_hVector_mxGen[17].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_step4_2);
		    m_hVector_multhf[17].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_step4_2);
		    m_hVector_etcalos[17].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_step4_2);
		    m_hVector_tracks[17].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_step4_2);
		    m_hVector_pfetamax[17].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_step4_2);
		    m_hVector_pfetamin[17].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_step4_2);
		    m_hVector_asumE[17].at(indexV)->Fill(aSumE_,totalweight*triggereff_step4_2);
		    m_hVector_deltaetajets[17].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_step4_2);
		    m_hVector_deltaphijets[17].at(indexV)->Fill(deltaphi_,totalweight*triggereff_step4_2);
		    m_hVector_deltaptjets[17].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_step4_2);
		    m_hVector_dijetmass[17].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_step4_2);
		    m_hVector_ptjet1[17].at(indexV)->Fill(ptJet1_,totalweight*triggereff_step4_2);
		    m_hVector_ptjet2[17].at(indexV)->Fill(ptJet2_,totalweight*triggereff_step4_2);
		    m_hVector_etajet1[17].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_step4_2);
		    m_hVector_etajet2[17].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_step4_2);
		    m_hVector_phijet1[17].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_step4_2);
		    m_hVector_phijet2[17].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHFplus[17].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHFminus[17].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHEplus[17].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHEminus[17].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHFpfplus[17].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_step4_2);
		    m_hVector_sumEHFpfminus[17].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_step4_2);
		    m_hVector_deltaEtaPF[17].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_step4_2);
		    m_hVector_absdeltaEtaPF[17].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_step4_2);
		    m_hVector_vertex[17].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_step4_2);
		    m_hVector_lumi[17].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		    m_hVector_sumEHFpfplusVsetaMax[17].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		    m_hVector_sumEHFpfminusVsetaMin[17].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		    m_hVector_sumEHFplusVsetaMax[17].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		    m_hVector_sumEHFminusVsetaMin[17].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		    m_hVector_sumECastor[17].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		    for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		      m_hVector_sumEHFplusVsiEta[17][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_step4_2);
		      m_hVector_sumEHFminusVsiEta[17][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_step4_2);
		    }

		  }

		  if ((eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ){

		    counterJetsEta2step4_1+=totalweight*triggereff_step4_1;
		    m_hVector_rjj[18].at(indexV)->Fill(eventexcl->GetRjjFromJets(),totalweight*triggereff_step4_1);
		    m_hVector_detagen[18].at(indexV)->Fill(eventexcl->GetDeltaEtaGen(),totalweight*triggereff_step4_1);
		    m_hVector_mxGen[18].at(indexV)->Fill(eventexcl->GetMxGenRange(),totalweight*triggereff_step4_1);
		    m_hVector_multhf[18].at(indexV)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight*triggereff_step4_1);
		    m_hVector_etcalos[18].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight*triggereff_step4_1);
		    m_hVector_tracks[18].at(indexV)->Fill(eventdiff->GetMultiplicityTracks(),totalweight*triggereff_step4_1);
		    m_hVector_pfetamax[18].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight*triggereff_step4_1);
		    m_hVector_pfetamin[18].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight*triggereff_step4_1);
		    m_hVector_asumE[18].at(indexV)->Fill(aSumE_,totalweight*triggereff_step4_1);
		    m_hVector_deltaetajets[18].at(indexV)->Fill(eventexcl->GetJetsDeltaEta(),totalweight*triggereff_step4_1);
		    m_hVector_deltaphijets[18].at(indexV)->Fill(deltaphi_,totalweight*triggereff_step4_1);
		    m_hVector_deltaptjets[18].at(indexV)->Fill(eventexcl->GetJetsDeltaPt(),totalweight*triggereff_step4_1);
		    m_hVector_dijetmass[18].at(indexV)->Fill(eventexcl->GetMassDijets(),totalweight*triggereff_step4_1);
		    m_hVector_ptjet1[18].at(indexV)->Fill(ptJet1_,totalweight*triggereff_step4_1);
		    m_hVector_ptjet2[18].at(indexV)->Fill(ptJet2_,totalweight*triggereff_step4_1);
		    m_hVector_etajet1[18].at(indexV)->Fill(eventexcl->GetLeadingJetEta(),totalweight*triggereff_step4_1);
		    m_hVector_etajet2[18].at(indexV)->Fill(eventexcl->GetSecondJetEta(),totalweight*triggereff_step4_1);
		    m_hVector_phijet1[18].at(indexV)->Fill(eventexcl->GetLeadingJetPhi(),totalweight*triggereff_step4_1);
		    m_hVector_phijet2[18].at(indexV)->Fill(eventexcl->GetSecondJetPhi(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHFplus[18].at(indexV)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHFminus[18].at(indexV)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHEplus[18].at(indexV)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHEminus[18].at(indexV)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHFpfplus[18].at(indexV)->Fill(eventexcl->GetSumEHFPFlowPlus(),totalweight*triggereff_step4_1);
		    m_hVector_sumEHFpfminus[18].at(indexV)->Fill(eventexcl->GetSumEHFPFlowMinus(),totalweight*triggereff_step4_1);
		    m_hVector_deltaEtaPF[18].at(indexV)->Fill(deltaetapf_,totalweight*triggereff_step4_1);
		    m_hVector_absdeltaEtaPF[18].at(indexV)->Fill(absdeltaetapf_,totalweight*triggereff_step4_1);
		    m_hVector_vertex[18].at(indexV)->Fill(eventexcl->GetNVertex(),totalweight*triggereff_step4_1);
		    m_hVector_lumi[18].at(indexV)->Fill(eventinfo->GetInstLumiBunch());
		    m_hVector_sumEHFpfplusVsetaMax[18].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventexcl->GetSumEHFPFlowPlus(),totalweight);
		    m_hVector_sumEHFpfminusVsetaMin[18].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventexcl->GetSumEHFPFlowMinus(),totalweight);
		    m_hVector_sumEHFplusVsetaMax[18].at(indexV)->Fill(eventdiff->GetEtaMaxFromPFCands(),eventdiff->GetSumEnergyHFPlus(),totalweight);
		    m_hVector_sumEHFminusVsetaMin[18].at(indexV)->Fill(eventdiff->GetEtaMinFromPFCands(),eventdiff->GetSumEnergyHFMinus(),totalweight);  
		    m_hVector_sumECastor[18].at(indexV)->Fill(eventdiff->GetSumETotCastor(),totalweight);

		    for(int ieta = 29,iring = 0; ieta <= 41; ++ieta,++iring){
		      m_hVector_sumEHFplusVsiEta[18][iring]->Fill(eventdiff->GetSumEHFPlusVsiEta(iring),totalweight*triggereff_step4_1);
		      m_hVector_sumEHFminusVsiEta[18][iring]->Fill(eventdiff->GetSumEHFMinusVsiEta(iring),totalweight*triggereff_step4_1);
		    }

		  }

		}

		}

	      } 

	    }

	  }  

	} 

      } 

    } 

    outstring << " >>----------------------End Info for Pick Events------------------------ " << std::endl;
    outstring << "" << std::endl;
    outstring << "" << std::endl;
    outstring << "<< INPUTS >>" << std::endl;
    outstring << " " << std::endl;
    outstring << "Input file: " << filein << std::endl;
    outstring << "Output file: " << savehistofile << std::endl;
    outstring << "TTree Name: " << processname << std::endl;
    outstring << " " << std::cout;
    outstring << "Input PU Data file: " << pudatafile << std::endl;
    outstring << "Output PU MC file: " << pumcfile << std::endl;
    outstring << " " << std::cout;
    outstring << "Input Efficiency File: " << filecor << std::endl;
    outstring << " " << std::cout;
    outstring << "pT(Jet1): " << jet1PT << "GeV" << std::endl;
    outstring << "pT(Jet2): " << jet2PT << "GeV" << std::endl;
    outstring << "# Vertex: " << optnVertex << std::endl;
    outstring << "Trigger Option: " << optTrigger << std::endl;
    outstring << " " << std::endl;
    outstring << "--> TRUE = 1 FALSE = 0" << std::endl;
    outstring << "PU Weight: " << switchWeightPU << std::endl;
    outstring << "Lumi. Weight: " << switchWeightLumi << std::endl;
    outstring << "Eff. Corr.: " << switchWeightEff << std::endl;
    outstring << "Trigger Eff. Corr.: " << switchTriggerEff << std::endl;
    outstring << "Evt. - Evt. Weight: " << switchWeightePw << std::endl;
    outstring << "Multiple PU Histograms: " << switchMultiple << std::endl;
    outstring << "Trigger Switch: " << switchTrigger << std::endl;
    outstring << "Pre-Selection Switch: " << switchPreSel << std::endl;
    outstring << " " << std::endl;
    outstring << "--> Factors" << std::endl;
    outstring << "Lumi. Weight: " << weightlumipass << std::endl;
    outstring << "" << std::endl;
    outstring << "<< EVENT INFO >> " << std::endl;
    outstring << " " << std::endl;
    outstring << "Total # of Events without Weight: " << TotalE << std::endl;
    outstring << "Total # of Events passed by Lumi Cut: " << counterlumi << std::endl;
    outstring << "Jet Scale Correction: " << jetstatus << std::endl;
    outstring << " " << std::endl;
    outstring << "Triggered: " << counterTrigger << std::endl;
    outstring << "STEP1: " << counterJetsstep1 << std::endl;
    outstring << "STEP2: " << counterJetsstep2 << std::endl;
    outstring << "STEP3 (CMS Acceptance): " << counterJetsAllstep3 << std::endl;
    outstring << "STEP4_4 (CMS Acceptance): " << counterJetsAllstep4_4 << std::endl;
    outstring << "STEP4_3 (CMS Acceptance): " << counterJetsAllstep4_3 << std::endl;
    outstring << "STEP4_2 (CMS Acceptance): " << counterJetsAllstep4_2 << std::endl;
    outstring << "STEP4_1 (CMS Acceptance): " << counterJetsAllstep4_1 << std::endl;
    outstring << " " << std::endl;
    outstring << "STEP3 (Tracker Acceptance): " << counterJetsTrackerstep3 << std::endl;
    outstring << "STEP4_4 (Tracker Acceptance): " << counterJetsTrackerstep4_4 << std::endl;
    outstring << "STEP4_3 (Tracker Acceptance): " << counterJetsTrackerstep4_3 << std::endl;
    outstring << "STEP4_2 (Tracker Acceptance): " << counterJetsTrackerstep4_2 << std::endl;
    outstring << "STEP4_1 (Tracker Acceptance): " << counterJetsTrackerstep4_1 << std::endl;
    outstring << " " << std::endl;
    outstring << "STEP3 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step3 << std::endl;
    outstring << "STEP4_4 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step4_4 << std::endl;
    outstring << "STEP4_3 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step4_3 << std::endl;
    outstring << "STEP4_2 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step4_2 << std::endl;
    outstring << "STEP4_1 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step4_1 << std::endl;
    outstring << "" << std::endl;
    outstring << "Histogram Trigger Efficiency Limits:" << std::endl;
    outstring << "" << std::endl;
    outstring << "Checking Histograms Range..." << std:: endl;
    outstring << "\nLumi. min4. : " << lumiminl4 << "  | Lumi. max4. : "<< lumimaxl4 << std::endl;
    outstring << "Lumi. min3. : " << lumiminl3 << "  | Lumi. max3. : "<< lumimaxl3 << std::endl;
    outstring << "Lumi. min2. : " << lumiminl2 << "  | Lumi. max2. : "<< lumimaxl2 << std::endl;
    outstring << "Lumi. min1. : " << lumiminl1 << "  | Lumi. max1. : "<< lumimaxl1 << std::endl;
    outstring << " " << std::endl;
    outstring << "<< LEGEND >> " << std::endl;
    outstring << "STEP1: # Vertex selection." << std::endl;
    outstring << "STEP2: # Vertex selection + Two Jets." << std::endl;
    outstring << "STEP3: # Vertex selection + Two Jets + Jets in Tracker Acceptance or All CMS." << std::endl;
    outstring << "STEP4_X: # Vertex selection + Two Jets + Jets in Tracker Acceptance or All CMS + Eta_max < X and Eta_min > X." << std::endl;
    outstring << " " << std::endl;


    outf->Write();
    outf->Close();
    outstring.close();

  }

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
  int main(int argc, char **argv)
  {
    AutoLibraryLoader::enable();

    const char *s1="*";
    std::string filein_;
    std::string savehistofile_;
    std::string pudatafile_;
    std::string pumcfile_;
    std::string processname_;
    std::string filecor_;
    std::string filetrigger_;
    double jet1PT_;
    double jet2PT_;
    std::string JetUn_;
    int optnVertex_;
    int optTrigger_;
    bool switchWeightPU_;
    bool switchWeightLumi_;
    bool switchWeightEff_;
    bool switchTriggerEff_;
    bool switchWeightePw_;
    bool switchMultiple_;
    bool switchPreSel_;
    bool switchTrigger_;
    double weightlumipass_;

    if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
    if (argc > 2 && strcmp(s1,argv[2]) != 0)  savehistofile_  = argv[2];
    if (argc > 3 && strcmp(s1,argv[3]) != 0)  pudatafile_  = argv[3];
    if (argc > 4 && strcmp(s1,argv[4]) != 0)  pumcfile_  = argv[4];
    if (argc > 5 && strcmp(s1,argv[5]) != 0)  processname_  = argv[5];
    if (argc > 6 && strcmp(s1,argv[6]) != 0)  filecor_  = argv[6];
    if (argc > 7 && strcmp(s1,argv[7]) != 0)  filetrigger_  = argv[7];
    if (argc > 8 && strcmp(s1,argv[8]) != 0)  jet1PT_  = atoi(argv[8]);
    if (argc > 9 && strcmp(s1,argv[9]) != 0)  jet2PT_ = atoi(argv[9]);
    if (argc > 10 && strcmp(s1,argv[10]) != 0)  JetUn_ = argv[10];
    if (argc > 11 && strcmp(s1,argv[11]) != 0)  optnVertex_ = atoi(argv[11]);
    if (argc > 12 && strcmp(s1,argv[12]) != 0)  optTrigger_   = atoi(argv[12]);
    if (argc > 13 && strcmp(s1,argv[13]) != 0)  switchWeightPU_  = atoi(argv[13]);
    if (argc > 14 && strcmp(s1,argv[14]) != 0)  switchWeightLumi_ = atoi(argv[14]);
    if (argc > 15 && strcmp(s1,argv[15]) != 0)  switchWeightEff_ = atoi(argv[15]);
    if (argc > 16 && strcmp(s1,argv[16]) != 0)  switchTriggerEff_ = atoi(argv[16]);
    if (argc > 17 && strcmp(s1,argv[17]) != 0)  switchWeightePw_   = atoi(argv[17]);
    if (argc > 18 && strcmp(s1,argv[18]) != 0)  switchMultiple_   = atoi(argv[18]);
    if (argc > 19 && strcmp(s1,argv[19]) != 0)  switchPreSel_   = atoi(argv[19]);
    if (argc > 20 && strcmp(s1,argv[20]) != 0)  switchTrigger_   = atoi(argv[20]);
    if (argc > 21 && strcmp(s1,argv[21]) != 0)  weightlumipass_  = atof(argv[21]);

    ExclDijetsComp* exclDijets = new ExclDijetsComp();   
    exclDijets->Run(filein_, savehistofile_, pudatafile_, pumcfile_, processname_, filecor_, filetrigger_, jet1PT_, jet2PT_, JetUn_, optnVertex_, optTrigger_, switchWeightPU_, switchWeightLumi_, switchWeightEff_, switchTriggerEff_, switchWeightePw_, switchMultiple_, switchPreSel_, switchTrigger_, weightlumipass_);

    return 0;
  }
#endif
