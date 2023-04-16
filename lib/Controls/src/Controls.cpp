
#include <Constants.h>
#include <Arduino.h>
#include <Controls.h>

Controls::Controls(){
    _direction_pin = -1;
    _select_pin = -1;
}

Controls::Controls(byte direction_pin, byte select_pin){
    _direction_pin = direction_pin;
    _select_pin = select_pin;
}

byte Controls::read_direction(){
    int val = analogRead(_direction_pin);
    byte map_val = map(val, 0, 1023, 0, 100);
    
    
    if ( map_val >= 22 && map_val < 30){
        return RIGHT;
    }
    else if ( map_val >= 30 && map_val < 45){
        return UP;
    }
    else if ( map_val >= 45 && map_val < 60){
        return LEFT;
    }
     else if ( map_val >= 90){
        return DOWN;
    }
    else{
        return 0;
    }
    
}

byte Controls::direction_state_change(){
    byte val = read_direction();
    if( val != _prev_direction){
        _prev_direction = val;
        return val; //Return the direction value only if its different
    }
    
    return -1; //Return -1 if no state change
}

byte Controls::read_select(){
    int val = analogRead(_select_pin);
    byte map_val = map(val, 0, 1023, 0, 100);
    
    
    if (  map_val < 40){
        return 0;
    }
    else if ( map_val < 60){
        return B_BUTTON;
    }
    else{
        return A_BUTTON;
    }
}

byte Controls::select_state_change(){
    byte val =  read_select();
    if( val != _prev_select){
        _prev_select = val;
        Serial.println(val);
        return val; //Return the direction value only if its different
    }
    
    return -1; //Return -1 if no state change
}
