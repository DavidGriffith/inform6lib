! ----------------------------------------------------------------------------
!  PARSER:  Front end to parser.
!
!  Supplied for use with Inform 6
!
!  (c) Graham Nelson 1993, 1994, 1995, 1996 but freely usable (see manuals)
! ----------------------------------------------------------------------------

Constant LibSerial "960912";
Constant LibRelease "6/2";
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

IFDEF USE_MODULES;
Link "parserm";
IFNOT;
Include "parserm";
ENDIF;
