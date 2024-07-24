// Main Function for the Root File
void uu_Fit_SameSignInvariantMass2();

// Makes the Graphs and Fit Equations for the Graphs
void getEquationsAndGraphs(double lowerBound, double upperBound, TH1* hist, TGraph*& graphTotal, TGraph*& graphSelection, TF1*& noPeakFit, 
std::vector<TF1*>& fits, std::vector<double> intervals);

// Gets the area of the peak by subtracting the total area from the area with the peak removed
double integrateDifference(double lower, double upper, TH1*& hist, std::vector<double> intervals, TF1*& noPeakFit);

// Prints the Chi Squared for all of the fits
void chiSquared(std::vector<double> interval, std::vector<TF1*>& peakFits, TF1* noPeakFit);

// Makes the Graphs by taking vector inputs
TGraph* makeGraph(double numCoords, std::vector<double>& xCoords, std::vector<double>& yCoords);

//Gets the coordinates based on the cutoffs for the z peak
void selectionCoords(double lowerBound, double upperBound, std::vector<double> xCoordsAll, std::vector<double> yCoordsAll,
 std::vector<double>& xSelection, std::vector<double>& ySelection);

// fits the Peak and non peak graphs
TF1* fitPeak(double minX, double maxX, TGraph*& graph);
TF1* fitNoPeak(double minX, double maxX, TGraph*& graph);




void uu_Fit_SameSignInvariantMass2()
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
	
	// List of fit functions for the Z Peak (more accurate fitting)
	std::vector<TF1*> sameFits;	
	std::vector<double> sameIntervals = {70, 77, 86, 94, 100, 110}; 

	// Boundaries to Cut out the Z-Peak
	double sameLowerCut = 70;
	double sameUpperCut = 150;
	

	// Function to make the graphs and fit equations with the cuts and fit intervals
	getEquationsAndGraphs(sameLowerCut, sameUpperCut, sameHist, sameGraphTotal, sameGraphSelection, 
	sameNoPeakFit, sameFits, sameIntervals);
	
	// Z Peak Cuts
	double oppLowerCut = 30;
	double oppUpperCut = 292;
	
	// Same as above, but with the opposite sign data
	TGraph* oppGraphTotal;
	TGraph* oppGraphSelection;
	TF1* oppNoPeakFit;
	std::vector<TF1*> oppFits;
	std::vector<double> oppIntervals = {oppLowerCut, 50, 65, 86, 94, 100, 150, 200, 250, oppUpperCut};



	// make the graphs and fit equations
	getEquationsAndGraphs(oppLowerCut, oppUpperCut, oppHist, oppGraphTotal, oppGraphSelection, 
	oppNoPeakFit, oppFits, oppIntervals);

	//Get the area from the Same and opposite sign fits
	double sameIntegral = integrateDifference(70, 110, sameHist, sameIntervals, sameNoPeakFit);
	double oppIntegral = integrateDifference(20, 292, oppHist, oppIntervals, oppNoPeakFit);

	// Get the Ratio of the integrals
	double signFlipRate = sameIntegral / oppIntegral;
	std::cout << "-------------------------------------"
	<<"\nSign Flip Rate: " << signFlipRate << std::endl; 
	/*<< "\nChi Square for all fit functions" << std::endl
	<< "\nSame Sign Chi Square:\n" << std::endl; 
	chiSquared(sameIntervals, sameFits, sameNoPeakFit);

	std::cout<<"\nOpposite Sign Chi Square: \n"<< std::endl;
	chiSquared(oppIntervals, oppFits, oppNoPeakFit);
	*/
	

	auto c1 = new TCanvas("c1", "Graphs", 1000, 2000);

	sameGraphTotal->SetLineColor(kGreen);
	sameGraphSelection->SetLineColor(kRed);
	sameNoPeakFit->SetLineColor(kMagenta);
	sameGraphTotal->Draw("AL");
	//sameGraphSelection->Draw("SAME");
	sameNoPeakFit->Draw("SAME");
	/*
	for (int i=0; i<sameFits.size(); i++)
	{
		if (i%2 == 0)
		{
			sameFits[i]->SetLineColor(kBlue);
			//oppFits[i]->SetLineColor(kOrange);
		}
		if (i%1 == 1)
		{
			sameFits[i]->SetLineColor(kOrange);
			//oppFits[i]->SetLineColor(kBlue);
		}
		sameFits[i]->Draw("SAME");
		//oppFits[i]->Draw("SAME");
	}
	*/
	auto c2 = new TCanvas("c2", "Graphs", 1000, 2000); 
	oppGraphTotal->Draw("AL");
	//oppGraphSelection->Draw("SAME");
	oppNoPeakFit->Draw("Same");
	
	
	
	
	
}

void getEquationsAndGraphs(double lowerBound, double upperBound, TH1* hist, TGraph*& graphTotal, TGraph*& graphSelection, 
TF1*& noPeakFit, std::vector<TF1*>& fits, std::vector<double> intervals)
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
	noPeakFit = fitNoPeak(lowerBound - 5, upperBound + 5, graphSelection);

	// Fit the peak using the intervals
	for (int i=0; i<intervals.size() -1; i++)
	{
		fits.push_back(fitPeak(intervals[i], intervals[i+1], graphTotal));
	}
}

double integrateDifference(double lower, double upper, TH1*& hist, std::vector<double> intervals, 
TF1*& noPeakFit)
{
	int minBin = hist->FindBin(lower, 0, 0); 
	int maxBin = hist->FindBin(upper, 0, 0);
	double peakIntegral = hist->Integral(minBin, maxBin, "");

	double noPeakIntegral = noPeakFit->Integral(lower, upper);

	return peakIntegral - noPeakIntegral;
}

void chiSquared(std::vector<double> interval, std::vector<TF1*>& peakFits, TF1* noPeakFit)
{
	std::cout<< "Z peak Chi Squared:" << std::endl;
	for (int i=0; i<peakFits.size(); i++)
	{
		std::cout<<"Interval: [" << interval[i] <<","<<interval[i+1] <<"]\t" << peakFits[i]->GetChisquare() <<endl;
	}
	std::cout <<"No Peak Chi Squared: " << noPeakFit->GetChisquare()<<endl;
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

TF1* fitPeak(double minX, double maxX, TGraph*& graph)
{
	auto fitEq = new TF1("fitEq", "[10]*x^10 + [9]*x^9 + [8]*x^8 + [7]*x^7 + [6]*x^6 + [5]*x^5 + [4]*x^4 + [3]*x^3 + [2]*x^2 + [1]*x^1 + [0]*x^0", 
	minX, maxX);
	graph->Fit("fitEq", "N", "", minX, maxX);
	return fitEq;
}

TF1* fitNoPeak(double minX, double maxX, TGraph*& graph)
{
	auto fitEq = new TF1("fitEq", "exp([1]*x^1 + [0]*x^0)", minX, maxX);
	graph->Fit("fitEq", "N", "", minX, maxX);
	return fitEq;
}