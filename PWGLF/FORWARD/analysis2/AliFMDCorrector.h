// This class calculates the exclusive charged particle density
// in each for the 5 FMD rings. 
//
#ifndef ALIFMDCORRECTOR_H
#define ALIFMDCORRECTOR_H
/**
 * @file   AliFMDCorrector.h
 * @author Christian Holm Christensen <cholm@dalsgaard.hehi.nbi.dk>
 * @date   Wed Mar 23 14:01:02 2011
 * 
 * @brief  
 * 
 * @ingroup pwglf_forward_aod
 * 
 */
#include <TNamed.h>
#include <TList.h>
#include "AliForwardUtil.h"
class TH2D;
class TH2;
/** 
 * @defgroup pwglf_forward_algo Algorithms 
 *
 * Various sub-algorithms used in the analysis 
 *
 * @ingroup pwglf_forward 
 */
/** 
 * This class calculates the exclusive charged particle density
 * in each for the 5 FMD rings. 
 *
 * @par Input:
 *   - 5 RingHistos objects - each with a number of vertex dependent 
 *     2D histograms of the inclusive charge particle density 
 *
 * @par Output:
 *   - 5 RingHistos objects - each with a number of vertex dependent 
 *     2D histograms of the exclusive charge particle density 
 * 
 * @par Corrections used: 
 *   - AliFMDCorrSecondaryMap;
 *   - AliFMDCorrVertexBias
 *   - AliFMDCorrMergingEfficiency
 *
 * @ingroup pwglf_forward_algo 
 * @ingroup pwglf_forward_aod
 */
class AliFMDCorrector : public TNamed
{
public:
  /** 
   * Constructor 
   */
  AliFMDCorrector();
  /** 
   * Constructor 
   * 
   * @param name Name of object
   */
  AliFMDCorrector(const char* name);
  /** 
   * Copy constructor 
   * 
   * @param o Object to copy from 
   */
  AliFMDCorrector(const AliFMDCorrector& o);
  /** 
   * Destructor 
   */
  virtual ~AliFMDCorrector();
  /** 
   * Assignement operator
   * 
   * @param o Object to assign from 
   * 
   * @return Reference to this object
   */
  AliFMDCorrector& operator=(const AliFMDCorrector&);
  /** 
   * Initialize this object
   *
   * @param etaAxis Eta axis to use  -- not used
   */
  virtual void SetupForData(const TAxis& etaAxis);
  /**
   * Do the calculations 
   * 
   * @param hists    Cache of histograms 
   * @param vtxBin   Vertex bin 
   * 
   * @return true on successs 
   */
  virtual Bool_t Correct(AliForwardUtil::Histos& hists, UShort_t vtxBin);
  /** 
   * Scale the histograms to the total number of events 
   * 
   * @param dir     Where the output is stored
   * @param output  Output list 
   * @param nEvents Number of events 
   */
  virtual void Terminate(const TList* dir, TList* output, Int_t nEvents);
  /** 
   * Output diagnostic histograms to directory 
   * 
   * @param dir List to write in
   */  
  virtual void CreateOutputObjects(TList* dir);
  /** 
   * Set the debug level.  The higher the value the more output 
   * 
   * @param dbg Debug level 
   */
  void SetDebug(Int_t dbg=1) { fDebug = dbg; }
  /** 
   * @{
   * @name Enable/disable parts of the corrections
   */
  /** 
   *  Whether to do correction for secondaries
   *
   * @param use If true, use the secondary correction
   */
  void SetUseSecondaryMap(Bool_t use=true) { fUseSecondaryMap = use; }
  /**
   * Check
   *
   * @return true if the correction for secondaries is done
   */
  Bool_t IsUseSecondaryMap() const { return fUseSecondaryMap; }
  /**
   *  Whether to do correction for vertex bias
   *
   * @param use If true, use the vertex bias correction
   */
  void SetUseVertexBias(Bool_t use=true) { fUseVertexBias = use; }
  /**
   * Check
   *
   * @return true if the correction for vertex bias is done
   */
  Bool_t IsUseVertexBias() const { return fUseVertexBias; }
  /**
   *  Whether to do correction for dead-channel acceptance
   *
   * @param use If true, use the dead-channel acceptance correction
   */
  void SetUseAcceptance(Bool_t use=true) { fUseAcceptance = use; }
  /**
   * Check
   *
   * @return true if the correction for dead-channel acceptance is done
   */
  Bool_t IsUseAcceptance() const { return fUseAcceptance; }
  /**
   * Whether to use the merging efficiency correction
   *
   * @param use If true, use the merging efficiency correction
   */
  void SetUseMergingEfficiency(Bool_t use=true) { fUseMergingEfficiency = use; }
  /**
   * Check
   *
   * @return true if the merging efficiency correction is used.
   */
  Bool_t IsUseMergingEfficiency() const { return fUseMergingEfficiency; }
  /* @} */

  /**
   * Print information
   * 
   * @param option Not used 
   */
  virtual void Print(Option_t* option="") const;
protected:
  /** 
   * Internal data structure to keep track of the histograms.  Never
   * streamed.
   */
  struct RingHistos : public AliForwardUtil::RingHistos 
  { 
    /** 
     * Default CTOR
     */
    RingHistos();
    /** 
     * Constructor
     * 
     * @param d detector
     * @param r ring 
     */
    RingHistos(UShort_t d, Char_t r);
    /** 
     * Copy constructor 
     * 
     * @param o Object to copy from 
     */
    RingHistos(const RingHistos& o);
    /** 
     * Assignment operator 
     * 
     * @param o Object to assign from 
     * 
     * @return Reference to this 
     */
    RingHistos& operator=(const RingHistos& o);
    /** 
     * Destructor 
     */
    ~RingHistos();
    /** 
     * Make output 
     * 
     * @param dir Where to put it 
     */
    void CreateOutputObjects(TList* dir);
    /** 
     * Scale the histograms to the total number of events 
     * 
     * @param dir     where the output is stored
     * @param nEvents Number of events 
     */
    void Terminate(TList* dir, Int_t nEvents);
    TH2D*     fDensity;      // Distribution primary Nch
    // ClassDef(RingHistos,2);
  };
  /** 
   * Get the ring histogram container 
   * 
   * @param d Detector
   * @param r Ring 
   * 
   * @return Ring histogram container 
   */
  RingHistos* GetRingHistos(UShort_t d, Char_t r) const;
  /** 
   * Divide a map with another map.  This is a reimplementation of
   * TH1::Divide, but we assume compatible histograms, and the under-
   * and overflow bins are only divided if the third argument is true.
   * 
   * @param num             Numerator. On return contains the result 
   * @param denom           Denominator
   * @param alsoUnderOver   If true, also divide under/overflow bins
   */
  void DivideMap(TH2* num, const TH2* denom, Bool_t alsoUnderOver=false) const;
  TList    fRingHistos;           // List of histogram containers
  Bool_t   fUseSecondaryMap;      // Whether to do correction for secondaries
  Bool_t   fUseVertexBias;        // Whether to do correction for vertex bias
  Bool_t   fUseAcceptance;        // Whether to do correction for dead ch's.
  Bool_t   fUseMergingEfficiency; // Whether to use the merging efficiency
  Int_t    fDebug;                //  Debug level 

  ClassDef(AliFMDCorrector,4); // Correct the inclusive d2N/detadphi
};

#endif
// Local Variables:
//   mode: C++
// End:

