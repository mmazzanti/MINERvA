from ROOT import *
import sys

# Setup TChain to read in multiple TTrees at once
liketree = TChain("error_iterations")

# Add all input files
for i in xrange(1,len(sys.argv)):

    # Check is file
    filetest = TFile(sys.argv[i],"READ")
    if not filetest: continue
    else: filetest.Close()

    # Add to TChain
    liketree.Add(sys.argv[i])



# Draw TChain Distribution to histogram
parameter = "MaCCRES"
binning = "(20,-3.0,3.0)"
liketree.Draw( parameter + " >> prior" + binning, "1.0") 
liketree.Draw( parameter + " >> posterior" + binning, "exp(-1.0 * total_likelihood)")

# Setup Prior
prior.SetTitle("prior " + 
               str(round(prior.GetMean(),2)) + 
               " +- " + 
               str(round(prior.GetRMS(),2)) )

# Setup posterior style and normalise to match prior integral
posterior.SetTitle("posterior " + 
                   str(round(posterior.GetMean(),2)) + 
                   " +- " + str(round(posterior.GetRMS(),2)) )
posterior.GetYaxis().SetTitle("Arb Units")
posterior.GetXaxis().SetTitle("MaCCRES (#sigma)")
posterior.SetLineColor(kRed)
posterior.Scale( prior.Integral() / posterior.Integral() )

# Draw both
posterior.Draw()
prior.Draw("SAME HIST")

# Make a legend and show stats
gPad.BuildLegend()
gPad.Update()

raw_input("WAIT")
