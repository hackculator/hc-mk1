//ESQUEMA//
/*
                     R1   M1
                 |--[==]--/ ---|                                     Ro1   Mo1
          R0     |   R2   M2   |                         Op1      |--[==]--/ ---|
   Vin --[==]----|--[==]--/ ---|---|                  ---|>|---   |  Ro2   Mo2  |
              |  |   R3   M3   |   |          Vin ----|       |---|--[==]--/ ---|---|
   Vs --------|  |--[==]--/ ---|  Gnd                 ---|>|---   |  Ron   Mon  |   |
                 |   Rn   Mn   |                         Op2      |--[==]--/ ---|  Gnd
                 |--[==]--/ ---|

*/

//PARÀMETRES//
//Valors de les resistències: R0 en primera posició seguida de R1 a Rn de MENOR A MAJOR.
long Re[] = {502700, 4616, 14750, 66900};

//Valors de les resistències: Ro0 en primera posició seguida de Ro1 a Ron de MAJOR A MENOR.
long Ro[] = {120, 15000, 2200, 510};

//Màxima intensitat optocopladors.
int OIMax = 50; //mA

//Caiguda de tensió als diodes del rectificador.
int Vr = 620; //mV

//Pins que activen el transistors, en el mateix ordre que les resistències associades.
byte Mg[] = { -1, 13, 11, 12};
byte Mog[] = { -1, 2, 3, 4};

//Pins analògics de lectura.

int Md[] = { -1, 1, 2, 3};
int S = 6;
int S2 = 6;
int So[] = {0, 5};

//Vector que emmagatzemarà les relacions ri = (R0+Ri)/Ri.
//[Important] Cal afegir tants zeros com divissors de tensió.
float r[] = { -1.0, 0, 0, 0};

//Vector de tensions màximes als optocopladors.
//[Important] Cal afegir tants zeros com resistències limitadores.
int OVMax[] = { -1, 0, 0, 0};


//VARIABLES//
//Nombre de divissors de tensió i resistències limitadores.
byte Nr, Nl;

//
byte v_[50] = {};
byte a_[50] = {};


//FUNCIONS//
void  inicia_voltimetre();    //  Inicialitzen els dispositius de mesura.
void  inicia_amperimetre();   //
void  inicia_polaritat();     //
float voltimetre();           //  Retornen els valors mesurats.
float amperimetre();          //
byte  polaritat(float);       //

void setup()
{
  Serial.begin(9600);
  inicia_voltimetre();
  inicia_polaritat();
}

void loop()
{
  float V = voltimetre();
  byte p = polaritat();
  //amperimetre();
  Serial.print(p); Serial.print(" "); Serial.println(V);
  delay(1000);
}

void inicia_voltimetre()
{
  Nr = (sizeof(Mg) / sizeof(byte)) - 1;                         //  Calcula el nº de divissors de tensió.
  for (byte i = 1; i <= Nr; i++)                                //| Recorre el vector de divissors de tensió.
  { //|
    pinMode(Mg[i], OUTPUT);                                     //| Defineix els pins G dels transistors com a sortida.
    r[i] = (float)(Re[0] + Re[i]) / (float)Re[i];               //| Calcula les relacions dels divissors de tensió.
  }
}

