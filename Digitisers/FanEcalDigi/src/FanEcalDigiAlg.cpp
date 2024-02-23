/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
// Unit in code: mm, ns. 
// Digitization for Fan crystal ECAL design from Huaqiao Zhang.

#ifndef FAN_ECAL_DIGI_ALG_C
#define FAN_ECAL_DIGI_ALG_C

#include "FanEcalDigiAlg.h"

#include "edm4hep/SimCalorimeterHit.h"
#include "edm4hep/CalorimeterHit.h"
#include "edm4hep/Vector3f.h"
#include "edm4hep/Cluster.h"

#include "DD4hep/Detector.h"
#include <DD4hep/Objects.h>
#include <DDRec/CellIDPositionConverter.h>

#include "TVector3.h"
#include <math.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <map>

#include "CLUEAlgo.cc"
#include "CLUEAlgo.h"

#define C 299.79  // unit: mm/ns
#define PI 3.141592653

using namespace std;
using namespace dd4hep;

DECLARE_COMPONENT( FanEcalDigiAlg )

FanEcalDigiAlg::FanEcalDigiAlg(const std::string& name, ISvcLocator* svcLoc)
  : GaudiAlgorithm(name, svcLoc),
    _nEvt(0),
    recEvt(0)
{
	// Input collections
	declareProperty("SimCaloHitCollection", r_SimCaloCol, "Handle of the Input SimCaloHit collection");
 	declareProperty("MCParticleCol", m_mcParCol, "MCParticle collection (input)");
	
	// Output collections
	declareProperty("CaloHitCollection", w_DigiCaloCol, "Handle of Digi CaloHit collection");
	declareProperty("CaloAssociationCollection", w_CaloAssociationCol, "Handle of CaloAssociation collection");
}

