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

#include <fstream>

#include "TFMultiD.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TROOT.h"
#include "Api.h" // for G__p2f2funcname

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

using namespace std;

TFMultiD::TFMultiD() 
  : TNamed(), fType(0), fNpfits(0), fNDF(0), fNdim(0) {
  for (Int_t i = 0; i < MAXDIM; i++) {
    fXmin[i] = 0;
    fXmax[i] = 0;
  }
}

TFMultiD::TFMultiD(const char *name, Double_t (*fcn)(Double_t *, Double_t *), 
		   Double_t *xmin, Double_t *xmax, Int_t ndim, Int_t npar)
  : TNamed(), fNpfits(0), fNDF(0), fNdim(ndim) {
  setup(name, fcn, xmin, xmax, ndim, npar);
}

TFMultiD::TFMultiD(const char *name, Double_t (*fcn)(Double_t *, Double_t *), 
		   Int_t ndim, Int_t npar)
  : TNamed(), fNpfits(0), fNDF(0), fNdim(ndim) {
  setup(name, fcn, 0, 0, ndim, npar);
}

void TFMultiD::setup(const char *name, Double_t (*fcn)(Double_t *, Double_t *),
		     Double_t *xmin, Double_t *xmax, Int_t ndim, Int_t npar) {
  if (xmin)
    for (Int_t i = 0; i < fNdim; i++) {
      fXmin[i] = xmin[i];
      fXmax[i] = xmax[i];
    }
  else
    for (Int_t i = 0; i < fNdim; i++)
      fXmin[i] = fXmax[i] = 0;

  char *funcname = G__p2f2funcname((void*)fcn);
  if (funcname) {
    fType = 2;
    SetTitle(funcname);
    fMethodCall = new TMethodCall();
    fMethodCall->InitWithPrototype(funcname,"Double_t*,Double_t*");
    fNumber = -1;
    fFunction = 0;
  }
  else {
    fType = 1;
    fMethodCall = 0;
    fFunction = fcn;
  }

  if (npar > 0 )
    fNpar = npar;

  if (fNpar) {
    fNames = new TString[fNpar];
    fParams = new Double_t[fNpar];
    fParErrors = new Double_t[fNpar];
    fParMin = new Double_t[fNpar];
    fParMax = new Double_t[fNpar];
    for (int i = 0; i < fNpar; i++) {
      fParams[i] = 0;
      fParErrors[i] = 0;
      fParMin[i] = 0;
      fParMax[i] = 0;
    }
  }
  else {
    fParErrors = 0;
    fParMin = 0;
    fParMax = 0;
  }

  // store formula in linked list of formula in ROOT
  TFMultiD *fm_old = (TFMultiD*)gROOT->GetListOfFunctions()->FindObject(name);
  delete fm_old;
  SetName(name);
  gROOT->GetListOfFunctions()->Add(this);
}

TFMultiD::~TFMultiD() {
  delete [] fParMin;
  delete [] fParMax;
  delete [] fParErrors;
  delete [] fParams;
  delete [] fNames;

  delete fMethodCall;
  gROOT->GetListOfFunctions()->Remove(this);
}

TFMultiD::TFMultiD(const TFMultiD &fm) : TNamed(fm) {
   ((TFMultiD&)fm).Copy(*this);
}

void TFMultiD::FixParameter(Int_t ipar, Double_t value) {
  // Fix the value of a parameter
  //   The specified value will be used in a fit operation

  if (ipar < 0 || ipar > fNpar-1) return;
  SetParameter(ipar,value);
  if (value != 0) SetParLimits(ipar,value,value);
  else            SetParLimits(ipar,1,1);
}

void
TFMultiD::GetParLimits(Int_t ipar, Double_t &parmin, Double_t &parmax) const {
  // Return limits for parameter ipar
  parmin = 0;
  parmax = 0;
  if (ipar < 0 || ipar > fNpar-1) return;
  if (fParMin) parmin = fParMin[ipar];
  if (fParMax) parmax = fParMax[ipar];
}

Double_t TFMultiD::GetParameter(Int_t ipar) const {
  // Return value of parameter number ipar
  if (ipar <0 && ipar >= fNpar) return 0;
  return fParams[ipar];
}

Int_t TFMultiD::GetParNumber(const char *parName) const {
  // Return parameter number by name
  for (Int_t i=0; i<fNpar; i++)
    if (fNames[i] == parName) return i;
  return -1;
}

