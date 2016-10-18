#include "../include/fonctions.h"
#include <sstream>

#include "TFile.h"
#include "TH2I.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TF1.h"
#include "TCanvas.h"

using namespace std;

int main(){
	//Output CSV file
	ofstream output("Results-8.csv",ios::app);

	/** PROGRAM CODE **/
	// Start with random seed
	random_device rd;
	Generator generator(rd());

	// Get the thing started
	generator.discard(100);

	//Usefu

	//Definition of the scintillators
	Scintillator botScint, topScint;

	//Definition of the RPC
	RPC Rpc;
	setRPC(Rpc);

	//Definitions of the coordinates and of the cosmic direction
	Point muonPos, RPCPos;
	Direction direction;

	TFile *outputFile = new TFile("Results-8.root", "recreate");

	TH2I* ProfilEvo = new TH2I("profevo","Muon hit profil evolution in RPC partition B with threshold",91,0,91,32,0.5,32.5);
	TH2I* ProfilEvoF = new TH2I("profevo_f","Forward muon hit profil evolution  in RPC partition B with threshold",91,0,91,32,0.5,32.5);
	TH2I* ProfilEvoB = new TH2I("profevo_b","Backward muon hit profil evolution  in RPC partition B with threshold",91,0,91,32,0.5,32.5);

	for(int t=0; t<91; t++){

		//List of root IDs and root Titles for all triggered muons
		string hitprofilID = "prof" + intTostring(t);
		string hitprofilTitle = "Muon hit profil in RPC partition B for a " + intTostring(t) + "mm threshold";
		string rpcprofilID = "rpc" + intTostring(t);
		string rpcprofilTitle = "Muon hit profil in RPC for a " + intTostring(t) + "mm threshold";
		string muondistribID = "muons" + intTostring(t);
		string muondistribTitle = "Triggered muon distribution in RPC plane for a " + intTostring(t) + "mm threshold";
		string muonoriginID = "origin" + intTostring(t);
		string muonoriginTitle = "Triggered muon origin on the generation plane";
		string muondirID = "direction" + intTostring(t);
		string muondirTitle = "Triggered muon direction components";
		string distscintID = "distance" + intTostring(t);
		string distscintTitle = "Path of triggered muons through scintillators for a " + intTostring(t) + "mm threshold";

		TH1I* HitProfil = new TH1I(hitprofilID.c_str(),hitprofilTitle.c_str(),32,0.5,32.5);
		TH2I* RPCProfil = new TH2I(rpcprofilID.c_str(),rpcprofilTitle.c_str(),3,-0.5,3.5,32,0.5,32.5);
		TH2F* MuonDistrib = new TH2F(muondistribID.c_str(),muondistribTitle.c_str(),200,-0.,2000.,100,-1.,1000.);
		TH2F* MuonOrigin = new TH2F(muonoriginID.c_str(),muonoriginTitle.c_str(),450,-1000.,3500.,600,-2000.,4000.);
		TH2F* MuonDir = new TH2F(muondirID.c_str(),muondirTitle.c_str(),10,0.,90.,40,0.,360.);
		TH2F* DistScint = new TH2F(distscintID.c_str(),distscintTitle.c_str(),150,0.,150.,150,0.,150.);

		//List of root IDs and root Titles for triggered forward muons
		string hitprofilIDF = "prof_f" + intTostring(t);
		string hitprofilTitleF = "Forward muon hit profil in RPC partition B for a " + intTostring(t) + "mm threshold";
		string rpcprofilIDF = "rpc_f" + intTostring(t);
		string rpcprofilTitleF = "Forward muon hit profil in RPC for a " + intTostring(t) + "mm threshold";
		string muondistribIDF = "muons_f" + intTostring(t);
		string muondistribTitleF = "Triggered forward muon distribution in RPC plane for a " + intTostring(t) + "mm threshold";
		string muonoriginIDF = "origin_f" + intTostring(t);
		string muonoriginTitleF = "Triggered forward muon origin on the generation plane";
		string muondirIDF = "direction_f" + intTostring(t);
		string muondirTitleF = "Triggered forward muon direction components";
		string distscintIDF = "distance_f" + intTostring(t);
		string distscintTitleF = "Path of triggered forward muons through scintillators for a " + intTostring(t) + "mm threshold";

		TH1I* HitProfilF = new TH1I(hitprofilIDF.c_str(),hitprofilTitleF.c_str(),32,0.5,32.5);
		TH2I* RPCProfilF = new TH2I(rpcprofilIDF.c_str(),rpcprofilTitleF.c_str(),3,-0.5,3.5,32,0.5,32.5);
		TH2F* MuonDistribF = new TH2F(muondistribIDF.c_str(),muondistribTitleF.c_str(),200,-0.,2000.,100,-1.,1000.);
		TH2F* MuonOriginF = new TH2F(muonoriginIDF.c_str(),muonoriginTitleF.c_str(),450,-1000.,3500.,600,-2000.,4000.);
		TH2F* MuonDirF = new TH2F(muondirIDF.c_str(),muondirTitleF.c_str(),10,0.,90.,40,0.,360.);
		TH2F* DistScintF = new TH2F(distscintIDF.c_str(),distscintTitleF.c_str(),150,0.,150.,150,0.,150.);

		//List of root IDs and root Titles for triggered backward muons
		string hitprofilIDB = "prof_b" + intTostring(t);
		string hitprofilTitleB = "Backward muon hit profil in RPC partition B for a " + intTostring(t) + "mm threshold";
		string rpcprofilIDB = "rpc_b" + intTostring(t);
		string rpcprofilTitleB = "Backward Muon hit profil in RPC for a " + intTostring(t) + "mm threshold";
		string muondistribIDB = "muons_b" + intTostring(t);
		string muondistribTitleB = "Triggered backward muon distribution in RPC plane for a " + intTostring(t) + "mm threshold";
		string muonoriginIDB = "origin_b" + intTostring(t);
		string muonoriginTitleB = "Triggered backward muon origin on the generation plane";
		string muondirIDB = "direction_b" + intTostring(t);
		string muondirTitleB = "Triggered backward muon direction components";
		string distscintIDB = "distance_b" + intTostring(t);
		string distscintTitleB = "Path of triggered backward muons through scintillators for a " + intTostring(t) + "mm threshold";

		TH1I* HitProfilB = new TH1I(hitprofilIDB.c_str(),hitprofilTitleB.c_str(),32,0.5,32.5);
		TH2I* RPCProfilB = new TH2I(rpcprofilIDB.c_str(),rpcprofilTitleB.c_str(),3,-0.5,3.5,32,0.5,32.5);
		TH2F* MuonDistribB = new TH2F(muondistribIDB.c_str(),muondistribTitleB.c_str(),200,-0.,2000.,100,-1.,1000.);
		TH2F* MuonOriginB = new TH2F(muonoriginIDB.c_str(),muonoriginTitleB.c_str(),450,-1000.,3500.,600,-2000.,4000.);
		TH2F* MuonDirB = new TH2F(muondirIDB.c_str(),muondirTitleB.c_str(),10,0.,90.,40,0.,360.);
		TH2F* DistScintB = new TH2F(distscintIDB.c_str(),distscintTitleB.c_str(),150,0.,150.,150,0.,150.);

		unsigned int i = 0;
		unsigned int I = 0;
		float progress = 0.0;

		while (I < 1e7){
			muonPos = getRandomMuonPosition(generator,a_Z0-TRIGGER_WIDTH*cos(beta));
			direction = getRandomDirection(generator);

			setScint(botScint,muonPos,direction,0.);
			setScint(topScint,muonPos,direction,TRIGGER_OFFSET);

			bool in_trigger = isInBottomScintillator(botScint) && isInTopScintillator(topScint);

			if (in_trigger){
				RPCPos = getHitPositionRPC(muonPos,direction);

				double topDist = getDistanceThroughTopScint(topScint);
				double botDist = getDistanceThroughBottomScint(botScint);

				bool too_short = (topDist <= t) || (botDist <= t);

				if(!too_short){
					DistScint->Fill(botDist,topDist);

					if (isInTrapezoid(RPCPos, Rpc.corners)){
						MuonOrigin->Fill(muonPos.x,muonPos.y);
						MuonDistrib->Fill(RPCPos.x,RPCPos.z);
						MuonDir->Fill(direction.first*180./PI, direction.second*180./PI);

						if(direction.second > PI){
							MuonOriginF->Fill(muonPos.x,muonPos.y);
							MuonDistribF->Fill(RPCPos.x,RPCPos.z);
							MuonDirF->Fill(direction.first*180./PI, direction.second*180./PI);
							DistScintF->Fill(botDist,topDist);
						} else {
							MuonOriginB->Fill(muonPos.x,muonPos.y);
							MuonDistribB->Fill(RPCPos.x,RPCPos.z);
							MuonDirB->Fill(direction.first*180./PI, direction.second*180./PI);
							DistScintB->Fill(botDist,topDist);
						}

						for(int p = 0; p < 3; p++){
							if(isInTrapezoid(RPCPos,Rpc.part[p].corners)){
								for(int s = 0; s < 32; s++){
									if(isInTrapezoid(RPCPos,Rpc.part[p].strips[s])){
										RPCProfil->Fill(p,s+1);
										if(p == 1){
											HitProfil->Fill(s+1);
											ProfilEvo->Fill(t,s+1);
										}
										if(direction.second > PI){
											RPCProfilF->Fill(p,s+1);
											if(p == 1){
												HitProfilF->Fill(s+1);
												ProfilEvoF->Fill(t,s+1);
											}
										} else {
											RPCProfilB->Fill(p,s+1);
											if(p == 1){
												HitProfilB->Fill(s+1);
												ProfilEvoB->Fill(t,s+1);
											}
										}
										i++;
									}
								}
							}
						}
					}
				}
			}
			++I;

			//Print progress bar
			progress = float(I/1.e7);

			int barWidth = 70;

			cout << "Progress T" << setw(2) << setfill('0') << t;
			cout << " [";
			int pos = barWidth * progress;
			for (int i = 0; i < barWidth; ++i) {
				if (i < pos) cout << "=";
				else if (i == pos) cout << ">";
				else cout << " ";
			}
			cout << "] " << int(progress * 100.0) << " %\r";
			cout.flush();
		}
		cout << endl;

		TF1* AllMuons = new TF1("fit","[0]*exp(-0.5*((x-[1])/[2])**2) / (1 + exp(-[3]*(x-[4])))+[5]*exp(-0.5*((x-[6])/[7])**2) / (1 + exp(-[8]*(x-[9])))",1,32);
		AllMuons->SetParLimits(0,1,1000);
		AllMuons->SetParameter(1,26);
		AllMuons->SetParLimits(1,1,32);
		AllMuons->SetParameter(2,6);
		AllMuons->SetParLimits(2,1,32);
		AllMuons->SetParameter(3,-1);
		AllMuons->SetParLimits(3,-10,-0.01);
		AllMuons->SetParameter(4,24);
		AllMuons->SetParLimits(4,1,32);
		AllMuons->SetParLimits(5,1,500);
		AllMuons->SetParameter(6,27);
		AllMuons->SetParLimits(6,1,32);
		AllMuons->SetParameter(7,2);
		AllMuons->SetParLimits(7,1,32);
		AllMuons->SetParameter(8,1);
		AllMuons->SetParLimits(8,0.01,10);
		AllMuons->SetParameter(9,24);
		AllMuons->SetParLimits(9,1,32);
		HitProfil->Fit(AllMuons,"QR");

		TF1* ForwardMuons = new TF1("fit","[0]*exp(-0.5*((x-[1])/[2])**2) / (1 + exp(-[3]*(x-[4])))",1,32);
		ForwardMuons->SetParLimits(0,1,1000);
		ForwardMuons->SetParameter(1,26);
		ForwardMuons->SetParLimits(1,1,32);
		ForwardMuons->SetParameter(2,6);
		ForwardMuons->SetParLimits(2,1,32);
		ForwardMuons->SetParameter(3,-1);
		ForwardMuons->SetParLimits(3,-10,-0.01);
		ForwardMuons->SetParameter(4,24);
		ForwardMuons->SetParLimits(4,1,32);
		HitProfilF->Fit(ForwardMuons,"QR");

		TF1* BackwardMuons = new TF1("fit","[0]*exp(-0.5*((x-[1])/[2])**2) / (1 + exp(-[3]*(x-[4])))",1,32);
		BackwardMuons->SetParLimits(0,1,500);
		BackwardMuons->SetParameter(1,27);
		BackwardMuons->SetParLimits(1,1,32);
		BackwardMuons->SetParameter(2,2);
		BackwardMuons->SetParLimits(2,1,32);
		BackwardMuons->SetParameter(3,1);
		BackwardMuons->SetParLimits(3,0.01,10);
		BackwardMuons->SetParameter(4,24);
		BackwardMuons->SetParLimits(4,1,32);
		HitProfilB->Fit(BackwardMuons,"QR");

		output << t << '\t'
			   << HitProfil->GetEntries() << '\t'
			   << AllMuons->GetParameter(0) << '\t'
			   << AllMuons->GetParameter(1) << '\t'
			   << AllMuons->GetParameter(2) << '\t'
			   << AllMuons->GetParameter(3) << '\t'
			   << AllMuons->GetParameter(4) << '\t'
			   << AllMuons->GetParameter(5) << '\t'
			   << HitProfilF->GetEntries() << '\t'
			   << ForwardMuons->GetParameter(0) << '\t'
			   << ForwardMuons->GetParameter(1) << '\t'
			   << ForwardMuons->GetParameter(2) << '\t'
			   << HitProfilB->GetEntries() << '\t'
			   << BackwardMuons->GetParameter(0) << '\t'
			   << BackwardMuons->GetParameter(1) << '\t'
			   << BackwardMuons->GetParameter(2) << '\n';

		HitProfil->Write();
		RPCProfil->Write();
		MuonDistrib->Write();
		MuonDir->Write();
		MuonOrigin->Write();
		DistScint->Write();

		HitProfilF->Write();
		RPCProfilF->Write();
		MuonDistribF->Write();
		MuonDirF->Write();
		MuonOriginF->Write();
		DistScintF->Write();

		HitProfilB->Write();
		RPCProfilB->Write();
		MuonDistribB->Write();
		MuonDirB->Write();
		MuonOriginB->Write();
		DistScintB->Write();
	}

	ProfilEvo->Write();
	ProfilEvoF->Write();
	ProfilEvoB->Write();

	outputFile->Close();

	return 0;
}
