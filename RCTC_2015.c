/*Remote Control Terrorist Coffin, a work by Adam Kalkin*/
/*Created by Christine Wang & Aaron Ray-Crichton. */

#include <PinChangeInt.h>

//INPUTS
#define input1 A8

//LED
int demoLED = 7;

//ACTUATORS
int dpdt1 = 46;
int dpdt2 = 47;
int dpdt3 = 48;
int dpdt4 = 49;
int dpdt5 = 50;

//PYRO RELAYS
int pyro1 = 30;
int pyro2 = 32;
int pyro3 = 34;
int pyro4 = 36;
int pyro5 = 38;

//INTERRUPT HANDLERS
#define BOUNCE_DURATION 1000   // define an appropriate bounce time in ms for your switches
volatile unsigned long bounceTime1=0; // variable to hold ms count to debounce a pressed switch

//COFFIN & HELITRAY
bool bothOpenRequested=false;
bool bothCloseRequested=false;
volatile bool coffinOpenRequested=false;
bool coffinCloseRequested=false;
bool heliTrayOpenRequested=false;
bool heliTrayCloseRequested=false;
bool bothOpenSet=false;
bool bothCloseSet=false;
volatile bool coffinOpenSet=false;
bool coffinCloseSet=false;
bool heliTrayOpenSet=false;
bool heliTrayCloseSet=false;
unsigned long timeBothOpening=4294967295;
unsigned long timeBothClosing=4294967295;
unsigned long timeCoffinOpening=4294967295;
unsigned long timeCoffinClosing=4294967295;
unsigned long timeHeliTrayOpening=4294967295;
unsigned long timeHeliTrayClosing=4294967295;
bool coffinOpen=false;
bool heliTrayOpen=false;
long timeVar=25000; //25000;

//GAS CYCLINDER
bool gasOpenRequested=false;
bool gasOpenSet=false;
bool gasCloseRequested=false;
bool gasCloseSet=false;
unsigned long timeGasOpening=4294967295;
unsigned long timeGasClosing=4294967295;
long timeGasVar=5000; 

//MORTAR
bool mortarSafetyCheckRequested=false;
bool mortarRequested=false;
bool mortarSet=false;
unsigned long timeMortarFiring=4294967295;
long timeMortarVar=2000;

//ROCKETS
bool rocketsSafetyCheckRequested=false;
bool rocketsRequested=false;
bool rocket1Requested=false;
bool rocket2Requested=false;
bool rocket3Requested=false;
bool rocket4Requested=false;
bool rocket1Fired=false;
bool rocket2Fired=false;
bool rocket3Fired=false;
bool rocket4Fired=false;
unsigned long timeRock1Firing=4294967295;
unsigned long timeRock2Firing=4294967295;
unsigned long timeRock3Firing=4294967295;
unsigned long timeRock4Firing=4294967295;
long timeRocketVar=2000; 

//DEMO MODE
bool demoMode=false;
bool initialCheck=false;
bool initialCheckSet=false;
bool mode1Requested=false;
bool mode2Requested=false;
bool mode3Requested=false;
bool mode4Requested=false;
bool action1Finished=false;
bool action2Finished=false;
bool action3Finished=false;
bool action4Finished=false;
bool mode1Finished=false;
bool mode2Finished=false;
bool mode3Finished=false;
bool mode4Finished=false;
bool stopDemoRequested=false;
bool stopDemoSet=false;
int demoRuns=0;
unsigned long mode1Demoing=4294967295;
unsigned long mode2Demoing=4294967295;
unsigned long mode3Demoing=4294967295;
unsigned long mode4Demoing=4294967295;
unsigned long action1Demoing=4294967295;
unsigned long action2Demoing=4294967295;
unsigned long action3Demoing=4294967295;
unsigned long action4Demoing=4294967295;
unsigned long timeStoppingDemo=4294967295;
long timeDemoVar=120000;//120000; //2 minutes total between each action

String command;
String userInput;
volatile bool runningFunction = false;
volatile bool interruptBtnPressed = false;

//INTERRUPT TRIGGERS
void triggerInterrupt() {
	if(millis() > bounceTime1 && !runningFunction)  
	{
		resetQueue("Interrupt");		//OPEN COFFIN
		interruptBtnPressed=true;
     	bounceTime1 = millis() + BOUNCE_DURATION;  // set whatever bounce time in ms is appropriate
 	}
}

