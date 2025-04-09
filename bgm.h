#ifndef MARIO_THEME_H
#define MARIO_THEME_H

#include "analogSound.h"

NoteWithDuration marioTheme[] = {
    {NOTE_E4, 125}, {NOTE_E4, 125}, {REST, 125}, {NOTE_E4, 125},
    {REST, 167}, {NOTE_C4, 125}, {NOTE_E4, 125}, {REST, 125}, {NOTE_G4, 125},
    {REST, 375}, {NOTE_G3, 125}, {REST, 375},

    {NOTE_C4, 125}, {REST, 250}, {NOTE_G3, 125}, {REST, 250},
    {NOTE_E3, 125}, {REST, 250},

    {NOTE_A3, 125}, {REST, 125}, {NOTE_B3, 125}, {REST, 125},
    {NOTE_AS3, 125}, {NOTE_A3, 125}, {REST, 125},

    {NOTE_G3, 84}, {NOTE_E4, 84}, {NOTE_G4, 84}, {NOTE_A4, 125},
    {NOTE_F4, 84}, {NOTE_G4, 125}, {REST, 125}, {NOTE_E4, 125},
    {NOTE_C4, 84}, {NOTE_D4, 84}, {NOTE_B3, 125}, {REST, 250},

    {NOTE_C4, 125}, {REST, 250}, {NOTE_G3, 125}, {REST, 250},
    {NOTE_E3, 125}, {REST, 250},

    {NOTE_A3, 125}, {REST, 125}, {NOTE_B3, 125}, {REST, 125},
    {NOTE_AS3, 125}, {NOTE_A3, 125}, {REST, 125},

    {NOTE_G3, 84}, {NOTE_E4, 84}, {NOTE_G4, 84}, {NOTE_A4, 125},
    {NOTE_F4, 84}, {NOTE_G4, 125}, {REST, 125}, {NOTE_E4, 125},
    {NOTE_C4, 84}, {NOTE_D4, 84}, {NOTE_B3, 125}, {REST, 375},

};

#define MARIO_THEME_LENGTH (sizeof(marioTheme) / sizeof(marioTheme[0]))

#endif 