StatusCode FanEcalDigiAlg::initialize()
{

	std::string s_outfile = _filename;
	m_wfile = new TFile(s_outfile.c_str(), "recreate");
	t_SimStep = new TTree("SimStep", "SimStep");
	t_SimBar = new TTree("SimBarHit", "SimBarHit");
	t_ClusBar = new TTree("ClusBar", "ClusBar");
        t_RecClus = new TTree("RecClus", "RecClus");
	t_RecCrossClus = new TTree("RecCrossClus", "RecCrossClus");
	t_evtClusBar = new TTree("evtClusBar", "evtClusBar");
	t_EvtClus = new TTree("EvtClus", "EvtClus");
        t_CrossClus = new TTree("CrossClus", "CrossClus");	
        t_CrossClusBar = new TTree("CrossClusBar", "CrossClusBar");
	t_MCdata = new TTree("MCdata", "MCdata");
	t_oddClusBar = new TTree("oddClusBar", "oddClusBar");
        t_evnClusBar = new TTree("evnClusBar", "evnClusBar");	

	t_SimStep->Branch("step_x", &m_step_x);
	t_SimStep->Branch("step_y", &m_step_y);
	t_SimStep->Branch("step_z", &m_step_z);
	t_SimStep->Branch("stepBar_x", &m_stepBar_x);
	t_SimStep->Branch("stepBar_y", &m_stepBar_y);
	t_SimStep->Branch("stepBar_z", &m_stepBar_z);
	t_SimStep->Branch("step_E", &m_step_E);
	t_SimStep->Branch("step_T1", &m_step_T1);
	t_SimStep->Branch("step_T2", &m_step_T2);
	t_SimBar->Branch("simBar_x", &m_simBar_x);
	t_SimBar->Branch("simBar_y", &m_simBar_y);
	t_SimBar->Branch("simBar_z", &m_simBar_z);
	t_SimBar->Branch("simBar_T1", &m_simBar_T1);
	t_SimBar->Branch("simBar_T2", &m_simBar_T2);
	t_SimBar->Branch("simBar_Q1", &m_simBar_Q1);
	t_SimBar->Branch("simBar_Q2", &m_simBar_Q2);
	t_SimBar->Branch("simBar_module", &m_simBar_module);
	t_SimBar->Branch("simBar_crystal", &m_simBar_crystal);
//=================bars in cluster=============================
	t_ClusBar->Branch("clusBar_x", &m_clusBar_x);
	t_ClusBar->Branch("clusBar_y", &m_clusBar_y);
	t_ClusBar->Branch("clusBar_z", &m_clusBar_z);
	t_ClusBar->Branch("clusBar_T1", &m_clusBar_T1);
	t_ClusBar->Branch("clusBar_T2", &m_clusBar_T2);
	t_ClusBar->Branch("clusBar_Q1", &m_clusBar_Q1);
	t_ClusBar->Branch("clusBar_Q2", &m_clusBar_Q2);
	t_ClusBar->Branch("clusBar_module", &m_clusBar_module);
	t_ClusBar->Branch("clusBar_crystal", &m_clusBar_crystal);

  t_RecClus->Branch("Ncluster", &m_Ncluster);
  t_RecClus->Branch("clus_phi", &m_clus_phi);
  t_RecClus->Branch("clus_phi_start", &m_clus_phi_start);
  t_RecClus->Branch("clus_E", &m_clus_E);
  t_RecClus->Branch("clus_Z", &m_clus_Z);
  t_RecClus->Branch("clus_aphi", &m_clus_aphi);
  t_RecClus->Branch("clus_aphi_E", &m_clus_aphi_E);
  t_RecClus->Branch("clus_Nbars", &m_clus_Nbars);
  t_RecClus->Branch("clus_chi2", &m_clus_chi2);
  t_RecClus->Branch("clus_alpha", &m_clus_alpha);
  t_RecClus->Branch("clus_beta", &m_clus_beta);
  
//  t_RecCrossClus->Branch("cluster", &m_cluster);
  t_RecCrossClus->Branch("rec_cross_R", &m_rec_cross_R);
  t_RecCrossClus->Branch("rec_cross_aphi", &m_rec_cross_aphi);
  t_RecCrossClus->Branch("rec_cross_aphi_E", &m_rec_cross_aphi_E);

//====================cross cluster============================
  t_CrossClus->Branch("cross_clus_E", &m_cross_clus_E);
  t_CrossClus->Branch("cross_clus_Z", &m_cross_clus_Z);
  t_CrossClus->Branch("cross_clus_aphi", &m_cross_clus_aphi);
  t_CrossClus->Branch("cross_clus_aphi_E", &m_cross_clus_aphi_E);
  t_CrossClus->Branch("cross_clus_R", &m_cross_clus_R);
  t_CrossClus->Branch("cross_clus_index", &m_cross_clus_index);

  t_CrossClusBar->Branch("cross_clus_bar_E", &m_cross_clus_bar_E);
  t_CrossClusBar->Branch("cross_clus_bar_Z", &m_cross_clus_bar_Z);
  t_CrossClusBar->Branch("cross_clus_bar_phi", &m_cross_clus_bar_phi);
  t_CrossClusBar->Branch("cross_clus_bar_R", &m_cross_clus_bar_R);
  t_CrossClusBar->Branch("cross_clus_bar_index", &m_cross_clus_bar_index);

//====================CLUE clustering============================
	t_oddClusBar->Branch("oclusBar_Q1", &m_oclusBar_Q1);
        t_oddClusBar->Branch("oclusBar_Q2", &m_oclusBar_Q2);
        t_oddClusBar->Branch("oclusBar_module", &m_oclusBar_module);
        t_oddClusBar->Branch("oclusBar_crystal", &m_oclusBar_crystal);
        t_oddClusBar->Branch("oclusBar_index", &m_oclusBar_index);

	t_evnClusBar->Branch("eclusBar_Q1", &m_eclusBar_Q1);
        t_evnClusBar->Branch("eclusBar_Q2", &m_eclusBar_Q2);
        t_evnClusBar->Branch("eclusBar_module", &m_eclusBar_module);
        t_evnClusBar->Branch("eclusBar_crystal", &m_eclusBar_crystal);
        t_evnClusBar->Branch("eclusBar_index", &m_eclusBar_index);	

  t_EvtClus->Branch("evt_clus_E", &m_evt_clus_E);
  t_EvtClus->Branch("evt_clus_Z", &m_evt_clus_Z);
  t_EvtClus->Branch("evt_clus_aphi", &m_evt_clus_aphi);
  t_EvtClus->Branch("evt_clus_aphi_E", &m_evt_clus_aphi_E);
  t_EvtClus->Branch("evt_clus_R", &m_evt_clus_R);
  t_EvtClus->Branch("evt_clus_R_start", &m_evt_clus_R_start);
  t_EvtClus->Branch("evt_clus_index", &m_evt_clus_index);

	t_evtClusBar->Branch("evt_clusBar_Q1", &m_evt_clusBar_Q1);
        t_evtClusBar->Branch("evt_clusBar_Q2", &m_evt_clusBar_Q2);
        t_evtClusBar->Branch("evt_clusBar_module", &m_evt_clusBar_module);
        t_evtClusBar->Branch("evt_clusBar_crystal", &m_evt_clusBar_crystal);
	t_evtClusBar->Branch("evt_clusBar_index", &m_evt_clusBar_index);
//===================MC data====================================
  	t_MCdata->Branch("MCendpoint_x", &m_MCendpoint_x);
	t_MCdata->Branch("MCendpoint_y", &m_MCendpoint_y);
	t_MCdata->Branch("MCendpoint_z", &m_MCendpoint_z);
	t_MCdata->Branch("MCendpoint_phi", &m_MCendpoint_phi);
        t_MCdata->Branch("MCendpoint_theta", &m_MCendpoint_theta);
	t_MCdata->Branch("MCendpoint_R", &m_MCendpoint_R);
	t_MCdata->Branch("MCmomentum", &m_MCmomentum);
	t_MCdata->Branch("MCmomentum_x", &m_MCmomentum_x);
	t_MCdata->Branch("MCmomentum_y", &m_MCmomentum_y);
	t_MCdata->Branch("MCmomentum_z", &m_MCmomentum_z);

	std::cout<<"FanEcalDigiAlg::m_scale="<<m_scale<<std::endl;
	m_geosvc = service<IGeomSvc>("GeomSvc");
	if ( !m_geosvc )  throw "FanEcalDigiAlg :Failed to find GeoSvc ...";
	dd4hep::Detector* m_dd4hep = m_geosvc->lcdd();
	if ( !m_dd4hep )  throw "FanEcalDigiAlg :Failed to get dd4hep::Detector ...";
	m_cellIDConverter = new dd4hep::rec::CellIDPositionConverter(*m_dd4hep);
   m_decoder = m_geosvc->getDecoder(_readout);
	if (!m_decoder) {
		error() << "Failed to get the decoder. " << endmsg;
		return StatusCode::FAILURE;
	}
	
	rndm.SetSeed(_seed);
	std::cout<<"FanEcalDigiAlg::initialize"<<std::endl;
	return GaudiAlgorithm::initialize();
}

