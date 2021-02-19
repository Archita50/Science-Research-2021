#include <math.h> 

#define CLK     2
#define SI      3
#define VOUT    7   //pixel intensity in the analog channel - A7 is the input.
//#define INTVAL  1   //integration time adjust in the analog channel 1.
#define PIXELS  64

int count = 0;
//double lightAbsorbance1[15][64];
//void calculateLightPowerAndIntensity(int value);

void setup(){
  pinMode(CLK, OUTPUT);
  pinMode(SI, OUTPUT);
  digitalWrite(CLK, LOW);
  digitalWrite(SI, LOW);

//  Serial.begin(9600);
  Serial.begin(115200);
  Serial.flush();
}

void loop(){

  //light absorbance array
  double lightAbsorbance[15];
  double lightAbsorbance1[15][64];
  
  //absorbance spectrum of Hemoglobin and Bilirubin
  double absorbanceSpectrumHbBili[64];
  
  Serial.print(">");                //Mark the start of a new frame
  Serial.println(536);   //Send Integration Period in microseconds.
//  delay(8);     // used with 9600 bauds - arrived after some testings
  delay(2);     // used with 115200 bauds - arrived after some testing

  readPixel(0);                     //Pixel 0 reading will be discarded. 
  delayMicroseconds(536);

  for(int i=0;i<PIXELS;i++){
    lightAbsorbance1[count][i] = readPixel(i);
    delayMicroseconds(536);    //Delay added to the integration period.
  }
  count++;

  if (count == 14) {
  //order the light absorbance array from least to greatest!
    for (int pix=0; pix<63;pix++) {
        for (int i=0;i<14;i++) {
          lightAbsorbance[i] = lightAbsorbance1[i][pix];
    }
       //sorting
       for (int c = 0; c <= 15; c++){
        for (int j = c; j > 0; j--){
            if (lightAbsorbance[j-1]>lightAbsorbance[j]){
                //swap! use the temp variable
                double temp = lightAbsorbance[j];
                lightAbsorbance[j] = lightAbsorbance[j-1];
                lightAbsorbance[j-1] = temp;
            }
        }
    }
     //so now the absorbance array is sorted. now we want to subtract, and instantiate:
      double absSpectrumHbBilli = lightAbsorbance[15] - lightAbsorbance[0];
      absorbanceSpectrumHbBili[pix] = absSpectrumHbBilli;
     //send this absorbanceSpectrumHbBili to processing and have processing graph it.
      //Serial.println(absSpectrumHbBilli);
      //Serial.println(lightAbsorbance[15], 9);
  }
   /// prompt user to turn on blue light
   // wait for some time.
   // read the pixel 1 again
   // calculate all those values again.
   // a= e* concentration * path length.
   // a* 
  } 
}

double calculateLightPowerAndIntensity(int value, int pixelPosition) {
  
//    //incident light intensity array
//    double incidentLight[64] = {268, 268, 261, 261, 266, 260, 
//    256, 258, 248, 245, 248, 243, 233, 235, 231, 227, 224, 231, 
//    226, 223, 225, 230, 225, 224, 229, 228, 222, 221, 224, 218, 
//    212, 216, 212, 207, 204, 208, 204, 202, 204, 209, 204, 206, 
//    213, 213, 196, 208, 216, 212, 208, 213, 209, 204, 203, 207, 
//    201, 197, 200, 201, 192, 198, 200, 201, 199};

        double incidentLight[64] = {184.827, 184.827, 71.723, 71.723, 71.723, 71.723, 
    71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 
    71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 
    71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 
    71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 
    71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723, 71.723};
  
    //array for the responsivity
    double  responsivity[64] = {1.45, 1.973, 1.500, 1.359, 2.395, 
    2.3956, 3.350, 2.450, 2.3953, 4.2940, 3.639, 2.396, 1.395, 
    2.4960, 2.395, 3.4964, 4.5063, 5.0593, 6.0329, 4.68492, 5.953, 
    4.6802, 3.2469, 6.3059, 7.3052, 2.4603, 4.305, 2.396, 
    3.405, 4.205, 1.3063, 2.305, 2.4023, 3.395, 3.5202, 
    3.663, 3.7013, 3.80358, 3.90953, 4.0024, 4.1029, 4.2029, 
    4.3035, 4.40295, 4.5024, 4.6039, 4.70395, 4.8059, 4.9092, 
    5.00395, 5.01391, 5.242, 3.064, 3.406, 6.305, 5.6052, 5.3857, 
    5.298, 5.9589, 6.9240, 2.406, 6.3952, 3.4604, 4.357};

    //array for the lightPower
    double lightPower = 0.0;
  
    //array for the lightIntensity 
    double lightIntensity = 0.0;
    double lightAbsorbance1 = 0.0;

    //spectral responsivity to light power:

    lightPower = value / responsivity[pixelPosition -1]; //pixel at value i. 
    
    //light power to light intensity 
    
    //declare and initialize variables for the triangle:
    double a = 1.461; //in centimeters
    double b = 0.0; //this value will be calculated in the for loop
    double hypotenuse = 0.0; //distance from light to pixel.
        
    //calculate the b value:
    b = 0.007*(pixelPosition-1) + 0.0055*(pixelPosition) + 0.6376;
    
    //calculate the hypotenuse:
    hypotenuse = sqrt( (a*a) + (b*b) );
    
    //use this hypotenuse value to calculate light intensity:
    lightIntensity = lightPower / (4*3.14*(hypotenuse*hypotenuse));
    
    lightAbsorbance1 = log10(incidentLight[pixelPosition] / lightIntensity);
    
   return lightAbsorbance1;
}

//------------------ Send the intensity of the pixel serially -----------------
double readPixel(int pixel){
  int value; //pixel intensity value.
  double lightAbsorbance1 = 0.0;
  
  digitalWrite(CLK, LOW);
  digitalWrite(SI, HIGH);        
  digitalWrite(CLK, HIGH);
  digitalWrite(SI, LOW);
 

  for(int i=0;i<pixel; i++){    //Clock pulses before pixel reading
    digitalWrite(CLK, LOW);
    digitalWrite(CLK, HIGH);    //Select next pixel and reset integrator of actual pixel.
  }
  value = analogRead(VOUT);
    Serial.println(value);
    lightAbsorbance1 = calculateLightPowerAndIntensity(value, pixel); 
    //Serial.println(lightAbsorbance1);


//  delay(8);       //used with 9600 bauds
  delay(1);         //used with 115200 bauds
  for(int i=0;i<=(PIXELS-pixel); i++){    //Clock pulses after pixel reading.
    digitalWrite(CLK, LOW);
    digitalWrite(CLK, HIGH);    //Select next pixel and reset integrator of actual pixel.
  }
  return lightAbsorbance1;
}