//SERIAL INPUTS TRIGGERS
void parseCommand(String com) {
	Serial.print("Recieved Command ");
	int choice=0;
	if(com == "97") //a
	{
		choice=1;
		Serial.println("A");
	}
	else if(com == "98") //b
	{
		choice=2;
		Serial.println("B");
	}
	else if(com == "99") //c
	{
		choice=3;
		Serial.println("C");
	}
	else if(com == "100") //d
	{	
		choice=4;
		Serial.println("D");
	}
	else if(com == "101") //e
	{	
		choice=5;
		Serial.println("E");
	}
	else if(com == "102") //f
	{	
		choice=6;
		Serial.println("F");
	}
	else if(com == "103") //g
	{	
		choice=7;
		Serial.println("G");
	}
	else if(com == "104") //h
	{	
		choice=8;
		Serial.println("H");
	}
	else if(com == "105") //i
	{
		choice=9;
		Serial.println("I");
	}
	else if(com == "106") //j
	{
		choice=10;
		Serial.println("J");
	}
	else if(com == "107") //k
	{
		choice=11;
		Serial.println("K");
	}
	else if(com == "108") //l
	{
		choice=12;
		Serial.println("L");
	}
	else if(com == "120") //x
	{
		choice=13;
		Serial.println("X");
	}
	else
	{
		Serial.println();
		Serial.println("****************ERROR: NOT A VALID COMMAND!! Enter A-L or X Only.****************");
		printInstructions();
	}
	

	switch(choice) 
	{
	    case 1: 							//OPEN BOTH
			if(!demoMode && !runningFunction && (!coffinOpen || !heliTrayOpen))
			{
				resetQueue("openBoth");
				bothOpenRequested=true;
				bothOpenSet=true;
				runningFunction=true;
			}
			else if(demoMode)
			{
				Serial.println("Cannot run while in DEMO Mode. Please enter 'L' to ESC");
			}
			else if(runningFunction)
			{
				Serial.println("Cannot Interrupt while Another Function is Running.");
			}
			else if(coffinOpen && heliTrayOpen)
			{
				Serial.print("Cannot run, both are already opened");
			}
	        break;
	    case 2:							//CLOSE BOTH
			if(!demoMode && !runningFunction && (coffinOpen || heliTrayOpen))
			{
				resetQueue("closeBoth");
				bothCloseRequested=true;
				bothCloseSet=true;
				runningFunction=true;
			}
			else if(demoMode)
			{
				Serial.println("Cannot run while in DEMO Mode. Please enter 'L' to ESC");
			}
			else if(runningFunction)
			{
				Serial.println("Cannot Interrupt while Another Function is Running.");
			}
			else if(!coffinOpen && !heliTrayOpen)
			{
				Serial.print("Cannot run, both are already closed");
			}
	        break;
	    case 3:	
	    	if(!demoMode && !runningFunction && !coffinOpen)
			{
			    resetQueue("openCoffin");		//OPEN COFFIN
				coffinOpenRequested=true;
				coffinOpenSet=true;
				runningFunction=true;
			}
			else if(demoMode)
			{
				Serial.println("Cannot run while in DEMO Mode. Please enter 'L' to ESC");
			}
			else if(runningFunction)
			{
				Serial.println("Cannot Interrupt while Another Function is Running.");
			}
			else if(coffinOpen)
			{
				Serial.print("Cannot run, coffin already open");
			}
	        break;
	    case 4:
	    	if(!demoMode && !runningFunction && coffinOpen)
			{
		        resetQueue("closeCoffin");		//CLOSE COFFIN
				coffinCloseRequested=true;
				coffinCloseSet=true;
				runningFunction=true;
			}
			else if(demoMode)
			{
				Serial.println("Cannot run while in DEMO Mode. Please enter 'L' to ESC");
			}
			else if(runningFunction)
			{
				Serial.println("Cannot Interrupt while Another Function is Running.");
			}
			else if(!coffinOpen)
			{
				Serial.print("Cannot run, coffin already closed");
			}
	        break;
	    case 5:
	    	if(!demoMode && !runningFunction && !heliTrayOpen)
			{
		        resetQueue("openHeli");			//OPEN HELI
				heliTrayOpenRequested=true;
				heliTrayOpenSet=true;
				runningFunction=true;
			}
			else if(demoMode)
			{
				Serial.println("Cannot run while in DEMO Mode. Please enter 'L' to ESC");
			}
			else if(runningFunction)
			{
				Serial.println("Cannot Interrupt while Another Function is Running.");
			}
			else if(heliTrayOpen)
			{
				Serial.print("Cannot run, heliTray already open");
			}
	        break;
	    case 6:
	    	if(!demoMode && !runningFunction && heliTrayOpen)
			{
		    	resetQueue("closeHeli");		//CLOSE HELI
				heliTrayCloseRequested=true;
				heliTrayCloseSet=true;					
				runningFunction=true;
			}
			else if(demoMode)
			{
				Serial.println("Cannot run while in DEMO Mode. Please enter 'L' to ESC");
			}
			else if(runningFunction)
			{
				Serial.println("Cannot Interrupt while Another Function is Running.");
			}
			else if(!heliTrayOpen)
			{
				Serial.print("Cannot run, heliTray already closed");
			}
	        break;
	    case 7:
	    	if(!demoMode && !runningFunction)
			{
		    	resetQueue("openGas");			//OPEN GAS
				gasOpenRequested=true;
				gasOpenSet=true;
				runningFunction=true;
			}
			else if(demoMode)
			{
				Serial.println("Cannot run while in DEMO Mode. Please enter 'L' to ESC");
			}
			else if(runningFunction)
			{
				Serial.println("Cannot Interrupt while Another Function is Running.");
			}
	        break;
	    case 8:
	    	if(!demoMode && !runningFunction)
			{
		    	resetQueue("closeGas");			//CLOSE GAS
				gasCloseRequested=true;
				gasCloseSet=true;
				runningFunction=true;
	        }
			else if(demoMode)
			{
				Serial.println("Cannot run while in DEMO Mode. Please enter 'L' to ESC");
			}
			else if(runningFunction)
			{
				Serial.println("Cannot Interrupt while Another Function is Running.");
			}
	        break;
	    case 9:								//STARTED MORTAR
	    	if(!heliTrayOpen && !demoMode && !runningFunction)
	    	{
		        resetQueue("clickedMortar");	
				mortarSafetyCheckRequested=true;				
				runningFunction=true;
	    	}
	    	else if(heliTrayOpen)
	    	{
	    		Serial.println("HeliTray needs to be closed to launch MORTAR");
	    	}
			else if(demoMode)
			{
				Serial.println("Cannot run while in DEMO Mode. Please enter 'L' to ESC");
			}
			else if(runningFunction)
			{
				Serial.println("Cannot Interrupt while Another Function is Running.");
			}
	        break;
	    case 10:								//STARTED ROCKET
	    	if(coffinOpen && !demoMode && !runningFunction)
	    	{
		        resetQueue("clickedRocket");	
				rocketsSafetyCheckRequested=true;
				runningFunction=true;
			}
			else if(!coffinOpen)
	    	{
	    		Serial.println("CoffinLid needs to be open to launch ROCKET");
	    	}
			else if(demoMode)
			{
				Serial.println("Cannot run while in DEMO Mode. Please enter 'L' to ESC");
			}
			else if(runningFunction)
			{
				Serial.println("Cannot Interrupt while Another Function is Running.");
			}
	        break;
	    case 11:								//STARTED DEMO MODE
	    	if(!demoMode)
			{
		        resetQueue("runDemo");	
		        demoRuns=1;	
		        Serial.print("DEMO RUN 1");
				demoMode=true;
				initialCheck=true;
				initialCheckSet=true;
				mode1Requested=true;
				runningFunction=true;
			}
			else
			{
				Serial.println("Cannot run while in DEMO Mode. Please enter 'L' to ESC");
			}
	        break;
	    case 12:								//STOPPED DEMO MODE
		    if(demoMode)
			{
		        resetQueue("stopDemo");			
				stopDemoRequested=true;
				stopDemoSet=true;
			}
			else
			{
				Serial.println("Can ONLY be run when in DEMO Mode. Please choose new command.");
			}
	        break;
	    case 13:								//EMERGENCY STOP
	        resetQueue("ESCAPE");			
			emergencyStop();
	        break;
	}
}

