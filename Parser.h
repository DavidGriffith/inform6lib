! ----------------------------------------------------------------------------
!  PARSER:  Front end to parser.
!
!  Supplied for use with Inform 6
!
!  (c) Graham Nelson 1993, 1994, 1995, 1996, 1997
!      but freely usable (see manuals)
! ----------------------------------------------------------------------------

Constant LibSerial  = "970405";
Constant LibRelease = "6/5";
Constant Grammar__Version = 2;
IFNDEF VN_1610;
Message fatalerror "*** Library 6/5 needs Inform v6.10 or later to work ***";
ENDIF;
System_file;
Include "linklpa";
Fake_Action LetGo;
Fake_Action Receive;
Fake_Action ThrownAt;
Fake_Action Order;
Fake_Action TheSame;
Fake_Action PluralFound;
Fake_Action ListMiscellany;
Fake_Action Miscellany;
Fake_Action Prompt;
Fake_Action NotUnderstood;
IFDEF NO_PLACES;
Fake_Action Places;
Fake_Action Objects;
ENDIF;
[ Main; InformLibrary.play(); ];

IFDEF USE_MODULES;
Link "parserm";
IFNOT;
Include "parserm";
ENDIF;
