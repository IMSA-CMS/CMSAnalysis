// Main Function for the Root File
void uu_Fit_SameSignInvariantMass3();

// Makes the Graphs and Fit Equations for the Graphs
void getEquationsAndGraphs(double lowerBound, double upperBound, double fitMin, double fitMax, TH1* hist, TGraph*& graphTotal, TGraph*& graphSelection, TF1*& noPeakFit);

// Gets the area of the peak by subtracting the total area from the area with the peak removed
double integrateDifference(double lower, double upper, TH1*& hist, TF1*& noPeakFit);

// Makes the Graphs by taking vector inputs
TGraph* makeGraph(double numCoords, std::vector<double>& xCoords, std::vector<double>& yCoords);

//Gets the coordinates based on the cutoffs for the z peak
void selectionCoords(double lowerBound, double upperBound, std::vector<double> xCoordsAll, std::vector<double> yCoordsAll,
 std::vector<double>& xSelection, std::vector<double>& ySelection);

// fits the Peak and non peak graphs
TF1* fitNoPeak(double minX, double maxX, double lower, double upper, TGraph*& graph);


void fillPeak(double xMin, double xMax, TH1*& hist, TF1*& fit);





void uu_Fit_SameSignInvariantMass3()
{
	// Create Histograms from Data
	auto myFile = TFile::Open("LeptonZPeakData.root");
	auto sameDir = myFile->GetDirectory("ee__hists");
	auto sameHist = sameDir->Get<TH1>("ee_Same Sign Invariant Mass");

	auto oppositeDir = myFile->GetDirectory("e e__hists");
	auto oppHist = oppositeDir->Get<TH1>("e e_Opposite Sign Invariant Mass");

	

	//Declare Same Sign Lepton Graphs and Fit Equations. 
	TGraph* sameGraphTotal; //Z Peak included
	TGraph* sameGraphSelection; // Z Peak Removed
	TF1* sameNoPeakFit; // Fit Equation for Z Peak Removed

	// Boundaries to Cut out the Z-Peak and Bounds for fitting the cut graph
	double sameLowerCut = 72; //[64, 69]
	double sameUpperCut = 106;
	double sameFitMin = 60;
	double sameFitMax =  250;
	

	// Function to make the graphs and fit equations with the cuts and fit intervals
	getEquationsAndGraphs(sameLowerCut, sameUpperCut, sameFitMin, sameFitMax, sameHist, sameGraphTotal, sameGraphSelection, 
	sameNoPeakFit);
	
	// Z Peak Cuts and fit bounds
	double oppLowerCut = 30;
	double oppUpperCut = 270;
	double oppFitMin = 25;
	double oppFitMax = 500;

	
	// Same as above, but with the opposite sign data
	TGraph* oppGraphTotal;
	TGraph* oppGraphSelection;
	TF1* oppNoPeakFit;



	// make the graphs and fit equations
	getEquationsAndGraphs(oppLowerCut, oppUpperCut, oppFitMin, oppFitMax, oppHist, oppGraphTotal, oppGraphSelection, 
	oppNoPeakFit);

	// Integration Bounds is cuts
	double samelowerIntBound = sameLowerCut;
	double sameupperIntBound = sameUpperCut;
	double opplowerIntBound = oppLowerCut;
	double oppupperIntBound = oppUpperCut;

	//Get the area from the Same and opposite sign fits
	double sameIntegral = integrateDifference(samelowerIntBound, sameupperIntBound, sameHist, sameNoPeakFit);
	double oppIntegral = integrateDifference(opplowerIntBound, oppupperIntBound, oppHist, oppNoPeakFit);

	// Get the Ratio of the integrals
	double signFlipRate = sameIntegral / oppIntegral;
	std::cout << "-------------------------------------"
	<<"\nSign Flip Rate: " << signFlipRate << std::endl; 
	

	auto c1 = new TCanvas("c1", "Same-Sign Electron Invariant Mass", 1000, 1000);
	sameGraphTotal->SetLineColor(kGreen);
	sameGraphTotal->SetTitle("Same-Sign Electron Invariant Mass;Invariant Mass;Events");
	sameGraphTotal->GetXaxis()->SetLimits(0.,400.);

	sameGraphSelection->SetLineColor(kRed);
	sameNoPeakFit->SetLineColor(kMagenta);
	sameGraphTotal->SetFillColorAlpha(kBlue, 0.50);
	//sameHist->Draw("APS");
	sameGraphTotal->Draw("AL");
	//sameGraphSelection->Draw("SAME");
	sameNoPeakFit->Draw("SAME");

	auto c2 = new TCanvas("c2", "Opposite-Sign Electron Invariant Mass", 1000, 1000); 
	oppGraphTotal->SetTitle("Opposite-Sign Electron Invariant Mass;Invariant Mass;Events");
	oppGraphTotal->GetXaxis()->SetLimits(0.,400.);


	oppGraphTotal->Draw("AL");
	//oppGraphSelection->Draw("SAME");
	oppNoPeakFit->Draw("Same");	

	auto c3 = new TCanvas("c3", "Sumperimposed Invariant Mass", 1000, 1000);
	oppGraphTotal->SetTitle("Superimposed Electron Invariant Mass;Invariant Mass;Events");
	oppGraphTotal->GetXaxis()->SetLimits(0.,400.);
	oppGraphTotal->Draw("AL");
	sameGraphTotal->Draw("SAME");

}

