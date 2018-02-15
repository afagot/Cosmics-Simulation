//******************************************************
//*      GIF Cosmic Muons Monte Carlo Simulation       *
//*                                                    *
//*      Alexis FAGOT                                  *
//*      alexis.fagot@ugent.be                         *
//*                                                    *
//*      October 18, 2016                              *
//*                                                    *
//*      HEADER FILE                                   *
//******************************************************


#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

using namespace std;

//pi Definition
const double PI = 4*atan(1);

//********************************************************************************************

//Function to cast an integer into a string
string intTostring(int value){
    string word;
    stringstream ss;
    ss << setw(2) << setfill('0') << value;
    ss >> word;

    return word;
}

//Function to cast a float into a string
string floatTostring(float value){
    string word;
    stringstream ss;
    ss.setf(ios::fixed, ios::floatfield);
    ss.precision(3);
    ss << value;
    ss >> word;

    return word;
}

//Function to convert deg angles to rad
float DegToRad(float deg){
    return deg*PI/180.;
}

//Function to convert rad angles to deg
float RadToDeg(float rad){
    return rad*180./PI;
}

//Function that returns the partition label
char getPartition(unsigned int part){
    if(part == 0) return 'A';
    if(part == 1) return 'B';
    if(part == 2) return 'C';
    else return 'X';
}

//********************************************************************************************

//CMS RPC constants
#define NPARTITIONS 3
#define NSTRIPS 32
#define GASTHICKNESS 2

//Dimensions of the RE2-2
#define A_WIDTH  868.
#define AB_WIDTH 758.8
#define BC_WIDTH 662.4
#define C_WIDTH  578.

#define A_LENGTH 620.8
#define B_LENGTH 548.
#define C_LENGTH 480.

#define AB_LENGTH   A_LENGTH+B_LENGTH
#define BC_LENGTH   B_LENGTH+C_LENGTH
#define ABC_LENGTH  A_LENGTH+B_LENGTH+C_LENGTH

//Reference point a of the RPC

#define A_X0 0.
#define A_Y0 0.
#define A_Z0 0.

//Angle of the RPC du to trapezoidal shape (in rad)
const double alpha = atan(2*A_LENGTH/(A_WIDTH-AB_WIDTH));

//Dimensions of the scintillator planes
#define TRIGGER_LENGTH 320.
#define TRIGGER_WIDTH 94.
#define TRIGGER_THICK 45.
#define TRIGGER_OFFSET 85.

//Reference point a of the scintillator
#define a_X0 770.
#define a_Y0 -40.
#define a_Z0 558.

//Inclination angle of the trigger (10deg in rad)
const double beta = DegToRad(10.);

//********************************************************************************************

//Definition of Coordinates
struct Point{
    double x;
    double y;
    double z;
};
typedef Point Vector;

//A (theta,phi) angle pair.
typedef std::pair<double,double> Direction;

//A structure that defines a scintillator as a rectangle parallelepiped having
//6 surfaces that can possibly be hit by cosmic muons.
//The convention used is :
// - 0 = large surface watching the RPC
// - 1 = large surface watching the sky
// - 2 = small side surface on the left (wide RPC base side)
// - 3 = small side surface on the right (narrow RPC base side
// - 4 = medium surface watching the ground
// - 5 = medium surface watching the sky
struct Scintillator{
    Point MuonHit[6];
};

//A structure that defines a trapezoid in the (x,0,z) plane using 4 points
//This structure is then used into the RPCPartition structure itself used
//in RPC
struct Trapezoid{
    Point corner[4];
};
typedef Trapezoid RPCStrip;

struct RPCPartition{
    RPCStrip strips[NSTRIPS];
    Trapezoid corners;
};

struct RPC{
    RPCPartition part[NPARTITIONS];
    Trapezoid corners;
};

//********************************************************************************************

//Definition of usefull points
const Point Orpc = {A_X0,A_Y0,A_Z0};
const Point Osct = {a_X0,a_Y0,a_Z0};

//Definition of usefull vectors
const Vector null  = {0.,0.,0.};
const Vector U_rpc = {cos(alpha),0.,sin(alpha)};    //translation along the Partition width
const Vector V_rpc = {1/sin(alpha),0.,0.};          //translation along the lower RPC edge

//********************************************************************************************

//function that builds a vector from 2 points
Vector makeVector(Point A, Point B){
    Vector AB;
    AB.x = B.x - A.x;
    AB.y = B.y - A.y;
    AB.z = B.z - A.z;

    return AB;
}