//FUNCTIONS
//Confirm if mortar/gas/rocket ready to launch
void confirmIfReadyToLaunch(String item){
	userInput = "";
	int func=0;
	if(item == "MORTAR")
		func=1;
	else if(item == "ROCKET")
		func=2;

	Serial.print("USER: Ready to Fire ");Serial.print(item);Serial.println("? Y or N:"); 	//ask user to confirm
	while(Serial.available()==0){} 															//wait until user responds
	int c = Serial.read();
	while(c != 13)
	{
		userInput += c;
		c = Serial.read();
		Serial.println(userInput);
	}
	if(c == 13)
	{
		switch(func) {
		    case 1: 						//MORTAR
		    	checkIfLaunchMortar(userInput);
		        break;
		    case 2:							//ROCKET
		    	checkIfLaunchRocket(userInput);
		        break;
	    }
		userInput = "";
	}

}

//Based on User's Input either Launch or Disable Mortar
void checkIfLaunchMortar(String userInput){
	Serial.println(userInput.substring(0,2));
	if(userInput.substring(0,3) == "121")
	{
		Serial.println("BOTH MORTAR CLICKED");
		timeMortarFiring=false;
		mortarRequested=true;
		mortarSet=true;
		mortarSafetyCheckRequested=false;		
	}
	else
	{
		resetQueue("User canceled Mortar");
	}
}
//Based on User's Input either Launch or Disable Rocket
void checkIfLaunchRocket(String userInput){
	if(userInput.substring(0,3) == "121")
	{
		Serial.println("BOTH ROCKETS CLICKED");
		timeRock1Firing=false;
		timeRock2Firing=false;
		timeRock3Firing=false;
		timeRock4Firing=false;
		rocketsRequested=true;
		rocket1Requested=true;
		rocketsSafetyCheckRequested=false;		
	}
	else
	{
		resetQueue("User canceled Rocket");	
	}
}

