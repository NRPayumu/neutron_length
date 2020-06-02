// source: geant4.9.6.p01/examples/novice/N04/include/ExN04EventAction.hh
// 2019/11/18
//
#ifndef EventAction_h
#define EventAction_h 1

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4UserEventAction.hh"
#include "G4VTrajectory.hh"
#include "G4Polyline.hh"
#include "G4Polymarker.hh"
#include "G4Colour.hh"

#include <vector>

#include "globals.hh"

class EventAction : public G4UserEventAction
{
public:
	EventAction(RunAction*, PrimaryGeneratorAction*);
	~EventAction();

	void BeginOfEventAction(const G4Event*);
	void EndOfEventAction  (const G4Event*);

	void TraceTrajectory(G4VTrajectory*, G4Polyline*, G4Polymarker*, G4Polymarker*);
	void SetColour(G4Colour&, G4String);

	//
	// original
	//
	inline void  SetEventID(G4int EventID) {fEventID = EventID;};
	inline G4int GetEventID()              {return fEventID;};

	void SetEne(G4double e, G4double x, G4double y, G4double z);
	void RecCap(G4int  tID, G4double x, G4double y, G4double z, G4double t);
	void AddEne(G4double e);
	void AddLos(G4double e);

private:
	RunAction*              fRunAction;
	PrimaryGeneratorAction* fPrimaryGeneratorAction;
	G4int                   fEventID;

	G4double  fEnergyIn    ;
	G4double  fPositionInX ;
	G4double  fPositionInY ;
	G4double  fPositionInZ ;
	G4int     fCaptureID   ;
	G4double  fPositionCapX;
	G4double  fPositionCapY;
	G4double  fPositionCapZ;
	G4double  fTimeCap     ;
	G4double  fEnergyAbs   ;
	G4double  fEnergyLos   ;
};

inline void EventAction::SetEne(G4double e, G4double x, G4double y, G4double z){
	fEnergyIn    = e;
	fPositionInX = x;
	fPositionInY = y;
	fPositionInZ = z;
}

inline void EventAction::RecCap(G4int  tID, G4double x, G4double y, G4double z,G4double t){
	fCaptureID     = tID;
	fPositionCapX  =   x;
	fPositionCapY  =   y;
	fPositionCapZ  =   z;
	fTimeCap       =   t;
}

inline void EventAction::AddEne(G4double e){
	fEnergyAbs += e;
}

inline void EventAction::AddLos(G4double e){
	fEnergyLos += e;
}

#endif
