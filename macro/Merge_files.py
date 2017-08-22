import ROOT
from ROOT import TChain, TFile, TTree

import os
import glob


def MergeTuples( path, oldTreeName, newTreeName ):
    myChain = TChain( oldTreeName )

    for fname in glob.glob(path):
        print(fname)
        myChain.Add(fname)

    myTree = myChain.CloneTree(-1, "fast")
    myTree.SetName( newTreeName )
    return myTree


def CreateRawTuple():
    pathToRaw = "/sps/lhcb/mazzanti/tuples_EE_00/raw_tuples/Raw.root"
    pathToSuperRaw = "/sps/lhcb/bifani/RD/Analysis/RKst/jobs/jobs/apply_EE_00/"
    myFile = TFile(pathToRaw, "RECREATE")

    Bd2KstGEE = MergeTuples( pathToSuperRaw+"Bd2KstG_EE/*/*/*.root","MCDecayTuple", "Bd2KstGEE" )
    Bd2KstEE = MergeTuples( pathToSuperRaw+"Bd2KstEE/*/*/*.root","MCDecayTuple", "Bd2KstEE" )
    Bd2KstJPsEE = MergeTuples( pathToSuperRaw+"Bd2KstJPs_EE/*/*/*.root","MCDecayTuple", "Bd2KstJPsEE")
    LPT = MergeTuples( pathToSuperRaw+"LPT/*/*/*.root","DecayTuple","data")
    myFile.Write()
