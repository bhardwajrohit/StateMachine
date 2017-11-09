/*
   FILE          : DebitMachine.ino
   PROJECT       : PROG8125
   PROGRAMMER    : Rohit Bhardwaj
   FIRST VERSION : 2016-07-14
   DESCRIPTION   : Demonstrates a debit machine banking transaction that implements a state machine.
*/
//Includes
#include <LiquidCrystal.h>
#include <Bounce.h>

//Globals
LiquidCrystal lcd(19, 18, 17, 16, 15, 14); //create an lcd object
//                RS, En, D4, D5, D6, D7
Bounce chequingPb(9, 10);                  //create a pb object
Bounce savingsPb(10, 10);                  //create a pb object
Bounce cancelPb(11, 10);                   //create a pb object
Bounce okPb(12, 10);                       //create a pb object

//Function Prototypes
void displayWelcome();
void displayAmount(float amount);
float checkIfAmountRecd();
void displayOkCancel();
int8_t checkOkOrCancel();
void displayCheqOrSav();
int8_t checkCheqOrSav();
void displayEnterPin();
int16_t checkIfPinRecd();
void displayEnteredPin();
int8_t bankFunction();
void transactionAuthorized();


// FUNCTION      : displayWelcome()
// DESCRIPTION   : clears the LCD display and displays
//                 Welcome on line 1 of the display
// PARAMETERS    : None
// RETURNS       : nothing
void displayWelcome()
{
  lcd.clear();
  lcd.printf("Welcome");
}

// FUNCTION      : displayAmount()
// DESCRIPTION   : clears the LCD display and displays
//                 the $amount received on line 1 of the display
// PARAMETERS    : float - amount to display
// RETURNS       : nothing
void displayAmount(float amount)
{
  lcd.clear();
  lcd.printf("$%-9.2f", amount);
  Serial.printf("$%9.2f\n", amount);
}

// FUNCTION      : checkIfAmountRecd()
// DESCRIPTION   : Checks the serial port to see if any chars
//                 are available. If so, reads the chars using
//                 the parseFloat method so that the $amount
//                 is read.
// PARAMETERS    : none
// RETURNS       : float, the amount in $ to be debited
float checkIfAmountRecd()
{
  if (Serial.available())
  {
    return (Serial.parseFloat());
  }
  else
  {
    return -1;
  }
}

// FUNCTION      : displayOkOrCancel()
// DESCRIPTION   : displays "OK or Cancel?" on line 2 of LCD
// PARAMETERS    : none
// RETURNS       : nothing.
void displayOkCancel()
{
  lcd.setCursor(0, 1); //move to second line first position
  lcd.printf("PressOKorCancel?");
}

// FUNCTION      : checkOkOrCancel()
// DESCRIPTION   : Checks either the OK or Cancel
//                 pushbutton has been pressed.
// PARAMETERS    : none
// RETURNS       : int8_t, 3 if cancel pressed, 4 if ok
//                 ok pressed. 0 returned if neither
//                 has pressed.
int8_t checkOkOrCancel()
{
  if (cancelPb.fallingEdge())
  {
    //then the cancel pushbutton has been pressed
    return 3;
  }
  else if (okPb.fallingEdge())
  {
    //then ok pressed
    return 4;
  }
  else
  {
    return 0; //as ok or cancel was not pressed.
  }
}


// FUNCTION      : displayCheqOrSav()
// DESCRIPTION   : clears the LCD and uses line 1 & 2 of LCD to
//                 display "Cheqing or Savings?"
// PARAMETERS    : none
// RETURNS       : nothing.
void displayCheqOrSav()
{
  lcd.clear();
  lcd.printf("Chequing or ");
  lcd.setCursor(0, 1); //move to second line first position
  lcd.printf("Savings? ");
  Serial.println("Chequing or Savings?");
}

// FUNCTION      : checkCheqOrSav()
// DESCRIPTION   : Checks either the Chequing or Saving
//                 pushbutton has been pressed.
// PARAMETERS    : none
// RETURNS       : int8_t, 1 if chequing pressed, 2 if saving
//                 pressed, 3 if cancel pressed. 0 returned if
//                 neither has pressed.
int8_t checkCheqOrSav()
{
  if (chequingPb.fallingEdge())
  {
    //then the chequing pushbutton has been pressed
    return 1;
  }
  else if (savingsPb.fallingEdge())
  {
    //then saving pressed
    return 2;
  }
  else if (cancelPb.fallingEdge())
  {
    //then the cancel pushbutton has been pressed
    return 3;
  }
  else
  {
    //then neither of these pushbuttons has been pressed.
    return 0;
  }
}

// FUNCTION      : displayEnterPin()
// DESCRIPTION   : clears the LCD and displays "Enter PIN " on
//                 line 1 of LCD
// PARAMETERS    : none
// RETURNS       : nothing.
void displayEnterPin()
{
  Serial.println("Enter Pin");
  lcd.clear();
  lcd.printf("Enter PIN ");
}

// FUNCTION      : checkIfPinRecd()
// DESCRIPTION   : Checks the serial port to see if any chars
//                 are available. If so, reads the chars using
//                 the parseInt method so that the PIN
//                 is read.
// PARAMETERS    : none
// RETURNS       : int, the pin
int16_t checkIfPinRecd()
{
  int16_t pin = 0;
  if (Serial.available())
  {
    Serial.println("PIN Entered");
    pin = Serial.parseInt();
    return pin;
  }
  else
  {
    return -1;
  }
}

