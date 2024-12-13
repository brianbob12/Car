#ifndef ACTIVITY_H
#define ACTIVITY_H

/**
 * Starts the activity by adding a buch of actions to the action queue.
 * 
 * This will aslo clear the action queue and abort any current action.
 */
void startActivity();

/**
 * Sets the x-coordinate offset used for position adjustments during the activity.
 * @param offset The x offset value in millimeters
 */
void set_x_offset(int offset);

/**
 * Sets the y-coordinate offset used for position adjustments during the activity.
 * @param offset The y offset value in millimeters 
 */
void set_y_offset(int offset);

#endif