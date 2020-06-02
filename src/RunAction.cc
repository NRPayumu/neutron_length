// 2019/11/18
//
#include "G4Run.hh"
#include "G4RunManager.hh"

#include "RunAction.hh"
#include "EventAction.hh"

#include <fstream>
#include <iostream>
#include <sstream>

#include "g4root.hh"

#include <time.h>

RunAction::RunAction()
{
	auto analysisManager = G4AnalysisManager::Instance();
	//G4String fileName = "Aroot";
	//analysisManager->OpenFile(fileName);
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetNtupleMerging(true);
	analysisManager->CreateNtuple("Tree", "Tree");
	analysisManager->CreateNtupleDColumn("SEne");
	analysisManager->CreateNtupleDColumn("SposX");
	analysisManager->CreateNtupleDColumn("SposY");
	analysisManager->CreateNtupleDColumn("SposZ");
	analysisManager->CreateNtupleDColumn("CapID");
	analysisManager->CreateNtupleDColumn("CposX");
	analysisManager->CreateNtupleDColumn("CposY");
	analysisManager->CreateNtupleDColumn("CposZ");
	analysisManager->CreateNtupleDColumn("CTime");
	analysisManager->CreateNtupleDColumn("Eabs");
	analysisManager->CreateNtupleDColumn("Elos");
	analysisManager->FinishNtuple();
}
RunAction::RunAction(G4String fileName)
{
	auto analysisManager = G4AnalysisManager::Instance();
	//analysisManager->OpenFile(fileName);
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetNtupleMerging(true);
	analysisManager->CreateNtuple("Tree", "Tree");
	analysisManager->CreateNtupleDColumn("SEne");
	analysisManager->CreateNtupleDColumn("SposX");
	analysisManager->CreateNtupleDColumn("SposY");
	analysisManager->CreateNtupleDColumn("SposZ");
	analysisManager->CreateNtupleDColumn("CapID");
	analysisManager->CreateNtupleDColumn("CposX");
	analysisManager->CreateNtupleDColumn("CposY");
	analysisManager->CreateNtupleDColumn("CposZ");
	analysisManager->CreateNtupleDColumn("CTime");
	analysisManager->CreateNtupleDColumn("Eabs");
	analysisManager->CreateNtupleDColumn("Elos");
	analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
	delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
	G4cout << "### Run " << aRun -> GetRunID() << " start." << G4endl;
	auto analysisManager = G4AnalysisManager::Instance();

	G4String fileName = "Aroot";

	struct tm tm;
	time_t t = time(NULL);
	localtime_r(&t, &tm);
	fileName = std::to_string( (tm.tm_year + 1900)*1000000 +
														 (tm.tm_mon+1)*10000 + (tm.tm_mday)*100 +(tm.tm_hour) );
	analysisManager->OpenFile(fileName);
}

void RunAction::EndOfRunAction(const G4Run*)
{
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->Write();
	analysisManager->CloseFile();
	//  G4double eventNo = aRun -> GetNumberOfEvent();
}