const char *TFMultiD::GetParName(Int_t ipar) const {
  // Return name of parameter ipar
  if (ipar <0 && ipar >= fNpar) return "";
  if (fNames[ipar].Length() > 0) return (const char*)(fNames[ipar]);
  return Form("p%d",ipar);
}

void TFMultiD::SetNDF(Int_t ndf) {
  // Set the number of degrees of freedom
  // ndf should be (the number of points used in a fit) 
  //                - (the number of free parameters)
  fNDF = ndf;
}

void TFMultiD::SetParError(Int_t ipar, Double_t error) {
  // Set error for parameter number ipar
  if (ipar < 0 || ipar > fNpar-1) return;
  fParErrors[ipar] = error;
}

void TFMultiD::SetParLimits(Int_t ipar, Double_t parmin, Double_t parmax) {
  // Set limits for parameter ipar
  //   The specified limits will be used in a fit operation
  //     when the option "B" is specified (Bounds).
  //  To fix a parameter, use TF1::FixParameter
  if (ipar < 0 || ipar > fNpar-1) return;
  Int_t i;
  if (!fParMin) {
    fParMin = new Double_t[fNpar]; 
    for (i=0;i<fNpar;i++) fParMin[i]=0;
  }
  if (!fParMax) {
    fParMax = new Double_t[fNpar]; 
    for (i=0;i<fNpar;i++) fParMax[i]=0;
  }
  fParMin[ipar] = parmin;
  fParMax[ipar] = parmax;
}

void TFMultiD::SetParameter(const char *name, Double_t value) {
  // Initialize parameter number ipar
  Int_t ipar = GetParNumber(name);
  if (ipar < 0 || ipar >= fNpar) return;
  fParams[ipar] = value;
  Update();
}

void TFMultiD::SetParameter(Int_t ipar, Double_t value) {
  // Initialize parameter number ipar
  if (ipar < 0 || ipar >= fNpar) return;
  fParams[ipar] = value;
  Update();
}

void TFMultiD::SetParameters(const Double_t *params) {
  // Initialize array of all parameters
  for (Int_t i=0; i<fNpar;i++)
    fParams[i] = params[i];
  Update();
}

void TFMultiD::SetParameters(Double_t p0, Double_t p1, Double_t p2, 
			     Double_t p3, Double_t p4, Double_t p5,
			     Double_t p6, Double_t p7, Double_t p8,
			     Double_t p9, Double_t p10) {
  // Initialize up to 11 parameters
  if (fNpar > 0) fParams[0] = p0;
  if (fNpar > 1) fParams[1] = p1;
  if (fNpar > 2) fParams[2] = p2;
  if (fNpar > 3) fParams[3] = p3;
  if (fNpar > 4) fParams[4] = p4;
  if (fNpar > 5) fParams[5] = p5;
  if (fNpar > 6) fParams[6] = p6;
  if (fNpar > 7) fParams[7] = p7;
  if (fNpar > 8) fParams[8] = p8;
  if (fNpar > 9) fParams[9] = p9;
  if (fNpar > 10) fParams[10] = p10;
  Update();
}

void TFMultiD::SetParName(Int_t ipar, const char *name) {
  // Set name of parameter number ipar
  if (ipar <0 || ipar >= fNpar) return;
  fNames[ipar] = name;
}

void TFMultiD::SetParNames(const char* name0, const char* name1, 
			   const char* name2, const char* name3,
			   const char* name4, const char* name5,
			   const char* name6, const char* name7,
			   const char* name8, const char* name9,
			   const char* name10) {
  // Set up to 11 parameter names
  if (fNpar > 0) fNames[0] = name0;
  if (fNpar > 1) fNames[1] = name1;
  if (fNpar > 2) fNames[2] = name2;
  if (fNpar > 3) fNames[3] = name3;
  if (fNpar > 4) fNames[4] = name4;
  if (fNpar > 5) fNames[5] = name5;
  if (fNpar > 6) fNames[6] = name6;
  if (fNpar > 7) fNames[7] = name7;
  if (fNpar > 8) fNames[8] = name8;
  if (fNpar > 9) fNames[9] = name9;
  if (fNpar > 10) fNames[10] = name10;
}

Double_t TFMultiD::EvalPar(Double_t *x, Double_t *par) {
  Double_t returnval = fFunction(x, par);
  return returnval;
}
 