StatusCode FanEcalDigiAlg::execute()
{
	if(_nEvt==0) std::cout<<"FanEcalDigiAlg::execute Start"<<std::endl;
	std::cout<<"Processing event: "<<_nEvt<<std::endl;
   if(_nEvt<_Nskip){ _nEvt++; return StatusCode::SUCCESS; }

	Clear();

  //=========Readin SimHit collections and create output collections============//
 	const edm4hep::SimCalorimeterHitCollection* SimHitCol =  r_SimCaloCol.get();

	edm4hep::CalorimeterHitCollection* caloVec = w_DigiCaloCol.createAndPut(); //Not used beacuse present edm4hep::CalorimeterHitCollection can not handle double-side (Q,T) readout. 
	edm4hep::MCRecoCaloAssociationCollection* caloAssoVec = w_CaloAssociationCol.createAndPut();
 	std::vector<edm4hep::MutableSimCalorimeterHit> m_simhitCol; m_simhitCol.clear();

//===========read MC data===================
 
   auto mcCol = m_mcParCol.get();
   int m_nParticles = 0;
   std::cout<<mcCol->size()<<std::endl; 
   for ( auto p : *mcCol ) {

	m_MCmomentum.push_back(p.getEnergy());		
     	m_MCmomentum_x.push_back(p.getMomentum().x);
	m_MCmomentum_y.push_back(p.getMomentum().y);
	m_MCmomentum_z.push_back(p.getMomentum().z);
	m_MCendpoint_x.push_back(p.getEndpoint().x);
	m_MCendpoint_y.push_back(p.getEndpoint().y);
	m_MCendpoint_z.push_back(p.getEndpoint().z);
    	m_MCendpoint_phi.push_back(getPhi(p.getEndpoint().x,p.getEndpoint().y));
	m_MCendpoint_theta.push_back(getTheta(p.getEndpoint().x,p.getEndpoint().y,p.getEndpoint().z));
	m_MCendpoint_R.push_back(getR(p.getEndpoint().x,p.getEndpoint().y,p.getEndpoint().z));
//       cout<<"MC R: "<<getR(p.getEndpoint().x,p.getEndpoint().y,p.getEndpoint().z)<<" ;Phi: "<<getPhi(p.getEndpoint().x,p.getEndpoint().y)<<endl;
    }
    t_MCdata->Fill();
  //================Digitization=================
  if(SimHitCol == 0) 
  {
     std::cout<<"not found SimCalorimeterHitCollection"<< std::endl;
     return StatusCode::SUCCESS;
  }
  if(_Debug>=1) std::cout<<"digi, input sim hit size="<< SimHitCol->size() <<std::endl;

	double totE_bar=0;

	//Merge input simhit(steps) to real simhit(bar).
	m_simhitCol = MergeHits(*SimHitCol);
	if(_Debug>=1) std::cout<<"Finish Hit Merge, with Nhit: "<<m_simhitCol.size()<<std::endl;

  std::vector<CaloBar> HitBarVec; HitBarVec.clear(); 
	//Loop in SimHit, digitalize SimHit to DigiBar
	for(int i=0;i<m_simhitCol.size();i++){

		edm4hep::MutableSimCalorimeterHit SimHit = m_simhitCol.at(i);
		if(SimHit.getEnergy()<_Eth) continue;


		unsigned long long id = SimHit.getCellID();
		CaloBar hitbar;
		hitbar.setcellID( id);
		hitbar.setcellID(	m_decoder->get(id, "system"), 
											m_decoder->get(id, "module"), 
											m_decoder->get(id, "crystal"));

		dd4hep::Position hitpos = m_cellIDConverter->position(id);
		dd4hep::Position barpos(10*hitpos.x(), 10*hitpos.y(), 10*hitpos.z());	//cm to mm.
		hitbar.setPosition(barpos);
		//hitbar.T1 = 99999; hitbar.T2 = 99999;
		//if(_Debug>=2) std::cout<<"SimHit contribution size: "<<SimHit.contributions_size()<<std::endl;


		std::vector<CaloStep> DigiLvec; DigiLvec.clear();
		std::vector<CaloStep> DigiRvec; DigiRvec.clear();
		double totQ1 = 0;
		double totQ2 = 0;

		//Loop in all SimHitContribution(G4Step). 
		for(int iCont=0; iCont < SimHit.contributions_size(); ++iCont){
			edm4hep::CaloHitContribution conb = SimHit.getContributions(iCont);
			if( !conb.isAvailable() ) { std::cout<<"FanEcalDigiAlg  Can not get SimHitContribution: "<<iCont<<std::endl; continue;}

			double en = conb.getEnergy();
			if(en == 0) continue;
      
			dd4hep::Position steppos(conb.getStepPosition().x, conb.getStepPosition().y, conb.getStepPosition().z);
			dd4hep::Position rpos = steppos-hitbar.getPosition();
      
			m_step_x.push_back(steppos.x());
			m_step_y.push_back(steppos.y());
			m_step_z.push_back(steppos.z());
			m_step_E.push_back(en);
			m_stepBar_x.push_back(hitbar.getPosition().x());
			m_stepBar_y.push_back(hitbar.getPosition().y());
			m_stepBar_z.push_back(hitbar.getPosition().z());

			if(_Debug>=3){
				cout<<"Cell Pos: "<<hitbar.getPosition().x()<<'\t'<<hitbar.getPosition().y()<<'\t'<<hitbar.getPosition().z()<<endl;
				cout<<"step pos: "<<steppos.x()<<'\t'<<steppos.y()<<'\t'<<steppos.z()<<endl;
				cout<<"Relative pos: "<<rpos.x()<<'\t'<<rpos.y()<<'\t'<<rpos.z()<<endl;
				cout<<"Cell: "<<hitbar.getModule()<<"  "<<hitbar.getCrystal()<<endl;
			}
      
			//Get digitalized signal(Q1, Q2, T1, T2) from step
			//Define: 1 is left, 2 is right, clockwise direction in phi. 
      int sign = (steppos.Mag2()-hitbar.getPosition().Mag2()>0) ? -1 : 1; // Inner: sign=-1; outer: sign=1; 
      double Qi_inner = en*exp( -( Lbar/2 - sign*sqrt(rpos.Mag2()) )/Latt );
      double Qi_outer = en*exp( -( Lbar/2 + sign*sqrt(rpos.Mag2()) )/Latt );



			double Ti_inner = -1; int looptime=0;
			while(Ti_inner<0){ 
				Ti_inner = Tinit + rndm.Gaus(nMat*(Lbar/2 - sign*sqrt(rpos.Mag2()))/C, Tres); 
				looptime++;
				if(looptime>500){ std::cout<<"ERROR: Step "<<iCont<<" can not get a positive left-side time!"<<std::endl; break;}
			}
			if(looptime>500) continue;		
			double Ti_outer = -1; looptime=0;
			while(Ti_outer<0){ 
				Ti_outer = Tinit + rndm.Gaus(nMat*(Lbar/2 + sign*sqrt(rpos.Mag2()))/C, Tres); 
				looptime++;
            if(looptime>500){ std::cout<<"ERROR: Step "<<iCont<<" can not get a positive right-side time!"<<std::endl; break;}
			}
			if(looptime>500) continue;		


			m_step_T1.push_back(Ti_inner);
			m_step_T2.push_back(Ti_outer);
			totQ1 += Qi_inner;
			totQ2 += Qi_outer;
	
			CaloStep stepoutL, stepoutR;
			stepoutL.setQ(Qi_inner); stepoutL.setT(Ti_inner);
			stepoutR.setQ(Qi_outer); stepoutR.setT(Ti_outer);
			DigiLvec.push_back(stepoutL);
			DigiRvec.push_back(stepoutR);


		}

		//Time digitalization
		//if(_Debug>=2) std::cout<<"Time Digitalize: time at Q >"<<_Qthfrac<<"*totQ"<<std::endl;
		std::sort(DigiLvec.begin(), DigiLvec.end());
		std::sort(DigiRvec.begin(), DigiRvec.end());
		double thQ1=0;
		double thQ2=0;
		double thT1, thT2; 
		for(int iCont=0;iCont<DigiLvec.size();iCont++){
			thQ1 += DigiLvec[iCont].getQ();
			if(thQ1>totQ1*_Qthfrac){
				thT1 = DigiLvec[iCont].getT(); 
				if(_Debug>=3) std::cout<<"Get T1 at index: "<<iCont<<std::endl;
				break;
			}
		}
		for(int iCont=0;iCont<DigiRvec.size();iCont++){
			thQ2 += DigiRvec[iCont].getQ();
			if(thQ2>totQ2*_Qthfrac){ 
				thT2 = DigiRvec[iCont].getT(); 
				if(_Debug>=3) std::cout<<"Get T2 at index: "<<iCont<<std::endl;
				break;
			}
		}
		hitbar.setT(thT1, thT2);
		hitbar.setQ(totQ1, totQ2);
    HitBarVec.push_back(hitbar);

		totE_bar+=hitbar.getEnergy();
		//unsigned long int blockID = coder(hitbar);
		//DigiBlocks[blockID].push_back(hitbar);
		
		m_simBar_x.push_back(hitbar.getPosition().x());
		m_simBar_y.push_back(hitbar.getPosition().y());
		m_simBar_z.push_back(hitbar.getPosition().z());
		m_simBar_Q1.push_back(hitbar.getQ1());
		m_simBar_Q2.push_back(hitbar.getQ2());
		m_simBar_T1.push_back(hitbar.getT1());
		m_simBar_T2.push_back(hitbar.getT2());
		m_simBar_module.push_back(hitbar.getModule());
		m_simBar_crystal.push_back(hitbar.getCrystal());
		m_simBar_id.push_back(_nEvt);
	}
	t_SimStep->Fill();
	t_SimBar->Fill();
	if(_Debug>=1) std::cout<<"End Loop: Bar Digitalization!"<<std::endl;
	std::cout<<"Total Bar Energy: "<<totE_bar<<std::endl;
  //=======Digitization end, results are stored in HitBarVec=======

  //====================Reconstruction=================================
  std::vector<CaloCluster> ClusVec; ClusVec.clear();
  NeighborClustering(HitBarVec,ClusVec);
  vector<CaloCluster> evn_clus; evn_clus.clear();
  vector<CaloCluster> odd_clus; odd_clus.clear();

  vector<CaloCluster> evt_clus; evt_clus.clear();
   //dc, deltao, deltac, rho  2 5 2 0.2/2
  CLUEClustering(HitBarVec,odd_clus,"odd",2,5,3,0.05);
  CLUEClustering(HitBarVec,evn_clus,"even",2,5,3,0.05);
  
//  CombineInCluster(odd_clus);
//  CombineInCluster(evn_clus);

  for(int io=0;io<odd_clus.size();io++){
    m_odd_clus_E.push_back(odd_clus[io].getEnergy());
    m_odd_clus_Z.push_back(odd_clus[io].getZ());
    m_odd_clus_aphi.push_back(odd_clus[io].getAvePhi(false));
    m_odd_clus_aphi_E.push_back(odd_clus[io].getAvePhi(true));
 //   cout<<"odd clus Phi: "<<odd_clus[io].getAvePhi(true)<<" ; Energy: "<<odd_clus[io].getEnergy()<<endl;
    vector<CaloBar> oddbars; oddbars.clear();
    oddbars = odd_clus[io].getBars();
    for(int iob = 0; iob<oddbars.size(); iob++){
        m_oclusBar_Q1.push_back(oddbars[iob].getQ1());
        m_oclusBar_Q2.push_back(oddbars[iob].getQ2());
        m_oclusBar_module.push_back(oddbars[iob].getModule());
        m_oclusBar_crystal.push_back(oddbars[iob].getCrystal());
        m_oclusBar_index.push_back(io);
    }
  }

  for(int ie=0;ie<evn_clus.size();ie++){
    m_evn_clus_E.push_back(evn_clus[ie].getEnergy());
    m_evn_clus_Z.push_back(evn_clus[ie].getZ());
    m_evn_clus_aphi.push_back(evn_clus[ie].getAvePhi(false));
    m_evn_clus_aphi_E.push_back(evn_clus[ie].getAvePhi(true));
//    cout<<"evn clus Phi: "<<evn_clus[ie].getAvePhi(true)<<" ; Energy: "<<evn_clus[ie].getEnergy()<<endl;
    vector<CaloBar> evnbars; evnbars.clear();
    evnbars = evn_clus[ie].getBars();
    for(int ieb = 0; ieb<evnbars.size(); ieb++){
        m_eclusBar_Q1.push_back(evnbars[ieb].getQ1());
        m_eclusBar_Q2.push_back(evnbars[ieb].getQ2());
        m_eclusBar_module.push_back(evnbars[ieb].getModule());
        m_eclusBar_crystal.push_back(evnbars[ieb].getCrystal());
        m_eclusBar_index.push_back(ie);
    }
  }
 
  bool merge = true;
  while(merge){
     merge = false;
     MergeCluster(odd_clus,evn_clus,evt_clus);
  }

  vector<TLorentzVector> sec_partile;   sec_partile.clear();
  
  int n_rec_clus = 0;
  for(int im=0;im<evt_clus.size();im++){
//     cout<<"Evt "<<im<<" E:"<<evt_clus[im].getEnergy()<<" phi: "<<evt_clus[im].getAvePhi()<<endl;
     if(evt_clus[im].getEnergy()>=1.){ n_rec_clus +=1; }

     vector<CaloCrossBar> cross_bars; cross_bars.clear();
     CaloCrossCluster cross_clus; cross_clus.Clear();
     vector<CaloBar> evt_bars; evt_bars.clear();
     evt_bars = evt_clus[im].getBars();
     for(int it=0; it<evt_bars.size(); it++){
        m_evt_clusBar_Q1.push_back(evt_bars[it].getQ1());
        m_evt_clusBar_Q2.push_back(evt_bars[it].getQ2());
        m_evt_clusBar_module.push_back(evt_bars[it].getModule());
        m_evt_clusBar_crystal.push_back(evt_bars[it].getCrystal());
        m_evt_clusBar_index.push_back(im);
        vector<CaloCrossBar> bar_cross_vec = evt_clus[im].getCrossPoint(evt_bars[it]);
	MergeCrossBars(bar_cross_vec,cross_bars);
     }

     for(int ic=0; ic<cross_bars.size();ic++){
	m_cross_clus_bar_E.push_back(cross_bars[ic].getEnergy());
	m_cross_clus_bar_phi.push_back(cross_bars[ic].getPositionPhi());
	m_cross_clus_bar_Z.push_back(cross_bars[ic].getPositionZ());
	m_cross_clus_bar_R.push_back(cross_bars[ic].getPositionR());
	m_cross_clus_bar_index.push_back(im);
     }
     ToCrossCluster(cross_bars,cross_clus);
     m_cross_clus_R.push_back(cross_clus.getCrossR(true));
     m_cross_clus_aphi_E.push_back(cross_clus.getAvePhi(true));
     m_cross_clus_aphi.push_back(cross_clus.getAvePhi(false));
     m_cross_clus_Z.push_back(cross_clus.getCrossZ());
     m_cross_clus_E.push_back(cross_clus.getEnergy());
     m_cross_clus_index.push_back(im);

     m_evt_clus_index.push_back(im);
     m_evt_clus_E.push_back(evt_clus[im].getEnergy());
     m_evt_clus_Z.push_back(evt_clus[im].getClusModule());
     m_evt_clus_aphi.push_back(evt_clus[im].getAvePhi(false));
     m_evt_clus_aphi_E.push_back(evt_clus[im].getAvePhi(true));
     m_evt_clus_R.push_back(cross_clus.getCrossR(true));// cross R
     m_evt_clus_R_start.push_back(cross_clus.getCrossRStart());// R_start
  }

  if(n_rec_clus > 0){ 
     recEvt +=1; cout<<" n_recEvt: "<<recEvt<<endl;
  }
 
  double tot_clus_E = 0;
  m_Ncluster = ClusVec.size();
  for(int ic=0; ic<m_Ncluster; ic++){

      m_clus_phi_start.push_back(ClusVec[ic].getPhiStart());
      m_clus_phi.push_back(ClusVec[ic].getPhiStartFit());
      m_clus_Z.push_back(ClusVec[ic].getZ());
      m_clus_aphi_E.push_back(ClusVec[ic].getAvePhi(true));
      m_clus_aphi.push_back(ClusVec[ic].getAvePhi(false));
      m_clus_E.push_back(ClusVec[ic].getEnergy());
      m_clus_chi2.push_back(ClusVec[ic].getFitChi2());
      m_clus_alpha.push_back(ClusVec[ic].getFitAlpha());
      m_clus_beta.push_back(ClusVec[ic].getFitBeta());
      m_clus_Nbars.push_back(ClusVec[ic].getBars().size());
            
	vector<CaloCrossBar> rec_cross_bars;  rec_cross_bars.clear();
        CaloCrossCluster rec_cross_clus;  rec_cross_clus.Clear();

        vector<CaloBar> n_hitBars;  n_hitBars.clear();
        n_hitBars = ClusVec[ic].getBars();
      for(int ibar=0;ibar<n_hitBars.size();ibar++){
	m_clusBar_x.push_back(n_hitBars[ibar].getPosition().x());
        m_clusBar_y.push_back(n_hitBars[ibar].getPosition().y());
        m_clusBar_z.push_back(n_hitBars[ibar].getPosition().z());
        m_clusBar_Q1.push_back(n_hitBars[ibar].getQ1());
        m_clusBar_Q2.push_back(n_hitBars[ibar].getQ2());
        m_clusBar_T1.push_back(n_hitBars[ibar].getT1());
        m_clusBar_T2.push_back(n_hitBars[ibar].getT2());
    	m_clusBar_module.push_back(n_hitBars[ibar].getModule());
        m_clusBar_crystal.push_back(n_hitBars[ibar].getCrystal());
        vector<CaloCrossBar> rec_bar_cross_vec = ClusVec[ic].getCrossPoint(n_hitBars[ibar]);
        MergeCrossBars(rec_bar_cross_vec,rec_cross_bars);     
      } 
      ToCrossCluster(rec_cross_bars,rec_cross_clus);
     m_rec_cross_R.push_back(rec_cross_clus.getCrossR(true));
     m_rec_cross_aphi.push_back(rec_cross_clus.getAvePhi(false));
     m_rec_cross_aphi_E.push_back(rec_cross_clus.getAvePhi(true));
  }
  t_RecClus->Fill();
  t_RecCrossClus->Fill();
  t_oddClusBar->Fill();
  t_evnClusBar->Fill();
  t_ClusBar->Fill();
  t_EvtClus->Fill();
  t_evtClusBar->Fill();
  t_CrossClus->Fill();
  t_CrossClusBar->Fill(); 
 
  _nEvt ++ ;
  return StatusCode::SUCCESS;
}

