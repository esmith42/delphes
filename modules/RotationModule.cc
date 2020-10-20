#define _USE_MATH_DEFINES
#include "modules/RotationModule.h"

#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"
#include "classes/DelphesFormula.h"

#include "ExRootAnalysis/ExRootClassifier.h"
#include "ExRootAnalysis/ExRootFilter.h"
#include "ExRootAnalysis/ExRootResult.h"

#include "TDatabasePDG.h"
#include "TFormula.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom3.h"
#include "TString.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cmath>

using namespace std;

//------------------------------------------------------------------------------

RotationModule::RotationModule() :
  fFormula(0), fItInputArray(0)
{
  fFormula = new DelphesFormula;
}

//------------------------------------------------------------------------------

RotationModule::~RotationModule()
{
  if(fFormula) delete fFormula;
}

//------------------------------------------------------------------------------

void RotationModule::Init()
{
  // read parameters

  fAngle = GetDouble("Angle", -1.0);  
  fAngleFile.open("testAngles.txt");
  fAngleFile << "[";  
  
  // import input array(s)

  fInputArray = ImportArray(GetString("InputArray", "GenJetFinder/jets"));
  fItInputArray = fInputArray->MakeIterator();

  // create output array(s)

  fOutputArray = ExportArray(GetString("OutputArray", "stableParticles"));
}

//------------------------------------------------------------------------------

void RotationModule::Finish()
{
  if(fItInputArray) delete fItInputArray;
  fAngleFile << "]";  
  fAngleFile.close();  
}

//------------------------------------------------------------------------------


void RotationModule::Process()
{
  
      
    
    
  Candidate *candidate;
  Double_t pt_sum = 0;

  // loop over all input candidates
  fItInputArray->Reset();
  while((candidate = static_cast<Candidate *>(fItInputArray->Next())))
  {
    const TObjArray *subCandidates = candidate->GetCandidates(); //These are supposed to be particles but won't necessarily be
    TIterator *itSubcandidates = subCandidates->MakeIterator();
    const TLorentzVector &candidateMomentum = candidate->Momentum;
    const TVector3& axis = candidateMomentum.Vect();
      
    Candidate *subCandidate;//, *mother;
    Double_t pt;
    Double_t a = fRand(0,2*M_PI);
      
    itSubcandidates->Reset();
    while((subCandidate = static_cast<Candidate *>(itSubcandidates->Next()))) {
    //for(tobj = subCandidates.First(); subCandidates.After(tobj) != 0; tobj = subCandidates.After(tobj)) {
    //while(itSubcandidates->operator*() != NULL) {
          
      //tobj = itSubcandidates->operator*();
      //subCandidate = static_cast<Candidate *> (tobj);  
      const TLorentzVector &subCandidateMomentum = subCandidate->Momentum;
      pt = subCandidateMomentum.Pt();
      pt_sum = pt + pt_sum;
      //mother = candidate;
      subCandidate = static_cast<Candidate *>(subCandidate->Clone());
      //subCandidate->AddCandidate(mother);
      //subCandidate->Momentum.SetPhi(14);
      //Double_t a = fRand(0,2*M_PI);
      if (fAngle != -1) {
          a = fAngle;
      }
      subCandidate->Momentum.Rotate(a, axis);
      subCandidate->Position.Rotate(a, axis);
      fAngleFile << std::to_string(a) << ", ";  
      fOutputArray->Add(subCandidate);
      
    }
    
        
  }
  //angleFile << "]";  
  //angleFile.close();  
  //cout << pt_sum;

}

//------------------------------------------------------------------------------

Double_t RotationModule::fRand(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
  }