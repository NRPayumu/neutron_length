// 2020/01/08
//
#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4UnionSolid.hh"
#include "G4EllipticalTube.hh"

#include "G4SDManager.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"

#include "G4SystemOfUnits.hh"
//
#include <iomanip>
#include <math.h>

#include "variable.h"

using namespace std;

DetectorConstruction::DetectorConstruction()
{
	//
	// define material
	//
	DefineMaterials();

	fWorldMaterial    = G4Material::GetMaterial("G4_AIR");
	fGdLSMaterial     = G4Material::GetMaterial("GdLS");

	// colour
	white   = G4Colour(1.0,  1.0,  1.0);
	grey    = G4Colour(0.5,  0.5,  0.5);
	red     = G4Colour(1.0,  0.0,  0.0);
	green   = G4Colour(0.0,  1.0,  0.0);
	blue    = G4Colour(0.0,  0.0,  1.0);
	yellow  = G4Colour(1.0,  1.0,  0.0);
}

DetectorConstruction::~DetectorConstruction()
{;}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	//
	// visibility flag
	//
	G4bool visFlagWorld    = 1; // white
	G4bool visFlagGdLS        = 1; //red

	//
	// copy number
	//
	G4int copyNoWorld      = 9;
	G4int copyNoGdLS         = 301;

	//
	// Geometory parameters
	//

	// geometory parameter values

	// World ////////////////////
	G4Box*             worldSolid    = new G4Box("World", worldBoxSizeXY / 2.0, worldBoxSizeXY / 2.0, worldBoxSizeZ / 2.0);
	G4LogicalVolume*   worldLogical  = new G4LogicalVolume(worldSolid, fWorldMaterial, "World");
	G4VPhysicalVolume* worldPhysical = new G4PVPlacement(0, G4ThreeVector(), worldLogical, "World", 0, false, copyNoWorld);
	G4VisAttributes* worldVisAtt = new G4VisAttributes(grey);
	worldVisAtt  -> SetVisibility(visFlagWorld);
	worldLogical -> SetVisAttributes(worldVisAtt);

	// GdLS /////////////
	// Mother = World
	G4Box*GdLSSolid   = new G4Box("GdLS", GdLSSizeX / 2.0, GdLSSizeY / 2.0, GdLSSizeZ / 2.0);
	G4LogicalVolume*   GdLSLogical = new G4LogicalVolume(GdLSSolid, fGdLSMaterial, "GdLS");
	G4double GdLSPositionZ = 0*cm;
	new G4PVPlacement(0, G4ThreeVector(0,0,GdLSPositionZ), GdLSLogical, "GdLS", worldLogical, false, copyNoGdLS);
	G4VisAttributes* GdLSVisAtt = new G4VisAttributes(red);
	GdLSVisAtt  -> SetVisibility(visFlagGdLS);
	GdLSLogical -> SetVisAttributes(GdLSVisAtt);

	// always return the physical world
	return worldPhysical;
}

void DetectorConstruction::DefineMaterials()
{
	// Material defined using NIST Manager
	// http://geant4.web.cern.ch/geant4/G4UsersDocuments/UsersGuides/ForApplicationDeveloper/html/Detector/materialNames.html
	G4NistManager* nistManager = G4NistManager::Instance();
	nistManager -> FindOrBuildMaterial("G4_AIR");
	nistManager -> FindOrBuildMaterial("G4_WATER");

	//for GdLS
	//
	// define elements
	G4Element* H   = nistManager -> FindOrBuildElement("H");
	G4Element* C    = nistManager -> FindOrBuildElement("C");
	G4Element* N   = nistManager -> FindOrBuildElement("N");
	G4Element* O    = nistManager -> FindOrBuildElement("O");
	G4Element* Gd  = nistManager -> FindOrBuildElement("Gd");

	G4double fractionmass, density;
	G4int    ncomponents, natoms;
	G4String name, symbol;

	// Materials for Lquid Scintillator //////////////////
	//
	//LAB
	//https://www.noe.jxtg-group.co.jp/business/chemical/product/alkyl_benzene.html
	//hino p71 http://www.awa.tohoku.ac.jp/Thesis/ThesisFile/hino_yota_m.pdf
	density = 0.852*g/cm3; // No data in MSDS, written in GLG4DetectorConstruction.cc
	G4Material* LAB = new G4Material("LAB", density, ncomponents = 2);
	LAB -> AddElement(C, natoms = 18);
	LAB -> AddElement(H, natoms = 30);

	//bis-MSB
	// bis-MSB (1.4-Bis(2-methylstynyl)benzene) = C24H22 !!! temporary density !!!
	density = 1.3*g/cm3; // No data in MSDS, written in GLG4DetectorConstruction.cc
	G4Material* BisMSB = new G4Material("Bis-MSB", density, ncomponents = 2);
	BisMSB -> AddElement(C, natoms = 24);
	BisMSB -> AddElement(H, natoms = 22);

	// DPO(PPO) (2.5-Diphenylozazole) C15H11NO !!! temporary density !!!
	density = 1.06 *g/cm3;  // No data in MSDS, written in GLG4DetectorConstruction.cc
	G4Material* PPO = new G4Material("PPO", density, ncomponents = 4);
	PPO -> AddElement(C, natoms = 15);
	PPO -> AddElement(H, natoms = 11);
	PPO -> AddElement(N, natoms = 1);
	PPO -> AddElement(O, natoms = 1);

	//Gd
	density =  7.9*g/cm3;  // No data in MSDS, written in GLG4DetectorConstruction.cc
	G4Material* Gd_mat = new G4Material("Gd_mat", density, ncomponents = 1);
	Gd_mat -> AddElement(Gd, natoms = 1);

	density = 0.853 *g/cm3;
	G4Material* NonGdLS = new G4Material("NonGdLS", density, ncomponents = 3);
	NonGdLS -> AddMaterial(LAB,     fractionmass = 99.6457 *perCent); //
	NonGdLS -> AddMaterial(PPO,     fractionmass =  0.3508   *perCent); // 3.0 g/l
	NonGdLS -> AddMaterial(BisMSB,  fractionmass =  0.0035 *perCent); // 30 mg/l

	G4Material* GdLS_inside = new G4Material("GdLS", GdLS_density, ncomponents = 2);
	//GdLS_inside -> AddMaterial(NonGdLS, fractionmass = 99.5 *perCent); //
	//GdLS_inside -> AddMaterial(Gd_mat,  fractionmass =  0.5 *perCent); // 0.5wt%
	GdLS_inside -> AddMaterial(NonGdLS, fractionmass = (100-Gd_fractionmass) *perCent); //
	GdLS_inside -> AddMaterial(Gd_mat,  fractionmass =      Gd_fractionmass  *perCent); //

	// Print materials
	G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}