//********************************************************************************************

//Shorthand notation to make the code more understadable and to allow for
//drop-in replacement by another C++11 random number generator.
typedef std::mt19937 Generator;

//A fonction that generate a random position within a certain rectangle
double getRandom(Generator& generator){
    return generate_canonical<double, 32>(generator);
}

double getRandomInRange(Generator& generator, double x0, double length){
    return length*getRandom(generator) + x0;
}

//********************************************************************************************

//A fonction that generates a cos^2 cosmics distribution
Direction getRandomDirection(Generator& generator){
    double phi = 2*PI*getRandom(generator);

    // Create cos²(theta) distribution
    double theta = PI*getRandom(generator)/2.;
    double test = getRandom(generator);

    while (test > cos(theta)*cos(theta)){
        theta = PI*getRandom(generator)/2.;
        test = getRandom(generator);
    }

    return make_pair(theta, phi);
}

//Generate a random hit in the cosmic plane that is defined to be situated
//right at the height of the lowest point of the scintillators via the
//variable height
Point getRandomMuonPosition(Generator& generator, double height){
    Point muon;

    muon.x = getRandomInRange(generator, -1000., 3000.);
    muon.y = getRandomInRange(generator, -2000., 2500.);
    muon.z = height;

    return muon;
}

//Compute the hit position in the scintillator planes knowing the equation of the planes
//and using the origin of the muon, its direction
Point getHitPositionBigFaces(const Point& P, const Direction& D, double offset, double beta){
    Point I;

    double t = (cos(beta)*(a_Y0-P.y)+sin(beta)*(P.z-a_Z0)-offset)
    /(sin(D.first)*sin(D.second)*cos(beta)-cos(D.first)*sin(beta));

    I.x = P.x + sin(D.first)*cos(D.second)*t;
    I.y = P.y + sin(D.first)*sin(D.second)*t;
    I.z = P.z + cos(D.first)*t;

    return I;
}

Point getHitPositionMediumFaces(const Point& P, const Direction& D, double offset, double beta){
    Point I;

    double t = (sin(beta)*(a_Y0-P.y)+cos(beta)*(a_Z0-P.z)-offset)
    /(cos(beta)*cos(D.first)+sin(beta)*sin(D.first)*sin(D.second));

    I.x = P.x + sin(D.first)*cos(D.second)*t;
    I.y = P.y + sin(D.first)*sin(D.second)*t;
    I.z = P.z + cos(D.first)*t;

    return I;
}

Point getHitPositionSmallFaces(const Point& P, const Direction& D, double offset, double beta){
    Point I;

    double t = (a_X0-P.x+offset)/(sin(D.first)*cos(D.second));

    I.x = P.x + sin(D.first)*cos(D.second)*t;
    I.y = P.y + sin(D.first)*sin(D.second)*t;
    I.z = P.z + cos(D.first)*t;

    return I;
}

//Compute the hit position in the RPC plan knowing that equation of the plane is y=0
Point getHitPositionRPC(const Point& P, const Direction& D){
    Point I;

    I.x = P.x - P.y*cos(D.second)/sin(D.second);
    I.y = 0.;
    I.z = P.z - P.y*cos(D.first)/sin(D.first)/sin(D.second);

    return I;
}

//********************************************************************************************

//Set the hits of the muons into the scintillator planes
void setScint(Scintillator& scint, const Point& P, const Direction& D, double offset, double beta){
    scint.MuonHit[0] = getHitPositionBigFaces(P,D,offset,beta);
    scint.MuonHit[1] = getHitPositionBigFaces(P,D,offset+TRIGGER_THICK,beta);
    scint.MuonHit[2] = getHitPositionSmallFaces(P,D,0.,beta);
    scint.MuonHit[3] = getHitPositionSmallFaces(P,D,TRIGGER_LENGTH,beta);
    scint.MuonHit[4] = getHitPositionMediumFaces(P,D,0.,beta);
    scint.MuonHit[5] = getHitPositionMediumFaces(P,D,TRIGGER_WIDTH,beta);
}

//Used to easily set the corners of the trapezoid strips using a vector and
//a factor that is proportionnal to the strip position or simply of the corners
//of the Partitions or of the RPC itself
void setPoint(Point& M, Point P, const Vector vect, double factor){
    M.x = P.x + factor*vect.x;
    M.y = P.y + factor*vect.y;
    M.z = P.z + factor*vect.z;
}

