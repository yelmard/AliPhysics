///
/// \file AliFemtoAnalysisPionLambda.cxx
///

#include "AliFemtoAnalysisPionLambda.h"


#include "AliESDtrack.h"

#include "AliFemtoV0TrackCut.h"
#include "AliFemtoBasicTrackCut.h"
#include "AliFemtoBasicEventCut.h"
#include "AliFemtoV0TrackPairCut.h"

#include <TROOT.h>
#include <TInterpreter.h>

static const double LambdaMass = 1.115683,
                    PionMass = 0.139;

const struct { unsigned int bin_count; float min; float max; }

VertexBinning = {16, -10.0, 10.0},
MultBinning = {30, 0, 10000};


const float default_lambda_PtMin = 0.2,
            default_lambda_PtMax = 5.0,
            default_lambda_Eta = 0.8,

            default_lambda_MaxDCAV0 = 5.0,
            default_lambda_MaxV0DecayLength = 100.0,
            default_lambda_MinCosPointingAngle = 0.990,

            default_lambda_InvariantMassMin = LambdaMass - 0.005, // 1.110683
            default_lambda_InvariantMassMax = LambdaMass + 0.005; // 1.120683

const float default_lambda_eta = 0.8,
            default_lambda_EtaDaughters = 0.8,
            default_lambda_TPCnclsDaughters = 80.0,
            default_lambda_MaxDcaV0Daughters = 1.5,

            default_lambda_PtProtonMin = 0.5,
            default_lambda_PtProtonMax = 0.4,

            default_lambda_PtPionMin = 0.5,
            default_lambda_PtPionMax = 4.0,

            default_lambda_MinToPrimVertexProton = 0.1,
            default_lambda_MinToPrimVertexPion = 0.3;


const int   default_lambda_StatusDaughters = AliESDtrack::kTPCrefit,
            default_lambda_NdofDaughters = 4;

const bool  default_lambda_OnFlyStatus = kFALSE;

const float default_pion_PtMin = 0.2,
            default_pion_PtMax = 2.0,

            default_pion_EtaMin = 0.8,
            default_pion_EtaMax = -0.8,

            default_pion_DCAMin = 0.5,
            default_pion_DCAMax = 4.0,

            default_pion_NSigmaPionMin = -3.0,
            default_pion_NSigmaPionMax = 3.0;



const float default_event_EventMultMin = 0,
            default_event_EventMultMax = 100000,

            default_event_VertZPosMin = -10.0,
            default_event_VertZPosMax = 10.0,

            default_event_EPVZEROMin = -1000.0,
            default_event_EPVZEROMax = 1000.0;

const  int  default_event_TriggerSelection = 0;
const  bool default_event_AcceptBadVertex = kFALSE;

const AliFemtoAnalysisPionLambda::PionType
  default_PionType = AliFemtoAnalysisPionLambda::kPiPlus;

const AliFemtoAnalysisPionLambda::LambdaType
  default_LambdaType = AliFemtoAnalysisPionLambda::kLambda;



void
AliFemtoAnalysisPionLambda::_Init(const AliFemtoAnalysisPionLambda::CutParams& p = AliFemtoAnalysisPionLambda::DefaultCutConfig())
{ // This function is to be called at the end of all constructors. It will
  // create default objects for any cuts that haven't been initialized

  if (fFirstParticleCut == NULL) {
    SetFirstParticleCut(BuildLambdaCut(p));
  }

  if (fSecondParticleCut == NULL) {
    SetSecondParticleCut(BuildPionCut(p));
  }

  if (fEventCut == NULL) {
    SetEventCut(BuildEventCut(p));
  }

  if (fPairCut == NULL) {
    SetPairCut(BuildPairCut(p));
  }
}

AliFemtoAnalysisPionLambda::AliFemtoAnalysisPionLambda():
  AliFemtoVertexMultAnalysis(VertexBinning.bin_count, VertexBinning.min, VertexBinning.max,
                               MultBinning.bin_count,   MultBinning.min,   MultBinning.max),
  fPionType(default_PionType),
  fLambdaType(default_LambdaType)
{
  _Init();
}

AliFemtoAnalysisPionLambda::AliFemtoAnalysisPionLambda(const char *name):
  AliFemtoVertexMultAnalysis(VertexBinning.bin_count, VertexBinning.min, VertexBinning.max,
                               MultBinning.bin_count,   MultBinning.min,   MultBinning.max),
  fPionType(default_PionType),
  fLambdaType(default_LambdaType)
{
  _Init();
}