//EMERGENCY STOP
void emergencyStop(){
	digitalWrite(dpdt1, LOW);
	digitalWrite(dpdt2, LOW);
	digitalWrite(dpdt3, LOW);
	digitalWrite(dpdt4, LOW);
	digitalWrite(dpdt5, LOW);
	digitalWrite(pyro1, LOW);
	digitalWrite(pyro2, LOW);
	digitalWrite(pyro3, LOW);
	digitalWrite(pyro4, LOW);
	digitalWrite(pyro5, LOW);
	runningFunction=false;
}

//restart demo
void restartDemo(){
	demoRuns++;
	Serial.print("DEMO RUN ");
	Serial.print(demoRuns);


	Serial.println("Restarting DEMO. Hit 'L' to ESC Demo.");
	demoMode=true;
	initialCheck=true;
	initialCheckSet=true;
	mode1Requested=true;
	mode2Requested=false;
	mode3Requested=false;
	mode4Requested=false;
	action1Finished=false;
	action2Finished=false;
	action3Finished=false;
	action4Finished=false;
	mode1Finished=false;
	mode2Finished=false;
	mode3Finished=false;
	mode4Finished=false;
	mode1Demoing=4294967295;
	mode2Demoing=4294967295;
	mode3Demoing=4294967295;
	mode4Demoing=4294967295;
	action1Demoing=4294967295;
	action2Demoing=4294967295;
	action3Demoing=4294967295;
	action4Demoing=4294967295;
}	

//resets any globals set on the queue
void resetQueue(String func){
	Serial.print("RESET ");Serial.println(func);
	//COFFIN & HELITRAY
	bothOpenRequested=false;
	bothCloseRequested=false;
	coffinOpenRequested=false; 
	coffinCloseRequested=false;
	heliTrayOpenRequested=false; 
	heliTrayCloseRequested=false;
	bothOpenSet=false;
	bothCloseSet=false;
	coffinOpenSet=false;
	coffinCloseSet=false;
	heliTrayOpenSet=false;
	heliTrayCloseSet=false;
	//GAS
	gasOpenRequested=false;
	gasOpenSet=false;
	gasCloseRequested=false;
	gasCloseSet=false;
	//MORTAR
	mortarSafetyCheckRequested=false;
	mortarRequested=false;
	mortarSet=false;
	//ROCKETS
	rocketsSafetyCheckRequested=false;
	rocketsRequested=false;
	rocket1Requested=false;
	rocket2Requested=false;
	rocket3Requested=false;
	rocket4Requested=false;
	rocket1Fired=false;
	rocket2Fired=false;
	rocket3Fired=false;
	rocket4Fired=false;
	//DEMO
	demoMode=false;
	initialCheck=false;
	initialCheckSet=false;
	mode1Requested=false;
	mode2Requested=false;
	mode3Requested=false;
	mode4Requested=false;
	action1Finished=false;
	action2Finished=false;
	action3Finished=false;
	action4Finished=false;
	mode1Finished=false;
	mode2Finished=false;
	mode3Finished=false;
	mode4Finished=false;
	stopDemoRequested=false;
	stopDemoSet=false;
	//Interrupt
	interruptBtnPressed = false;
}

void printInstructions()
{
	Serial.println();
	Serial.println();
	Serial.println("*****OPERATIONS KEY*****");
	Serial.println("A: Open Both");
	Serial.println("B: Close Both");
	Serial.println("C: Open Coffin Lid");
	Serial.println("D: Close Coffin Lid");
	Serial.println("E: Open HeliTray");
	Serial.println("F: Close HeliTray");
	Serial.println("G: Open Gas");
	Serial.println("H: Close Gas");
	Serial.println("I: Launch Mortar");
	Serial.println("J: Launch Rocket");
	Serial.println("K: Start Demo");
	Serial.println("L: Stop Demo");
	Serial.println("X: EMERGENCY STOP");
	Serial.println("************************");
	Serial.println();
	Serial.println();
	runningFunction=demoMode; //clear flag to run other functions if not in demo
}

