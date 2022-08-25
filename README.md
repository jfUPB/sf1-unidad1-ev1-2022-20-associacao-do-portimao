# Evaluación Unidad 1: Software para sistemas embebidos

## Equipo: Associacao do portimao


**Enunciado:**

*En un escape room se requiere implementar un control compuesto por: 1 LED y 2 botones. El funcionamiento del control es así:*

- El LED tiene tres modos de operación: lento, medio y rápido.

- En el modo lento enciende y apaga cada segundo, en el medio cada 500 ms y en el rápido cada 250 ms.

- Al ingresar a cada modo el LED inicia encendido.

- Si en el modo lento se presiona el botón 1 el LED debe quedar PERMANENTEMENTE APAGADO, pero DEBE ESPERAR a que termine el segundo ya sea encendido o apagado.

- Para regresar el modo lento se debe presionar el botón 1.

- Para pasar del modo lento al modo medio y viceversa se debe presionar el botón 2.

- Si en el modo medio se presiona el botón 1 el LED debe quedar PERMANENTEMENTE ENCENDIDO, pero DEBE ESPERAR a que terminen los 500 ms ya sea encendido o apagado.

- Para regresar al modo medio se debe presionar de nuevo el botón 1.

- Al modo rápido se ingresa cuando el LED está PERMANENTEMENTE apagado o encendido y se presiona el botón 2.

- Si estando en modo rápido se presiona la secuencia botón 1, botón 1, botón 2, botón 2, botón 1 el LED debe regresar al modo desde el que venía cuando ingresó al modo rápido.


**Criterios de evaluación**

- Funcionamiento: 2 unidades para la implementación de los modos lento y medio con sus condiciones de permanentemente encendido o apagado. 1 unidad para el modo rápido y la condición de regreso a los modos lentos y medio.

- Sustentación: 2 unidades por contestar correctamente las preguntas realizadas a cada miembro del equipo.


# Exam Overview

