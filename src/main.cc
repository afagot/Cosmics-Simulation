//******************************************************
//*      GIF Cosmic Muons Monte Carlo Simulation       *
//*      RPC School Mexico 2018                        *
//*                                                    *
//*      Alexis FAGOT                                  *
//*      alexis.fagot@ugent.be                         *
//*                                                    *
//*      February 15, 2018                             *
//*                                                    *
//*      MAIN FILE                                     *
//******************************************************


#include "../include/fonctions.h"

#include <fstream>
#include <random>

using namespace std;

int main(){
    //Output CSV file
    ofstream output("Results.csv",ios::out);

    /** PROGRAM CODE **/
    // Start with random seed
    random_device rd;
    Generator generator(rd());

    // Get the thing started
    generator.discard(100);

    //Definition of the scintillators
    Scintillator botScint, topScint;

    //Definition of the RPC
    RPC Rpc;
    setRPC(Rpc);

    //Definitions of the coordinates and of the cosmic direction
    Point muonPos, RPCPos;
    Direction direction;

    //Set thresholds
    float Scint_threshold = 0.;
    float RPC_threshold = GASTHICKNESS;

    //Data storage table.
    unsigned int StripData[NPARTITIONS][NSTRIPS] = {{0}};

    //Set the number of muons to generate during the simulation
    unsigned int Nmuons = 1e7;

    //Set number to follow the status of the simulation through
    //a progression bar
    float Stat = (float)Nmuons;
    float progress = 0.;

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

            bool too_short = (topDist <= Scint_threshold) || (botDist <= Scint_threshold) || (rpcDist <= RPC_threshold);

            if(!too_short){
                if(isInTrapezoid(RPCPos, Rpc.corners)){
                    for(unsigned int p = 0; p < NPARTITIONS; p++){
                        if(isInTrapezoid(RPCPos,Rpc.part[p].corners)){
                            for(unsigned int s = 0; s < NSTRIPS; s++){
                                if(isInTrapezoid(RPCPos,Rpc.part[p].strips[s])){
                                    StripData[p][s]++;
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

        cout << "Progress [";
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

    //Print number of hits per strip into output file
    output << "Strip_A\tnHits_A\tStrip_B\tHits_B\tStrip_C\tnHits_C\n";
    for(unsigned int s = 0; s < NSTRIPS; s++){
        output << s+1 << '\t' << StripData[0][s] << '\t'
               << s+1 << '\t' << StripData[1][s] << '\t'
               << s+1 << '\t' << StripData[2][s] << endl;
    }

    output.close();

    return 0;
}
