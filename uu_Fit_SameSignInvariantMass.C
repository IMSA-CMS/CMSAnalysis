TGraph* makeGraph(double numCoords, std::vector<double>& xCoords, std::vector<double>& yCoords);

void uu_Fit_SameSignInvariantMass()
{
	// Create Histogram gram from Data
	auto myFile = TFile::Open("LeptonZPeakData.root");
	auto sameDir = myFile->GetDirectory("uu__hists");
	auto sameHist = sameDir->Get<TH1>("uu_Same Sign Invariant Mass");

	auto oppositeDir = myFile->GetDirectory("u u__hists");
	auto oppositeHist = oppositeDir->Get<TH1>("u u_Opposite Sign Invariant Mass");
	double sameBin = sameHist->GetNbinsX();

	double bin = oppositeHist->GetNbinsX();
	// Get the data points of each histogram
	std::vector <double> xTotal;
	std::vector <double> yTotal;
	for  (int i=1; i<=bin; i++)
	{
		xTotal.push_back(oppositeHist->GetBinCenter(i));
		yTotal.push_back(oppositeHist->GetBinContent(i));
	}

	auto graphTotal = makeGraph(bin, xTotal, yTotal);

	auto c2 = new TCanvas("c2", "Opposite-Sign Muon Invariant Mass", 1000, 1000); 
	graphTotal->SetTitle("Opposite-Sign Muon Invariant Mass;Invariant Mass;Events");
	graphTotal->GetXaxis()->SetLimits(0.,200.);
	graphTotal -> Draw("AL");
 /*
	// Get the data points of each histogram
	std::vector <double> sameXTotal;
	std::vector <double> sameYTotal;
	for  (int i=1; i<=sameBin; i++)
	{
		sameXTotal.push_back(sameHist->GetBinCenter(i));
		sameYTotal.push_back(sameHist->GetBinContent(i));
	}

	//Transfer the vector input to an array (used in TGraph)
	double xSameAll[sameXTotal.size()], ySameAll[sameYTotal.size()];
	for (int i=0; i < sameXTotal.size(); i++)
	{
		xSameAll[i] = sameXTotal[i];
		ySameAll[i] = sameYTotal[i];
	}
	
	auto sameGraphTotal =new TGraph(sameBin, xSameAll, ySameAll);
	sameGraphTotal->SetLineColor(kBlue);

	//Create the Selection Graphs
	double sameLower = 10;
	double sameUpper = 60;
	
	// Get the Coordinates for cutting out the peak from the data
	std::vector<double> xSameSelection;
	std::vector<double> ySameSelection;
	for (int i=0; i<sameXTotal.size(); i++)
	{
		if ((sameXTotal[i] < sameLower) or (sameUpper < sameXTotal[i]))
		{
			xSameSelection.push_back(sameXTotal[i]);
			ySameSelection.push_back(sameYTotal[i]);
		}
	}

	// create array
	double xArraySameSelection[xSameSelection.size()], yArraySameSelection[ySameSelection.size()];
	
	for (int i=0; i < xSameSelection.size(); i++)
	{
		xArraySameSelection[i] = xSameSelection[i];
		yArraySameSelection[i] = ySameSelection[i];
	}

	//Create peak graph object
	auto sameGraphSelection = new TGraph(xSameSelection.size(), xArraySameSelection, yArraySameSelection);
		

	// Fit the Graphs with polynomials
	auto peakFit = new TF1("peakFit", "[10]*x^10 + [9]*x^9 + [8]*x^8 + [7]*x^7 + [6]*x^6 + [5]*x^5 + [4]*x^4 + [3]*x^3 + [2]*x^2 + [1]*x^1 + [0]*x^0", 0, 400);
	auto noPeakFit = new TF1("noPeakFit", "[10]*x^10 + [9]*x^9 + [8]*x^8 + [7]*x^7 + [6]*x^6 + [5]*x^5 + [4]*x^4 + [3]*x^3 + [2]*x^2 + [1]*x^1 + [0]*x^0", 0, 400);
	
	//double peakFitIntegral = In

	peakFit->SetLineColor(kGreen);
	noPeakFit->SetLineColor(kMagenta);

	//sameGraphTotal->Draw("AC");
	//sameGraphTotal->Fit("peakFit", "W", "SAME", 5, 150);
	sameGraphSelection->Draw("AL");
	sameGraphSelection->Fit("noPeakFit", "W", "SAME", 0, 200);
	noPeakFit->Draw("SAME");

	//hist->Draw("SAME");
*/
}

TGraph* makeGraph(double numCoords, std::vector<double>& xCoords, std::vector<double>& yCoords)
{
	//Transfer the vector input to an array (used in TGraph)
	double xArray[xCoords.size()], yArray[yCoords.size()];
	for (int i=0; i < xCoords.size(); i++)
	{
		xArray[i] = xCoords[i];
		yArray[i] = yCoords[i];
	}
	
	auto graph =new TGraph(numCoords, xArray, yArray);
	return graph;
}