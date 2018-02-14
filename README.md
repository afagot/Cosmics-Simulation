# RPC School Mexico 2018
## GIF Cosmic Muon Monte Carlo Simulation
#### supervised by Alexis Fagot
#### email : alexis.fagot@ugent.be

## Download from Github

To download this C++ simulation, open a terminal and type :

    git clone -b RPC-School-Mexico-2018 --single-branch https://github.com/afagot/Cosmics-Simulation

## Detail of the exercise

The goal of this Monte Carlo simulation is to understand the cosmic muon distribution observed into a Resistive Plate Chamber (RPC) installed at the Gamma Irradiation Facility (GIF) at CERN. An RPC is placed vertically in front of a gamma source for its performance to be tested under irradiation.
The particle used to test the detector are cosmic muons detected through a telescope composed of 2 small scintillators inclined with respect to the RPC plane.
The RPC is a gaseous detector used in CMS for muon detection. In this case, this RPC is a model that is meant to be mounted on the CMS endcap and thus has a trapezoidal shape.
It is divided into 3 partitions containing each 32 trapezoidal copper strips as readout.
Muons are generated following a cos^2(theta) distribution where theta is the azimutal incidence angle of muons on Earth.
The goal is to store the strip in which they pass and then the final distribution.

The setup is as follows :

![ScreenShot](https://raw.github.com/afagot/Cosmics-Simulation/RPC-School-Mexico-2018/img/Setup.jpg)

and the corresponding cosmic muon distribution measured was :

![ScreenShot](https://raw.github.com/afagot/Cosmics-Simulation/RPC-School-Mexico-2018/img/Distribution.jpg)

On this hit distribution histograms, a structure with 2 peaks can be identified. In order to confirm that the distribution is indeed due to muons, it is necessary to control the geometrical acceptance of the setup to cosmic muons.

## Compilation

To compile the gif geometrical simulation, simply do :

    make

## Running the simulation

As it is now you simply need to launch the executable file to start the simulation for this given setup:

    bin/GIF-Geo