StatusCode FanEcalDigiAlg::finalize()
{
	m_wfile->cd();
	t_SimStep->Write();
	t_SimBar->Write();
	t_ClusBar->Write();
 	t_RecClus->Write();
	t_RecCrossClus->Write();
        t_oddClusBar->Write();
	t_evnClusBar->Write();
        t_evtClusBar->Write();
	t_EvtClus->Write();
        t_CrossClus->Write();
        t_CrossClusBar->Write();
	t_MCdata->Write(); 
	m_wfile->Close();

  info() << "Processed " << _nEvt << " events " << endmsg;
  cout<<"there are "<<double(recEvt)/_nEvt<<" evts reconstructed."<<endl;
  return GaudiAlgorithm::finalize();
}

std::vector<edm4hep::MutableSimCalorimeterHit> FanEcalDigiAlg::MergeHits(const edm4hep::SimCalorimeterHitCollection& m_col){
	std::vector<edm4hep::MutableSimCalorimeterHit> m_mergedhit;
	m_mergedhit.clear();

	for(int iter=0;iter<m_col.size();iter++){
		edm4hep::SimCalorimeterHit m_step = m_col[iter];
		if(!m_step.isAvailable()){ cout<<"ERROR HIT!"<<endl; continue;}
		if(m_step.getEnergy()==0) continue;
		unsigned long long cellid = m_step.getCellID();
		dd4hep::Position hitpos = m_cellIDConverter->position(cellid);
		edm4hep::Vector3f pos(hitpos.x()*10, hitpos.y()*10, hitpos.z()*10);

		edm4hep::MutableCaloHitContribution conb;
		conb.setEnergy(m_step.getEnergy());
		conb.setStepPosition(m_step.getPosition());
		edm4hep::MutableSimCalorimeterHit m_hit = find(m_mergedhit, cellid);
		if(m_hit.getCellID()==0){
			//m_hit = new edm4hep::SimCalorimeterHit();
			m_hit.setCellID(cellid);
			m_hit.setPosition(pos);
			m_mergedhit.push_back(m_hit);
		}
		m_hit.addToContributions(conb);
		m_hit.setEnergy(m_hit.getEnergy()+m_step.getEnergy());
	}
	return m_mergedhit;
}