### Video demostrativo del funiconamiento en el ensamblaje físico
	Click [aquí para ver el video](https://youtu.be/prhbxSGy8qI).

## review del código

* Empezamos por la función de la lógica del programa, **Task3**, donde vamos a declarar todos los estados necesarios antes de recurrir a ellos
```
void task3()
{
    enum class TaskStates
    {
        INIT,
        //MOF States
        SLOW,
        WAIT_OFF,
        MEDIUM,
        WAIT_ON,
        FAST,
        PERMA_OFF,
        PERMA_ON
    };
```

* Posteriormente nos decidimos a declarar las variables del programa, incluyendo el objeto taskState, las frecuencias de parpadeo del LED, Etc...
```

    static TaskStates taskState = TaskStates::INIT;
    const uint8_t ledBlink = 25;
    static uint32_t lasTime;
    static bool ledStatus = false;
    const uint32_t SlowInterval = 1000;
    const uint32_t MediumInterval = 500U;
    const uint32_t FastInterval = 250U;
    static bool switchState;

    //Password Var
    static BUTTONS secret[5] = {BUTTONS::U1_BTN, BUTTONS::U1_BTN,
                                BUTTONS::U2_BTN, BUTTONS::U2_BTN,
                                BUTTONS::U1_BTN};

    static BUTTONS disarmKey[5] = {BUTTONS::NONE};
    static uint8_t keyCounter;
```

* A continuación inicializamos el ciclo **Switch** el cual recibe al objeto **taskState** para poder cambiar los estados

```

 switch (taskState)
    {
        //Initialization of Vars
        case TaskStates::INIT:
        {
            pinMode(ledBlink, OUTPUT);
            digitalWrite(ledBlink, LOW);
            lasTime = millis();

            keyCounter = 0;
            taskState = TaskStates::SLOW;
            break;
        }
```

* El mostrado anteriormente es el estado **INIT**, donde se inicializan las variables de la lógica del programa dentro del ciclo,
el siguiente es el estado de parpadeo **SLOW**, donde se revisa que el tiempo actual menos el último tiempo sea mayor o igual a 
**SlowInterveal**, después revisa si algún botón fue pulsado, si fué el **U1** o **U2**, si fue el U1, entra en el estado **WAIT_OFF**
si es U2, entra en el estado **MEDIUM**
```

 case TaskStates::SLOW:
        {
            uint32_t currentTime = millis();

            if( (currentTime - lasTime) >= SlowInterval )
            {
                lasTime = currentTime;
                digitalWrite(ledBlink,ledStatus);
                ledStatus = !ledStatus;
            }

            if(buttonEvt.trigger == true)
            {
                buttonEvt.trigger = false;
                if(buttonEvt.whichButton == BUTTONS::U1_BTN)
                {
                    taskState = TaskStates::WAIT_OFF;
                    printf("Change to Wait: %d\n");
                }
                if (buttonEvt.whichButton == BUTTONS::U2_BTN)
                {
                    keyCounter = 0;
                    taskState = TaskStates::MEDIUM;
                    printf("Change to Medium Mode: %d\n");
                }            
            }   
            break;
        }
```

* El siguiente es el estado **WAIT_OFF** el cual está hehco para esperar a que el LED termine su ciclo de parpadeo y
EN CASO DE ESTAR APAGADO y proceda a **PERMA_OFF**
```

case TaskStates::WAIT_OFF:
        {
            uint32_t currentTime = millis();
            if( (currentTime - lasTime) >= SlowInterval )
            {
                digitalWrite(ledBlink, LOW);
                taskState = TaskStates::PERMA_OFF;
                printf("Change to Perma_OFF: %d\n");
            } 
            break;
        }
```

* Antes de mostrar el estado **PERMA_OFF**, revisemos **MEDIUM**. Este estado vuelve a chequear la frecuencia del LED
después revisa si algún botón fue pulsado. En caso de que sea U1, entra en el estado **WAIT_ON**, si se presiona U2
Nos regresa al estado **SLOW**

```

 case TaskStates::MEDIUM:
        {
            uint32_t currentTime = millis();
            if( (currentTime - lasTime) >= MediumInterval )
            {
                lasTime = currentTime;
                digitalWrite(ledBlink,ledStatus);
                ledStatus = !ledStatus;
            }

            if(buttonEvt.trigger == true)
            {
                buttonEvt.trigger = false;
                if(buttonEvt.whichButton == BUTTONS::U1_BTN)
                {
                    taskState = TaskStates::WAIT_ON;
                    printf("Change to Wait: %d\n");
                }
                if (buttonEvt.whichButton == BUTTONS::U2_BTN)
                {
                    keyCounter = 0;
                    taskState = TaskStates::SLOW;
                    printf("Change to Slow Mode: %d\n");
                }
            }  
            break;
```

* El siguiente es el estado **WAIT_ON** el cual está hehco para esperar a que el LED termine su ciclo de parpadeo y
 EN CASO DE ESTAR ENCENDIDO y proceda a **PERMA_OFF**

```

  case TaskStates::WAIT_ON:
        {
            uint32_t currentTime = millis();
            if( (currentTime - lasTime) >= MediumInterval )
            {
                digitalWrite(ledBlink, HIGH);
                taskState = TaskStates::PERMA_ON;
                printf("Change to Perma_ON: %d\n");
            } 
            break;
        }
```

* Tenemos 2 estados permanentes, EL primero **PERMA_ON**, revisa si algún botón fue pulsado y cual, si fué **U1** y pasa al 
estado **MEDIUM**, si fué el **U2**, entra en modo **FAST**

```

 case TaskStates::PERMA_ON:
        {
            if(buttonEvt.trigger == true)
            {
                buttonEvt.trigger = false;
                if(buttonEvt.whichButton == BUTTONS::U1_BTN)
                {
                    taskState = TaskStates::MEDIUM;
                    printf("Change to Medium Mode: %d\n");
                }
                else if (buttonEvt.whichButton == BUTTONS::U2_BTN)
                {
                    switchState = true; 
                    keyCounter = 0;
                    taskState = TaskStates::FAST;
                    printf("Change to Fast Mode: %d\n");
                    printf("Boolean: %d\n", switchState);
                }
            }    
            break;
        }
```

* Tenemos 2 estados permanentes, EL segundo **PERMA_OFF**, revisa si algún botón fue pulsado y cual, si fué **U1** y pasa al 
estado **SLOW**, si fué el **U2**, entra en modo **FAST**

```

case TaskStates::PERMA_OFF:
        { 
            if(buttonEvt.trigger == true)
            {
                buttonEvt.trigger = false;
                if(buttonEvt.whichButton == BUTTONS::U1_BTN)
                {
                    taskState = TaskStates::SLOW;
                    printf("Change to Slow Mode: %d\n");
                }
                else if (buttonEvt.whichButton == BUTTONS::U2_BTN)
                {
                    switchState = false;  
                    keyCounter = 0;
                    taskState = TaskStates::FAST;
                    printf("Change to Fast Mode: %d\n");
                    printf("Boolean: %d\n", switchState);
                }
            }    
            break;
        }
```

* Procedemos con el estado **FAST**, este vuelve a revisar la frecuencia de parpadeo del modo  y establece las variables, después
revisda si se pulsó algún botón, en caso de que si, accede al arreglo de la contraseña y compara las teclas presionadas con las
almacenadas en el arreglo.

```

 case TaskStates::FAST:
        {
            uint32_t currentTime = millis();
            if( (currentTime - lasTime) >= FastInterval )
            {
                lasTime = currentTime;
                digitalWrite(ledBlink,ledStatus);
                ledStatus = !ledStatus;
            }

            if(buttonEvt.trigger == true)
            {
                buttonEvt.trigger = false;
                disarmKey[keyCounter] = buttonEvt.whichButton;
                keyCounter++;
                if (keyCounter == 5)
                {
                    keyCounter = 0;
                    if (compareKeys(secret, disarmKey) == true)
                    {
                        if(switchState == true)
                        {
                            digitalWrite(ledBlink, HIGH);
                            printf("Back to ON\n");
                            taskState = TaskStates::PERMA_ON;
                        }
                        else if(switchState == false)
                        {
                            digitalWrite(ledBlink, LOW);
                            printf("Back to OFF\n");
                            taskState = TaskStates::PERMA_OFF;
                        }                        
                    }
                    else
                    {
                        printf("Failed Password\n");
                    }
                }
            }    
            break;
        }
```