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

## 3. Guide lines

### 3.1 Representation of the setup in the code

The GIF setup is represented in the code as shown on the following figures. The RPC is represented in yellow, the scintillator telescope in blue and the cosmic generation plane in green.

![ScreenShot](https://raw.github.com/afagot/Cosmics-Simulation/RPC-School-Mexico-2018/img/3Dview1.png)
![ScreenShot](https://raw.github.com/afagot/Cosmics-Simulation/RPC-School-Mexico-2018/img/3Dview2.png)

To achieve this results are are some keys of understanding of the code.

#### 3.1.1. Setup dimensions

First of all, the dimensions of the RPC and scintillators are defined as constants. Note that the angle `alpha` corresponds to the angle of the of the base of the RPC trapezoid with the floor. The angle `beta` is the inclination angle of the scintillator telescope.

** fonctions.h**
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

** fonctions.h**
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

### Cosmic muon generator

Muons are generated following a cos^2(theta) distribution where theta is the azimutal incidence angle of muons on Earth.

### Running the simulation

Once your modifications are over and you want to test, compile the gif geometrical simulation via :

    make

and, to launch the executable file to start the simulation for this given setup, use:

    bin/GIF-Geo
