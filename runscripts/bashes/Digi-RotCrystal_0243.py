import os
import sys
from Gaudi.Configuration import *

############## GeomSvc #################
geometry_option = "/publicfs/cms/user/wanghan/CEPCSW/Detector/DetCRD/compact/Standalone/Standalone-EcalRotCrystal.xml"
geometry_path = geometry_option

if not os.path.exists(geometry_path):
    print("Can't find the compact geometry file: %s"%geometry_path)
    sys.exit(-1)

from Configurables import GeomSvc
geomsvc = GeomSvc("GeomSvc")
geomsvc.compact = geometry_path
#######################################

########### k4DataSvc ####################
from Configurables import k4DataSvc
import glob

folder_path = '/publicfs/cms/user/wanghan/DATA/CEPCSIM/50000_pi-_2-20GeV_gamma_1-10GeV_5-160mm'
Name_suffix = '_SCECal_gamma'
start_index = 243-1
end_index =  243
all_root_files = glob.glob(f"{folder_path}/*.root")  #get all root files
print(f"There are {len(all_root_files)} files in total.")
sorted_root_files = sorted(all_root_files, key=lambda x: int(x.split("_")[-1].split(".")[0]))   #sort files
batch_files = sorted_root_files[start_index:end_index]

if batch_files:
    podioevent = k4DataSvc("EventDataSvc")
    podioevent.inputs = batch_files

else:
    print("Files not found.")
#podioevent = k4DataSvc("EventDataSvc")
#podioevent.inputs = ["~/script/Data/SimData/5GeV_30mm_103ver/Sim_gam_10GeV_1709384366.root"]

########## Podio Input ###################
from Configurables import PodioInput
inp = PodioInput("InputReader")
#inp.collections = ["EcalBarrelCollection", "MCParticleG4"]
inp.collections = ["EcalBarrelCollection", "EcalBarrelContributionCollection", "MCParticle"]
##########################################

########## Digitalization ################
from Configurables import FanEcalDigiAlg
EcalDigi = FanEcalDigiAlg("FanEcalDigiAlg")
EcalDigi.ReadOutName = "EcalBarrelCollection"
EcalDigi.SimCaloHitCollection = "EcalBarrelCollection"
EcalDigi.CaloHitCollection = "ECALBarrel"
EcalDigi.CaloAssociationCollection = "ECALBarrelAssoCol"
EcalDigi.CaloMCPAssociationCollection = "ECALBarrelParticleAssoCol"
#EcalDigi.SkipEvt = 3
EcalDigi.Seed = 2079
#Digitalization parameters
EcalDigi.CalibrECAL = 1
EcalDigi.CrystalBarLength = 338 
EcalDigi.AttenuationLength = 7e10
EcalDigi.TimeResolution = 0.5        #unit: ns
EcalDigi.EnergyThreshold = 0.001   #0.1 MeV  0.0001
EcalDigi.ChargeThresholdFrac = 0.002
EcalDigi.Debug=0
EcalDigi.OutFileName = "/publicfs/cms/user/wanghan/DATA/CEPCDIGI/50000_pi-_2-20GeV_gamma_1-10GeV_5-160mm/DigiEcal_243"+Name_suffix+".root"
#########################################


########################################

from Configurables import ApplicationMgr
ApplicationMgr( 
    TopAlg=[inp, EcalDigi],
    EvtSel="NONE",
    EvtMax=100,
    ExtSvc=[podioevent, geomsvc],
    #OutputLevel=DEBUG
)

