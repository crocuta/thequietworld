/* Serialreading for multiple HCSR-04
Using Paul Badger's digitalSmooth for noise removing
The sketch is made for 6 ultrasonic sensors pointed on one target person at their middle to make sure he or she does not move at all.
Interferencies are avoided by looping the triggers one after the other.
For now, it only does it for two of them, but that will change very soon n_n */


//pin which triggers ultrasonic sound
const int trigPin1 = 2;
const int trigPin2 = 3;

const int filterSamples = 13;
int sensSmoothArray1 [filterSamples];
int sensSmoothArray2 [filterSamples];
 
//pin which delivers time to receive echo using pulseIn()
int echoPin1 = 9;
int echoPin2 = 8;

int rawData1, rawData2;

 
void setup() {
  // initialize serial communication
  Serial.begin(9600);
  
  

}
 
void loop()
{
  
int smoothData1, smoothData2;

  //initializing the pin states
pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
 
  //sending the signal, starting with LOW for a clean signal
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  //delayMicroseconds(20);



  rawData1 = pulseIn(echoPin1, HIGH);
  
  smoothData1 = digitalSmooth(rawData1, sensSmoothArray1);
  
  //printing the current readings to ther serial display
  Serial.print(smoothData1);
  Serial.print(", ");
  
  delayMicroseconds(100);

  //Phase2
  
    pinMode(trigPin2, OUTPUT);
    
  pinMode(echoPin2, INPUT);
  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
 
  //setting up the input pin, and receiving the duration in
  //microseconds for the sound to bounce off the object infront
  
    

  rawData2 = pulseIn(echoPin2, HIGH);
  

 
  // convert the time into a distance
  
  smoothData2 = digitalSmooth(rawData2, sensSmoothArray2);

  //Serial.print(duration2);
  Serial.print(smoothData2);
  Serial.println();


}

  int digitalSmooth(int rawIn, int *sensSmoothArray){     // "int *sensSmoothArray" passes an array to the function - the asterisk indicates the array name is a pointer
  int j, k, temp, top, bottom;
  long total;
  static int i;
 // static int raw[filterSamples];
  static int sorted[filterSamples];
  boolean done;

  i = (i + 1) % filterSamples;    // increment counter and roll over if necc. -  % (modulo operator) rolls over variable
  sensSmoothArray[i] = rawIn;                 // input new data into the oldest slot

  // Serial.print("raw = ");

  for (j=0; j<filterSamples; j++){     // transfer data array into anther array for sorting and averaging
    sorted[j] = sensSmoothArray[j];
  }

  done = 0;                // flag to know when we're done sorting              
  while(done != 1){        // simple swap sort, sorts numbers from lowest to highest
    done = 1;
    for (j = 0; j < (filterSamples - 1); j++){
      if (sorted[j] > sorted[j + 1]){     // numbers are out of order - swap
        temp = sorted[j + 1];
        sorted [j+1] =  sorted[j] ;
        sorted [j] = temp;
        done = 0;
      }
    }
  }

/*
  for (j = 0; j < (filterSamples); j++){    // print the array to debug
    Serial.print(sorted[j]); 
    Serial.print("   "); 
  }
  Serial.println();
*/

  // throw out top and bottom 15% of samples - limit to throw out at least one from top and bottom
  bottom = max(((filterSamples * 15)  / 100), 1); 
  top = min((((filterSamples * 85) / 100) + 1  ), (filterSamples - 1));   // the + 1 is to make up for asymmetry caused by integer rounding
  k = 0;
  total = 0;
  for ( j = bottom; j< top; j++){
    total += sorted[j];  // total remaining indices
    k++; 
    // Serial.print(sorted[j]); 
    // Serial.print("   "); 
  }

//  Serial.println();
//  Serial.print("average = ");
//  Serial.println(total/k);
  return total / k /100;    // divide by number of samples + 100 factor for reducing range of values, centimeters doesn't matter
}
