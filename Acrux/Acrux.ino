#include <math.h>
#define PI 3.141592653589793


int t_start = 0, t_stop = 0;

// Acrux
int Y = 2023; 
int M = 12; 
int D = 19;
int Hora = 20;
int Min = 00;
int Seg = 00;
//  AR = 12 26 35.89522
// Dec = -63 5 56.7343
float AR = 12.44330422;
float Declinacao = -63.09909286;
//float AR = 0.71231;
//float Declinacao = 41.269028;
float longitude = -51.62455420400137;
float latitude = -29.96412156195;
float A = 0;
float B = 0;
float m = 3.075;
float n = 20.04;                     
float nn = 1.336;

void setup() {

Serial.begin(9600);

}

void loop() {


   delay(100);
      t_start=micros();
  
  if (M==1|M==2) {
    M=M+12;                                                                 
    Y=Y-1;
} else {                          
    M=M;                                                                    
    Y+Y;
}    

if (Y > 1582){
    A = floor(Y/100);                                                           
    B = 2 - A + floor(A/4); 
} else if (Y == 1582) {
    if (M >= 10) {
        A = floor(Y/100);                                                           
        B = 2 - A + floor(A/4);
    } else if (M == 10) {
            if (D >= 15) {
                A = floor(Y/100);                                                           
                B = 2 - A + floor(A/4);
            } else {
                B = 0;
            }
   

    } 
}


// Correção das coordenadas

    float ar2000 = AR/15;
    //float d2000 = abs(Declinacao);
    //float Nd = 0;
        
    
    float delta_ano = Y-2000+M/12+D/30;
   

    float ar2000_rad = (AR*15) * PI / 180.0;
    float d2000_rad = Declinacao * PI / 180.0;
    
    float Narr = (m + nn * sin(ar2000_rad) * tan(d2000_rad)) * delta_ano;
    float Ndd = n * cos(ar2000_rad) * delta_ano;
    
    float Nar = (AR + (Narr/3600));
    float Nd = Declinacao + (Ndd/3600);

float JD = floor(365.25*(Y+4716))+floor(30.6001*(M+1)) + D + B - 1524.5;


// Século Juliano

float T = (JD - 2451545)/36525;


// Tempo Sideral Greenwich Zero Hora

float TSG_zero_hora = 100.46061837 + 36000.770053608*T + 0.000387933*pow(T,2) - (pow(T,3))/38710000;  
float TSG_zero_hora_frac = TSG_zero_hora/360;


// Tempo Sideral Greenwich Qualquer Instante

float UT = (Hora+3 + Min/60 + Seg/3600);
float TSG_qualq_hora = ((UT)/24)*1.00273790935 + TSG_zero_hora_frac;
float TSG_q_h_int = floor(TSG_qualq_hora);
float TSG_qualq_hora_frac = (TSG_qualq_hora - TSG_q_h_int)*24;


// Tempo Sideral Local

float LST = TSG_qualq_hora_frac + longitude/15;


// Ângulo Horário


float Hgraus = (LST-Nar)*15;
float H = (Hgraus * PI) / 180;





// Azimute

float phi = (latitude * PI) / 180;
float delta = (Nd * PI) / 180;


float A_num = (sin(H)); 
float A_den = (cos(H)*sin(phi)-tan(delta)*cos(phi));
float Azimute_rad = atan2(A_num,A_den);
float Azimute = ((Azimute_rad * 180) / PI)+180;


float Az = floor(Azimute);
float Azgraus = abs(Azimute-Az);
float Azmin = Azgraus*60;
float Azmin2 = floor(Azmin);
float Azseg = abs(Azmin-Azmin2);
float Azseg2 = Azseg*60;
float Azseg3 = floor(Azseg2);




// Altura

float h = sin(delta)*sin(phi)+cos(delta)*cos(phi)*cos(H);  
float Altura_rad = asin(h);
float Altura = ((Altura_rad * 180) / PI); 

float Alt = floor(Altura);
float Altgraus = abs(Altura-Alt);
float Altmin = Altgraus*60;
float Altmin2 = floor(Altmin);
float Altseg = abs(Altmin-Altmin2);
float Altseg2 = Altseg*60;
float Altseg3 = floor(Altseg2);

t_stop = micros();

  delay(100);

// float Altura_graus = [Alt Altmin2 Altseg3];
  Serial.print("Dia juliano: ");
  Serial.println(JD);
  Serial.print("Século juliano: ");
  Serial.println(T);
  Serial.print("Tempo universal: ");
  Serial.println(UT);
  Serial.print("Tempo sideral de Greenwich qualquer hora: ");
  Serial.println(TSG_qualq_hora_frac);
  Serial.print("Tempo sideral local: ");
  Serial.println(LST);
  Serial.print("Ângulo horário: ");
  Serial.println(Hgraus/15);
  Serial.print("Ascensão reta média: ");
  Serial.println(Nar);
  Serial.print("Declinação média: ");
  Serial.print("Nd ");
  Serial.println(Nd);
  Serial.print("Ndd ");
  Serial.println(Ndd);
  

  Serial.print("Azimute: ");
  Serial.print(Az);
  Serial.print(" ");
  Serial.print(Azmin2);
  Serial.print(" ");
  Serial.println(Azseg3);
  
  Serial.print("Altura: ");
  Serial.print(Alt);
  Serial.print(" ");
  Serial.print(Altmin2);
  Serial.print(" ");
  Serial.println(Altseg3);
  

  
    
    Serial.println(t_start);
    Serial.println(t_stop);
    Serial.print("Tempo de execução: ");
    Serial.println(t_stop-t_start);
    while(1);



}
