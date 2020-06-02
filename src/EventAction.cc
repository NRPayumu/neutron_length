// 2019/11/18
//
#include "EventAction.hh"

#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4TrajectoryPoint.hh"
#include "G4SystemOfUnits.hh"

#include <iomanip>

#include "RunAction.hh"
#include "g4root.hh"

EventAction::EventAction(RunAction* run, PrimaryGeneratorAction* primary)
: fEnergyIn(0.),
fPositionInX(0.),
fPositionInY(0.),
fPositionInZ(0.),
fCaptureID(0),
fPositionCapX(0.),
fPositionCapY(0.),
fPositionCapZ(0.),
fTimeCap(0.),
fEnergyAbs(0.),
fEnergyLos(0.)
{
	fRunAction              = run;
	fPrimaryGeneratorAction = primary;
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* evt)
{
	fEnergyIn    = 0.,
	fPositionInX = 0.,
	fPositionInY = 0.,
	fPositionInZ = 0.,
	fCaptureID   = 0 ,
	fPositionCapX= 0.,
	fPositionCapY= 0.,
	fPositionCapZ= 0.,
	fTimeCap     = 0.,
	fEnergyAbs   = 0.,
	fEnergyLos   = 0.;

	fEventID = evt -> GetEventID();
	if (fEventID % 5000 == 0) {
		G4cout << "Begin of event: " << fEventID << G4endl;
	}
	SetEventID(fEventID);
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
	//
	// Visualization -->
	//
	G4VVisManager* pVisManager = G4VVisManager::GetConcreteInstance();
	if (pVisManager) {
		G4TrajectoryContainer* trajectoryContainer = evt -> GetTrajectoryContainer();
		G4int n_trajectories = 0;
		if (trajectoryContainer)  n_trajectories = trajectoryContainer -> entries();

		G4Colour colour;
		G4VisAttributes lineAttributes = G4VisAttributes();
		for (G4int i = 0; i < n_trajectories; i++) {
			G4Polyline* lines         = new G4Polyline();
			G4Polymarker* steps       = new G4Polymarker();
			G4Polymarker* auxiliaries = new G4Polymarker();

			G4VTrajectory* trajectory = (*trajectoryContainer)[i];
			TraceTrajectory(trajectory, lines, steps, auxiliaries);
			G4String particleName = trajectory -> GetParticleName();

			if      (particleName == "neutron")	SetColour(colour, "green");
			else if (particleName == "gamma")	        SetColour(colour, "blue");
			else if (particleName == "e-")            SetColour(colour, "yellow");
			else if (particleName == "proton") 	SetColour(colour, "gray");
			else if (particleName == "opticalphoton") SetColour(colour, "cyan");
			else if (particleName == "mu-") 	        SetColour(colour, "white");
			else 					SetColour(colour, "red");

			lineAttributes.SetColour(colour);
			lines -> SetVisAttributes(&lineAttributes);
			pVisManager -> Draw(*lines);

			delete lines;
			delete steps;
			delete auxiliaries;
		}
	}
	auto analysisManager = G4AnalysisManager::Instance();/*
	analysisManager->FillNtupleDColumn(0, fEnergyIn);
  analysisManager->FillNtupleDColumn(1, fPositionInX);
  analysisManager->FillNtupleDColumn(2, fPositionInY);
  analysisManager->FillNtupleDColumn(3, fPositionInZ);
	analysisManager->FillNtupleDColumn(4, fCaptureID);
  analysisManager->FillNtupleDColumn(5, fPositionCapX);
  analysisManager->FillNtupleDColumn(6, fPositionCapY);
  analysisManager->FillNtupleDColumn(7, fPositionCapZ);
  analysisManager->FillNtupleDColumn(8, fTimeCap);
  analysisManager->FillNtupleDColumn(9, fEnergyAbs);
  analysisManager->FillNtupleDColumn(10, fEnergyLos);
  analysisManager->AddNtupleRow();*/
}

void EventAction::TraceTrajectory(G4VTrajectory* trajectory,
	G4Polyline* lines, G4Polymarker* steps, G4Polymarker* auxiliaries)
	{
		for (G4int i = 0; i < trajectory -> GetPointEntries(); i++) {  //GetPointEntries():Returns the number of tarjectory points

			G4VTrajectoryPoint* point = trajectory -> GetPoint(i);
			const std::vector<G4ThreeVector>* auxiliaryPoints = point -> GetAuxiliaryPoints();

			if (auxiliaryPoints) {
				for (size_t j = 0; j<auxiliaryPoints -> size(); j++) {
					const G4ThreeVector position((*auxiliaryPoints)[j]);
					lines -> push_back(position);
					auxiliaries -> push_back(position);
				}
			}
			const G4ThreeVector position(point -> GetPosition());
			lines -> push_back(position);
			steps -> push_back(position);
		}
	}

	void EventAction::SetColour(G4Colour& colour, G4String colourName)
	{
		if (colourName == "white") {
			colour = G4Colour(1.0, 1.0, 1.0);
		}
		else if (colourName == "gray") {
			colour = G4Colour(0.5, 0.5, 0.5);
		}
		else if (colourName == "black") {
			colour = G4Colour(0.0, 0.0, 0.0);
		}
		else if (colourName == "red") {
			colour = G4Colour(1.0, 0.0, 0.0);
		}
		else if (colourName == "green") {
			colour = G4Colour(0.0, 1.0, 0.0);
		}
		else if (colourName == "blue") {
			colour = G4Colour(0.0, 0.0, 1.0);
		}
		else if (colourName == "cyan") {
			colour = G4Colour(0.0, 1.0, 1.0);
		}
		else if (colourName == "magenta") {
			colour = G4Colour(1.0, 0.0, 0.0);
		}
		else if (colourName == "yellow") {
			colour = G4Colour(1.0, 1.0, 0.0);
		}
		else {
			// Default colour is "brack"
			colour = G4Colour(0.0, 0.0, 0.0);
		}
	}
