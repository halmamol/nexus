// ----------------------------------------------------------------------------
// nexus | MigdalSignalGenerator.cc
//
// This class is the a generator of the Migdal Signal from ** in the detector.
// Particle propiertes are taken from an external file defined in the configuration
// file. Probabilites are evaluated in this code. 
//
// The NEXT Collaboration
// ----------------------------------------------------------------------------

#include "MigdalSignalGenerator.h"

#include "G4Event.hh"
#include "DetectorConstruction.h"
#include "BaseGeometry.h"

#include <G4GenericMessenger.hh>
#include <G4RunManager.hh>
#include <G4ParticleTable.hh>
#include <G4RandomDirection.hh>
#include <Randomize.hh>
#include <G4IonTable.hh>
#include <fstream>
#include <iostream>
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
//
// in Global space... temporary hack.
//
// #include <fstream>
// std::ofstream fOutCheckKr83mTmp;

namespace nexus {

  using namespace CLHEP;

  MigdalSignalGenerator::MigdalSignalGenerator() : msg_(0), 
  particle_defXray_(0), particle_defME_(0), particle_defXraye_(0),
  particle_defXe_(0),
  atomic_number_(0), mass_number_(0), energy_level_(0.), 
  opened_(false),
  geom_(0), region_("")
  {

     msg_ = new G4GenericMessenger(this, "/Generator/MigdalSignalGenerator/",
    "Control commands of Migdal generator.");
     
     //msg_->DeclareMethod("inputFile", &MigdalSignalGenerator::OpenInputFile, "");
     
     // Set particle type searching in particle table by name
    particle_defXray_ = G4ParticleTable::GetParticleTable()->
      FindParticle("gamma");
    particle_defME_ = G4ParticleTable::GetParticleTable()->
      FindParticle("e-");
    particle_defXraye_ = G4ParticleTable::GetParticleTable()->
      FindParticle("e-");
      
      
     msg_->DeclareMethod("inputFile", &MigdalSignalGenerator::OpenInputFile, "");
     msg_->DeclareProperty("region", region_,
			   "Set the region of the geometry where the vertex will be generated.");


      
    DetectorConstruction* detconst = (DetectorConstruction*)
      G4RunManager::GetRunManager()->GetUserDetectorConstruction();
    geom_ = detconst->GetGeometry();
  }

  MigdalSignalGenerator::~MigdalSignalGenerator()
  {
    if (file_.is_open()) file_.close();
  }

  void MigdalSignalGenerator::OpenInputFile(G4String filename)
  {
    if (filename.find("none") != std::string::npos) {
      std::cerr << " We will use the new interface to Decay00 " << std::endl;
      opened_ = false;
      return;
    }

    file_.open(filename.data());

    if (file_.good()) {
      opened_ = true;
      //ProcessHeader();
    }
    else {
      G4Exception("[MigdalSignalGenerator]", "SetInputFile()", JustWarning,
      "Cannot open Decay0 input file.");
    }
  }

  void MigdalSignalGenerator::GeneratePrimaryVertex(G4Event* event)
  {
      
  // Generate an initial position for the particle using the geometry
  G4ThreeVector position = geom_->GenerateVertex(region_);

  // Particle generated at start-of-event
  G4double time = 0.;

  // Create a new vertex
  G4PrimaryVertex* vertex = new G4PrimaryVertex(position, time);
      

   G4double evt_ID = event->GetEventID();
   std::cout << "Event ID " << evt_ID << std::endl;
   // reading event-related information
   G4int evt_no;     // event number
   G4int test; 
  
   file_ >> evt_no >> test;
//    std::cout << evt_no << " " << test << std::endl;
   if(evt_no != evt_ID){std::cout << " Error! File value not in agreement with event number" << std::endl; }


  
  //--- Nuclear Recoil
  
    // Calculate cartesian components of momentum
    G4double mass_NR;
    G4double px_NR;
    G4double py_NR;
    G4double pz_NR;
    
    G4int particleID1;
    G4int XenonNR;
    
    

    file_ >> particleID1 >> XenonNR >> px_NR >> py_NR >> pz_NR >> mass_NR;
//    std::cout << particleID1 << " " << XenonNR <<  " " << px_NR << " " << py_NR << " " << pz_NR << " " << mass_NR << std::endl;
    if(particleID1 != 1){std::cout << " Error! Particle value not in agreement with Nuclear Recoil" << std::endl; }
    
   particle_defXe_ = 
    G4IonTable::GetIonTable()->GetIon(54, XenonNR, 0.);   
  // Create the new primary particle and set it some properties
    G4PrimaryParticle* particle_XeNR =
     new G4PrimaryParticle(particle_defXe_, px_NR, py_NR, pz_NR);    
   
     
     
    //--- Migdal Electron ---

    // Calculate cartesian components of momentum
  G4double mass_ME;
  G4double px_ME;
  G4double py_ME;
  G4double pz_ME;
  
  G4int particleID2;
  G4int pdgME;
  
  file_ >> particleID2 >> pdgME >> px_ME >> py_ME >> pz_ME >> mass_ME;
//    std::cout << particleID2 << " " << pdgME <<  " " << px_ME << " " << py_ME << " " << pz_ME << " " << mass_ME << std::endl;
  if(particleID2 != 2){std::cout << " Error! Particle value not in agreement with Migdal Electron" << std::endl; }
  // Create the new primary particle and set it some properties
  G4PrimaryParticle* particle_ME =
    new G4PrimaryParticle(particle_defME_, px_ME, py_ME, pz_ME);  
    
    
    
 
  //--- De-excitation particles ---
    // Calculate cartesian components of momentum
  G4double px_XR;
  G4double py_XR;
  G4double pz_XR;
  G4int mass_XR;
  
  G4int particleID3;
  G4int pdgXR;
  
  file_ >> particleID3 >> pdgXR >> px_XR >> py_XR >> pz_XR >> mass_XR;
//     std::cout << particleID3 << " " << pdgXR <<  " " << px_XR << " " << py_XR << " " << pz_XR << " " << mass_XR <<std::endl;
  if(particleID3 != 3){std::cout << " Error! Particle value not in agreement with Xray" << std::endl; }
  // Create the new primary particle and set it some properties
  G4PrimaryParticle* particle_XR =
    new G4PrimaryParticle(particle_defXray_, px_XR, py_XR, pz_XR);  
    
    
    // Calculate cartesian components of momentum
  G4double mass_XRe;
  G4double px_XRe;
  G4double py_XRe;
  G4double pz_XRe;
  
  G4int particleID4;
  G4int pdgXRe;
  
  file_ >> particleID4 >> pdgXRe >> px_XRe >> py_XRe >> pz_XRe >> mass_XRe;
   std::cout << particleID4 << " " << pdgXRe <<  " " << px_XRe << " " << py_XRe << " " << pz_XRe << " " << mass_XRe << std::endl;
  // Create the new primary particle and set it some properties
  G4PrimaryParticle* particle_XRe =
    new G4PrimaryParticle(particle_defXraye_, px_XRe, py_XRe, pz_XRe);  
    
  
  // Add ion to the vertex and this to the event
  vertex->SetPrimary(particle_XeNR);
  vertex->SetPrimary(particle_ME);
  vertex->SetPrimary(particle_XR);
  vertex->SetPrimary(particle_XRe);
  
  event->AddPrimaryVertex(vertex);
  

} // Name space nexus
}