//Set the strip corners in a given partition
void setStrips(RPCStrip& strip, Trapezoid part, Vector V[2], int factor){
    setPoint(strip.corner[0],part.corner[0],V[0],factor*1./32.);
    setPoint(strip.corner[1],part.corner[0],V[0],(factor+1)*1./32.);
    setPoint(strip.corner[2],part.corner[3],V[1],(factor+1)*1./32.);
    setPoint(strip.corner[3],part.corner[3],V[1],factor*1./32.);
}

//Set the corners of the 3 partitions and call the function that sets the
//strips in every partition
void setPartitions(RPCPartition (&part)[3], Trapezoid rpc){
    setPoint(part[0].corners.corner[0],rpc.corner[0],null,0.);
    setPoint(part[0].corners.corner[1],rpc.corner[1],null,0.);
    setPoint(part[0].corners.corner[3],part[0].corners.corner[0],V_rpc,A_LENGTH);
    setPoint(part[0].corners.corner[2],part[0].corners.corner[3],U_rpc,AB_WIDTH);

    setPoint(part[1].corners.corner[0],part[0].corners.corner[3],null,0.);
    setPoint(part[1].corners.corner[1],part[0].corners.corner[2],null,0.);
    setPoint(part[1].corners.corner[3],part[1].corners.corner[0],V_rpc,B_LENGTH);
    setPoint(part[1].corners.corner[2],part[1].corners.corner[3],U_rpc,BC_WIDTH);

    setPoint(part[2].corners.corner[0],part[1].corners.corner[3],null,0.);
    setPoint(part[2].corners.corner[1],part[1].corners.corner[2],null,0.);
    setPoint(part[2].corners.corner[2],rpc.corner[2],null,0.);
    setPoint(part[2].corners.corner[3],rpc.corner[3],null,0.);

    Vector V_strip[3][2];

    for(int p = 0; p < 3; p++){
        V_strip[p][0] = makeVector(part[p].corners.corner[0],part[p].corners.corner[1]);
        V_strip[p][1] = makeVector(part[p].corners.corner[3],part[p].corners.corner[2]);

        for(int s=0; s<32; s++)
            setStrips(part[p].strips[s],part[p].corners,V_strip[p],s);
    }
}

//Set the RPC corners and call the function to set the partitions and their strips
void setRPC(RPC& rpc){
    setPoint(rpc.corners.corner[0],Orpc,null,0.);
    setPoint(rpc.corners.corner[1],Orpc,U_rpc,A_WIDTH);
    setPoint(rpc.corners.corner[3],Orpc,V_rpc,ABC_LENGTH);
    setPoint(rpc.corners.corner[2],rpc.corners.corner[3],U_rpc,C_WIDTH);

    setPartitions(rpc.part, rpc.corners);
}

//********************************************************************************************

//Check if the muon is in the scintillator surfaces. For the big and medium faces,
//it only is needed to express 2 conditions (on x and/or y and/or z) to restrict the
//possible area to the surface of the face cause the faces are not rotated.
//for the little side faces, it is needed to exclude using the line equations.
bool isInBigFace(const Point& I, double offset, double beta){
    bool x_condition = (I.x >= a_X0 && I.x <= (a_X0+TRIGGER_LENGTH));

    double z_high = a_Z0 + offset*sin(beta);
    double z_low = z_high - TRIGGER_WIDTH*cos(beta);

    bool z_condition = ( I.z >= z_low && I.z <= z_high );
    return (x_condition && z_condition);
}

bool isInMediumFace(const Point& I, double Yoffset, double Zoffset, double beta){
    bool x_condition = (I.x >= a_X0 && I.x <= (a_X0+TRIGGER_LENGTH));

    double z_low = a_Z0 - Zoffset*cos(beta) + Yoffset*sin(beta);
    double z_high = z_low + TRIGGER_THICK*sin(beta);

    bool z_condition = ( I.z >= z_low && I.z <= z_high );
    return (x_condition && z_condition);
}

bool isInSmallFace(const Point& I, double offset, double beta){
    bool is_below = (I.z <= (a_Z0 + (a_Y0-I.y)*sin(beta)/cos(beta)));
    bool is_above = (I.z >= (a_Z0 + (a_Y0-I.y)*sin(beta)/cos(beta)) - TRIGGER_WIDTH/cos(beta));
    bool is_on_right_of = (I.z >= (a_Z0 + (I.y-a_Y0)*cos(beta)/sin(beta) + offset/sin(beta)));
    bool is_on_left_of = (I.z <= (a_Z0 + (I.y-a_Y0)*cos(beta)/sin(beta) + (offset+TRIGGER_THICK)/sin(beta)));

    return (is_below && is_above && is_on_left_of && is_on_right_of);
}

