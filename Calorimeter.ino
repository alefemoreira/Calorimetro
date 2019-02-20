#define motor 13
#define termometro A1

float temperaturaI, temperaturaF, CalorEspecifico, Massa;

void setup() {
  Serial.begin(9600);
  pinMode(motor, OUTPUT);
}

void loop() {
  //MENUa
  Serial.println("VC QUER :");
  Serial.println("[ a ] Agitar");
  Serial.println("[ b ] Ver Temperatura");
  Serial.println("[ c ] Informacoes Calolimetricas\n");

  esperaOrdem();
  lerOpcao(Serial.read());

  Serial.println("VC QUER CONTINUAR [ENTER]");
  esperaOrdem();
}

void lerOpcao(char opcao) {
  switch (opcao) {
    case 'a':
      Serial.println("tempo (em segundos):");
      esperaOrdem();
      ativaAgitador(Serial.parseFloat());
      break;

    case 'b':
      imprimeTemperatura();
      break;

    case 'c':
      limpaBuffer();
      delay(2000);

      Serial.println("Coloque a substância... [ENTER]\n");
      esperaOrdem();
      Serial.println("Pegando temperatura inicial");
      temperaturaI = getTemperaturaCelsius(analogRead(termometro));
      imprimeDados("Temperatura Inicial:", temperaturaI, " °C");

      Serial.println("Espere chegar em equilíbrio [ENTER]\n");
      esperaOrdem();
      temperaturaF = getTemperaturaCelsius(analogRead(termometro));
      imprimeDados("Temperatura Final: ", temperaturaF, " °C");

      Serial.println("Diga a massa da substancia (g)");
      esperaOrdem();
      Massa = Serial.parseFloat();
      imprimeDados("Massa(g): ", Massa, " g");

      Serial.println("Diga o calor especifico da substância: (cal/g°C)");
      esperaOrdem();
      CalorEspecifico = Serial.parseFloat();
      imprimeDados("Calor Especifico", CalorEspecifico, " cal/g°C");

      informacoesCalolimetricas(temperaturaI, temperaturaF, CalorEspecifico, Massa);

      break;

  }
  limpaBuffer();
}

void imprimeDados(String nome, float valor, String unidade) {
  Serial.print(nome);
  Serial.print(" ");
  Serial.print(valor);
  Serial.println(unidade);
  Serial.println("\n");
}

void esperaOrdem() {
  boolean condicao = true;

  limpaBuffer();
  delay(2000);

  while (condicao) {
    if (Serial.available() > 0) {
      condicao = false;
    }
  }
}

void imprimeTemperatura() {
  Serial.println("\nCalor: ");
  Serial.print(getTemperaturaCelsius(analogRead(termometro)));
  Serial.println(" °C\n");
}

float getTemperaturaCelsius(float leitura) {
  return ((leitura / 1024) * 5000) / 10;
}

void ativaAgitador(float segundos) {
  Serial.println("Agitando\n");
  digitalWrite(motor, HIGH);
  delay(segundos * 1000);
  digitalWrite(motor, LOW);
}

void informacoesCalolimetricas(float temperaturaInicial, float temperaturaFinal, float calorEspecifico, float massa) {
  float deltaTemperatura = temperaturaFinal - temperaturaInicial;
  float valorEnergetico = massa * calorEspecifico * deltaTemperatura;

  if (deltaTemperatura < 0) {
    Serial.print("Reação Exotérmica");
  } else if (deltaTemperatura > 0) {
    Serial.print("Reação Endotérmica");
  }
  
  Serial.println(deltaTemperatura);
  
  if (calorEspecifico == 1) {
    Serial.println("A substância é a água\n");
  }

  imprimeDados("Calorias :", valorEnergetico, " cal");
  imprimeDados("KiloCalorias :", (valorEnergetico / 1000), " Kcal");
  imprimeDados("Joules :", (valorEnergetico * 4.184), " J");
  imprimeDados("KiloJoules :", (valorEnergetico * 4.184 / 1000), " KJ");
}

void limpaBuffer() {
  Serial.end();
  Serial.begin(9600);
}