edm4hep::MutableSimCalorimeterHit FanEcalDigiAlg::find(std::vector<edm4hep::MutableSimCalorimeterHit>& m_col, unsigned long long& cellid){
   for(int i=0;i<m_col.size();i++){
		edm4hep::MutableSimCalorimeterHit hit=m_col.at(i);
		if(hit.getCellID() == cellid) return hit;
	}
	edm4hep::MutableSimCalorimeterHit hit;
	hit.setCellID(0);
	return hit;
}


StatusCode FanEcalDigiAlg::NeighborClustering(std::vector<CaloBar>& m_barVec, std::vector<CaloCluster>& m_clusVec ){

  if(m_barVec.size()==0) return StatusCode::SUCCESS;

  m_clusVec.clear();
  for(int ib=0; ib<m_barVec.size(); ib++ ){

    bool isNew = true;
    for(int ic=0; ic<m_clusVec.size(); ic++){
      if( m_clusVec[ic].isNeighbor(m_barVec[ib]) ){
        m_clusVec[ic].AddBar(m_barVec[ib]); 
        isNew = false; 
        break; 
    }}
    if(isNew){
      CaloCluster m_clus; m_clus.Clear();
      m_clus.AddBar( m_barVec[ib] );
      m_clusVec.push_back( m_clus );
    }
  }

  return StatusCode::SUCCESS;
}

