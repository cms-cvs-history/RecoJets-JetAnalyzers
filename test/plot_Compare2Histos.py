#!/usr/bin/env python

###################
# plot_Compare2Histos.py
# description: compares two histograms from one root file
# instructions:
#  python -i plot_Compare2Histos.py
#  python -i plot_Compare2Histos.py --inputFile1=jetSubstructurePlotsExample_QCDMC.root
###################


from ROOT import *

from optparse import OptionParser


parser = OptionParser()

parser.add_option('--inputFile1', metavar='F', type='string', action='store',
                  default='jetSubstructurePlotsExample_RStoWW.root',
                  dest='inputFile1',
                  help='Input file 1')


parser.add_option('--dir', metavar='D', type='string', action='store',
                  default='pf/',
                  dest='dir',
                  help='directory to find plots')

parser.add_option('--outname', metavar='O', type='string', action='store',
                  default='substructureComparison2hist.root',
                  dest='outname',
                  help='Output name')

(options, args) = parser.parse_args()

argv = []


gROOT.Reset()
#gStyle.SetTitleOffset(1.5,"Y")
gStyle.SetTitleSize(0.05,"XY")


canvs = []
outhists = []
stacks = []
leg = []

f1 = TFile(options.inputFile1)

###########################
# PLOTTING FUNCTION
#   plot("histogram1","histogram2","Title;XaxisLabel;YaxisLabel","histo1legendEntry","histo2legendEntry","OutputFileName.png")

def plot(hist1,hist2,title,label1,label2,savename) :


  gROOT.Reset()
  gStyle.SetPadRightMargin(0.2);



  h1 = f1.Get(options.dir + hist1)
  h2 = f1.Get(options.dir + hist2)

  h1clone = h1.Clone("h1")
  h2clone = h2.Clone("h2")

  hs = THStack( h1.GetName()+h2.GetName(), h1.GetTitle() )

  h2clone.Scale(1.0 / h2clone.Integral() * h1clone.Integral() )
  
  h1clone.SetLineColor(1)
  h2clone.SetLineColor(6)
  h1clone.SetLineWidth(2)
  h2clone.SetLineWidth(2)

  hs.SetTitle(title)
  hs.Add( h1clone, 'hist' )
  hs.Add( h2clone, 'hist' )

  c = TCanvas('c_' + hist1 + '_' + hist2, 'c_' + hist1 + '_' + hist2,  200, 10, 750, 500 )
  hs.Draw('nostack')

  legend=TLegend(0.83,0.4,0.98,0.8)
  legend.SetFillColor(0);
  legend.AddEntry(h1clone,label1, "l")
  legend.AddEntry(h2clone,label2, "l")    
  legend.Draw()
  
  outhists.append( [h1clone,h2clone] )
  stacks.append( hs )
  leg.append( legend )
  canvs.append(c)

  c.SaveAs(savename)
  return

############################################
## EXERCISE 1A

# compare jet mass
plot( "hAK5JetMass","hCA8JetMass","Jet Mass - Compare Algorithms;Jet Mass (GeV/c^{2});Number of jets","AK5","CA8","plots/Plot_Mass_CompareJetAlgos.png")
plot( "hAK5JetArea","hCA8JetArea","Jet Area - Compare Algorithms;Jet Area;Number of jets","AK5","CA8","plots/Plot_Area_CompareJetAlgos.png")
plot( "hAK5JetNconstituents","hCA8JetNconstituents","Number of Constituents- Compare Algorithms;Number of Constituents;Number of jets","AK5","CA8","plots/Plot_Nconst_CompareJetAlgos.png")
plot( "hAK5JetPt","hCA8JetPt","Jet Pt - Compare Algorithms;Jet pT (GeV/c);Number of jets","AK5","CA8","plots/Plot_pt_CompareJetAlgos.png")
plot( "hAK5JetRapidity","hCA8JetRapidity","Jet Rapidity- Compare Algorithms;Jetrapidity;Number of jets","AK5","CA8","plots/Plot_pt_CompareJetAlgos.png")


############################################
## EXERCISE 1B
# use script plot_mass_vs_pt.cpp

############################################
## EXERCISE 1C

# ###### compare jet mass for groomed and ungroomed jets
# plot("hCA8JetMass","hPrunedMass","Pruned mass;Jet Mass;Number of jets","Ungroomed","Pruned","plots/Plot_Mass_ComparePruned.png")
# plot("hCA8JetMass","hTrimmedMass","Trimmed mass;Jet Mass;Number of jets","Ungroomed","Trimmed","plots/Plot_Mass_CompareTrimmed.png")
# plot("hCA8JetMass","hFilteredMass","Filtered mass;Jet Mass;Number of jets","Ungroomed","Filtered","plots/Plot_Mass_CompareFiltered.png")
# plot("hTrimmedMass","hPrunedMass","Compare trimmed and pruned area;Jet Mass;Number of jets","Trimmed","Pruned","plots/Plot_Mass_CompareTrimmedPruned.png")

# ###### compare jet area for groomed and ungroomed jets
# plot("hCA8JetArea","hPrunedArea","Compare ungroomed and pruned jet area;Jet Area;Number of jets","Ungroomed","Pruned","plots/Plot_Area_ComparePruned.png")
# plot("hCA8JetArea","hTrimmedArea","Compare ungroomed and trimmed jet area;Jet Area;Number of jets","Ungroomed","Trimmed","plots/Plot_Area_CompareTrimmed.png")
# plot("hCA8JetArea","hFilteredArea","Compare ungroomed and filtered jet area;Jet Area;Number of jets","Ungroomed","Filtered","plots/Plot_Area_CompareFiltered.png")
# plot("hTrimmedArea","hPrunedArea","Compare trimmed and pruned area;Jet Area;Number of jets","Trimmed","Pruned","plots/Plot_Area_CompareTrimmedPruned.png")

############################################
## EXERCISE 1D
# use script plot_mass_vs_nvtx.cpp



