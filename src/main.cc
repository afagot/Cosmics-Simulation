//******************************************************
//*      GIF Cosmic Muons Monte Carlo Simulation       *
//*                                                    *
//*      Alexis FAGOT                                  *
//*      alexis.fagot@ugent.be                         *
//*                                                    *
//*      October 18, 2016                              *
//*                                                    *
//*      MAIN FILE                                     *
//******************************************************


#include "../include/fonctions.h"
#include <sstream>

#include "TFile.h"
#include "TH1I.h"
#include "TH1F.h"
#include "TH2I.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TF1.h"
#include "TCanvas.h"

using namespace std;

int main(){
    for(unsigned int b=4; b<=20; b+=2){

        //definition of the inclination angle
        double beta = b*PI/180.;

        string baseName = "Results-" + intTostring(b);

        //Output CSV file
        string CSVname = baseName + ".csv";
        ofstream output(CSVname.c_str(),ios::app);

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

        string ROOTname = baseName + ".root";
        TFile *outputFile = new TFile(ROOTname.c_str(),"recreate");

        unsigned int maxDiscriThresh = 45;
        unsigned int RPCThreshSteps = 5;

        TH2I* ProfilEvo = new TH2I("profevo","Muon hit profil evolution in RPC partition B with threshold",maxDiscriThresh+1,0,maxDiscriThresh+1,32,0.5,32.5);
        TH2I* ProfilEvoF = new TH2I("profevo_f","Forward muon hit profil evolution in RPC partition B with threshold",maxDiscriThresh+1,0,maxDiscriThresh+1,32,0.5,32.5);
        TH2I* ProfilEvoB = new TH2I("profevo_b","Backward muon hit profil evolution in RPC partition B with threshold",maxDiscriThresh+1,0,maxDiscriThresh+1,32,0.5,32.5);

        for(unsigned int ts=0; ts<maxDiscriThresh+1; ts+=5){
            for(unsigned int tr=0; tr<RPCThreshSteps; tr++){

                float RPCthreshold = 2. + tr*0.25;

                //List of root IDs and root Titles for all triggered muons
                string hitprofilID = "prof-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string hitprofilTitle = "Muon hit profil in RPC part. B - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";
                string rpcprofilID = "rpc-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string rpcprofilTitle = "Muon hit profil in RPC - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";
                string distrpcID = "rpc-distance-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string distrpcTitle = "Path of triggered muons through rpc - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";
                string muondistribID = "muons-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string muondistribTitle = "Triggered muon distribution in RPC plane - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";
                string muonoriginID = "origin-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string muonoriginTitle = "Triggered muon origin on the generation plane";
                string muondirID = "direction-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string muondirTitle = "Triggered muon direction components";
                string distscintID = "scint-distance-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string distscintTitle = "Path of triggered muons through scintillators - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";

                TH1I* HitProfil = new TH1I(hitprofilID.c_str(),hitprofilTitle.c_str(),32,0.5,32.5);
                TH2I* RPCProfil = new TH2I(rpcprofilID.c_str(),rpcprofilTitle.c_str(),3,-0.5,3.5,32,0.5,32.5);
                TH1F* DistRPC = new TH1F(distrpcID.c_str(),distrpcTitle.c_str(),100,0.,10.);
                TH2F* MuonDistrib = new TH2F(muondistribID.c_str(),muondistribTitle.c_str(),200,-0.,2000.,100,-1.,1000.);
                TH2F* MuonOrigin = new TH2F(muonoriginID.c_str(),muonoriginTitle.c_str(),450,-1000.,3500.,600,-2000.,4000.);
                TH2F* MuonDir = new TH2F(muondirID.c_str(),muondirTitle.c_str(),10,0.,90.,40,0.,360.);
                TH2F* DistScint = new TH2F(distscintID.c_str(),distscintTitle.c_str(),150,0.,150.,150,0.,150.);

                //List of root IDs and root Titles for triggered forward muons
                string hitprofilIDF = "prof_f-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string hitprofilTitleF = "Forward muon hit profil in RPC partition B - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";
                string rpcprofilIDF = "rpc_f-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string rpcprofilTitleF = "Forward muon hit profil in RPC - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";
                string distrpcIDF = "rpc-distance_f-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string distrpcTitleF = "Path of triggered forward muons through rpc - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";
                string muondistribIDF = "muons_f-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string muondistribTitleF = "Triggered forward muon distribution in RPC plane - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";
                string muonoriginIDF = "origin_f-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string muonoriginTitleF = "Triggered forward muon origin on the generation plane";
                string muondirIDF = "direction_f-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string muondirTitleF = "Triggered forward muon direction components";
                string distscintIDF = "scint-distance_f-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string distscintTitleF = "Path of triggered forward muons through scintillators - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";

                TH1I* HitProfilF = new TH1I(hitprofilIDF.c_str(),hitprofilTitleF.c_str(),32,0.5,32.5);
                TH2I* RPCProfilF = new TH2I(rpcprofilIDF.c_str(),rpcprofilTitleF.c_str(),3,-0.5,3.5,32,0.5,32.5);
                TH1F* DistRPCF = new TH1F(distrpcIDF.c_str(),distrpcTitleF.c_str(),100,0.,10.);
                TH2F* MuonDistribF = new TH2F(muondistribIDF.c_str(),muondistribTitleF.c_str(),200,-0.,2000.,100,-1.,1000.);
                TH2F* MuonOriginF = new TH2F(muonoriginIDF.c_str(),muonoriginTitleF.c_str(),450,-1000.,3500.,600,-2000.,4000.);
                TH2F* MuonDirF = new TH2F(muondirIDF.c_str(),muondirTitleF.c_str(),10,0.,90.,40,0.,360.);
                TH2F* DistScintF = new TH2F(distscintIDF.c_str(),distscintTitleF.c_str(),150,0.,150.,150,0.,150.);

                //List of root IDs and root Titles for triggered backward muons
                string hitprofilIDB = "prof_b-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string hitprofilTitleB = "Backward muon hit profil in RPC B part. - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";
                string rpcprofilIDB = "rpc_b-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string rpcprofilTitleB = "Backward Muon hit profil in RPC - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";
                string distrpcIDB = "rpc-distance_b-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string distrpcTitleB = "Path of triggered backward muons through rpc - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";
                string muondistribIDB = "muons_b-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string muondistribTitleB = "Triggered backward muon distribution in RPC plane - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";
                string muonoriginIDB = "origin_b-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string muonoriginTitleB = "Triggered backward muon origin on the generation plane";
                string muondirIDB = "direction_b-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string muondirTitleB = "Triggered backward muon direction components";
                string distscintIDB = "scint-distance_b-" + intTostring(ts) + "-" + floatTostring(RPCthreshold);
                string distscintTitleB = "Path of triggered backward muons through scintillators - " + intTostring(ts) + "mm scint. thr. - " + floatTostring(RPCthreshold) + "mm RPC thr. - ";

                TH1I* HitProfilB = new TH1I(hitprofilIDB.c_str(),hitprofilTitleB.c_str(),32,0.5,32.5);
                TH2I* RPCProfilB = new TH2I(rpcprofilIDB.c_str(),rpcprofilTitleB.c_str(),3,-0.5,3.5,32,0.5,32.5);
                TH1F* DistRPCB = new TH1F(distrpcIDB.c_str(),distrpcTitleB.c_str(),100,0.,10.);
                TH2F* MuonDistribB = new TH2F(muondistribIDB.c_str(),muondistribTitleB.c_str(),200,-0.,2000.,100,-1.,1000.);
                TH2F* MuonOriginB = new TH2F(muonoriginIDB.c_str(),muonoriginTitleB.c_str(),450,-1000.,3500.,600,-2000.,4000.);
                TH2F* MuonDirB = new TH2F(muondirIDB.c_str(),muondirTitleB.c_str(),10,0.,90.,40,0.,360.);
                TH2F* DistScintB = new TH2F(distscintIDB.c_str(),distscintTitleB.c_str(),150,0.,150.,150,0.,150.);

                unsigned int Nmuons = 1e8;  //The number of muons to generate
                float Stat = 1e8;           //Number used for the progress bar only - is equal to Nmuons
                float progress = 0.0;       //Estimation of the progress in percent for each threshold value

                for(unsigned int n=0; n<Nmuons; n++){
                    muonPos = getRandomMuonPosition(generator,a_Z0-TRIGGER_WIDTH*cos(beta));
                    direction = getRandomDirection(generator);

                    setScint(botScint,muonPos,direction,0.,beta);
                    setScint(topScint,muonPos,direction,TRIGGER_OFFSET,beta);

                    bool in_trigger = isInBottomScintillator(botScint,beta) && isInTopScintillator(topScint,beta);

                    if (in_trigger){
                        RPCPos = getHitPositionRPC(muonPos,direction);

                        double topDist = getDistanceThroughTopScint(topScint,beta);
                        double botDist = getDistanceThroughBottomScint(botScint,beta);
                        double rpcDist = getDistanceThroughRPC(direction);

                        bool too_short = (topDist <= ts) || (botDist <= ts) || (rpcDist <= RPCthreshold);

                        if(!too_short){
                            DistRPC->Fill(rpcDist);
                            DistScint->Fill(botDist,topDist);

                            if (isInTrapezoid(RPCPos, Rpc.corners)){
                                MuonOrigin->Fill(muonPos.x,muonPos.y);
                                MuonDistrib->Fill(RPCPos.x,RPCPos.z);
                                MuonDir->Fill(direction.first*180./PI, direction.second*180./PI);

                                if(direction.second > PI){
                                    MuonOriginF->Fill(muonPos.x,muonPos.y);
                                    MuonDistribF->Fill(RPCPos.x,RPCPos.z);
                                    MuonDirF->Fill(direction.first*180./PI, direction.second*180./PI);
                                    DistRPCF->Fill(getDistanceThroughRPC(direction));
                                    DistScintF->Fill(botDist,topDist);
                                } else {
                                    MuonOriginB->Fill(muonPos.x,muonPos.y);
                                    MuonDistribB->Fill(RPCPos.x,RPCPos.z);
                                    MuonDirB->Fill(direction.first*180./PI, direction.second*180./PI);
                                    DistRPCB->Fill(getDistanceThroughRPC(direction));
                                    DistScintB->Fill(botDist,topDist);
                                }

                                for(int p = 0; p < 3; p++){
                                    if(isInTrapezoid(RPCPos,Rpc.part[p].corners)){
                                        for(int s = 0; s < 32; s++){
                                            if(isInTrapezoid(RPCPos,Rpc.part[p].strips[s])){
                                                RPCProfil->Fill(p,s+1);
                                                if(p == 1){
                                                    HitProfil->Fill(s+1);
                                                    ProfilEvo->Fill(ts,s+1);
                                                }
                                                if(direction.second > PI){
                                                    RPCProfilF->Fill(p,s+1);
                                                    if(p == 1){
                                                        HitProfilF->Fill(s+1);
                                                        ProfilEvoF->Fill(ts,s+1);
                                                    }
                                                } else {
                                                    RPCProfilB->Fill(p,s+1);
                                                    if(p == 1){
                                                        HitProfilB->Fill(s+1);
                                                        ProfilEvoB->Fill(ts,s+1);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    //Print progress bar
                    progress = float(n/Stat);

                    int barWidth = 70;

                    cout << "Progress Ts" << intTostring(ts);
                    cout << "-Tr" << floatTostring(RPCthreshold);
                    cout << "-B" << intTostring(b);
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
                AllMuons->SetParLimits(0,1,100000);
                AllMuons->SetParameter(1,26);
                AllMuons->SetParLimits(1,1,32);
                AllMuons->SetParameter(2,6);
                AllMuons->SetParLimits(2,1,32);
                AllMuons->SetParameter(3,-1);
                AllMuons->SetParLimits(3,-10,-0.01);
                AllMuons->SetParameter(4,24);
                AllMuons->SetParLimits(4,1,32);
                AllMuons->SetParLimits(5,1,50000);
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
                ForwardMuons->SetParLimits(0,1,100000);
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
                BackwardMuons->SetParLimits(0,1,50000);
                BackwardMuons->SetParameter(1,27);
                BackwardMuons->SetParLimits(1,1,32);
                BackwardMuons->SetParameter(2,2);
                BackwardMuons->SetParLimits(2,1,32);
                BackwardMuons->SetParameter(3,1);
                BackwardMuons->SetParLimits(3,0.01,10);
                BackwardMuons->SetParameter(4,24);
                BackwardMuons->SetParLimits(4,1,32);
                HitProfilB->Fit(BackwardMuons,"QR");

                output << ts << '\t' << RPCthreshold << '\t';

                for(int a=0; a<10; a++){
                    output << AllMuons->GetParameter(a) << '\t'
                    << AllMuons->GetParError(a) << '\t';
                }

                output << HitProfilF->GetEntries() << '\t'
                << ForwardMuons->Integral(1.,32.) << '\t'
                << HitProfilF->Integral(1,16) << '\t'
                << ForwardMuons->Integral(1.,16.) << '\t'
                << HitProfilF->GetMaximum() << '\t'
                << HitProfilF->GetMaximumBin() << '\t'
                << ForwardMuons->GetMaximum() << '\t'
                << ForwardMuons->GetMaximumX() << '\t';

                for(int f=0; f<5; f++){
                    output << AllMuons->GetParameter(f) << '\t'
                    << AllMuons->GetParError(f) << '\t';
                }

                output << HitProfilB->GetEntries() << '\t'
                << BackwardMuons->Integral(1.,32.) << '\t'
                << HitProfilB->GetMaximum() << '\t'
                << HitProfilB->GetMaximumBin() << '\t'
                << BackwardMuons->GetMaximum() << '\t'
                << BackwardMuons->GetMaximumX() << '\t';

                for(int b=0; b<5; b++){
                    output << AllMuons->GetParameter(b) << '\t'
                    << AllMuons->GetParError(b) << '\t';
                }

                output << endl;

                HitProfil->Write();
                RPCProfil->Write();
                DistRPC->Write();
                MuonDistrib->Write();
                MuonDir->Write();
                MuonOrigin->Write();
                DistScint->Write();

                HitProfilF->Write();
                RPCProfilF->Write();
                DistRPCF->Write();
                MuonDistribF->Write();
                MuonDirF->Write();
                MuonOriginF->Write();
                DistScintF->Write();

                HitProfilB->Write();
                RPCProfilB->Write();
                DistRPCB->Write();
                MuonDistribB->Write();
                MuonDirB->Write();
                MuonOriginB->Write();
                DistScintB->Write();
            }
        }

        ProfilEvo->Write();
        ProfilEvoF->Write();
        ProfilEvoB->Write();

        outputFile->Close();

    }
    return 0;
}
