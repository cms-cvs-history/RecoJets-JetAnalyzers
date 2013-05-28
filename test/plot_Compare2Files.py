#!/usr/bin/env python

###################
# plot_Compare2Files.py
# description: compares histograms from two root files
# instructions:
# default input files:
#  python -i plot_Compare2Files.py
# choose input files:
#  python -i plot_Compare2Files.py --inputFile1=jetSubstructurePlotsExample_ZprimeMC.root --inputFile2=jetSubstructurePlotsExample_QCDMC.root --label1=tt --label2=QCD
###################

from ROOT import *

from optparse import OptionParser


parser = OptionParser()

parser.add_option('--inputFile1', metavar='F', type='string', action='store',
                  default='jetSubstructurePlotsExample_RStoWW.root',
                  dest='inputFile1',
                  help='Input file 1')

parser.add_option('--inputFile2', metavar='F', type='string', action='store',
                  default='jetSubstructurePlotsExample_QCDMC.root',
                  dest='inputFile2',
                  help='Input file 2')


parser.add_option('--outname', metavar='O', type='string', action='store',
                  default='substructureComparisonQuick.root',
                  dest='outname',
                  help='Output name')


parser.add_option('--dir', metavar='D', type='string', action='store',
                  default='pf/',
                  dest='dir',
                  help='directory to find plots')

parser.add_option('--label1', metavar='L', type='string', action='store',
                  default='RS->WW',
                  dest='label1',
                  help='Input file 1 label')

parser.add_option('--label2', metavar='L', type='string', action='store',
                  default='QCD',
                  dest='label2',
                  help='Input file 2 label')

(options, args) = parser.parse_args()

argv = []

#gROOT.Macro("rootlogon.C")
gROOT.Reset()
gStyle.SetPadRightMargin(0.2);

f1 = TFile(options.inputFile1)
f2 = TFile(options.inputFile2)


hists = [
    'hAK5JetMass',
    'hCA8JetMass',
    'hTrimmedMass',
    'hPrunedMass',
    'hFilteredMass',
    'hQjetVolatility',
    'hTau3',
    'hTau2',
    'hTau1',
    'hTau32',
    'hTau21',
    'h_prunedJet_Pt',
    'h_prunedJet_Mass',
    'h_prunedJet_Area',
    'h_prunedJet_Subjet0Pt',
    'h_prunedJet_Subjet0Mass',
    'h_prunedJet_Subjet0Area',
    'h_prunedJet_Subjet0DeltaRCore',
    'h_prunedJet_Subjet0PtRelCore',
    'h_prunedJet_Subjet1Pt',
    'h_prunedJet_Subjet1Mass',
    'h_prunedJet_Subjet1Area',
    'h_prunedJet_Subjet1DeltaRCore',
    'h_prunedJet_Subjet1PtRelCore',
    'h_prunedJet_DeltaRSubjet0Subjet1',
    'h_prunedJet_MassDrop',
    'h_prunedJet_SubjetAsymmetry',
    'h_prunedJet_cutMass_MassDrop',
    'h_prunedJet_cutMass_SubjetAsymmetry',
    'hCATopMass',
    'hCATopMinMass',
    'hCATopNsubjets'
    ]



outhists = []
stacks = []
canvs = []
leg = []

for ihist in hists:
    print ihist
    h1 = f1.Get(options.dir + ihist)
    h2 = f2.Get(options.dir + ihist)
    hs = THStack( h1.GetName(), h1.GetTitle() )
    outhists.append( [h1,h2] )
    stacks.append( hs )

    if h2.Integral() * h1.Integral() !=0:
      h2.Scale(1.0 / h2.Integral() * h1.Integral() )
    h1.SetLineColor(2)
    h2.SetLineColor(1)
    h1.SetLineWidth(2)
    h2.SetLineWidth(2)
    
    hs.Add( h2, 'hist' )
    hs.Add( h1, 'hist' )

    c = TCanvas('c' + ihist, 'c' + ihist,  200, 10, 750, 500 )
    hs.Draw('nostack')

    legend=TLegend(0.83,0.4,0.98,0.8)
    legend.SetFillColor(0);
    legend.AddEntry(h1,options.label1, "l")
    legend.AddEntry(h2,options.label2, "l")    
    legend.Draw()

    leg.append(legend)
    canvs.append(c)
    c.SaveAs('plots/Compare2_'+ihist+'.png')
