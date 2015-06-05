#include "RecoTauTag/RecoTau/interface/TauDiscriminationProducerBase.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "RecoTauTag/TauTagTools/interface/TauTagTools.h"
#include "FWCore/Utilities/interface/isFinite.h"
#include "RecoTauTag/TauTagTools/interface/ECALBounds.h"

/* class CaloRecoTauDiscriminationAgainstElectron
 * created : Feb 17 2008,
 * revised : ,
 * contributors : Konstantinos Petridis, Sebastien Greder, 
 *                Maiko Takahashi, Alexandre Nikitenko (Imperial College, London), 
 *                Evan Friis (UC Davis)
 */

using namespace reco;

class CaloRecoTauDiscriminationAgainstElectron final  : public  CaloTauDiscriminationProducerBase {
   public:
      explicit CaloRecoTauDiscriminationAgainstElectron(const edm::ParameterSet& iConfig):CaloTauDiscriminationProducerBase(iConfig){   
         CaloTauProducer_                            = iConfig.getParameter<edm::InputTag>("CaloTauProducer");
         leadTrack_HCAL3x3hitsEtSumOverPt_minvalue_  = iConfig.getParameter<double>("leadTrack_HCAL3x3hitsEtSumOverPt_minvalue");  
         ApplyCut_maxleadTrackHCAL3x3hottesthitDEta_ = iConfig.getParameter<bool>("ApplyCut_maxleadTrackHCAL3x3hottesthitDEta");
         maxleadTrackHCAL3x3hottesthitDEta_          = iConfig.getParameter<double>("maxleadTrackHCAL3x3hottesthitDEta");
         ApplyCut_leadTrackavoidsECALcrack_          = iConfig.getParameter<bool>("ApplyCut_leadTrackavoidsECALcrack");
      }
      ~CaloRecoTauDiscriminationAgainstElectron(){} 
      double discriminate(const CaloTauRef& theCaloTauRef) const override;
      void beginEvent(const edm::Event& event, const edm::EventSetup& eventSetup) override;
   private:  
      edm::ESHandle<MagneticField> theMagneticField;
      edm::InputTag CaloTauProducer_;
      double leadTrack_HCAL3x3hitsEtSumOverPt_minvalue_;   
      bool ApplyCut_maxleadTrackHCAL3x3hottesthitDEta_;
      double maxleadTrackHCAL3x3hottesthitDEta_;
      bool ApplyCut_leadTrackavoidsECALcrack_;
      ECALBounds ecalBounds_;
};

void CaloRecoTauDiscriminationAgainstElectron::beginEvent(const edm::Event& event, const edm::EventSetup& eventSetup)
{
   if (ApplyCut_leadTrackavoidsECALcrack_)
   {
      // get the magnetic field, if we need it
      eventSetup.get<IdealMagneticFieldRecord>().get(theMagneticField);
   }
}


double CaloRecoTauDiscriminationAgainstElectron::discriminate(const CaloTauRef& theCaloTauRef) const 
{
   if (ApplyCut_maxleadTrackHCAL3x3hottesthitDEta_){
      // optional selection : ask for small |deta| between direction of propag. leading Track - ECAL inner surf. contact point and direction of highest Et hit among HCAL hits inside a 3x3 calo. tower matrix centered on direction of propag. leading Track - ECAL inner surf. contact point
      if (edm::isNotFinite((*theCaloTauRef).leadTrackHCAL3x3hottesthitDEta()) || (*theCaloTauRef).leadTrackHCAL3x3hottesthitDEta()>maxleadTrackHCAL3x3hottesthitDEta_) return 0.;
   }
   if (ApplyCut_leadTrackavoidsECALcrack_){
      // optional selection : ask that leading track - ECAL inner surface contact point does not fall inside any ECAL eta crack 
     math::XYZPoint thepropagleadTrackECALSurfContactPoint = TauTagTools::propagTrackECALSurfContactPoint(theMagneticField.product(),(*theCaloTauRef).leadTrack(),ecalBounds_);
      if(thepropagleadTrackECALSurfContactPoint.R()==0. ||
            fabs(thepropagleadTrackECALSurfContactPoint.eta())<ecalBounds_.crack_absEtaIntervalA().second || 
            (fabs(thepropagleadTrackECALSurfContactPoint.eta())>ecalBounds_.crack_absEtaIntervalB().first && fabs(thepropagleadTrackECALSurfContactPoint.eta())<ecalBounds_.crack_absEtaIntervalB().second) ||
            (fabs(thepropagleadTrackECALSurfContactPoint.eta())>ecalBounds_.crack_absEtaIntervalC().first && fabs(thepropagleadTrackECALSurfContactPoint.eta())<ecalBounds_.crack_absEtaIntervalC().second) ||
            (fabs(thepropagleadTrackECALSurfContactPoint.eta())>ecalBounds_.crack_absEtaIntervalD().first && fabs(thepropagleadTrackECALSurfContactPoint.eta())<ecalBounds_.crack_absEtaIntervalD().second) ||
            (fabs(thepropagleadTrackECALSurfContactPoint.eta())>ecalBounds_.crack_absEtaIntervalE().first && fabs(thepropagleadTrackECALSurfContactPoint.eta())<ecalBounds_.crack_absEtaIntervalE().second))
      {
         return 0.;
      }     
   }
   if (edm::isNotFinite((*theCaloTauRef).leadTrackHCAL3x3hitsEtSum()))
   {
      return 0.;
   } else
   {
      if ((*theCaloTauRef).leadTrackHCAL3x3hitsEtSum()/(*theCaloTauRef).leadTrack()->pt()<=leadTrack_HCAL3x3hitsEtSumOverPt_minvalue_) return 0.;
      else return 1.;
   }
}

DEFINE_FWK_MODULE(CaloRecoTauDiscriminationAgainstElectron);