AliFemtoAnalysisPionLambda::AliFemtoAnalysisPionLambda(const char *name,
                                                       PionType pion,
                                                       LambdaType lambda
):
  AliFemtoVertexMultAnalysis(VertexBinning.bin_count, VertexBinning.min, VertexBinning.max,
                               MultBinning.bin_count,   MultBinning.min,   MultBinning.max),
  fPionType(pion),
  fLambdaType(lambda)
{
  _Init();
}

AliFemtoAnalysisPionLambda
  ::AliFemtoAnalysisPionLambda(const char *name,
                               const PionType pion,
                               const LambdaType lambda,
                               const CutParams& params):
  AliFemtoVertexMultAnalysis(VertexBinning.bin_count, VertexBinning.min, VertexBinning.max,
                             MultBinning.bin_count,   MultBinning.min,   MultBinning.max),
  fPionType(pion),
  fLambdaType(lambda)
{
  _Init(params);
}

AliFemtoAnalysisPionLambda
  ::AliFemtoAnalysisPionLambda(const char *name,
                               const AnalysisParams &params,
                               const CutParams &cut_params):
  AliFemtoVertexMultAnalysis(params.vertex_bins, params.vertex_min, params.vertex_max,
                             params.mult_bins, params.mult_min, params.mult_max),
  fPionType(params.pion_type),
  fLambdaType(params.lambda_type)
{
  cout << "Created with : " <<
    params.vertex_bins <<
    " " << params.vertex_min <<
    " " << params.vertex_max <<
    " " << params.mult_bins <<
    " " << params.mult_min <<
    " " << params.mult_max <<'\n';
  _Init(cut_params);
}


AliFemtoAnalysisPionLambda::AnalysisParams
AliFemtoAnalysisPionLambda::DefaultConfig()
{
  AliFemtoAnalysisPionLambda::AnalysisParams params = {
    VertexBinning.bin_count,
    VertexBinning.min,
    VertexBinning.max,

    MultBinning.bin_count,
    MultBinning.min,
    MultBinning.max,

    default_PionType,
    default_LambdaType
  };

  cout << "[AliFemtoAnalysisPionLambda::DefaultConfig]\n"
       << "  " << params.vertex_bins <<
           " " << params.vertex_min <<
           " " << params.vertex_max <<
           " " << params.mult_bins <<
           " " << params.mult_min <<
           " " << params.mult_max << '\n';

 return params;
}


AliFemtoAnalysisPionLambda::CutParams
AliFemtoAnalysisPionLambda::DefaultCutConfig()
{
  AliFemtoAnalysisPionLambda::CutParams params = {
    // Event
    {
      default_event_EventMultMin,
      default_event_EventMultMax,
      default_event_VertZPosMax,
      default_event_VertZPosMin,
      default_event_EPVZEROMin,
      default_event_EPVZEROMax
    },
    // Pion
    {
      default_pion_PtMin,
      default_pion_PtMax,
      default_pion_EtaMin,
      default_pion_EtaMax,
      default_pion_DCAMin,
      default_pion_DCAMax
    },
    // Lambda
    {
      default_lambda_PtMin,
      default_lambda_PtMax,
      default_lambda_Eta,

      default_lambda_MaxDCAV0,
      default_lambda_MaxV0DecayLength,
      default_lambda_MinCosPointingAngle,

      default_lambda_InvariantMassMin,
      default_lambda_InvariantMassMax,

      default_lambda_OnFlyStatus,

      // daughter params
      {
        default_lambda_EtaDaughters,
        default_lambda_TPCnclsDaughters,
        default_lambda_MaxDcaV0Daughters,
        default_lambda_StatusDaughters,
        default_lambda_NdofDaughters,

        // pion daughter
        {
          default_lambda_PtPionMin,
          default_lambda_PtPionMax,
          default_lambda_MinToPrimVertexPion
        },
        // proton daughter
        {
          default_lambda_PtProtonMin,
          default_lambda_PtProtonMax,
          default_lambda_MinToPrimVertexProton
        }
      }
    }
  };
  return params;
}


AliFemtoV0TrackCut*
AliFemtoAnalysisPionLambda::BuildLambdaCut(const CutParams &p) const
{
  AliFemtoV0TrackCut* cut = new AliFemtoV0TrackCut();

  cut->SetMass(LambdaMass);
  cut->SetInvariantMassLambda(p.lambda.MassMin, p.lambda.MassMax);
  cut->SetPt(p.lambda.PtMin, p.lambda.PtMax);
  cut->SetEta(p.lambda.Eta);

  cut->SetOnFlyStatus(p.lambda.OnFlyStatus);
  cut->SetMaxDcaV0(p.lambda.MaxDCA);
  cut->SetMaxV0DecayLength(p.lambda.MaxDecayLength);
  cut->SetMinCosPointingAngle(p.lambda.MinCosPointingAngle);

  cut->SetEtaDaughters(p.lambda.daughter.Eta);
  cut->SetTPCnclsDaughters(p.lambda.daughter.TPCncls);
  cut->SetNdofDaughters(p.lambda.daughter.Ndof);
  cut->SetStatusDaughters(p.lambda.daughter.StatusDaughters);
  cut->SetMaxDcaV0Daughters(p.lambda.daughter.MaxDCA);

  switch (fLambdaType) {
  case kLambda:
    cut->SetParticleType(AliFemtoV0TrackCut::kLambda);
    cut->SetPtPosDaughter(p.lambda.daughter.proton.PtMin,
                          p.lambda.daughter.proton.PtMax);
    cut->SetPtNegDaughter(p.lambda.daughter.pion.PtMin,
                          p.lambda.daughter.pion.PtMax);
    cut->SetMinDaughtersToPrimVertex(p.lambda.daughter.pion.MinToPrimVertex,
                                     p.lambda.daughter.proton.MinToPrimVertex);

  case kAntiLambda:
    cut->SetParticleType(AliFemtoV0TrackCut::kAntiLambda);
    cut->SetPtPosDaughter(p.lambda.daughter.pion.PtMin,
                          p.lambda.daughter.pion.PtMax);
    cut->SetPtNegDaughter(p.lambda.daughter.proton.PtMin,
                          p.lambda.daughter.proton.PtMax);
    cut->SetMinDaughtersToPrimVertex(p.lambda.daughter.proton.MinToPrimVertex,
                                     p.lambda.daughter.pion.MinToPrimVertex);
  }

  return cut;
}

AliFemtoBasicTrackCut*
AliFemtoAnalysisPionLambda::BuildPionCut(const CutParams &p) const
{
  AliFemtoBasicTrackCut *cut = new AliFemtoBasicTrackCut();

  const int charge = (fPionType == kPiMinus) ? -1 :
                      (fPionType == kPiPlus) ? +1 :
                                              -999;
  if (charge == -999) {
    std::cerr << "E-AliFemtoAnalysisPionLambda::BuildPionCut: Invalid pion type: '" << fPionType << "'\n";
  }

  cut->SetCharge(charge);
  cut->SetMass(PionMass);

  cut->SetNSigmaPion(p.pion.NSigmaPionMin, p.pion.NSigmaPionMax);
  cut->SetPt(p.pion.PtMin, p.pion.PtMax);
  cut->SetRapidity(p.pion.EtaMin, p.pion.EtaMax);
  cut->SetDCA(p.pion.DCAMin, p.pion.DCAMax);

  return cut;
}

AliFemtoBasicEventCut*
AliFemtoAnalysisPionLambda::BuildEventCut(
  const AliFemtoAnalysisPionLambda::CutParams& params
) const
{
  AliFemtoBasicEventCut* cut = new AliFemtoBasicEventCut();

  cut->SetEventMult(params.event.MultMin,
                    params.event.MultMax);
  cut->SetVertZPos(params.event.VertexZMin,
                   params.event.VertexZMax);
  cut->SetEPVZERO(params.event.EP_VZeroMin,
                  params.event.EP_VZeroMax);
  cut->SetTriggerSelection(params.event.TriggerSelection);
  cut->SetAcceptBadVertex(params.event.AcceptBadVertex);

  return cut;
}


const Bool_t default_pair_TPCOnly = kTRUE;
const Float_t default_pair_TPCExitSepMin = -1.0;

AliFemtoV0TrackPairCut*
AliFemtoAnalysisPionLambda::BuildPairCut(const CutParams &p) const
{
  AliFemtoV0TrackPairCut *cut = new AliFemtoV0TrackPairCut();

  cut->SetTPCOnly(p.pair.TPCOnly);
  cut->SetTPCExitSepMinimum(p.pair.TPCExitSepMin);

  return cut;
}


AliFemtoV0TrackCut* AliFemtoAnalysisPionLambda::GetLambdaCut()
{
  return static_cast<AliFemtoV0TrackCut*>(fFirstParticleCut);
}

AliFemtoTrackCut* AliFemtoAnalysisPionLambda::GetPionCut()
{
  return static_cast<AliFemtoTrackCut*>(fSecondParticleCut);
}

const AliFemtoV0TrackCut* AliFemtoAnalysisPionLambda::GetLambdaCut() const
{
  return static_cast<const AliFemtoV0TrackCut*>(fFirstParticleCut);
}

const AliFemtoTrackCut* AliFemtoAnalysisPionLambda::GetPionCut() const
{
  return static_cast<const AliFemtoTrackCut*>(fSecondParticleCut);
}