// FUNCTION      : displayEnetredPin()
// DESCRIPTION   : displays "****" on line 2 of LCD and then clears the
//                 LCD to display Processing on line 1 of the display
// PARAMETERS    : none
// RETURNS       : nothing.
void displayEnteredPin()
{
  lcd.setCursor(0, 1); //move to second line first position
  lcd.printf("****");
  delay(3000);         //this appears only for 3s
  lcd.clear();
  lcd.printf("Processing...");
}

// FUNCTION      : bankFunction()
// DESCRIPTION   : Checks the serial port to see if any chars
//                 are available. If so, reads the chars using
//                 the serial.read method and compares them with
//                 OK or NotOk
// PARAMETERS    : none
// RETURNS       : int8_t, 1 if ok is read , 2 if notok is read
int8_t bankFunction()
{
  char bank[5];                                            //create a character array
  if (Serial.available())                                  //checks if any character is available on the serial port
  {
    for (int x = 0; x < 5; x++)
    {
      bank[x] = Serial.read();                            //updating the char array with characters that are read from serial port
    }
    if (bank[0] == 'o' && bank[1] == 'k')
    {
      return 1;
    }
    else if ((bank[0] == 'n' && bank[1] == 'o') &&
             (bank[2] == 't' && bank[3] == 'o') &&
             (bank[4] == 'k'))
    {
      return 2;
    }
  }
}


// FUNCTION      : transactionAuthorized()
// DESCRIPTION   : clears the LCD and displays trannsaction authorized on
//                 line 1 and 2 of LCD and after that it displays printing
//                 receipt
// PARAMETERS    : none
// RETURNS       : nothing
void transactionAuthorized()
{
  Serial.println("Transaction Authorized ");
  lcd.clear();
  lcd.printf("Transaction");
  lcd.setCursor(0, 1);
  lcd.printf("Authorized");
  delay(3000);
  lcd.clear();
  lcd.printf("Printing");
  lcd.setCursor(0, 1);
  lcd.printf("Receipt");
  delay(3000);
  displayWelcome();
}
void setup()
{
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);

  Serial.begin(9600);

  lcd.begin (2, 16);
  lcd.printf("debit Demo");
  delay(3000);
  lcd.clear();
  displayWelcome();
}

void loop()
{
  float amount = 0;                //used to hold the transaction amount
  int16_t myPin = 0;               //used to hold the pin
  int8_t bankResponse ;            //used to hold the response from the bankfunction
  static int8_t transactionState = 1;
  int8_t pbPressed = 0;            //holds pushbutton 1 = Chequing,
  //2 = savings, 3 = Cancel
  //4 = OK
  /*states:   1   display Welcome Screen, wait for $ amount input from
                   Serial port
               2   @ amount Received, waiting for Ok or Cancel button
               3   OK received, waiting for chequing or Savings button
               4   C or S received, waiting for PIN to be entered from
                   Serial Port
               5   Pin Correct, send transaction data to bank. Waiting
                   for OK back from Bank If OK from Bank received. Print
                   Reciept, Record transaction. Move back to State 1.
               6   Cancel Pressed. Display "Transaction Cancelled" back to
                   state 1
  */
  //update the pb's
  chequingPb.update();
  savingsPb.update();
  cancelPb.update();
  okPb.update();

  switch (transactionState)
  {
    case 1:  //checking if an amount has been received
      amount = checkIfAmountRecd();
      if (amount != -1 )
      { //returns a -1 if an transaction amount has
        //NOT been received on the serial port.
        displayAmount(amount);
        transactionState = 2;
      }
      break;

    case 2:  //amount has been received waiting for Ok or Cancel to be pressed.
      displayOkCancel();
      pbPressed = checkOkOrCancel();
      if (pbPressed != 0)
      {
        if (pbPressed == 3)
        {
          //then cancel was pressed.
          Serial.println("Cancel Pressed");
          transactionState = 6;
        }
        else if (pbPressed == 4)
        {
          //then ok pressed
          Serial.println("OK Pressed");
          displayCheqOrSav();
          transactionState = 3;
        }
      }
      break;

    case 3:  //chequing or Saving has been displayed waiting for chequing or saving to be pressed

      pbPressed = checkCheqOrSav();
      if (pbPressed != 0)
      {
        if (pbPressed == 1)
        {
          //then chequing was pressed.
          Serial.println("C Pressed");
          displayEnterPin();
          transactionState = 4;
        }
        else if (pbPressed == 2)
        {
          //then saving pressed
          Serial.println("S Pressed");
          displayEnterPin();
          transactionState = 4;
        }
        else if (pbPressed == 3)
        {
          //then cancel pressed
          Serial.println("Cancel Pressed");
          transactionState = 6;
        }
      }
      break;

    case 4: //checking if pin has been received
      myPin = checkIfPinRecd();
      if (myPin != -1 )
      { //returns a -1 if the pin has
        //NOT been received on the serial port.
        //if we're here a pin was entered
        displayEnteredPin();
        transactionState = 5;
      }
      else if (cancelPb.fallingEdge())
      {
        Serial.println("Cancel Pressed");
        transactionState = 6;
      }
      break;

    case 5: //now wait for OK back from the bank(from the serial port)
      bankResponse = bankFunction();
      if (bankResponse == 1)
      {
        transactionAuthorized();
        transactionState = 1;
      }
      else if (bankResponse == 2)
      {
        Serial.println("Transaction Cancelled ");
        transactionState = 6;
      }
      break;

    case 6: //Transaction Cancelled
      lcd.clear();                                      // clears the LCD DISPLAY
      lcd.printf("Transaction");
      lcd.setCursor(0, 1);                              //sets cursor at line 2
      lcd.printf("Cancelled");
      delay(3000);
      displayWelcome();
      transactionState = 1;
      break;

    default:
      displayWelcome();
      break;
  }
}


