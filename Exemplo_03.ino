#define led 4 //nosso led
#define external_pin 3 //nosso botão como interrupção externa

using ISR_callback = void(*)(); //definição de tipo para nossa callback

volatile byte estado_led = LOW;//estado atual do led
volatile byte estado_prescaler;//Prescaler atual

//Uma enumeração da tabela de prescaler vista no datasheet, uma forma facil de vizualizar no codigo
enum Prescaler{
  OFF = 0,
  Prescaler_1,
  Prescaler_8,
  Prescaler_64,
  Prescaler_256,
  Prescaler_1024
};

//nossa ISR, repare no tipo volatile, significa que ISR_function vai ser chamada na nossa ISR
//mesmo sendo um callback ainda é necessário o modificador "volatile"
volatile ISR_callback ISR_function; 
ISR(TIMER1_OVF_vect){
  ISR_function();
}

//nossa função para configurar o timer
void Event_handler(ISR_callback callback, Prescaler Feq){
  cli(); // desliga as interrupções para não haver error durante a configuração
  
  ISR_function = callback; //recebe uma função do tipo ISR
  
  estado_prescaler = Feq; //salva a configuração atual do prescaler
  
  TCCR1A = 0b00000000;//modo normal
  
  TCCR1B = Feq & 0x7;//configuração do prescaler, "& 0x7" serve para ganrantir que vamos manipular somente os bits de Prescaler 
  
  TIMSK1 = 0b00000001;//habilita a interrupção por overflow do timer 
  
  sei();//habilita interrupções globais
}

//muda o estado do led
void pisca_led(){
  digitalWrite(led, estado_led);
  estado_led = !estado_led;
}

//função para modificar o prescaler do timer1
void mod_prescaler(){
  estado_prescaler++;//muda para o proximo prescaler (8 - 64 - 256 - 1024)
  if(estado_prescaler > Prescaler_1024){
   estado_prescaler = Prescaler_8; //abaixo disso não é possivel visualizar o led piscar
  }
  TCCR1B = estado_prescaler & 0x7;//configuração do prescaler, "& 0x7" serve para ganrantir que vamos manipular somente os bits de Prescaler 
}
//-----------------Principal--------------------------------------
void setup() {
  pinMode(led, OUTPUT);
  
  //Pino da interrupção externa como input
  pinMode(external_pin, INPUT);
  
  //Nossa função que recebe uma ISR e o prescaler 
  Event_handler(pisca_led, Prescaler_256);
  
  //interrupção no pino 3 na mudança do estado LOW para HIGH
  attachInterrupt(digitalPinToInterrupt(external_pin),mod_prescaler,RISING);
}
void loop() {
  //void loop vazia
}