void TFMultiD::InitArgs(const Double_t *x, const Double_t *params) {
  // Initialize parameters addresses
  if (fMethodCall) {
    Long_t args[2];
    args[0] = (Long_t)x;
    if (params) args[1] = (Long_t)params;
    else        args[1] = (Long_t)fParams;
    fMethodCall->SetParamPtrs(args);
  }
}

Double_t TFMultiD::Integral(Int_t n, const Double_t *a, const Double_t *b, 
			    Double_t eps, Double_t &relerr, Int_t &status) {
//  Adaptive Quadrature for Multiple Integrals over N-Dimensional
//  Rectangular Regions
//
// Author(s): A.C. Genz, A.A. Malik
// converted/adapted by R.Brun to C++ from Fortran CERNLIB routine RADMUL (D120)
// The new code features many changes compared to the Fortran version.
// Note that this function is currently called only by TF2::Integral (n=2)
// and TF3::Integral (n=3).
//
// This function computes, to an attempted specified accuracy, the value of
// the integral over an n-dimensional rectangular region.
//
// N Number of dimensions.
// A,B One-dimensional arrays of length >= N . On entry A[i],  and  B[i],
//     contain the lower and upper limits of integration, respectively.
// EPS    Specified relative accuracy.
// RELERR Contains, on exit, an estimation of the relative accuray of RESULT.
//
// Method:
//
// An integration rule of degree seven is used together with a certain
// strategy of subdivision.
// For a more detailed description of the method see References.
//
// Notes:
//
//   1.Multi-dimensional integration is time-consuming. For each rectangular
//     subregion, the routine requires function evaluations.
//     Careful programming of the integrand might result in substantial saving
//     of time.
//   2.Numerical integration usually works best for smooth functions.
//     Some analysis or suitable transformations of the integral prior to
//     numerical work may contribute to numerical efficiency.
//
// References:
//
//   1.A.C. Genz and A.A. Malik, Remarks on algorithm 006:
//     An adaptive algorithm for numerical integration over
//     an N-dimensional rectangular region, J. Comput. Appl. Math. 6 (1980) 295-302.
//   2.A. van Doren and L. de Ridder, An adaptive algorithm for numerical
//     integration over an n-dimensional cube, J.Comput. Appl. Math. 2 (1976) 207-217.
//
//=========================================================================
   Double_t ctr[15], wth[15], wthl[15], z[15];

   const Double_t xl2 = 0.358568582800318073;
   const Double_t xl4 = 0.948683298050513796;
   const Double_t xl5 = 0.688247201611685289;
   const Double_t w2  = 980./6561;
   const Double_t w4  = 200./19683;
   const Double_t wp2 = 245./486;
   const Double_t wp4 = 25./729;

   Double_t wn1[14] = {     -0.193872885230909911, -0.555606360818980835,
     -0.876695625666819078, -1.15714067977442459,  -1.39694152314179743,
     -1.59609815576893754,  -1.75461057765584494,  -1.87247878880251983,
     -1.94970278920896201,  -1.98628257887517146,  -1.98221815780114818,
     -1.93750952598689219,  -1.85215668343240347,  -1.72615963013768225};

   Double_t wn3[14] = {     0.0518213686937966768,  0.0314992633236803330,
     0.0111771579535639891,-0.00914494741655235473,-0.0294670527866686986,
    -0.0497891581567850424,-0.0701112635269013768, -0.0904333688970177241,
    -0.110755474267134071, -0.131077579637250419,  -0.151399685007366752,
    -0.171721790377483099, -0.192043895747599447,  -0.212366001117715794};

   Double_t wn5[14] = {         0.871183254585174982e-01,  0.435591627292587508e-01,
     0.217795813646293754e-01,  0.108897906823146873e-01,  0.544489534115734364e-02,
     0.272244767057867193e-02,  0.136122383528933596e-02,  0.680611917644667955e-03,
     0.340305958822333977e-03,  0.170152979411166995e-03,  0.850764897055834977e-04,
     0.425382448527917472e-04,  0.212691224263958736e-04,  0.106345612131979372e-04};

   Double_t wpn1[14] = {   -1.33196159122085045, -2.29218106995884763,
     -3.11522633744855959, -3.80109739368998611, -4.34979423868312742,
     -4.76131687242798352, -5.03566529492455417, -5.17283950617283939,
     -5.17283950617283939, -5.03566529492455417, -4.76131687242798352,
     -4.34979423868312742, -3.80109739368998611, -3.11522633744855959};

   Double_t wpn3[14] = {     0.0445816186556927292, -0.0240054869684499309,
    -0.0925925925925925875, -0.161179698216735251,  -0.229766803840877915,
    -0.298353909465020564,  -0.366941015089163228,  -0.435528120713305891,
    -0.504115226337448555,  -0.572702331961591218,  -0.641289437585733882,
    -0.709876543209876532,  -0.778463648834019195,  -0.847050754458161859};

   Double_t result = 0;
   Double_t abserr = 0;
   Int_t ifail = 3;
   if (n < 2 || n > 15) return 0;

   Double_t twondm = TMath::Power(2,n);
   Int_t ifncls = 0;
   Bool_t ldv   = kFALSE;
   Int_t irgnst = 2*n+3;
   Int_t irlcls = Int_t(twondm) +2*n*(n+1)+1;
   Int_t isbrgn = irgnst;
   Int_t isbrgs = irgnst;

// The original algorithm expected a parameter MAXPTS
//   where MAXPTS = Maximum number of function evaluations to be allowed.
//   Here we set MAXPTS to 1000*(the lowest possible value)
   Int_t maxpts = 1000*irlcls;
   //Int_t maxpts = 10000*irlcls;
   Int_t minpts = 1;

// The original agorithm expected a working space array WK of length IWK
// with IWK Length ( >= (2N + 3) * (1 + MAXPTS/(2**N + 2N(N + 1) + 1))/2).
// Here, this array is allocated dynamically

   Int_t iwk = irgnst*(1 +maxpts/irlcls)/2;
   Double_t *wk = new Double_t[iwk+10];
   Int_t j;
   for (j=0;j<n;j++) {
      ctr[j] = (b[j] + a[j])*0.5;
      wth[j] = (b[j] - a[j])*0.5;
   }

   Double_t rgnvol, sum1, sum2, sum3, sum4, sum5, difmax, f2, f3, dif, aresult;
   Double_t rgncmp=0, rgnval, rgnerr;
   Int_t j1, k, l, m, idvaxn=0, idvax0=0, isbtmp, isbtpp;

   InitArgs(z,fParams);

L20:
   rgnvol = twondm;
   for (j=0;j<n;j++) {
      rgnvol *= wth[j];
      z[j]    = ctr[j];
   }
   sum1 = EvalPar(z,fParams); //evaluate function

   difmax = 0;
   sum2   = 0;
   sum3   = 0;
   for (j=0;j<n;j++) {
      z[j]    = ctr[j] - xl2*wth[j];
      f2      = EvalPar(z,fParams);
      z[j]    = ctr[j] + xl2*wth[j];
      f2     += EvalPar(z,fParams);
      wthl[j] = xl4*wth[j];
      z[j]    = ctr[j] - wthl[j];
      f3      = EvalPar(z,fParams);
      z[j]    = ctr[j] + wthl[j];
      f3     += EvalPar(z,fParams);
      sum2   += f2;
      sum3   += f3;
      dif     = TMath::Abs(7*f2-f3-12*sum1);
      if (dif >= difmax) {
         difmax=dif;
         idvaxn=j+1;
      }
      z[j]    = ctr[j];
   }

   sum4 = 0;
   for (j=1;j<n;j++) {
      j1 = j-1;
      for (k=j;k<n;k++) {
         for (l=0;l<2;l++) {
            wthl[j1] = -wthl[j1];
            z[j1]    = ctr[j1] + wthl[j1];
            for (m=0;m<2;m++) {
               wthl[k] = -wthl[k];
               z[k]    = ctr[k] + wthl[k];
               sum4 += EvalPar(z,fParams);
            }
         }
         z[k] = ctr[k];
      }
      z[j1] = ctr[j1];
   }

   sum5 = 0;
   for (j=0;j<n;j++) {
      wthl[j] = -xl5*wth[j];
      z[j] = ctr[j] + wthl[j];
   }
L90:
   sum5 += EvalPar(z,fParams);
   for (j=0;j<n;j++) {
      wthl[j] = -wthl[j];
      z[j] = ctr[j] + wthl[j];
      if (wthl[j] > 0) goto L90;
   }

   rgncmp  = rgnvol*(wpn1[n-2]*sum1+wp2*sum2+wpn3[n-2]*sum3+wp4*sum4);
   rgnval  = wn1[n-2]*sum1+w2*sum2+wn3[n-2]*sum3+w4*sum4+wn5[n-2]*sum5;
   rgnval *= rgnvol;
   rgnerr  = TMath::Abs(rgnval-rgncmp);
   result += rgnval;
   abserr += rgnerr;
   ifncls += irlcls;
   aresult = TMath::Abs(result);
   if (aresult < 1e-100) {
      delete [] wk;
      return result;
   }

   if (ldv) {
L110:
       isbtmp = 2*isbrgn;
       if (isbtmp > isbrgs) goto L160;
       if (isbtmp < isbrgs) {
          isbtpp = isbtmp + irgnst;
          if (wk[isbtmp-1] < wk[isbtpp-1]) isbtmp = isbtpp;
       }
       if (rgnerr >= wk[isbtmp-1]) goto L160;
       for (k=0;k<irgnst;k++) {
          wk[isbrgn-k-1] = wk[isbtmp-k-1];
       }
       isbrgn = isbtmp;
       goto L110;
    }
L140:
    isbtmp = (isbrgn/(2*irgnst))*irgnst;
    if (isbtmp >= irgnst && rgnerr > wk[isbtmp-1]) {
       for (k=0;k<irgnst;k++) {
          wk[isbrgn-k-1] = wk[isbtmp-k-1];
       }
       isbrgn = isbtmp;
       goto L140;
    }

L160:
   wk[isbrgn-1] = rgnerr;
   wk[isbrgn-2] = rgnval;
   wk[isbrgn-3] = Double_t(idvaxn);
   for (j=0;j<n;j++) {
      isbtmp = isbrgn-2*j-4;
      wk[isbtmp]   = ctr[j];
      wk[isbtmp-1] = wth[j];
   }
   if (ldv) {
      ldv = kFALSE;
      ctr[idvax0-1] += 2*wth[idvax0-1];
      isbrgs += irgnst;
      isbrgn  = isbrgs;
      goto L20;
   }
   relerr = abserr/aresult;
   if (relerr < 1e-1 && aresult < 1e-20) ifail = 0;
   if (relerr < 1e-3 && aresult < 1e-10) ifail = 0;
   if (relerr < 1e-5 && aresult < 1e-5)  ifail = 0;
   if (isbrgs+irgnst > iwk) ifail = 2;
   if (ifncls+2*irlcls > maxpts) ifail = 1;
   if (relerr < eps && ifncls >= minpts) ifail = 0;
   if (ifail == 3) {
      ldv = kTRUE;
      isbrgn  = irgnst;
      abserr -= wk[isbrgn-1];
      result -= wk[isbrgn-2];
      idvax0  = Int_t(wk[isbrgn-3]);
      for (j=0;j<n;j++) {
         isbtmp = isbrgn-2*j-4;
         ctr[j] = wk[isbtmp];
         wth[j] = wk[isbtmp-1];
      }
      wth[idvax0-1]  = 0.5*wth[idvax0-1];
      ctr[idvax0-1] -= wth[idvax0-1];
      goto L20;
   }
// IFAIL On exit:
//     0 Normal exit.  . At most MAXPTS calls to the function F were performed.
//     1 MAXPTS is too small for the specified accuracy EPS. RESULT and RELERR
//              contain the values obtainable for the specified value of MAXPTS.
//
   delete [] wk;
   status = ifail;
//   Int_t nfnevl = ifncls; //number of function evaluations performed.
   return result;         //an approximate value of the integral
}