StatusCode FanEcalDigiAlg::CLUEClustering(std::vector<CaloBar>& m_barVec, std::vector<CaloCluster>& m_clus, string input, double dc, double deltao, double deltac, double rho){
   
   if(m_barVec.size()==0) return StatusCode::SUCCESS;

   std::vector<float> x;        x.clear();
   std::vector<float> y;        y.clear();
   std::vector<int> layer;      layer.clear();
   std::vector<float> weight;   weight.clear();

   for(int ib=0; ib<m_barVec.size(); ib++ ){
      if(input.compare("even") == 0){
	if((int)m_barVec[ib].getModule()%2 == 0){
	   x.push_back(m_barVec[ib].getModule());
	   y.push_back(m_barVec[ib].getCrystal());
	   layer.push_back(0);
	   weight.push_back(m_barVec[ib].getEnergy());
	}
      }
      if(input.compare("odd") == 0){
	if((int)m_barVec[ib].getModule()%2 != 0){
           x.push_back(m_barVec[ib].getModule());
           y.push_back(m_barVec[ib].getCrystal());
           layer.push_back(0);
           weight.push_back(m_barVec[ib].getEnergy());
        }
      }
   }
   //dc: critical distance used to compute the local density; 
   //deltao: maximum distance for a point to be linked to a nearest higher point
   //deltac: minimum distance for a local high density point to be promoted as a Seed
   //rho: minimum local density for a point to be promoted as a Seed;
   CLUEAlgo clueAlgo(dc,deltao,deltac,rho,-1);  //dc deltao deltac rho 1,5,2,0.2
//   for (int r = 0; r<4; r++){
      clueAlgo.setPoints(x.size(), &x[0],&y[0],&layer[0],&weight[0]);
      clueAlgo.makeClusters();
//   }
   Points resultPoints; resultPoints.clear();
   string outputFileType = "csvFile";
   string outputFileName = "result";
   clueAlgo.verboseResults(resultPoints, outputFileType, outputFileName, -1);
   clueAlgo.verboseToCluster(resultPoints, m_clus); 
   
   return StatusCode::SUCCESS;
}

