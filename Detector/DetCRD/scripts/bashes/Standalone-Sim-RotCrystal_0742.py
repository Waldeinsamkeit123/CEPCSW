#!/usr/bin/env python

import os
print(os.environ["DD4HEP_LIBRARY_PATH"])
import sys
import random

from Gaudi.Configuration import *

from Configurables import k4DataSvc #event data
dsvc = k4DataSvc("EventDataSvc")

from Configurables import RndmGenSvc, HepRndm__Engine_CLHEP__RanluxEngine_ #random number

#import argparse
#parser = argparse.ArgumentParser(description='args')
#parser.add_argument("--seed", "-s", type=int, help="random seed", default=3 )
#args = parser.parse_args()

#seed = [20]
# rndmengine = HepRndm__Engine_CLHEP__RanluxEngine_() # The default engine in Gaudi
rndmengine = HepRndm__Engine_CLHEP__HepJamesRandom_("RndmGenSvc.Engine") # The default engine in Geant4
rndmengine.SetSingleton = True
#rndmengine.Seeds = seed
rndmengine.Seeds = [768]
#rndmengine.Seeds = [20]

rndmgensvc = RndmGenSvc("RndmGenSvc")
rndmgensvc.Engine = rndmengine.name()

geometry_option = "Standalone/Standalone-EcalRotCrystal.xml"
#...

if not os.getenv("DETCRDROOT"):
    print("Can't find the geometry. Please setup envvar DETCRDROOT." )
    sys.exit(-1)

geometry_path = os.path.join(os.getenv("DETCRDROOT"), "compact", geometry_option)
if not os.path.exists(geometry_path):
    print("Can't find the compact geometry file: %s"%geometry_path)
    sys.exit(-1)

from Configurables import GeomSvc
geosvc = GeomSvc("GeomSvc")
geosvc.compact = geometry_path

##############################################################################
# Physics Generator
##############################################################################
from Configurables import GenAlgo
from Configurables import GtGunTool
from Configurables import StdHepRdr
from Configurables import SLCIORdr
from Configurables import HepMCRdr
from Configurables import GenPrinter
gun = GtGunTool("GtGunTool")
gun.Particles = ["pi-","gamma"]
#gun.Particles = ["nu_e"]
#gun.PositionXs = [0]
#gun.PositionYs = [0]
#gun.PositionZs = [0]
gun.EnergyMins = [2.,1.] # GeV
gun.EnergyMaxs = [20.,10.] # GeV
gun.ThetaMins  = [90,90]    # deg
gun.ThetaMaxs  = [90,90]  # deg
gun.PhiMins    = [10.151,10.151]    # deg pi- 10.09
gun.PhiMaxs    = [14.825,14.825]  # deg
# stdheprdr = StdHepRdr("StdHepRdr")
# stdheprdr.Input = "/cefs/data/stdhep/CEPC250/2fermions/E250.Pbhabha.e0.p0.whizard195/bhabha.e0.p0.00001.stdhep"
# lciordr = SLCIORdr("SLCIORdr")
# lciordr.Input = "/cefs/data/stdhep/lcio250/signal/Higgs/E250.Pbbh.whizard195/E250.Pbbh_X.e0.p0.whizard195/Pbbh_X.e0.p0.00001.slcio"
# hepmcrdr = HepMCRdr("HepMCRdr")
# hepmcrdr.Input = "example_UsingIterators.txt"

genprinter = GenPrinter("GenPrinter")

genalg = GenAlgo("GenAlgo")
genalg.GenTools = ["GtGunTool"]
#genalg.GenTools = ["StdHepRdr"]
# genalg.GenTools = ["StdHepRdr", "GenPrinter"]
# genalg.GenTools = ["SLCIORdr", "GenPrinter"]
# genalg.GenTools = ["HepMCRdr", "GenPrinter"]

##############################################################################
# Detector Simulation
##############################################################################
from Configurables import DetSimSvc
detsimsvc = DetSimSvc("DetSimSvc")

from Configurables import DetSimAlg
detsimalg = DetSimAlg("DetSimAlg")
detsimalg.RandomSeeds = [768]
#detsimalg.RandomSeeds = seed
# detsimalg.VisMacs = ["vis.mac"]
detsimalg.RunCmds = [
#    "/tracking/verbose 1",
]
detsimalg.AnaElems = [
    # example_anatool.name()
#    "ExampleAnaElemTool",
    "Edm4hepWriterAnaElemTool"
]
detsimalg.RootDetElem = "WorldDetElemTool"

# output
from Configurables import PodioOutput
out = PodioOutput("outputalg")
out.filename = "/publicfs/cms/user/wanghan/DATA/CEPCSIM/100000_pi-_2-20GeV_gamma_1-10GeV__5-160mm/SimSCECal_gpi_100000_pi-_2-20GeV_gamma_1-10GeV__5-160mm_768.root"
out.outputCommands = ["keep *"]

# ApplicationMgr
from Configurables import ApplicationMgr
ApplicationMgr(
    TopAlg = [genalg, detsimalg, out],
    EvtSel = 'NONE',
    EvtMax = 100,
    ExtSvc = [rndmengine, rndmgensvc, dsvc, geosvc],
    OutputLevel=INFO
)
