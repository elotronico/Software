#include "mbed.h"
#include "OpenChair.h"

AnalogIn potX(PA_0);
DigitalIn pulsador(PA_10);    //D2
//DigitalIn pulsadorDOWN(PB_3);   //bajar velocidad

Serial pc(SERIAL_TX, SERIAL_RX);
OpenChair chair(PC_12,PD_2,PC_10,PC_11,60,150); //50,150


void acelera(int);
void frena(int);
void manten(void);

int marcha = 0;
double Ma = 0, Mb = 0;
double potenciometroX = 0;

//####################################################################
//                   MAIN
//####################################################################
int main() {
    pc.baud(115200);

    while(1){
          potenciometroX =potX.read();
          pc.printf("%f \t %d\n", potenciometroX, marcha);

          //si el poteneciometro es mayor de 0.8, habilitamos el interruptor  de acelerar.
          if( potenciometroX > 0.8 ){
          pc.printf("pulsador habilitado \t");

            if (pulsador == 1)  {
                   if (marcha == 0){
                        marcha = 1;
                        pc.printf("MARCHA 1\n");
                        acelera(6);  }
                    else if (marcha == 1){
                        marcha = 2;
                        pc.printf("MARCHA 2\n");
                        acelera(2);  }
                    else if (marcha == 2){
                        marcha = 3;
                        pc.printf("MARCHA 3\n");
                        acelera(2); }
                    else if (marcha == 3){
                        marcha = 1;
                        pc.printf("MARCHA 1\n");
                        frena(4);  }

                    else{
                        pc.printf("- - - - ERROR - - - - \n");
                        }
                    }

            //parado
          else{
            pc.printf("mantementos\n");
            manten();
          }
        }

        //valor pot muy bajo. paramos silla
        else{
            pc.printf("pot muy bajo. Paramos.\n");

            if(marcha == 1){
                marcha = 0;
                frena(6);   }

            else if(marcha == 2){
                marcha = 0;
                frena(8);  }

            else if (marcha == 3){
                marcha = 0;
                frena(4);  }

            else{
                chair.writeMotor1(0,0);
                chair.writeMotor2(0,0);  }
            }


  wait_ms(300);
  }
}

//------------------------------------------

void frena(int z){

  for(int i=0; i<z; i++){
      pc.printf("%d f\n", i);
      chair.writeMotor1( 65505, (int)1);
      chair.writeMotor2( 65565, (int)1);
      wait_ms(250);
      }
}

void acelera(int z){

  for(int i=0; i<z; i++){
      pc.printf("%d a\n", i);
      chair.writeMotor1( 65565, (int)1);
      chair.writeMotor2( 65505, (int)1);
      wait_ms(250);
      }
}


void manten(void){
  chair.writeMotor1( 0, (int)1);
  chair.writeMotor2( 0, (int)1);
}
