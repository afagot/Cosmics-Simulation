# RPC School Mexico 2018
## GIF Cosmic Muon Monte Carlo Simulation
#### supervised by Alexis Fagot
#### email : alexis.fagot@ugent.be

## 1. Goal of the exercise

The goal of this Monte Carlo simulation is to understand the cosmic muon distribution observed into a Resistive Plate Chamber (RPC) installed at the Gamma Irradiation Facility (GIF) at CERN.

An RPC is placed vertically in front of a gamma source for its performance to be tested under irradiation. To test the detector, cosmic muons detected through a telescope composed of 2 small scintillators, inclined with respect to the RPC plane, are used. In this case, the RPC is a model that is meant to be mounted on the CMS endcap and thus has a trapezoidal shape. It is divided into 3 partitions containing each 32 trapezoidal copper strips as readout.

The goal is to store the strip in which they pass and then the final distribution.

The setup is as follows :

![ScreenShot](https://raw.github.com/afagot/Cosmics-Simulation/RPC-School-Mexico-2018/img/Setup.jpg)

and the corresponding cosmic muon distribution measured was :

![ScreenShot](https://raw.github.com/afagot/Cosmics-Simulation/RPC-School-Mexico-2018/img/Distribution.jpg)

On this hit distribution histograms, a structure with 2 peaks can be identified. In order to confirm that the distribution is indeed due to muons, it is necessary to control the geometrical acceptance of the setup to cosmic muons. The goal is thus to complete this template of geometrical acceptance simulation and to check whether the distribution measured at GIF was right.

## 2. Installation

### 2.1. Check for git to be installed on the computer

To check if git is installed on your machine, you can try :

    which git

If this command returns nothing, this means you will need to install the package.

- **On Debian based distributions**
```
sudo apt-get install git
```
- **On RedHat based distributions**
```
sudo yum install git
```
- **On MacOS** (see [http://happygitwithr.com/install-git.html#mac-os](http://happygitwithr.com/install-git.html#mac-os))


### 2.2. Download from Github

Now that git is installed on your machine, to download this C++ simulation, open a terminal and type :

    git clone -b RPC-School-Mexico-2018 --single-branch https://github.com/afagot/Cosmics-Simulation

### 2.3. Structure of the simulation

- img *(images used to illustrate this README.md file)*
  - Distribution.jpg
  - Setup.jpg
- include
  - fonctions.h
- src
  - main.cc
- makefile
- README.md

## 3. Code details

### 3.1 Representation of the setup in the code

The GIF setup is represented in the code as shown on the following figures. The RPC is represented in yellow, the scintillator telescope in blue and the cosmic generation plane in green.

![ScreenShot](https://raw.github.com/afagot/Cosmics-Simulation/RPC-School-Mexico-2018/img/3Dview1.png)
![ScreenShot](https://raw.github.com/afagot/Cosmics-Simulation/RPC-School-Mexico-2018/img/3Dview2.png)

To achieve this results are are some keys of understanding of the code.

### 3.2. Setup dimensions

First of all, the dimensions of the RPC and scintillators are defined as constants. Note that the angle `alpha` corresponds to the angle of the of the base of the RPC trapezoid with the floor. The angle `beta` is the inclination angle of the scintillator telescope.

**fonctions.h**
```c++
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
```
![ScreenShot](https://raw.github.com/afagot/Cosmics-Simulation/RPC-School-Mexico-2018/img/RPC-dimensions.png)

**fonctions.h**
```c++
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
```
![ScreenShot](https://raw.github.com/afagot/Cosmics-Simulation/RPC-School-Mexico-2018/img/Trigger-dimensions.png)

Finally, the function `getRandomMuonPosition` allows to generate a muon in a predefined (x,y) plane. The user decides on the height. In this example, the plane is defined right bellow the lowest edge of the trigger telescope.

**fonctions.h**
```c++
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
```

**main.cc**
```c++
muonPos = getRandomMuonPosition(generator,a_Z0-TRIGGER_WIDTH*cos(beta));
```

### 3.3. Cosmic muon generator

Muons are generated following a cos^2(theta) distribution where theta is the azimutal incidence angle of muons on Earth. this is done using the functions described bellow. By convention, `theta` is the zenithal angle and `phi` the azimuthal angle. These 2 angles associated to a random position in the generation plane are enough to compute the muon trajectory.

**fonctions.h**
```c++
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
```

**main.cc**
```c++
// Start with random seed
random_device rd;
Generator generator(rd());

// Get the thing started
generator.discard(100);

muonPos = getRandomMuonPosition(generator,a_Z0-TRIGGER_WIDTH*cos(beta));
direction = getRandomDirection(generator);
```

### 3.4. Checking for muon hits in the trigger telescope

Here, each scintillator is not defined as a physical volume. Instead, a scintillator is defined as 6 planes where muon hits can occur. Thus, the scintillator is defined as the coordinate of the intersection points of the muon track with these planes. Are used for this step the functions `getHitPositionBigFaces`, `getHitPositionMediumFaces`, `getHitPositionSmallFaces` and `setScint`.

After getting these coordinate, the simulation checks whether the coordinates are contained within the scintillator surface or not using the functions `isInBigFace`, `isInMediumFace`, `isInSmallFace`, and `isInBottomScintillator` and `isInTopScintillator`.

**fonctions.h**
```c++
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

//Set the hits of the muons into the scintillator planes
void setScint(Scintillator& scint, const Point& P, const Direction& D, double offset, double beta){
    scint.MuonHit[0] = getHitPositionBigFaces(P,D,offset,beta);
    scint.MuonHit[1] = getHitPositionBigFaces(P,D,offset+TRIGGER_THICK,beta);
    scint.MuonHit[2] = getHitPositionSmallFaces(P,D,0.,beta);
    scint.MuonHit[3] = getHitPositionSmallFaces(P,D,TRIGGER_LENGTH,beta);
    scint.MuonHit[4] = getHitPositionMediumFaces(P,D,0.,beta);
    scint.MuonHit[5] = getHitPositionMediumFaces(P,D,TRIGGER_WIDTH,beta);
}

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
```

**main.cc**
```c++
//Definition of the scintillators
Scintillator botScint, topScint;

setScint(botScint,muonPos,direction,0.,beta);
setScint(topScint,muonPos,direction,TRIGGER_OFFSET,beta);

bool in_trigger = isInBottomScintillator(botScint,beta) && isInTopScintillator(topScint,beta);
```

### 3.5. Checking for muon hits in the RPC

Once we made sure the muon passed through both scintillators, generating a trigger in the DAQ software of GIF, we need to check for the corresponding hit within the RPC plane. Using the available functions, it is possible to assign to each muon, a partition and a strip. What it means is that we are able to determine in which one of the 3 partitions the muon passed and, within this partition, to determine in which strip the hit occured.

The RPC, partitions and strips are defined as trapezoids using the 4 corners. An RPC possesses 3 partitions and a partition possesses 32 strips. Setting an RPC is done via `setRPC` that calls `setPartitions` that, itself, call `setStrips`.

The hit position within the RPC plane can be obtained through `getHitPositionRPC`.

Finally, to check if the hit is in the RPC itself, a specific partition, or a specific strip, you can call `isInTrapezoid`.

**fonctions.h**
```c++
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

//Compute the hit position in the RPC plan knowing that equation of the plane is y=0
Point getHitPositionRPC(const Point& P, const Direction& D){
    Point I;

    I.x = P.x - P.y*cos(D.second)/sin(D.second);
    I.y = 0.;
    I.z = P.z - P.y*cos(D.first)/sin(D.first)/sin(D.second);

    return I;
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
```

**main.cc**
```c++
//Definition of the RPC
RPC Rpc;
setRPC(Rpc);

if (in_trigger){
    RPCPos = getHitPositionRPC(muonPos,direction);

    if(isInTrapezoid(RPCPos, Rpc.corners))
        for(unsigned int p = 0; p < NPARTITIONS; p++)
            if(isInTrapezoid(RPCPos,Rpc.part[p].corners))
                for(unsigned int s = 0; s < NSTRIPS; s++)
                    if(isInTrapezoid(RPCPos,Rpc.part[p].strips[s]))
                        StripData[p][s]++;
}
```

## 4. Guide line for the exercise

### 4.1. Assembling the puzzle and extracting the data

The file **main.cc** will be missing most of its original code. You will need to reconstitute the puzzle thanks to the indications given above in Section 3.. All the pieces are there and you will need to understand the algorithm used to make the simulation.

When you find the right way to order the different pieces provided above, it is suggested for you to write the results into a file. The file object already exist as `output` that will open and write into a file called `Results.dat`. It is advised to write the data using 3 columns:

- Column 1 : partition ID
- Column 2 : strip number within the partition
- Column 3 : number of hits recorded in the strip

As an example, here are results generated using wisely `object` and the table `StripData`, which already exist as well:

**Results.dat**
```

```

### 4.2. Plotting the data

## 5. Running the simulation

Once your modifications are over and you want to test, compile the gif geometrical simulation via :

    make

and, to launch the executable file to start the simulation for this given setup, use:

    bin/GIF-Geo
