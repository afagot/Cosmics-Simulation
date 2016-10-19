#GIF Cosmic Muon Monte Carlo Simulation
####Author: Alexis Fagot
####email : alexis.fagot@ugent.be
####Tel.: +32 9 264 65 69

##Compilation

This application requires 1 non-optional dependencies:

* **[ROOT](https://root.cern.ch/downloading-root) to organize the collected data into a TFile**

To compile the gif daq project, simply do :

    make

## Usage

This Monte Carlo simulation has been done to understand the cosmic muon distribution observed into a Resistive Plate Chamber (RPC) installed at the Gamma Irradiation Facility (GIF) at CERN. The setup is as follows :

![ScreenShot](https://raw.github.com/afagot/Cosmics-Simulation/master/img/Setup.jpg)

An RPC is placed vertically in front of a gamma source for its performance to be tested under irradiation.
The particle used to test the detector are cosmic muons detected through a telescope composed of 2 small scintillators inclined with respect to the RPC plane.
The RPC is a gaseous detector used in CMS for muon detection. In this case, this RPC is a model that is meant to be mounted on the CMS endcap and thus has a trapezoidal shape.
It is divided into 3 partitions containing each 32 trapezoidal copper strips as readout.
Muons are generated following a cos^2(theta) distribution where theta is the azimutal incidence angle of muons on Earth.
The goal is to store the strip in which they pass and then the final distribution.

In this version of the code, the 2 free parameters that are left to vary are the threshold and the inclination angle of the telescope.
The threshold corresponds to the effect given by the thrshold of the discriminators used to digitize the scintillator signals.
Assuming that the output signal of a photomultiplier is proportionnal to the ammount of photons collected from the scintillators, and thus to the energy deposition of the muons in the scintillators, the threshold can be seen as a minimum distance to travel through the scintillators for the muons to be detected.
Muons travelling shorter distances through the scintillators than the specified threshold are simply "unseen" by the telescope and don't give a trigger signal to the data acquisition.
The muon track is then not extrapolated to the RPC plane.

As it is now you simply need to launch the executable file to start the simulation for this given setup:

    bin/GIF-Geo