void setup() {
	Serial.begin(115200);
	
	//INPUTS
	pinMode(input1, INPUT_PULLUP);

	//LED
	pinMode(demoLED, OUTPUT);
	
	//OUTPUTS
	//Actuators  
	pinMode(dpdt1, OUTPUT);
	pinMode(dpdt2, OUTPUT);
	pinMode(dpdt3, OUTPUT);
	pinMode(dpdt4, OUTPUT);
	pinMode(dpdt5, OUTPUT);
	//Pyros
	pinMode(pyro1, OUTPUT);
	pinMode(pyro2, OUTPUT);
	pinMode(pyro3, OUTPUT);
	pinMode(pyro4, OUTPUT);
	pinMode(pyro5, OUTPUT);

	//Set all outputs to default off
	digitalWrite(dpdt1, LOW);
	digitalWrite(dpdt2, LOW);
	digitalWrite(dpdt3, LOW);
	digitalWrite(dpdt4, LOW);
	digitalWrite(dpdt5, LOW);
	digitalWrite(pyro1, LOW);
	digitalWrite(pyro2, LOW);
	digitalWrite(pyro3, LOW);
	digitalWrite(pyro4, LOW);
	digitalWrite(pyro5, LOW);

	//INTERRUPTS
	attachPinChangeInterrupt(input1, triggerInterrupt, FALLING);
}

