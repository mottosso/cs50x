/**
 * bounce.c
 *
 * David J. Malan
 * malan@harvard.edu
 *
 * Bounces a circle back and forth in a window.
 */

// standard libraries
#include <stdio.h>
#include <string.h>

// Stanford Portable Library
#include "spl/gevents.h"
#include "spl/ginteractors.h"
#include "spl/gwindow.h"


int main(void)
{
    // instantiate window
    GWindow window = newGWindow(320, 240);
    
    GCheckBox checkBox = newGCheckBox("I agree");
    setActionCommand(checkBox, "check");
    addToRegion(window, checkBox, "SOUTH");
    
    while (true)
    {
        GActionEvent event = waitForEvent(ACTION_EVENT);
        
        if (getEventType(event) == WINDOW_CLOSED)
        {
            break;
        }
        
        if (strcmp(getActionCommand(event), "check") == 0)
        {
            if (isSelected(checkBox))
            {
                printf("Checkbox was checked\n");
            }
            else
            {
                printf("Checkbox was unchecked\n");
            }
        }
    }
    
    closeGWindow(window);
    return 0;
}
