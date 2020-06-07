/*---------------------------------------------------------------------------------------------------
|   KEYBOARD_LIB.C    |                                                                             |
|----------------------                                                                             |
| This library wraps the READ syscall, and handles the keyboard layout and special key combinations |
---------------------------------------------------------------------------------------------------*/

#include <syscalls.h>
#include <keyboard_lib.h>


// Backspace: 14, Tab: 15, Enter: 28, Ctrl: 29, LShift: 42/170, RShift: 54/182, Alt: 56, Space: 57, F1-F10: 59-68, Cmd: 91
// These keys are preceded by a 224. LA: 75/203, RA: 77/205, DEL: 83/211

// Keys represented with a 0 are not ASCII representable

char keyboard_map[64] = {
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=',8,9,
    'q','w','e','r','t','y','u','i','o','p','[',']',13,0,'a',
    's','d','f','g','h','j','k','l',';','\'','`',0,'\\','z','x',
    'c','v','b','n','m',',','.','/',0,0,0,' '
};

char shifted_keyboard_map[64] = {
    0,27,'!','@','#','$','%','^','&','*','(',')','_','+',8,9,
    'q','w','e','r','t','y','u','i','o','p','[',']',13,0,'a',
    's','d','f','g','h','j','k','l',':','"','~',0,'|','z','x',
    'c','v','b','n','m','<','>','?',0,0,0,' '
};

// These special codes have been defined for representing special key as ASCII characters that wouldn't be used in this app otherwise

int f1Code = 1;
int f2Code = 2;
int suprCode = 3;

int shift=0;
int caps=0;
int specKey=0;


char getChar(){

    int keyCode = read();

    // Shift in
    if(keyCode == 42 || keyCode == 54){
        shift = 1;
        return 15;
    }

    // Shift out
    if(keyCode == 170 || keyCode == 182){
        shift = 0;
        return 14;
    }

    // Caps lock
    if(keyCode == 58){
        caps = caps ? 0 : 1;
        return 14;
    }

    // Special key, has to get the next keyCode
    if(keyCode == 224){
        specKey = specKey ? 0 : 1;
        return getChar();
    }

    // TAB is used for storing the state of registers, so ask for a new char
    if(keyCode == 15)
        return getChar();

    if(keyCode == 59)
        return f1Code;

    if(keyCode == 60)
        return f2Code;

    if(specKey && keyCode == 83)
        return suprCode;


    // Unused key, wait for another one
    if(keyCode >= 129){
        return getChar();
    }
    
    // If it is a letter check for shift/caps
    if((keyCode>=16 && keyCode<=25) || (keyCode>=30 && keyCode<=38) || (keyCode>=44 && keyCode<=50)){
        
        int cap = (shift || caps) && !(shift && caps);
        return cap ? keyboard_map[keyCode]-'a'+'A' : keyboard_map[keyCode];
    
    }else{

        return shift ? shifted_keyboard_map[keyCode] : keyboard_map[keyCode];

    }

}