
/* Analog and Digital Input and Output Server for MATLAB    */
/* Giampiero Campa, Copyright 2009 The MathWorks, Inc       */

/* This file is meant to be used with the MATLAB arduino IO 
   package, however, it can be used from the IDE environment
   (or any other serial terminal) by typing commands like:
   
   0e0 : assign digital pin #4 (e) as input
   0f1 : assign digital pin #5 (f) as output
   0n1 : assign digital pin #13 (n) as output   
   
   1c  : read digital pin #2 (c) 
   1e  : read digital pin #4 (e) 
   2n0 : set digital pin #13 (d) low
   2n1 : set digital pin #13 (n) high
   2f1 : set digital pin #5 (f) high
   2f0 : set digital pin #5 (f) low
   4f2 : set digital pin #5 (f) to  50=ascii(2) over 255
   4fz : set digital pin #5 (f) to 122=ascii(z) over 255
   3a  : read analog pin #0 (a) 
   3f  : read analog pin #5 (f)                           

   99  : return script type (1 basic, 2 motor)   
                                                             */   


void setup() {
  /* Make sure all pins are put in high impedence state and 
     that their registers are set as low before doing anything.
     This puts the board in a known (and harmless) state     */ 
  int i;
  for (i=0;i<20;i++) {
    pinMode(i,INPUT);
    digitalWrite(i,0);
  }
  /* initialize serial                                       */
  Serial.begin(9600);
}


void loop() {
  
  /* variables declaration and initialization                */
  
  static int  s   = -1;    /* state                          */
  static int  pin = 13;    /* generic pin number             */

  int  val =  0;           /* generic value read from serial */
  int  agv =  0;           /* generic analog value           */
  int  dgv =  0;           /* generic digital value          */

  if (Serial.available() >0) {
    val = Serial.read();
    
    switch (s) {
		
      /* s=-1 means NOTHING RECEIVED YET ******************* */
      case -1:      

      /* calculate next state                                */
      if (val>47 && val<58) {
	/* the first received value indicates the mode       
           49 is ascii for 1, ... 90 is ascii for Z          
           s=0 is change-pin mode
           s=10 is DI;  s=20 is DO;  s=30 is AI;  s=40 is AO; 
           s=50 is servo status; s=60 is aervo attach/detach;  
           s=70 is servo read;   s=80 is servo write         
           s=90 is query script type (1 basic, 2 motor)         
                                                             */                                                             
        s=10*(val-48);
      }
      break; /* s=-1 (initial state) taken care of       */
	  
     
      /* s=0 or 1 means CHANGE PIN MODE                      */
      
      case 0:
      /* the second received value indicates the pin 
         from abs('c')=99, pin 2, to abs('t')=116, pin 19    */
      if (val>98 && val <117) {
        pin=val-97;                /* calculate pin          */
        s=1; /* next we will need to get 0 or 1 from serial  */
      } 
      else {
        s=-1; /* if value is not a pin then return to -1     */
      }
      break; /* s=0 taken care of                            */


      case 1:
      /* the third received value indicates the value 0 or 1 */ 
      if (val>47 && val <50) {
        /* set pin mode                                      */
        if (val==48) {
          pinMode(pin,INPUT);
        }
        else {
          pinMode(pin,OUTPUT);
        }
      }
      s=-1;  /* we are done with CHANGE PIN so go to -1      */
      break; /* s=1 taken care of                            */
      

      /* s=10 means DIGITAL INPUT ************************** */
      
      case 10:
      /* the second received value indicates the pin 
         from abs('c')=99, pin 2, to abs('t')=116, pin 19    */
      if (val>98 && val <117) {
        pin=val-97;                /* calculate pin          */
        dgv=digitalRead(pin);      /* perform Digital Input  */
        Serial.println(dgv);       /* send value via serial  */
      }
      s=-1;  /* we are done with DI so next state is -1      */
      break; /* s=10 taken care of                           */
      

      /* s=20 or 21 mean DIGITAL OUTPUT ******************** */
      
      case 20:
      /* the second received value indicates the pin 
         from abs('c')=99, pin 2, to abs('t')=116, pin 19    */
      if (val>98 && val <117) {
        pin=val-97;                /* calculate pin          */
        s=21; /* next we will need to get 0 or 1 from serial */
      } 
      else {
        s=-1; /* if value is not a pin then return to -1     */
      }
      break; /* s=20 taken care of                           */

      case 21:
      /* the third received value indicates the value 0 or 1 */ 
      if (val>47 && val <50) {
        dgv=val-48;                /* calculate value        */
	digitalWrite(pin,dgv);     /* perform Digital Output */
      }
      s=-1;  /* we are done with DO so next state is -1      */
      break; /* s=21 taken care of                           */

	
      /* s=30 means ANALOG INPUT *************************** */
      
      case 30:
      /* the second received value indicates the pin 
         from abs('a')=97, pin 0, to abs('f')=102, pin 6,     
         note that these are the digital pins from 14 to 19  
         located in the lower right part of the board        */
      if (val>96 && val <103) {
        pin=val-97;                /* calculate pin          */
        agv=analogRead(pin);       /* perform Analog Input   */
	Serial.println(agv);       /* send value via serial  */
      }
      s=-1;  /* we are done with AI so next state is -1      */
      break; /* s=30 taken care of                           */
	

      /* s=40 or 41 mean ANALOG OUTPUT ********************* */
      
      case 40:
      /* the second received value indicates the pin 
         from abs('c')=99, pin 2, to abs('t')=116, pin 19    */
      if (val>98 && val <117) {
        pin=val-97;                /* calculate pin          */
        s=41; /* next we will need to get value from serial  */
      }
      else {
        s=-1; /* if value is not a pin then return to -1     */
      }
      break; /* s=40 taken care of                           */


      case 41:
      /* the third received value indicates the analog value */ 
      analogWrite(pin,val);        /* perform Analog Output  */
      s=-1;  /* we are done with AO so next state is -1      */
      break; /* s=41 taken care of                           */         
      
      
      /* s=90 means Query Script Type (1 basic, 2 motor)     */
      case 90:
      if (val==57) { 
        /* if string sent is 99  send script type via serial */
        Serial.println(1);
      }
      s=-1;  /* we are done with this so next state is -1    */
      break; /* s=90 taken care of                           */

    } /* end switch on state s                               */

  } /* end if serial available                               */
  
} /* end loop statement                                      */