StatusCode FanEcalDigiAlg::MergeCluster(std::vector<CaloCluster>& m_oddVec, std::vector<CaloCluster>& m_evnVec, std::vector<CaloCluster>& m_evtVec){
  if(m_oddVec.size() == 0 || m_evnVec.size() ==0 ) return StatusCode::SUCCESS;

  m_evtVec.clear();
  std::sort(m_oddVec.begin(), m_oddVec.end(), [](CaloCluster& a, CaloCluster& b) {
        return a.getEnergy() > b.getEnergy();//getAvePhi()
  });

  std::sort(m_evnVec.begin(), m_evnVec.end(), [](CaloCluster& a, CaloCluster& b) {
        return a.getEnergy() > b.getEnergy();
  });

  
  bool found = true;
  while(found){
     found = false;
     for(int io=0; io<m_oddVec.size(); io++){
        for(int ie=0; ie<m_evnVec.size(); ie++){
           double diff = m_evnVec[ie].getAvePhi(true)-m_oddVec[io].getAvePhi(true);
           cout<<"============================================== diff = "<<diff<<endl;
           if(abs(diff) < 3) {
                CombineDiffCluster(m_oddVec[io], m_evnVec[ie]);
		m_evtVec.push_back(m_evnVec[ie]);
                m_oddVec.erase(m_oddVec.begin()+io);
		m_evnVec.erase(m_evnVec.begin()+ie);
                found = true;
                break;
           }

        }
        if(found) {break;}
     }
  }

  for(int it=0;it<m_oddVec.size();it++){ m_evtVec.push_back(m_oddVec[it]);}
  for(int it=0;it<m_evnVec.size();it++){ m_evtVec.push_back(m_evnVec[it]);}
  return StatusCode::SUCCESS;
}

StatusCode FanEcalDigiAlg::ToCrossCluster(vector<CaloCrossBar>& m_bars, CaloCrossCluster& m_cross_clus){
  m_cross_clus.Clear();
  for(int i=0;i<m_bars.size();i++){
     m_cross_clus.AddBar(m_bars[i]);
  }
  return StatusCode::SUCCESS;
}

StatusCode FanEcalDigiAlg::MergeCrossBars(vector<CaloCrossBar>& m_bars, vector<CaloCrossBar>& n_bars){

  for(int i=0;i<m_bars.size();i++){
     n_bars.push_back(m_bars[i]);
  }
  return StatusCode::SUCCESS;
}

