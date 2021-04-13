#include <Esplora.h>  // Necessário para utilizar os periférios do Arduino Esplora
#include <Mouse.h>    // Para simular um mouse
#include <Keyboard.h> // Para simular um teclado

#define VELOCIDADE_MOUSE 15 // Altera sensibilidade do mouse com movimento do joystick

// Macros para soltar skills com o slider
#define SLIDER_LEFT(skill) (skill > 500 && skill_active == false)
#define SLIDER_RIGHT(skill) (skill < 400 && skill_active == false)
#define SLIDER_CENTER(skill) (skill > 400 && skill < 500)

// Botões de ação
int botao_atirar = HIGH;
int botao_avanca_arma = HIGH;
int botao_retorna_arma = HIGH;
int botao_pulo = HIGH;
int grito = 0;

int slider_skill = 0;
bool skill_active = false;

// Array para troca de armas com botões
char armas[4] = {'1', '2', '3', '4'};
int armaAtual = 0;

void setup(){
  Serial.begin(9600);       // Inicializa comunicação Serial
  Mouse.begin();            // Toma controle do mouse
  Keyboard.begin();         // Toma controle do teclado
}

void loop() {
  // Leitura dos botões
  botao_atirar = Esplora.readButton(SWITCH_DOWN);
  botao_pulo = Esplora.readButton(SWITCH_LEFT);
  botao_avanca_arma = Esplora.readButton(SWITCH_UP);
  botao_retorna_arma = Esplora.readButton(SWITCH_RIGHT);

  //Leitura do Acelerômetro
  int xAxis = Esplora.readAccelerometer(X_AXIS);    // eixo x, anda para esquerda e direita
  int yAxis = Esplora.readAccelerometer(Y_AXIS);    // eixo y, anda para frente e trás

  // Leitura do Joystick
  int xValue = Esplora.readJoystickX();        // read the joystick's X position
  int yValue = Esplora.readJoystickY();        // read the joystick's Y position
  int button = Esplora.readJoystickSwitch();   // read the joystick pushbuttonw

  // Leitura do microfone
  grito = Esplora.readMicrophone();

  // Leitura do slider
  slider_skill = Esplora.readSlider();

  // Condições para a liberação de skills do slider
  if (SLIDER_RIGHT(slider_skill)){
    skill_active = true;
    Keyboard.print('q');
  }else if(SLIDER_LEFT(slider_skill)){
    skill_active = true;
    Keyboard.print('c');
  }else if (SLIDER_CENTER(slider_skill)){
    skill_active = false;
  }

  if (grito > 850){
    Keyboard.print('e');
  }

  // Comandos dos botões -------------------------
  if (botao_atirar == LOW){
    Mouse.press();
  }
  if (botao_atirar == HIGH){
    Mouse.release();
  }

  if (botao_pulo == LOW){
    Keyboard.press(KEY_F4);
  }
  if (botao_pulo == HIGH){
    Keyboard.release(KEY_F4);
  }

  // Comandos para troca de arma ----------------------------
  if (botao_avanca_arma == LOW){
    armaAtual += 1;
    if (armaAtual > 3){
      armaAtual = 0;
    }
    Keyboard.print(armas[armaAtual]);
    delay(100);
  }

  if (botao_retorna_arma == LOW){
    armaAtual -= 1;
    if (armaAtual < 0){
      armaAtual = 3;
    }
    Keyboard.print(armas[armaAtual]);
    delay(100);
  }
  
  // Comandos do Joystick -----------------------------------------------
  int mouseX = map(xValue, -512, 512, VELOCIDADE_MOUSE, -VELOCIDADE_MOUSE);  // map the X value to a range of movement for the mouse X
  int mouseY = map(yValue, -512, 512, -VELOCIDADE_MOUSE, VELOCIDADE_MOUSE);  // map the Y value to a range of movement for the mouse Y
  Mouse.move(mouseX, mouseY, 0);                 // move the mouse

  if (button == 0) {                         // if the joystick button is pressed
    Keyboard.press('x');                             // send a mouse click
  } else {
    Keyboard.release('x');                           // if it's not pressed, release the mouse button 
  }

  // Comandos do acelerômetro ------------------------------------------

  // ANDA DIREITA + FRENTE (DIAGONAL PARA DIREITA INDO PARA FRENTE)
  if (yAxis < 0 && xAxis < -20){
    Keyboard.release('s');
    Keyboard.release('a');
    Keyboard.press('w');
    Keyboard.press('d');
  }
  // ANDA ESQUERDA + FRENTE (DIAGONAL PARA ESQUERDA INDO PARA FRENTE)
  if (yAxis < 0 && xAxis > 20){
    Keyboard.release('s');
    Keyboard.release('d');
    Keyboard.press('w');
    Keyboard.press('a');
  }

  // ANDA DIREITA + TRÁS (DIAGONAL PARA DIREITA INDO PARA TRÁS)
  if (yAxis > 90 && xAxis < -20){
    Keyboard.release('a');
    Keyboard.release('w');
    Keyboard.press('d');
    Keyboard.press('s');
  }

  // ANDA ESQUERDA + TRÁS (DIAGONAL PARA ESQUERDA INDO PARA TRÁS)
  if (yAxis > 90 && xAxis > 20){
    Keyboard.release('d');
    Keyboard.release('w');
    Keyboard.press('a');
    Keyboard.press('s');
  }
  // ANDA PARA FRENTE
  // Quando o valor de y diminui, personagem anda para frente
  if (yAxis < 0){
    Keyboard.press('w');
  }

  // ANDA PARA TRÁS
  // Quando o valor de y aumenta, personagem anda para trás
  if (yAxis > 90){
    Keyboard.press('s');
  }

  // ANDA PARA ESQUERDA
  // Quando o valor de X aumenta, personagem anda para esquerda
  if (xAxis > 20){
    Keyboard.press('a');
  }

  // ANDA PARA DIREITA
  // Quando o valor de x diminui, personagem anda para direita
  if (xAxis < -20){
    Keyboard.press('d');
  }
  
  if ((yAxis > 0 && yAxis < 90) && (xAxis < 20 && xAxis > -20)){
    Keyboard.release('w');
    Keyboard.release('a');
    Keyboard.release('s');
    Keyboard.release('d');
  }
  //Keyboard.release(armas[armaAtual]);
}
