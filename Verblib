! ----------------------------------------------------------------------------
!  "VERBLIB" header for Inform 6
!
!  (c) Graham Nelson 1993, 1994, 1995, but freely usable (see documentation)
! ----------------------------------------------------------------------------
System_file;
Default MAX_CARRIED  100;
Default MAX_SCORE    0;
Default NUMBER_TASKS 1;
Default OBJECT_SCORE 4;
Default ROOM_SCORE   5;
Default SACK_OBJECT  0;   
Default AMUSING_PROVIDED 1;
Default TASKS_PROVIDED   1;
#IFNDEF task_scores; Constant MAKE__TS; #ENDIF;
#IFDEF MAKE__TS;
Array  task_scores --> 0 0;
#ENDIF;
Array  task_done --> NUMBER_TASKS;
#IFDEF USE_MODULES;
Link "VerblibM";
#IFNOT;
Include "VerblibM";
#ENDIF;