void getEquationsAndGraphs(double lowerBound, double upperBound, double fitMin, double fitMax, TH1* hist, TGraph*& graphTotal, TGraph*& graphSelection, 
TF1*& noPeakFit)
{
	double bin = hist->GetNbinsX();
	// Get the data points of each histogram
	std::vector <double> xTotal;
	std::vector <double> yTotal;
	for  (int i=1; i<=bin; i++)
	{
		xTotal.push_back(hist->GetBinCenter(i));
		yTotal.push_back(hist->GetBinContent(i));
	}

	graphTotal = makeGraph(bin, xTotal, yTotal);

	// Get the Coordinates for cutting out the peak from the data. Make selection graph
	std::vector<double> xSelection;
	std::vector<double> ySelection;
	selectionCoords(lowerBound, upperBound, xTotal, yTotal, xSelection, ySelection);

	graphSelection = makeGraph(xSelection.size(), xSelection, ySelection);
		

	// Fit the Graphs with polynomials
	noPeakFit = fitNoPeak(fitMin, fitMax, lowerBound, upperBound, graphSelection);
}

double integrateDifference(double lower, double upper, TH1*& hist, TF1*& noPeakFit)
{
	int minBin = hist->FindBin(lower, 0, 0); 
	int maxBin = hist->FindBin(upper, 0, 0);
	double peakIntegral = hist->Integral(minBin, maxBin, "width");

	double noPeakIntegral = noPeakFit->Integral(lower, upper);
	
	std::cout<<"Histogram Integral: "<<peakIntegral<< std::endl
	<< "No Peak Integral: " << noPeakIntegral <<  std::endl
	<< "Difference: " << peakIntegral - noPeakIntegral <<  std::endl;
	return peakIntegral - noPeakIntegral;
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

void selectionCoords(double lowerBound, double upperBound, std::vector<double> xCoordsAll, std::vector<double> yCoordsAll,
 std::vector<double>& xSelection, std::vector<double>& ySelection)
{
	// Get the Coordinates for cutting out the peak from the data

	for (int i=0; i<xCoordsAll.size(); i++)
	{
		if ((xCoordsAll[i] < lowerBound) or (upperBound < xCoordsAll[i]))
		{
			xSelection.push_back(xCoordsAll[i]);
			ySelection.push_back(yCoordsAll[i]);
		}
	}
}

TF1* fitNoPeak(double minX, double maxX, double lower, double upper, TGraph*& graph)
{
	//auto fitEq = new TF1("fitEq", "exp([1]*x^1 + [0]*x^0)", minX, maxX);
	auto fitEq = new TF1("fitEq", "exp([1]*x^1 + [0]*x^0)", lower, upper);
	graph->Fit("fitEq", "N", "", minX, maxX);
	return fitEq;
}

/*
void fillPeak(double xMin, double xMax, TH1*& hist, TF1*& fit)
{
	double bin = hist->GetNbinsX();
	// Get the data points of each histogram
	std::vector <double> xTotal;
	std::vector <double> yTotal;
	TGraph* fillGraph;
	for  (int i=1; i<=bin; i++)
	{	
		if ((xMin < hist->getBinCenter(i)) or (hist->getBinCenter(i) < xMax))
		{
			xTotal.push_back(hist->GetBinCenter(i));
			yTotal.push_back(hist->GetBinContent(i));
		}
	}

	std::vector <double> xSelect;
	std::vector <double> ySelect;
	double delta = (xMax - xMin) / xTotal.size(); 
	double par0 = fit->GetParameter(0);
	double par1 = fit->GetParameter(1);
	for (int i=0; i < xTotal.size(); i++)
	{
		xSelect.push_back()
	}

	auto graphTotal = makeGraph(bin, xTotal, yTotal);
}
*/