//Due to the geometry of the setup, the only way for the muons to pass through the
//telescope is to pass through the inner surfaces, i.e. the 2 surface in between
//the 2 scintillators, the ones that are "looking to each other".
bool isInBottomScintillator(const Scintillator& scint, double beta){
    return isInBigFace(scint.MuonHit[1],TRIGGER_THICK,beta);
}

bool isInTopScintillator(const Scintillator& scint, double beta){
    return isInBigFace(scint.MuonHit[0],TRIGGER_OFFSET,beta);
}

//Function to test if the muon is in a given trapezoid (RPC, Partition or Strip)
bool isInTrapezoid(const Point& I, const Trapezoid& trap){
    Point A = trap.corner[0];
    Point B = trap.corner[1];
    Point C = trap.corner[2];
    Point D = trap.corner[3];

    return      (I.z <= (I.x-A.x)*(B.z-A.z)/(B.x-A.x)+A.z)
    &&  (I.z <= (I.x-B.x)*(C.z-B.z)/(C.x-B.x)+B.z)
    &&  (I.z >= (I.x-C.x)*(D.z-C.z)/(D.x-C.x)+C.z)
    &&  (I.z >= (I.x-D.x)*(A.z-D.z)/(A.x-D.x)+D.z);
}

//********************************************************************************************

//Get the distance traveled through the scintillators. This will be used to discriminate
//the muons using the threshold. Only muons travelling a distance greater than a given
//"threshold" through the scintillators are considered as detected by the setup.
double getDistanceThroughBottomScint(const Scintillator& scint, double beta){
    double d = 0.;
    unsigned int i = 0;

    if(isInBigFace(scint.MuonHit[0],0.,beta))
        i = 0;
    else if(isInSmallFace(scint.MuonHit[2],0.,beta))
        i = 2;
    else if(isInSmallFace(scint.MuonHit[3],0.,beta))
        i = 3;
    else if(isInMediumFace(scint.MuonHit[4],0.,0.,beta))
        i = 4;
    else if(isInMediumFace(scint.MuonHit[5],0.,TRIGGER_WIDTH,beta))
        i = 5;

    d = sqrt((scint.MuonHit[i].x-scint.MuonHit[1].x)*(scint.MuonHit[i].x-scint.MuonHit[1].x)
    + (scint.MuonHit[i].y-scint.MuonHit[1].y)*(scint.MuonHit[i].y-scint.MuonHit[1].y)
    + (scint.MuonHit[i].z-scint.MuonHit[1].z)*(scint.MuonHit[i].z-scint.MuonHit[1].z));

    return d;
}

double getDistanceThroughTopScint(const Scintillator& scint, double beta){
    double d = 0.;
    unsigned int i = 0;

    if(isInBigFace(scint.MuonHit[1],TRIGGER_OFFSET+TRIGGER_THICK,beta))
        i = 1;
    else if(isInSmallFace(scint.MuonHit[2],TRIGGER_OFFSET,beta))
        i = 2;
    else if(isInSmallFace(scint.MuonHit[3],TRIGGER_OFFSET,beta))
        i = 3;
    else if(isInMediumFace(scint.MuonHit[4],TRIGGER_OFFSET,0.,beta))
        i = 4;
    else if(isInMediumFace(scint.MuonHit[5],TRIGGER_OFFSET,TRIGGER_WIDTH,beta))
        i = 5;

    d = sqrt((scint.MuonHit[i].x-scint.MuonHit[0].x)*(scint.MuonHit[i].x-scint.MuonHit[0].x)
    + (scint.MuonHit[i].y-scint.MuonHit[0].y)*(scint.MuonHit[i].y-scint.MuonHit[0].y)
    + (scint.MuonHit[i].z-scint.MuonHit[0].z)*(scint.MuonHit[i].z-scint.MuonHit[0].z));

    return d;
}

//To study the effect of the distance the muon travels through the RPC gas
//volume. The gas volume is 2mm thick along the y axis. The muon direction
//vector is given by the direction (theta,phi) and is :
// v = (v.x;v.y;v.z) = ( sin(theta)cos(phi); sin(theta)sin(phi); cos(theta) )
//The relation that needs to be satisfied is : d*v.y = 2 <=> d = 2/v.y
double getDistanceThroughRPC(Direction direction){
    double d = abs(2./(sin(direction.first)*sin(direction.second)));

    return d;
}

#endif // FONCTIONS_H
