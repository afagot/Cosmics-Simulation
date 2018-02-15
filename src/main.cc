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
    ofstream output("Results.dat",ios::out);

    //Data storage table.
    unsigned int StripData[NPARTITIONS][NSTRIPS] = {{0}};

    //Set the number of muons to generate during the simulation
    unsigned int Nmuons = 1e7;

    //Set number to follow the status of the simulation through
    //a progression bar
    float Stat = (float)Nmuons;
    float progress = 0.;

    /** PROGRAM CODE **/

    //here write the code to declare the useful objects you will need

    //...

    for(unsigned int n=0; n<Nmuons; n++){

        //here write the needed calculation.
        //for each muon, generate a point and directions,
        //get the position in the trigger scintillators
        //check if the hits are in the trigger
        //check the coordinates in the RPC
        //increment the right StripData cell by checking
        //in which partition and which strip the hit is

        //...

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

    //...

    output.close();

    return 0;
}