float voltimetre()
{
  int lectura_S, lectura_D;                                     // Lectures al pin comú de tensió i al drain del MOSFET actiu.
  float tensio;                                                 // Variable que contindrá el valor final de la tensió.
  bool continuar = true;                                        // Finalitza la cerca del divissor de tensió més addient.
  byte i = 1;                                                   // Variable que recorre yi emmagatzema el divissor de tensió òptim.
  while (continuar && i <= Nr)                                  //| Recorre el vector de divissors de tensió fins que s'hagi arribat a l'òptim.
  {
    digitalWrite(Mg[i], HIGH);                                  //| Activa un divissor de tensió.
    //Serial.print(i); Serial.println(" on");
    if (i == 1)                                                 //|| Si s'acaba d'ativar el primer del vector,
    {
      digitalWrite(Mg[Nr], LOW);                                //|| n'apaga l'últim.
      //Serial.print(Nr), Serial.println(" off");
    }
    else {                                                      //|| Si s'ha activat qualsevol altre,
      digitalWrite(Mg[i - 1], LOW);                             //|| n'apaga linmediatament anterior.
      //Serial.print(i-1); Serial.println(" off");
    }
    delay(5);                                                   //|
    lectura_S = analogRead(S);                                  //| Realitza les lescures dels dos pins analògics.
    delay(5);                                                   //| Cal realitzar una breu pàusa per a que funcioni correctament.
    lectura_D = analogRead(Md[i]);                              //|

    tensio = ((lectura_S - lectura_D) * r[i] + lectura_D)  * (5.00 / 920.0) + 2.0 * Vr / 1000.0; //| Calcula un primer valor provisional de la tensió.
    if (i < Nr) {
      //Serial.print("("); Serial.print(tensio / r[i + 1]); Serial.print(")");
      if (tensio / r[i + 1] > 5) {                                                               //|| Si activant el següent divissor (de menor valor màxim) se sobrepassa el màxim valor admés per l'ADC,
        continuar = false;                                                                       //|| s'atura la cerca.
      }
    }
    else {                                                                                       //|| Si s'ha arribat a l'últim divissor,
      continuar = false;                                                                         //|| s'atura la cerca.
    }

    //Serial.print(i); Serial.print("  "); Serial.print(lectura_S); Serial.print("  ");
    //Serial.print(lectura_S * (5.00 / 920.0)); Serial.print(" "); Serial.println(lectura_D * (5.00 / 920.0));
    //Serial.println(tensio);

    i++;

  }
  i--;                                                            // D'aquest punt en endavant el divissor de tensió (variable i) está fixat.
  boolean OK = false;                                             // Comptador 1: interromp el mostreig si s'agafen 50 valors seguits sense canvis de més del 10%.
  byte n = 0;                                                     // Comptador 2: interromp el mostreig després d'un total de 200 valors.
  byte n2 = 0;                                                    // Emmagatzema la suma de totes les lectures per a fer la mitjana.
  long suma = 0;                                                  // Màxim valor llegit des de l'últim canvi superior al 5%.
  byte max_ = 0;                                                  // Mínim valor llegit des de l'últim canvi superior al 5%.
  byte min_ = 1023;
  while (!((n > 50 && (max_ - min_) < lectura_S * 0.08) || n2 > 200)) {  //| Mentre els comptadors no s'esgotin, continua el mostratge.
    n2++;
    delay(5);
    lectura_S = analogRead(S);
    n++;
    if (lectura_S > v_[1] * 1.05 || lectura_S < v_[1] * 1.05) {        //|| Si la lectura divergeix més d'un 5% de l'anterior,
      for (byte i = 0; i < 50; i++) {                                  //|| s'esborren les lectures antigues emmagatzemades i comencen a enrregistrar les noves.
        v_[i] = lectura_S;
      }
      //Serial.println("Canvi");
      n = 0;                                                           //|| En aquest cas, es reseteja el comptador.
    }

    suma = lectura_S;
    max_ = 0;
    min_ = 1023;
    v_[0] = lectura_S;
    for (byte f = 49; f > 0; f--) {                                    //| S'actualitzen els valos màxims i mínims.
      suma += v_[f];                                                   //| Se sumen les dades guardades a v_.
      if (v_[f] > max_) {
        max_ = v_[f];
      }
      if (v_[f] < min_) {
        min_ = v_[f];
      }
      //Serial.print(v_[f]); Serial.print("("); Serial.print(f);Serial.print(")|"); delay(5);
      v_[f] = v_[f - 1];                                              //| Es traslladen els valors de f-1 a f, i s'afegeix la nova lectura a 0.
    }
    //Serial.println();
  }

  float LECTURA_S = suma / 50.0;
  //Serial.println();
  //Serial.print("TOTAL "); Serial.print(suma); Serial.print("  "); Serial.print(LECTURA_S); Serial.print("  ");

  lectura_D = analogRead(Md[i]);

  tensio = ((LECTURA_S - lectura_D) * r[i] + lectura_D)  * (5.00 / 920.0);// + 2.0 * Vr / 1000.0;
  //Serial.println(tensio); Serial.println(); Serial.println();


  return tensio;
}

void inicia_amperimetre() {
  ;
}
float amperimetre() {

  byte lectura = analogRead(S2);
  float amperatge = map(lectura - 512, -512, 512, -15, 15);
  //Serial.print(lectura); Serial.print(" "); Serial.println(amperatge);
  return amperatge;
}


void inicia_polaritat() {
  Nl = (sizeof(Mog) / sizeof(byte)) - 1;
  for (byte i = 1; i <= Nl; i++)
  {
    OVMax[i] = OIMax * 0.9 * Ro[i] / 1000;
    Serial.println(OVMax[i]);
  }
}

byte polaritat() {
  byte i_max;
  float v = voltimetre();
  for (byte i = 1; i <= Nl; i++) {
    if (v < OVMax[i]) i_max = i;
  }

  digitalWrite(Mog[i_max], HIGH);
  delay(4); int O1 = analogRead(So[1]); int O2 = analogRead(So[2]);
  delay(2);     O1 += analogRead(So[1]);    O2 += analogRead(So[2]);
  delay(2);     O1 += analogRead(So[1]);    O2 += analogRead(So[2]);
  digitalWrite(Mog[i_max], LOW);

  Serial.print(OVMax[i_max]); Serial.print(" | "); Serial.print(O1); Serial.print(" "); Serial.println(O2);

  if (O1 > 2 && O2 > 2) return 2;
  if (O1 > 2) return 0;
  if (O2 > 2) return 1;

}


