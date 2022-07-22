#define led 4
#define led_timer1 5

volatile byte led1_status = LOW;

//ISR + nome da interrupção
ISR(TIMER1_OVF_vect){
    digitalWrite(led_timer1, led1_status);
    //inverte o estado atual do led
    led1_status = !led1_status;
    
}

void setup() {
  pinMode(led, OUTPUT);
  pinMode(led_timer1, OUTPUT);
 
  // o timer 1 tem 16bits, ou seja conta de 0 a 65536
  // TODOS OS BITS EM 0, Nota: não é nescessario fazer essa atribuição se todos os valores forem 0
  TCCR1A = 0b00000000; // WGM11 E WGM10 em 0
  TIMSK1 = 0b00000001; //Interrupção por overflow do timer 1
  
  sei(); //interrupções globais habilitadas

  //definição do Prescaler
  TCCR1B = 0b00000100; //Prescaler de 256, Aproximadamente 1seg ---- WGM12 em 0
  
}

void loop() {
  digitalWrite(led, HIGH);
  delay(250);
  digitalWrite(led, LOW);
  delay(250);
}