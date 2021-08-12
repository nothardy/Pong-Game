# Pong-Game

Pong Game in C language, implemented on a PIC16F887 (microcontroller)

# How it works?

This setup contains a pcb board adapted for a PIC16F887 microcontroller, 5 buttons and two led matrices that represents the screen (or field).

- Middle button is a kick-off button that sets the "ball" on motion.
- This ball is represented by a single led turning on and off consecutely as if it were a pixel. This can be seen in "Trayectoria" function.
- Left and Right buttons each side move the player's bar respectively.
- If the ball touches left or right screen limits, its a goal. First player to reach 10 goals wins.
- If ball hits a bar or top/bottom limits, it will ricochet in a 45° degree angle.

# Can I try this at home?

Not really, unless you have a PIC16F887 and a board to handle it :(

BUT, here is a demonstration of how it moves, how each goal is represented and what happens when a player wins ---> DEMO IN PROGRESS
