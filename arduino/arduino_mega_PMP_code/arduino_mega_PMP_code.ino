//This code was written to be easy to understand.
//Code efficiency was not considered.
//Modify this code as you see fit.
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the EZO-PMP Embedded Dosing Pump.
//This code was written in the Arduino 1.6.11 IDE
//An Arduino Mega 2560 was used to test this code.

//Commands are issued as strings ending in carriage return.
// Here are some useful ones
// "c,*" continuous mode, report once per second
// "c,1" continuous mode, report while pumping
// "c,0" continuous mode, do not report
//
// "x" stop pump
// "p" pause/unpause pumping
//
// "clear" clear amount pumped
//
// "dc,10,*" constant flow dc,[ml/min],[mins or *]
//
// "name,membrane01"

String inputstring = "";                              //a string to hold incoming data from the PC
String devicestring = "";                             //a string to hold the data from the Atlas Scientific product
boolean device_string_complete = false;               //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product
float ml;                                             //used to hold a floating point number that is the volume
float rate = 5.5;

void setup() {                                        //set up the hardware
  Serial.begin(115200);                                 //set baud rate for the hardware serial port_0 to 9600
  Serial3.begin(115200);                                //set baud rate for software serial port_3 to 9600
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  devicestring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
  delay(1);

  Serial.print("Powering up! Checking status\n");
  Serial3.print("status\r");
  delay(1);
  
  Serial.print("Clearing volume pumped.\n");
  Serial3.print("clear\r");
  delay(1);
  
  Serial.print("Continuous Mode without Reporting.\n");
  Serial3.print("c,0\r");
  delay(1);
  
  Serial.print("Setting pump rate to ");
  Serial.print(rate);
  Serial.print(" ml/min \n");
  Serial3.print("dc,");
  Serial3.print(rate);
  Serial3.print(",*\r");
  delay(1);

}

void setPumpRate(float _rate) {
  Serial3.print("dc,");
  Serial3.print(_rate);
  Serial3.print(",*\r");
}

void pausePump() {
  Serial3.print("p\r");
}

void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  device_string_complete = true;                      //set the flag used to tell if we have received a completed string from the PC
}


void serialEvent3() {                                 //if the hardware serial port_3 receives a char
  devicestring = Serial3.readStringUntil(13);         //read the string until we see a <CR>
  sensor_string_complete = true;                      //set the flag used to tell if we have received a completed string from the PC
}


void loop() {                                         //here we go...


  if (device_string_complete == true) {                //if a string from the PC has been received in its entirety
    Serial3.print(inputstring);                       //send that string to the Atlas Scientific product
    Serial3.print('\r');                              //add a <CR> to the end of the string
    inputstring = "";                                 //clear the string
    device_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the PC
  }


  if (sensor_string_complete == true) {                          //if a string from the Atlas Scientific product has been received in its entirety
    Serial.println(devicestring);                                //send that string to the PC's serial monitor
    if (isdigit(devicestring[0]) || devicestring[0] == '-') {    //if the first character in the string is a digit or a "-" sign
      ml = devicestring.toFloat();                               //convert the string to a floating point number so it can be evaluated by the Arduino
    }                                                               //in this code we do not use "ml", But if you need to evaluate the ml as a float, this is how it’s done
    devicestring = "";                                             //clear the string:
    sensor_string_complete = false;                                //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }

}