Double_t TFMultiD::crudeMCIntegral(UInt_t n, const Double_t *a,
				   const Double_t *b, Double_t epsilon, 
				   Double_t &relerr, UInt_t &status,
				   Bool_t debug, UInt_t max_points, 
				   UInt_t seed) {
  // Monte Carlo integration using crude M.C. method.  
  // Input:
  //    n = number of dimensions of function to integrate
  //    a = array of lower integration limits
  //    b = array of upper integration limits
  //    epsilon = desired relative error;
  //    max_points = maximum number of points to use for integration.  If
  //                 set to 0, use MAX_POINTS and do integration until relerr
  //                 is less than epsilon.
  //    seed = starting seed for random number generator
  // Output:
  //    relerr = final relative error
  //    status = 1 if desired relative error is achieved, 0 if not
  //    return integral of function

  //debug = false;
  // Could be larger than 15, but set to this for now
  const UInt_t MAX_DIM = 15;
  const UInt_t MAX_POINTS = 1000000000;
  const UInt_t MIN_POINTS = 10000;
  //const Double_t EPSILON = 1.e-6;


  Double_t rndm[MAX_DIM];
  Double_t range[MAX_DIM];
  Double_t f_sum = 0., f_square_sum = 0., err = 0.;
  Double_t a_val = 0., b_val = 0., integral = 0.;
  Double_t vol = 1.;
  ULong_t npoints = 1;
  Double_t temp_val;

  // Set seed for random number generator
  TRandom3 rndmGen;
  rndmGen.SetSeed(seed);

  if (n > MAX_DIM)
    throw cms::Exception("crudeMCIntegral") << n << " dimensions > 15; "
					    << "  refusing to integrate\n";
  
  // initialize parameters
  InitArgs(rndm,fParams);

  // Compute volume of integration space
  for (UInt_t i = 0; i < n; i++) {
    range[i] = b[i] - a[i];
    vol *= range[i];
  }

  if (debug) {
    ostringstream out;
    out << "  ndim = " << n << ", epsilon = " << epsilon << ", vol = " << vol
	<< "  lower lims = " << a[0];
    for (UInt_t i = 1; i < n; i++) out << ", " << a[i];
    out << endl << "  upper lims = " << b[0];
    for (UInt_t i = 1; i < n; i++) out << ", " << b[i];
    LogDebug("crudeMCIntegral") << out.str();
  }
  
  // If max points is set to zero, operate in mode where integration is 
  // done until relerr is less than epsilon.  Set max number of points
  // so that infinite loop is avoided.
  if (max_points == 0) max_points = MAX_POINTS;
  
  // Loop over max points
  for (UInt_t i = 0; i < max_points; i++) {
    // Generate array of n random numbers between 0.-1.
    rndmGen.RndmArray(n, rndm);

    // Rescale random numbers to fit integration space
    for (UInt_t j = 0; j < n; j++) {
      rndm[j] *= range[j];
      rndm[j] += a[j];
    }
    // Evaluate function with given random numbers and add to summation
    temp_val = EvalPar(rndm, fParams);
    //if (temp_val > EPSILON) f_sum += temp_val;
    f_sum += temp_val;

    // Running sum of function squared (used for error calculation)
    f_square_sum += temp_val*temp_val;

    // Error calculation
    a_val = f_square_sum/npoints;
    b_val = f_sum/npoints;
    err = vol*sqrt(fabs(a_val - (b_val*b_val))/npoints);

    // Calculate integral and relative error
    integral = b_val*vol;
    relerr = err/integral;
    //if (debug) {
    if (debug && ((npoints % 1000) == 1)) {
      ostringstream out;
      out << "  " << npoints << " points, rndm(1,..," 
	   << n << ") = (" << rndm[0];
      for (UInt_t j = 1; j < n; j++) {
	out << ", " << rndm[j];
      }
      out << "), temp_val = " << temp_val << ", f_sum = " << f_sum << endl; 
      out << "  int = " << integral << " +/- " << err 
	   << ", relerr = " << relerr << ", vol = " << vol;
      //out << endl << "a_val = " << a_val << ", b_val = " << b_val 
      //     << ", (a_val-(b_val*b_val))/npoints = " 
      //     << (a_val-(b_val*b_val))/npoints << ", err = " << err;
      LogDebug("crudeMCIntegral") << out.str();
    }
    if (max_points == MAX_POINTS) {
      if ((npoints > MIN_POINTS) && relerr <= epsilon) {
	status = 1;
	if (debug) {
	  ostringstream out;
	  out << "  Final result = " << integral << " +/- " << err << endl
	      << "  relerr = " << relerr << ", npoints = " << npoints-1 
	      << ", status = " << status;
	  LogDebug("crudeMCIntegral") << out.str();
	}
	return integral;
      }
    }
    npoints++;
  }

  // Status is 1 if relerr is less than epsilon
  if (relerr <= epsilon) status = 1;
  else status = 0;

  if (debug) {
    ostringstream out;
    out << "  Final result = " << integral << " +/- " << err << endl
	<< "  relerr = " << relerr << ", npoints = " << npoints-1 
	<< ", status = " << status;
    LogDebug("crudeMCIntegral") << out.str();
  }

  if (max_points == MAX_POINTS)
    edm::LogWarning("crudeMCIntegral")
      << relerr << " less than desired accuracy " << epsilon;

  return integral;
}