StatusCode FanEcalDigiAlg::CombineInCluster(std::vector<CaloCluster>& m_clus){
  for(int i=0;i<m_clus.size();++i){
     for (int j=i+1;j<m_clus.size();++j){
	double diff = abs(m_clus[i].getAvePhi(true)-m_clus[j].getAvePhi(true));
	if(diff<=1){
	   CombineDiffCluster(m_clus[j],m_clus[i]);
//	cout<<"Merge in Cluster : "<<m_clus[i].getAvePhi(true)<<" & "<<m_clus[j].getAvePhi(true)<<endl;	   
	   m_clus.erase(m_clus.begin()+j);
	   i=0;
	   j=0;
	}
     }
  }
  return StatusCode::SUCCESS;
}

StatusCode FanEcalDigiAlg::CombineDiffCluster(CaloCluster& m_odd_clus, CaloCluster& m_evn_clus){

   vector<CaloBar> barVec; barVec.clear();
   barVec = m_odd_clus.getBars();

   for(int i=0;i<barVec.size();i++){
      m_evn_clus.AddBar(barVec[i]);
   }
   return StatusCode::SUCCESS;
}

double FanEcalDigiAlg::getPhi(double x, double y){
  double phi=0;
  if(x>0&&y>0) {phi = atan2(y,x)*180./PI;}
  if(x<0&&y>0) {phi = 180.-atan2(abs(y),abs(x))*180./PI;}
  if(x<0&&y<0) {phi = 180.+atan2(abs(y),abs(x))*180./PI;}
  if(x>0&&y<0) {phi = 360.-atan2(abs(y),abs(x))*180./PI;}
  return phi;  
}

double FanEcalDigiAlg::getR(double x, double y, double z){
      
      return sqrt( x*x + y*y + z*z) ; 
}

double FanEcalDigiAlg::getTheta(double x, double y, double z){
  double r = getR(x,y,z);
  if(x == 0.0 && y == 0.0 && z == 0.0){return 0.0;}
  return acos(z/r)*180./PI;
}

//=================================

void FanEcalDigiAlg::Clear(){
	m_step_x.clear();
	m_step_y.clear();
	m_step_z.clear();
	m_step_E.clear();
	m_stepBar_x.clear();
	m_stepBar_y.clear();
	m_stepBar_z.clear();
	m_step_T1.clear();
	m_step_T2.clear();
	m_simBar_x.clear();
	m_simBar_y.clear();
	m_simBar_z.clear();
	m_simBar_T1.clear();
	m_simBar_T2.clear();
	m_simBar_Q1.clear();
	m_simBar_Q2.clear();
	m_simBar_module.clear();
	m_simBar_crystal.clear();
	m_simBar_id.clear();

  m_Ncluster = -99;
  m_clus_phi.clear();
  m_clus_phi_start.clear();
  m_clus_Z.clear();
  m_clus_aphi_E.clear();
  m_clus_aphi.clear();
  m_clus_E.clear();
  m_clus_chi2.clear();
  m_clus_alpha.clear();
  m_clus_beta.clear();
  m_clus_Nbars.clear();
 
  m_evt_clusBar_Q1.clear();
  m_evt_clusBar_Q2.clear();
  m_evt_clusBar_module.clear();
  m_evt_clusBar_crystal.clear();
  m_evt_clusBar_index.clear();

  m_evt_clus_E.clear();
  m_evt_clus_index.clear();
  m_evt_clus_aphi_E.clear();
  m_evt_clus_aphi.clear();
  m_evt_clus_R.clear();
  m_evt_clus_R_start.clear();
  m_evt_clus_Z.clear();
  
  m_oclusBar_Q1.clear();
  m_oclusBar_Q2.clear();
  m_oclusBar_module.clear();
  m_oclusBar_crystal.clear();
  m_oclusBar_index.clear();
  m_eclusBar_Q1.clear();
  m_eclusBar_Q2.clear();
  m_eclusBar_module.clear();
  m_eclusBar_crystal.clear();
  m_eclusBar_index.clear();

  m_cross_clus_E.clear();
  m_cross_clus_aphi_E.clear();
  m_cross_clus_aphi.clear();
  m_cross_clus_Z.clear();
  m_cross_clus_R.clear();
  m_cross_clus_index.clear();
  m_cross_clus_bar_E.clear();
  m_cross_clus_bar_R.clear();
  m_cross_clus_bar_phi.clear();
  m_cross_clus_bar_Z.clear();
  m_cross_clus_bar_index.clear();

  m_rec_cross_aphi_E.clear();
  m_rec_cross_aphi.clear();
  m_rec_cross_R.clear();

//==============MC data====================
  m_MCmomentum.clear();
  m_MCmomentum_x.clear();
  m_MCmomentum_y.clear(); 
  m_MCmomentum_z.clear();
  m_MCendpoint_x.clear();
  m_MCendpoint_y.clear();
  m_MCendpoint_z.clear();
  m_MCendpoint_phi.clear();
  m_MCendpoint_theta.clear();
  m_MCendpoint_R.clear();
//==============bars in cluster=============
	m_clusBar_x.clear();
        m_clusBar_y.clear();
        m_clusBar_z.clear();
        m_clusBar_T1.clear();
        m_clusBar_T2.clear();
        m_clusBar_Q1.clear();
        m_clusBar_Q2.clear();
        m_clusBar_module.clear();
        m_clusBar_crystal.clear();
        m_nclusBar.clear();
	m_ncrossBar.clear();
//	m_clusBar_NR.clear();
//	m_clusBar_Nphi.clear();
//	m_clusBar_Nz.clear();
//	m_clusBar_NE.clear();
//==========================================
}

#endif
