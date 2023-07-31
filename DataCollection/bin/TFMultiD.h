#ifndef TFMULTID_H
#define TFMULTID_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TFMultiD                                                             //
//                                                                      //
// The multi-dimensional parametric function                            //
//                                                                      //
// Written by Jason Mumford 11/30/2003                                  //
// email: Jason.Mumford@cern.ch                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include "TMethodCall.h"

const Int_t MAXDIM = 20;

class TFMultiD : public TNamed {
 protected:
  Double_t fXmin[MAXDIM]; // Lower bounds for the range
  Double_t fXmax[MAXDIM]; // Upper bounds for the range
  Int_t fType; // (0 for standard functions, 1 if pointer to function)
  Int_t fNpfits; // Number of points used in the fit
  Int_t fNDF; // Number of degrees of freedom in the fit
  Int_t fNpar;
  Int_t fNdim;
  Int_t fNumber;
  Double_t *fParErrors; // [fNpar] Array of errors of the fNpar parameters
  Double_t *fParMin; // [fNpar] Array of lower limits of the fNpar parameters
  Double_t *fParMax; // [fNpar] Array of upper limits of the fNpar parameters
  TMethodCall *fMethodCall; // Pointer to MethodCall in case of interpreted function
  Double_t (*fFunction) (Double_t *, Double_t *); // Pointer to function
  Double_t *fParams; // [fNpar] Array of fNpar parameters
  TString *fNames; // [fNpar] Array of parameter names

 public:
  TFMultiD();
  TFMultiD(const char *name, Double_t (*fcn)(Double_t *, Double_t *), 
	   Double_t *xmin, Double_t *xmax, Int_t ndim=0, Int_t npar=0);
  TFMultiD(const char *name, Double_t (*fcn)(Double_t *, Double_t *), 
	   Int_t ndim=0, Int_t npar=0);
  void setup(const char *name, Double_t (*fcn)(Double_t *, Double_t *), 
	     Double_t *xmin, Double_t *xmax, Int_t ndim, Int_t npar);
  TFMultiD(const TFMultiD &fm);
  ~TFMultiD();

  void FixParameter(Int_t ipar, Double_t value);
  Int_t GetNdim() const { return fNdim; }
  Int_t GetNpar() const { return fNpar; }
  void GetParLimits(Int_t ipar, Double_t &parmin, Double_t &parmax) const;
  Double_t GetParameter(Int_t ipar) const;
  Int_t GetParNumber(const char *parName) const;
  const char *GetParName(Int_t ipar) const;
  void SetNDF(Int_t ndf);
  Int_t GetNumberFitPoints() const { return fNpfits; }
  void SetParameter(const char *name, Double_t parvalue);
  void SetParameter(Int_t ipar, Double_t parvalue);
  void SetParameters(const Double_t *params);
  void SetParameters(Double_t p0, Double_t p1, Double_t p2=0,
		     Double_t p3=0, Double_t p4=0, Double_t p5=0,
		     Double_t p6=0, Double_t p7=0, Double_t p8=0,
		     Double_t p9=0, Double_t p10=0);
  void SetParError(Int_t ipar, Double_t error);
  void SetParLimits(Int_t ipar, Double_t parmin, Double_t parmax);
  void SetParName(Int_t ipar, const char *name);
  void SetParNames(const char *name0="p0", const char *name1="p1",
		   const char *name2="p2", const char *name3="p3",
		   const char *name4="p4", const char *name5="p5",
		   const char *name6="p6", const char *name7="p7",
		   const char *name8="p8", const char *name9="p9",
		   const char *name10="p10");
  void Update() {}
  Double_t EvalPar(Double_t *x, Double_t *par);
  void InitArgs(const Double_t *x, const Double_t *params);
  Double_t Integral(Int_t n, const Double_t *a, const Double_t *b, 
		    Double_t epsilon, Double_t &relerr, Int_t &status);
  Double_t crudeMCIntegral(UInt_t n, const Double_t *a, const Double_t *b, 
			   Double_t epsilon, Double_t &relerr, 
			   UInt_t &status, Bool_t debug, UInt_t max_points,
			   UInt_t seed = 102374);
};

#endif