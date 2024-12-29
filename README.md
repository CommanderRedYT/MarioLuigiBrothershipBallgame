# Mario Luigi Brothership Ballgame

This is a script for beating the **Mario & Luigi Brothership** Ball minigames. It uses a esp32 connected to a relay board which has wires going to a old 8bitdo bluetooth controller (as i unfortunately could not get any linux-based controller emulation working). The wires are soldered onto the two sides of the push buttons of the analog sticks as these were the easiest to solder to. After that, I just had to remap the analog stick buttons to X/Y buttons to make mario and luigi hit the ball.

This is more a script for trying to get the timings about right to reach the 100 hit goal. As this is just a script for pressing buttons over and over again, it could easily be used for things like the mario odyssey jump-rope game.

Unfortunately, this script doesn't always work because Mario & Luigi Brothership is a very unoptimized game and when the game lags, the script will not work. So if it does not work, just give it another try.