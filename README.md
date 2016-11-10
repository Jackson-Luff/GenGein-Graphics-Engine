# GenGein-Graphics-Engine
The development of GenGein 

**DISCLAIMER:**
GenGein is an on-going expreiment that has one general function - to be my very own experimental playing field. Here I will design, create and play to inevitably shape a functional engine.
Full disclaimer; this engine is for personal-use only. Bug submittions [When I make the spreadsheet] and use is welcome, however I will not be taking on board discrete 'ideas'.

## Functionality
'static_cast<TO_BE_DEFINED> list;'

## Internal Debugging

### COMMENTING
Generally I try to keep things pretty simple. For debugging purposes I've marked in places to remind myself what I need to do. 

1. "#NOTE:" is marking a note, for whatever it shall be. (e.g optimisation/idea)
2. "#TODO:" is marking an area in which is lacking or can be improved upon. (e.g effiective/efficiency)

### Console Logging
When utilising Console::Log() [printf() wrap] use the following to tag the desired log

1. "#WAR:" when logging a warning.
2. "#SUC:" when logging a success.
3. "#ERR:" when logging a error.
4. If a log is called without defining a tag, the default console properties are assigned.