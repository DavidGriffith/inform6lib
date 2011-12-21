! ----------------------------------------------------------------------------
!  "PARSER": The core of the Inform library
!
!  Supplied for use with Inform 5.5
!
!  (c) Graham Nelson 1993, 1994, 1995, but freely usable (see documentation)
! ----------------------------------------------------------------------------

Constant LibSerial "960429";
Constant LibRelease "6/1";
System_file;
Constant MAX_TIMERS  64;
Array  the_timers  -> MAX_TIMERS;
Array  timer_flags -> MAX_TIMERS;
Array  buffer          string 120;
Array  parse           string 64;
Include "linklpa";
Fake_Action LetGo;
Fake_Action Receive;
Fake_Action ThrownAt;
Fake_Action Order;
Fake_Action TheSame;
Fake_Action PluralFound;
Fake_Action Miscellany;
Fake_Action Prompt;
Fake_Action NotUnderstood;
[ Main; PlayTheGame(); ];

!trace symbols;
!message "Hello!";

IFDEF USE_MODULES;
Link "ParserM";
IFNOT;
!message "And here!";
Include "ParserM";
ENDIF;

!message "Goodbye!";
