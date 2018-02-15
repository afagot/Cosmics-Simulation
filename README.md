# RPC School Mexico 2018
## GIF Cosmic Muon Monte Carlo Simulation
#### supervised by Alexis Fagot
#### email : alexis.fagot@ugent.be

## Goal of the exercise

The goal of this Monte Carlo simulation is to understand the cosmic muon distribution observed into a Resistive Plate Chamber (RPC) installed at the Gamma Irradiation Facility (GIF) at CERN.

An RPC is placed vertically in front of a gamma source for its performance to be tested under irradiation. To test the detector, cosmic muons detected through a telescope composed of 2 small scintillators, inclined with respect to the RPC plane, are used. In this case, the RPC is a model that is meant to be mounted on the CMS endcap and thus has a trapezoidal shape. It is divided into 3 partitions containing each 32 trapezoidal copper strips as readout.

The goal is to store the strip in which they pass and then the final distribution.

The setup is as follows :

![ScreenShot](https://raw.github.com/afagot/Cosmics-Simulation/RPC-School-Mexico-2018/img/Setup.jpg)

and the corresponding cosmic muon distribution measured was :

![ScreenShot](https://raw.github.com/afagot/Cosmics-Simulation/RPC-School-Mexico-2018/img/Distribution.jpg)

On this hit distribution histograms, a structure with 2 peaks can be identified. In order to confirm that the distribution is indeed due to muons, it is necessary to control the geometrical acceptance of the setup to cosmic muons. The goal is thus to complete this template of geometrical acceptance simulation and to check whether the distribution measured at GIF was right.

## Guide lines

### Check for git to be installed on the computer

To check if git is installed on your machine, you can try :

    which git

If this command returns nothing, this means you will need to install the package.

- **On Debian based distributions**

    sudo apt-get install git

- **On RedHat based distributions**

    sudo yum install git

- **On MacOS** (see [http://happygitwithr.com/install-git.html#mac-os](http://happygitwithr.com/install-git.html#mac-os))


### Download from Github

Now that git is installed on your machine, to download this C++ simulation, open a terminal and type :

    git clone -b RPC-School-Mexico-2018 --single-branch https://github.com/afagot/Cosmics-Simulation

### Structure of the simulation

- img *(images used to illustrate this README.md file)*
  - \> Distribution.jpg
  - \> Setup.jpg
- include
  - fonctions.h
- src
  - \> main.cc
- makefile
- README.md

### Cosmic muon generator

Muons are generated following a cos^2(theta) distribution where theta is the azimutal incidence angle of muons on Earth.

### Running the simulation

Once your modifications are over and you want to test, compile the gif geometrical simulation via :

    make

and, to launch the executable file to start the simulation for this given setup, use:

    bin/GIF-Geo
