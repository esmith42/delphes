#ifndef RotationModule_h
#define RotationModule_h

/** \class ExampleModule
 *
 *  Selects candidates from the InputArray according to the efficiency formula.
 *
 *  \author P. Demin - UCL, Louvain-la-Neuve
 *
 */

#include "classes/DelphesModule.h"

#include <deque>
#include <iostream>
#include <fstream>


class TObjArray;
class DelphesFormula;

class RotationModule: public DelphesModule
{
public:
  RotationModule();
  ~RotationModule();

  void Init();
  void Process();
  void Finish();

private:
  Int_t fIntParam;
  Double_t fDoubleParam;
  Double_t fAngle;
  Double_t fRand(double fMin, double fMax);
  std::ofstream fAngleFile;

  std::deque<Double_t> fArrayParam;

  DelphesFormula *fFormula; //!

  TIterator *fItInputArray; //!

  const TObjArray *fInputArray; //!

  TObjArray *fOutputArray; //!

  ClassDef(RotationModule, 1)
};

#endif