void loop() {
	//INTERRUPT OPEN COFFIN
	if(interruptBtnPressed)
	{
		Serial.print("Start opening coffin based on interrupt");
		interruptBtnPressed=false;
		parseCommand("99");
	}
	///////////////--------------*****READ SERIAL INPUTS*****--------------\\\\\\\\\\\\\\\\ 
	if (Serial.available()>0) {
		int c = Serial.read();
		Serial.print("RECIEVED ");Serial.println(c);
		if(c == 13){ //enter
			parseCommand(command);
			command = "";
		}
		else{
			command += c;
		}
	}
	///////////////--------------*****NONDEMO MODE COMMANDS*****--------------\\\\\\\\\\\\\\\\ 
	if(!demoMode)
	{
		digitalWrite(demoLED, LOW); //signify outta demo mode

		/////-----------STOP DEMO MODE------------\\\\\
		if(stopDemoRequested && !demoMode)
		{
			if(stopDemoSet)
			{
				Serial.println("Start stopping DEMO");
				stopDemoSet=false;
				digitalWrite(dpdt1, HIGH);
				digitalWrite(dpdt3, HIGH);
				timeStoppingDemo=millis();
			}
			if(stopDemoRequested && (millis()-timeStoppingDemo) >= timeVar)
			{
				Serial.println("Time's passed STOPPED DEMO OPENED BOTH");
				digitalWrite(dpdt1, LOW);
				digitalWrite(dpdt3, LOW);
				coffinOpen=true;
				heliTrayOpen=true;
				stopDemoRequested=false;
				printInstructions();
			}
		}	
		/////-----------OPEN COFFIN && HELITRAY------------\\\\\
		if(bothOpenRequested && (!coffinOpen || !heliTrayOpen))
		{
			//Serial.println("OPEN BOTH FUNCTION");
			if(bothOpenSet && bothOpenRequested)
			{
				bothOpenSet=false;
				if (!coffinOpen && !heliTrayOpen)
				{
					Serial.println("Start opening BOTH");
					digitalWrite(dpdt1, HIGH);
					digitalWrite(dpdt3, HIGH);
				}
				else if(!coffinOpen && heliTrayOpen)
				{
					Serial.println("Only OPEN COFFIN");
					digitalWrite(dpdt1, HIGH);
				}
				else if(!heliTrayOpen && coffinOpen)
				{
					Serial.println("Only OPEN HELI");
					digitalWrite(dpdt3, HIGH);
				}
				timeBothOpening=millis();
			}
			if(bothOpenRequested && (millis()-timeBothOpening) >= timeVar)
			{
				Serial.println("Time's passed OPEN BOTH");
				digitalWrite(dpdt1, LOW);
				digitalWrite(dpdt3, LOW);
				coffinOpen=true;
				heliTrayOpen=true;
				bothOpenRequested=false;
				printInstructions();
			}
		}
		/////-----------CLOSE COFFIN && HELITRAY------------\\\\\
		if(bothCloseRequested && (coffinOpen || heliTrayOpen))
		{
			//Serial.println("CLOSE BOTH FUNCTION");
			if(bothCloseSet && bothCloseRequested)
			{
				bothCloseSet=false;
				if (coffinOpen && heliTrayOpen)
				{
					Serial.println("Start closing BOTH");
					digitalWrite(dpdt1, HIGH);
					digitalWrite(dpdt2, HIGH);
					digitalWrite(dpdt3, HIGH);
					digitalWrite(dpdt4, HIGH);
				}
				else if(coffinOpen && !heliTrayOpen)
				{
					Serial.println("Only CLOSE COFFIN");
					digitalWrite(dpdt1, HIGH);
					digitalWrite(dpdt2, HIGH);
				}
				else if(heliTrayOpen && !coffinOpen)
				{
					Serial.println("Only CLOSE HELI");
					digitalWrite(dpdt3, HIGH);
					digitalWrite(dpdt4, HIGH);
				}
				timeBothClosing = millis();
			}
			if(bothCloseRequested && (millis()-timeBothClosing) >= timeVar)
			{
				Serial.println("Time's passed CLOSED BOTH");
				digitalWrite(dpdt1, LOW);
				digitalWrite(dpdt2, LOW);
				digitalWrite(dpdt3, LOW);
				digitalWrite(dpdt4, LOW);
				coffinOpen=false;
				heliTrayOpen=false;
				bothCloseRequested=false;
				printInstructions();
			}
		}

		/////-----------OPEN COFFIN------------\\\\\
		if(coffinOpenRequested && !coffinOpen)
		{
			//Serial.println("OPEN COFFIN");
			if(coffinOpenSet)
			{
				Serial.println("Start opening COFFIN");
				coffinOpenSet=false;
				digitalWrite(dpdt1, HIGH);
				timeCoffinOpening=millis();
			}
			if(coffinOpenRequested && (millis()-timeCoffinOpening) >= timeVar)
			{
				Serial.println("Time's passed OPEN COFFIN");
				digitalWrite(dpdt1, LOW);
				coffinOpen=true;
				coffinOpenRequested=false;
				printInstructions();
			}
		}
		/////-----------CLOSE COFFIN------------\\\\\
		if(coffinCloseRequested && coffinOpen)
		{
			//Serial.println("CLOSE COFFIN");
			if(coffinCloseSet)
			{
				Serial.println("Start closing COFFIN");
				coffinCloseSet=false;
				digitalWrite(dpdt1, HIGH);
				digitalWrite(dpdt2, HIGH);
				timeCoffinClosing=millis();
			}
			if(coffinCloseRequested && (millis()-timeCoffinClosing) >= timeVar)
			{
				Serial.println("Time's passed CLOSE COFFIN");
				digitalWrite(dpdt1, LOW);
				digitalWrite(dpdt2, LOW);
				coffinOpen=false;
				coffinCloseRequested=false;
				printInstructions();
			}
		}

		/////-----------OPEN HELITRAY------------\\\\\
		if(heliTrayOpenRequested && !heliTrayOpen)
		{
			//Serial.println("OPEN HELI");
			if(heliTrayOpenSet)
			{
				Serial.println("Start opening HELITRAY");
				heliTrayOpenSet=false;
				digitalWrite(dpdt3, HIGH);
				timeHeliTrayOpening=millis();
			}
			if(heliTrayOpenRequested && (millis()-timeHeliTrayOpening) >= timeVar)
			{
				Serial.println("Time's passed OPEN HELITRAY");
				digitalWrite(dpdt3, LOW);
				heliTrayOpen=true;
				heliTrayOpenRequested=false;
				printInstructions();
			}
		}
		/////-----------CLOSE HELITRAY------------\\\\\
		if(heliTrayCloseRequested && heliTrayOpen)
		{
			//Serial.println("CLOSE HELI");
			if(heliTrayCloseSet)
			{
				Serial.println("Start closing HELITRAY");
				heliTrayCloseSet=false;
				digitalWrite(dpdt3, HIGH);
				digitalWrite(dpdt4, HIGH);
				timeHeliTrayClosing=millis();
			}
			if(heliTrayCloseRequested && (millis()-timeHeliTrayClosing) >= timeVar)
			{
				Serial.println("Time's passed CLOSE HELI");
				digitalWrite(dpdt3, LOW);
				digitalWrite(dpdt4, LOW);
				heliTrayOpen=false;
				heliTrayCloseRequested=false;
				printInstructions();
			}
		}

		/////-----------OPEN GAS------------\\\\\
		if(gasOpenRequested)
		{
			if(gasOpenSet)					
			{
				Serial.println("Start opening GAS");
				gasOpenSet=false;
				digitalWrite(dpdt5, HIGH);
				timeGasOpening=millis();
			}
			if(gasOpenRequested && (millis()-timeGasOpening) >= timeGasVar)
			{
				Serial.println("Time's passed stop OPENing GAS");
				gasOpenRequested=false;
				printInstructions();
			}
		}
		/////-----------CLOSE GAS------------\\\\\
		if(gasCloseRequested)
		{
			if(gasCloseSet)						
			{
				Serial.println("Start closing GAS");
				gasCloseSet=false;
				digitalWrite(dpdt5, LOW);
				timeGasClosing=millis();
			}
			if(gasCloseRequested && (millis()-timeGasClosing) >= timeGasVar)
			{
				Serial.println("Time's passed stop CLOSing GAS");
				gasCloseRequested=false;
				printInstructions();
			}
		}
		
		/////-----------MORTAR------------\\\\\
		if(mortarSafetyCheckRequested){
			confirmIfReadyToLaunch("MORTAR");
		}	
		if(mortarRequested && !heliTrayOpen)
		{
			if(mortarSet)
			{
				mortarSet=false;
				digitalWrite(pyro5, HIGH);
				timeMortarFiring=millis();
				Serial.print("Start launching MORTAR ");Serial.println(timeMortarFiring);
			}
			if(mortarRequested && (millis()-timeMortarFiring) >= timeMortarVar)
			{
				Serial.println("Time's passed LAUNCHED MORTAR");
				digitalWrite(pyro5, LOW);
				mortarRequested=false;
				printInstructions();
			}
		}

		/////-----------LAUNCH ROCKETS------------\\\\\
		if(rocketsSafetyCheckRequested){
			confirmIfReadyToLaunch("ROCKET");
		}
		if(rocketsRequested && coffinOpen)
		{
			if(rocket1Requested && !rocket1Fired)
			{
				Serial.println("Launch 1");
				rocket1Requested=false; //make sure loop doesnt enter this again after time has been set
				digitalWrite(pyro1, HIGH);
				timeRock1Firing=millis();
			}
			if(!rocket1Fired && (millis()-timeRock1Firing) >= timeRocketVar)
			{
				Serial.println("Time's passed LAUNCHED 1");
				digitalWrite(pyro1, LOW);
				rocket1Fired=true;
				rocket2Requested=true;
			}
			if(rocket1Fired && rocket2Requested && !rocket2Fired)
			{
				Serial.println("Launch 2");
				rocket2Requested=false; //make sure loop doesnt enter this again after time has been set
				digitalWrite(pyro2, HIGH);
				timeRock2Firing=millis();
			}
			if(rocket1Fired && !rocket2Fired && (millis()-timeRock2Firing) >= timeRocketVar)
			{
				Serial.println("Time's passed LAUNCHED 2");
				digitalWrite(pyro2, LOW);
				rocket2Fired=true;
				rocket3Requested=true;
			}
			if(rocket2Fired && rocket3Requested && !rocket3Fired)
			{
				Serial.println("Launch 3");
				rocket3Requested=false; //make sure loop doesnt enter this again after time has been set
				digitalWrite(pyro3, HIGH);
				timeRock3Firing=millis();
			}
			if(rocket2Fired && !rocket3Fired && (millis()-timeRock3Firing) >= timeRocketVar)
			{
				Serial.println("Time's passed LAUNCHED 3");
				digitalWrite(pyro3, LOW);
				rocket3Fired=true;
				rocket4Requested=true;
			}
			if(rocket3Fired && rocket4Requested && !rocket4Fired)
			{
				Serial.println("Launch 4");
				rocket4Requested=false; //make sure loop doesnt enter this again after time has been set
				digitalWrite(pyro4, HIGH);
				timeRock4Firing=millis();
			}
			if(rocket3Fired && !rocket4Fired && (millis()-timeRock4Firing) >= timeRocketVar)
			{
				Serial.println("Time's passed LAUNCHED 4");
				digitalWrite(pyro4, LOW);
				rocket4Fired=true;
				rocketsRequested=false;
				printInstructions();
				resetQueue("FINISHED ROCKET");
			}
		}
	}

	///////////////--------------*****DEMO MODE*****--------------\\\\\\\\\\\\\\\\ 
	if(demoMode)
	{
		digitalWrite(demoLED, HIGH);
		if(!coffinOpen && !heliTrayOpen) //Both are already closed, skip initial check
		{
			initialCheck=false;
		}
		else if(initialCheck && (coffinOpen || heliTrayOpen))
		{
			if(initialCheckSet && initialCheck)
			{
				initialCheckSet=false;
				if(coffinOpen && heliTrayOpen)
				{
					Serial.println("Start closing BOTH");
					digitalWrite(dpdt1, HIGH);
					digitalWrite(dpdt2, HIGH);
					digitalWrite(dpdt3, HIGH);
					digitalWrite(dpdt4, HIGH);
				}
				else if(coffinOpen && !heliTrayOpen)
				{
					Serial.println("Only CLOSE COFFIN");
					digitalWrite(dpdt1, HIGH);
					digitalWrite(dpdt2, HIGH);
				}
				else if(heliTrayOpen && !coffinOpen)
				{
					Serial.println("Only CLOSE HELI");
					digitalWrite(dpdt3, HIGH);
					digitalWrite(dpdt4, HIGH);
				}
				timeBothClosing=millis();
			}
			if(initialCheck && (millis()-timeBothClosing) >= timeVar)
			{
				Serial.println("Time's passed CLOSED BOTH START DEMO");
				digitalWrite(dpdt1, LOW);
				digitalWrite(dpdt2, LOW);
				digitalWrite(dpdt3, LOW);
				digitalWrite(dpdt4, LOW);
				coffinOpen=false;
				heliTrayOpen=false;
				initialCheck=false;
			}
		}

		if(!initialCheck)
		{
			if(mode1Requested && !action1Finished)
			{
				Serial.println("1stMODE OPEN COFFIN");
				digitalWrite(dpdt1, HIGH);
				mode1Requested=false;
				action1Demoing=millis();
			}
			if(!action1Finished && (millis()-action1Demoing) >= timeVar)
			{
				Serial.println("Time's passed OPEN COFFIN IN DEMO");
				digitalWrite(dpdt1, LOW);
				coffinOpen=true;
				action1Finished=true;
				mode1Demoing=millis();
			}
			if(action1Finished && !mode1Finished && !mode2Requested && (millis()-mode1Demoing) >= timeDemoVar)
			{
				Serial.println("Time's passed END MODE 1");
				mode1Finished=true;
				mode2Requested=true;
			}
			if(mode2Requested && !action2Finished)
			{
				Serial.println("2ndMODE OPEN HELI");
				digitalWrite(dpdt3, HIGH);
				mode2Requested=false;
				action2Demoing=millis();
			}
			if(action1Finished && mode1Finished && !action2Finished && (millis()-action2Demoing) >= timeVar)
			{
				Serial.println("Time's passed OPEN HELI IN DEMO");
				digitalWrite(dpdt3, LOW);
				heliTrayOpen=true;
				action2Finished=true;
				mode2Demoing=millis();
			}
			if(action1Finished && mode1Finished && action2Finished && !mode2Finished && !mode3Requested && (millis()-mode2Demoing) >= timeDemoVar)
			{
				Serial.println("Time's passed END MODE 2");
				mode2Finished=true;
				mode3Requested=true;
			}
			if(mode3Requested && !action3Finished)
			{
				Serial.println("3rdMODE CLOSE COFFIN");
				digitalWrite(dpdt1, HIGH);
				digitalWrite(dpdt2, HIGH);
				mode3Requested=false;
				action3Demoing=millis();
			}
			if(action1Finished && mode1Finished && action2Finished && mode2Finished && !action3Finished && (millis()-action3Demoing) >= timeVar)
			{
				Serial.println("Time's passed CLOSE COFFIN IN DEMO");
				digitalWrite(dpdt1, LOW);
				digitalWrite(dpdt2, LOW);
				coffinOpen=false;
				action3Finished=true;
				mode3Demoing=millis();
			}
			if(action1Finished && mode1Finished && action2Finished && mode2Finished && action3Finished && !mode3Finished && !mode4Requested && (millis()-mode3Demoing) >= timeDemoVar)
			{
				Serial.println("Time's passed END MODE 3");
				mode3Finished=true;
				mode4Requested=true;
			}

			if(mode4Requested && !action4Finished)
			{
				Serial.println("4thMODE CLOSE HELI");
				digitalWrite(dpdt3, HIGH);
				digitalWrite(dpdt4, HIGH);
				mode4Requested=false;
				action4Demoing=millis();
			}
			if(action1Finished && mode1Finished && action2Finished && mode2Finished && action3Finished && mode3Finished && !action4Finished && (millis()-action4Demoing) >= timeVar)
			{
				Serial.println("Time's passed CLOSE HELI IN DEMO");
				digitalWrite(dpdt3, LOW);
				digitalWrite(dpdt4, LOW);
				heliTrayOpen=false;
				action4Finished=true;
				mode4Demoing=millis();
			}
			if(action1Finished && mode1Finished && action2Finished && mode2Finished && action3Finished && mode3Finished && action4Finished && !mode4Finished && (millis()-mode4Demoing) >= timeDemoVar)
			{
				Serial.println("Time's passed END MODE 4");
				mode4Finished=true;
				printInstructions();
				restartDemo();
			}
		}
	}
}//loop

