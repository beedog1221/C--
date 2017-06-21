/* Specification:
Braylin Jones - Mcmullen  
Lab 5 Exercise 1
This program does plasma pistol  */

#include <iostream>
#include <ctime>

using namespace std;

class PlasmaPistol 
{
	private:
		int ammo;
		int rateOfFire;
		int destructivePower;
	public:
		bool safetyOn;
		int maxAmmo;
		void pressTrigger(void);
		void load(int nmbrOfBolts);
		void setDestructivePower(int powerSetting);
		int showDestructivePower(void);
		void setRateOfFire(int boltsPerTriggerPress);
		int showRateOfFire(void);
		int ammoRemaining(void);
		PlasmaPistol();
		PlasmaPistol(int, int);
};

int main()
{
	PlasmaPistol plasmaPistol1;
	PlasmaPistol plasmaPistol2(8, 7);
	cout << "**************************************************" << endl;
	cout << "plasma pistol 1 " << endl;
	cout << "destructive power: " << plasmaPistol1.showDestructivePower() <<endl;
	cout << "number of bolts left: " << plasmaPistol1.ammoRemaining() << endl;
	cout << "rate of fire: " << plasmaPistol1.showRateOfFire() << endl;
	cout << endl << endl;
	cout << "FIRE!!" << endl;
	plasmaPistol1.pressTrigger();
	cout << "FIRE!!" << endl;
	plasmaPistol1.pressTrigger();
	cout << "FIRE!!" << endl;
	plasmaPistol1.pressTrigger();
	cout << endl << endl;
	cout << "number of bolts left : " << plasmaPistol1.ammoRemaining() << endl;
	cout << "reload" << endl;
	plasmaPistol1.load(1);
	cout << "number of bolts left : " << plasmaPistol1.ammoRemaining() << endl;
	cout << "**************************************************" << endl;
	cout << endl << endl;

	cout << "**************************************************" << endl;
	cout << "plasma pistol 2 " << endl;
	cout << "destructive power: " << plasmaPistol2.showDestructivePower() <<endl;
	cout << "number of bolts left: " << plasmaPistol2.ammoRemaining() << endl;
	cout << "rate of fire: " << plasmaPistol2.showRateOfFire() << endl;
	cout << endl << endl;
	cout << "FIRE!!" << endl;
	plasmaPistol2.pressTrigger();
	cout << "FIRE!!" << endl;
	plasmaPistol2.pressTrigger();
	cout << "FIRE!!" << endl;
	plasmaPistol2.pressTrigger();
	cout << endl << endl;
	cout << "number of bolts left : " << plasmaPistol2.ammoRemaining() << endl;
	cout << "reload" << endl;
	plasmaPistol2.load(1);
	cout << "number of bolts left : " << plasmaPistol2.ammoRemaining() << endl;
	cout << "**************************************************" << endl;
	cout << endl << endl;

	return 0;
}
PlasmaPistol::PlasmaPistol()
{
		 ammo = 30;
		 rateOfFire = 7;
		 destructivePower = 5;
}
PlasmaPistol::PlasmaPistol(int rof , int dp)
{
		ammo = 30;
		rateOfFire = rof;
		destructivePower = dp;
}
void PlasmaPistol::pressTrigger(void)

{
	if(safetyOn == false)
		ammo -= rateOfFire;
}
void PlasmaPistol::load(int nmbrOfBolts)

{
	ammo += nmbrOfBolts;
		if(ammo>50)
			ammo = 50;

}
void PlasmaPistol::setDestructivePower(int powerSetting)
{
	destructivePower = powerSetting;
		if(destructivePower > 10)
			destructivePower = 10;
		else if (destructivePower < 1)
			destructivePower = 1;
}
int PlasmaPistol::showDestructivePower(void)
{
	return destructivePower;
}
void PlasmaPistol::setRateOfFire(int boltsPerTriggerPress)
{
	rateOfFire = boltsPerTriggerPress;
	if( rateOfFire > 10)
		rateOfFire = 10;
	else if (rateOfFire < 1)
		rateOfFire = 1;

}
int PlasmaPistol::showRateOfFire(void)
{
	return rateOfFire;
}
int PlasmaPistol::ammoRemaining(void)
{
	return ammo;
}

