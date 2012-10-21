! ==============================================================================
!   ROUTINES:  Assorted routines and objects to support the parser
!
!   Supplied for use with Inform 6 -- Release 6/12 -- Serial number 121020
!
!   Copyright Graham Nelson 1993-2012 but freely usable (see manuals)
!
!   This file is automatically Included in your game file by "Parser".
! ------------------------------------------------------------------------------
!   During development of 6/12 it was noticed that too much stuff was
!   crammed into parserm.h.  V5 modules cannot exceed 65535 bytes, so
!   various auxiliary routines were moved into this new file.
! ------------------------------------------------------------------------------
!
!   The InformLibrary object
!       main game loop
!       action processing
!       end of turn sequence
!       scope looping, before/after sequence, sending messages out
!       timers, daemons, time of day, score notification
!       light and darkness
!       changing player personality
!       tracing code (only present if DEBUG is set)

!   LibraryExtensions object
!
!   Input / Output routines
!   Status line printing, menu display
!   Printing object names with articles
!   Miscellaneous utility routines
!   Game banner, "version" verb, run-time errors
! ==============================================================================

System_file;

#Ifdef MODULE_MODE;
Constant DEBUG;
Constant Grammar__Version 2;
Include "linklpa";
Include "linklv";
import global no_infer_message;
import global gg_statuswin_cursize;
#Endif; ! MODULE_MODE


Object  InformLibrary "(Inform Library)"
  with  play [ i j k l;

            no_infer_message = false;

            #Ifdef TARGET_ZCODE;
            ZZInitialise();
            #Ifnot; ! TARGET_GLULX
            GGInitialise();
            #Endif; ! TARGET_

            GamePrologue();
            while (~~deadflag) {    ! everything happens in this loop

                #Ifdef EnglishNaturalLanguage;
                PronounOldEnglish();
                old_itobj = PronounValue('it');
                old_himobj = PronounValue('him');
                old_herobj = PronounValue('her');
                #Endif; ! EnglishNaturalLanguage

              .very__late__error;

                if (score ~= last_score) {
                    if (notify_mode == 1) NotifyTheScore();
                    last_score = score;
                }

              .late__error;

                inputobjs-->0 = 0; inputobjs-->1 = 0;
                inputobjs-->2 = 0; inputobjs-->3 = 0; meta = false;

                ! The Parser writes its results into inputobjs and meta,
                ! a flag indicating a "meta-verb".  This can only be set for
                ! commands by the player, not for orders to others.

                InformParser.parse_input(inputobjs);

                action = inputobjs-->0;

                ! --------------------------------------------------------------

                ! Reverse "give fred biscuit" into "give biscuit to fred"

                if (action == ##GiveR or ##ShowR) {
                    i = inputobjs-->2; inputobjs-->2 = inputobjs-->3; inputobjs-->3 = i;
                    if (action == ##GiveR) action = ##Give; else action = ##Show;
                }

                ! Convert "P, tell me about X" to "ask P about X"

                if (action == ##Tell && inputobjs-->2 == player && actor ~= player) {
                    inputobjs-->2 = actor; actor = player; action = ##Ask;
                }

                ! Convert "ask P for X" to "P, give X to me"

                if (action == ##AskFor && inputobjs-->2 ~= player && actor == player) {
                    actor = inputobjs-->2; inputobjs-->2 = inputobjs-->3;
                    inputobjs-->3 = player; action = ##Give;
                }

                ! For old, obsolete code: special_word contains the topic word
                ! in conversation

                if (action == ##Ask or ##Tell or ##Answer)
                    special_word = special_number1;

                !  --------------------------------------------------------------

                multiflag = false; onotheld_mode = notheld_mode; notheld_mode = false;
                ! For implicit taking and multiple object detection

              .begin__action;
                inp1 = 0; inp2 = 0; i = inputobjs-->1;
                if (i >= 1) inp1 = inputobjs-->2;
                if (i >= 2) inp2 = inputobjs-->3;

                ! inp1 and inp2 hold: object numbers, or 0 for "multiple object",
                ! or 1 for "a number or dictionary address"

                if (inp1 == 1) noun = special_number1; else noun = inp1;
                if (inp2 == 1) {
                    if (inp1 == 1) second = special_number2;
                    else           second = special_number1;
                }
                else second = inp2;

                ! --------------------------------------------------------------
                ! Generate the action...

                if ((i == 0) ||
                    (i == 1 && inp1 ~= 0) ||
                    (i == 2 && inp1 ~= 0 && inp2 ~= 0)) {
                    if (self.actor_act(actor, action, noun, second)) jump begin__action;
                    jump turn__end;
                }

                ! ...unless a multiple object must be substituted.  First:
                ! (a) check the multiple list isn't empty;
                ! (b) warn the player if it has been cut short because too long;
                ! (c) generate a sequence of actions from the list
                !     (stopping in the event of death or movement away).

                multiflag = true;
                j = multiple_object-->0;
                if (j == 0) {
                    L__M(##Miscellany, 2);
                    jump late__error;
                }
                if (toomany_flag) {
                    toomany_flag = false;
                    L__M(##Miscellany, 1);
                }
                i = location;
                for (k=1 : k<=j : k++) {
                    if (deadflag) break;
                    if (location ~= i) {
                        L__M(##Miscellany, 51);
                        break;
                    }
                    l = multiple_object-->k;
                    PronounNotice(l);
                    print (name) l, ": ";
                    if (inp1 == 0) {
                        inp1 = l;
                        if (self.actor_act(actor, action, l, second)) jump begin__action;
                        inp1 = 0;
                    }
                    else {
                        inp2 = l;
                        if (self.actor_act(actor, action, noun, l)) jump begin__action;
                        inp2 = 0;
                    }
                }

                ! --------------------------------------------------------------

              .turn__end;

                keep_silent = 0;    ! should be zero anyway, but just in case...

                ! No time passes if either (i) the verb was meta, or
                ! (ii) we've only had the implicit take before the "real"
                ! action to follow.

                if (notheld_mode == 1) { NoteObjectAcquisitions(); continue; }
                if (meta) continue;
                if (~~deadflag) self.end_turn_sequence();
                else if (START_MOVE ~= 1) turns++;

            } ! end of while()

            if (deadflag ~= 2 && AfterLife() == false)
                 LibraryExtensions.RunAll(ext_afterlife);
            if (deadflag == 0) jump very__late__error;
            GameEpilogue();
        ], ! end of 'play' property

        end_turn_sequence [;
            AdvanceWorldClock();        if (deadflag) return;
            RunTimersAndDaemons();      if (deadflag) return;
            RunEachTurnProperties();    if (deadflag) return;
            if (TimePasses() == 0)  LibraryExtensions.RunAll(ext_timepasses);
                                        if (deadflag) return;
            AdjustLight();              if (deadflag) return;
            NoteObjectAcquisitions();
        ],

        actor_act [ p a n s  j sp sa sn ss;
            sp = actor; actor = p;
            if (p ~= player) {

                ! The player's "orders" property can refuse to allow
                ! conversation here, by returning true.  If not, the order is
                ! sent to the other person's "orders" property.  If that also
                ! returns false, then: if it was a misunderstood command
                ! anyway, it is converted to an Answer action (thus
                ! "floyd, grrr" ends up as "say grrr to floyd").  If it was a
                ! good command, it is finally offered to the Order: part of
                ! the other person's "life" property, the old-fashioned
                ! way of dealing with conversation.

                sa = action; sn = noun; ss = second;
                action = a; noun = n; second = s;
                j = RunRoutines(player, orders);
                if (j == 0) {
                    j = RunRoutines(actor, orders);
                    if (j == 0) {
                        if (action == ##NotUnderstood) {
                            inputobjs-->3 = actor; actor = player; action = ##Answer;
                            rtrue; ! abort, not resetting action globals
                        }
                        if (RunLife(actor, ##Order) == 0) L__M(##Order, 1, actor);
                    }
                }
                action = sa; noun = sn; second = ss;
            }
            else
                self.begin_action(a, n, s, 0);
            actor = sp;
            ],

        begin_action [ a n s source   sa sn ss;
            sa = action; sn = noun; ss = second;
            action = a; noun = n; second = s;
            #Ifdef DEBUG;
            if (debug_flag & DEBUG_ACTIONS) TraceAction(source);
            #Ifnot;
            source = 0;
            #Endif; ! DEBUG
            #Iftrue (Grammar__Version == 1);
            if ((meta || BeforeRoutines() == false) && action < 256)
                ActionPrimitive();
            #Ifnot;
            if ((meta || BeforeRoutines() == false) && action < 4096)
                ActionPrimitive();
            #Endif; ! Grammar__Version
            action = sa; noun = sn; second = ss;
        ],
  has   proper;


Object  LibraryExtensions "(Library Extensions)"
  with  RunAll [ prop a1 a2 a3
            obj rval max;
            objectloop (obj in self)
                if (obj provides prop && obj.prop ofclass Routine) {
                    rval = obj.prop(a1, a2, a3);
                    if (rval > max) max = rval;
                    if (self.BetweenCalls) self.BetweenCalls();
                }
            return max;
        ],
        RunUntil [ prop exitval a1 a2 a3
            obj rval;
            objectloop (obj in self)
                if (obj provides prop && obj.prop ofclass Routine) {
                    rval = obj.prop(a1, a2, a3);
                    if (rval == exitval) return rval;
                    if (self.BetweenCalls) self.BetweenCalls();
                }
            return ~~exitval;
        ],
        RunWhile [ prop exitval a1 a2 a3
            obj rval;
            objectloop (obj in self)
                if (obj provides prop && obj.prop ofclass Routine) {
                    rval = obj.prop(a1, a2, a3);
                    if (rval ~= exitval) return rval;
                    if (self.BetweenCalls) self.BetweenCalls();
                }
            return exitval;
        ],

        ext_number_1 0, ! general temporary workspace

        ! can be set to a function (e.g. RestoreWN) meant for execution
        ! after non-terminating calls to extension objects
        ! (via RunUntil/While/All)
        BetweenCalls 0,
        RestoreWN [; wn = self.ext_number_1; ],

        ! Special interception points
        ext_messages            0,  ! Called if LibraryMessages.before()
                                    !    returns false
                                    ! Extensions run while they return false

        ! Cross-platform entry points
        !                             Called/Runs
        ext_afterlife           0,  ! [C2/R1]
        ext_afterprompt         0,  ! [C2/R1]
        ext_amusing             0,  ! [C2/R1]
        ext_beforeparsing       0,  ! [C2/R2]
        ext_chooseobjects       0,  ! [C2/R2]
        ext_darktodark          0,  ! [C2/R1]
        ext_deathmessage        0,  ! [C2/R1]
        ext_gamepostroutine     0,  ! [C2/R2]
        ext_gamepreroutine      0,  ! [C2/R2]
        ext_initialise          0,  ! [C1/R1]
        ext_inscope             0,  ! [C2/R2]
        ext_lookroutine         0,  ! [C2/R1]
        ext_newroom             0,  ! [C2/R1]
        ext_objectdoesnotfit    0,  ! [C2/R2]
        ext_parsenoun           0,  ! [C3/R3]
        ext_parsenumber         0,  ! [C2/R2]
        ext_parsererror         0,  ! [C2/R2]
        ext_printrank           0,  ! [C2/R1]
        ext_printtaskname       0,  ! [C2/R1]
        ext_printverb           0,  ! [C2/R2]
        ext_timepasses          0,  ! [C2/R1]
        ext_unknownverb         0,  ! [C2/R2]
        !                             [C1] = Called in all cases
        !                             [C2] = Called if EP is undefined, or returns false
        !                             [C3] = called if EP is undefined, or returns -1
        !                             [R1] = All extensions run
        !                             [R2] = Extensions run while they return false
        !                             [R3] = Extensions run while they return -1
        #Ifdef TARGET_GLULX;
        ! Glulx entry points
        !                             Called:           Runs:
        ext_handleglkevent      0,  ! if EP undefined   while extensions return false
        ext_identifyglkobject   0,  ! if EP undefined   while extensions return false
        ext_initglkwindow       0,  ! if EP undefined   while extensions return false
        #Endif; ! TARGET_GLULX;
        has   proper;


!=============================================================================
! Input / Output Routines
!=============================================================================

! ----------------------------------------------------------------------------
!   The Keyboard routine actually receives the player's words,
!   putting the words in "a_buffer" and their dictionary addresses in
!   "a_table".  It is assumed that the table is the same one on each
!   (standard) call.
!
!   It can also be used by miscellaneous routines in the game to ask
!   yes-no questions and the like, without invoking the rest of the parser.
!
!   Return the number of words typed
! ----------------------------------------------------------------------------

#Ifdef TARGET_ZCODE;

[ KeyboardPrimitive  a_buffer a_table;
    read a_buffer a_table;

    #Iftrue (#version_number == 6);
    @output_stream -1;
    @loadb a_buffer 1 -> sp;
    @add a_buffer 2 -> sp;
    @print_table sp sp;
    new_line;
    @output_stream 1;
    #Endif;
];

[ KeyCharPrimitive win  key;
    if (win) @set_window win;
    @read_char 1 -> key;
    return key;
];

[ KeyTimerInterrupt;
    rtrue;
];

[ KeyDelay tenths  key;
    @read_char 1 tenths KeyTimerInterrupt -> key;
    return key;
];

#Ifnot; ! TARGET_GLULX

[ KeyCharPrimitive win nostat done res ix jx ch;
    jx = ch; ! squash compiler warnings
    if (win == 0) win = gg_mainwin;
    if (gg_commandstr ~= 0 && gg_command_reading ~= false) {
        ! get_line_stream
        done = glk($0091, gg_commandstr, gg_arguments, 31);
        if (done == 0) {
            glk($0044, gg_commandstr, 0); ! stream_close
            gg_commandstr = 0;
            gg_command_reading = false;
            ! fall through to normal user input.
        }
        else {
            ! Trim the trailing newline
            if (gg_arguments->(done-1) == 10) done = done-1;
            res = gg_arguments->0;
            if (res == '\') {
                res = 0;
                for (ix=1 : ix<done : ix++) {
                    ch = gg_arguments->ix;
                    if (ch >= '0' && ch <= '9') {
                        @shiftl res 4 res;
                        res = res + (ch-'0');
                    }
                    else if (ch >= 'a' && ch <= 'f') {
                        @shiftl res 4 res;
                        res = res + (ch+10-'a');
                    }
                    else if (ch >= 'A' && ch <= 'F') {
                        @shiftl res 4 res;
                        res = res + (ch+10-'A');
                    }
                }
            }
        jump KCPContinue;
        }
    }
    done = false;
    glk($00D2, win); ! request_char_event
    while (~~done) {
        glk($00C0, gg_event); ! select
        switch (gg_event-->0) {
          5: ! evtype_Arrange
            if (nostat) {
                glk($00D3, win); ! cancel_char_event
                res = $80000000;
                done = true;
                break;
            }
            DrawStatusLine();
          2: ! evtype_CharInput
            if (gg_event-->1 == win) {
                res = gg_event-->2;
                done = true;
                }
        }
        ix = HandleGlkEvent(gg_event, 1, gg_arguments);
        if (ix == 0) ix = LibraryExtensions.RunWhile(ext_handleglkevent, 0, gg_event, 1, gg_arguments);
        if (ix == 2) {
            res = gg_arguments-->0;
            done = true;
        }
        else if (ix == -1) {
            done = false;
        }
    }
    if (gg_commandstr ~= 0 && gg_command_reading == false) {
        if (res < 32 || res >= 256 || (res == '\' or ' ')) {
            glk($0081, gg_commandstr, '\'); ! put_buffer_char
            done = 0;
            jx = res;
            for (ix=0 : ix<8 : ix++) {
                @ushiftr jx 28 ch;
                @shiftl jx 4 jx;
                ch = ch & $0F;
                if (ch ~= 0 || ix == 7) done = 1;
                if (done) {
                    if (ch >= 0 && ch <= 9) ch = ch + '0';
                    else                    ch = (ch - 10) + 'A';
                    glk($0081, gg_commandstr, ch); ! put_buffer_char
                }
            }
        }
        else {
            glk($0081, gg_commandstr, res); ! put_buffer_char
        }
        glk($0081, gg_commandstr, 10); ! put_char_stream (newline)
    }
  .KCPContinue;
    return res;
];

[ KeyDelay tenths  key done ix;
    glk($00D2, gg_mainwin); ! request_char_event
    glk($00D6, tenths*100); ! request_timer_events
    while (~~done) {
        glk($00C0, gg_event); ! select
        ix = HandleGlkEvent(gg_event, 1, gg_arguments);
        if (ix == 0) ix = LibraryExtensions.RunWhile(ext_handleglkevent, 0, gg_event, 1, gg_arguments);
        if (ix == 2) {
            key = gg_arguments-->0;
            done = true;
        }
        else if (ix >= 0 && gg_event-->0 == 1 or 2) {
            key = gg_event-->2;
            done = true;
        }
    }
    glk($00D3, gg_mainwin); ! cancel_char_event
    glk($00D6, 0); ! request_timer_events
    return key;
];

[ KeyboardPrimitive  a_buffer a_table done ix;
    if (gg_commandstr ~= 0 && gg_command_reading ~= false) {
        ! get_line_stream
        done = glk($0091, gg_commandstr, a_buffer+WORDSIZE, (INPUT_BUFFER_LEN-WORDSIZE)-1);
        if (done == 0) {
            glk($0044, gg_commandstr, 0); ! stream_close
            gg_commandstr = 0;
            gg_command_reading = false;
            ! L__M(##CommandsRead, 5); would come after prompt
            ! fall through to normal user input.
        }
        else {
            ! Trim the trailing newline
            if ((a_buffer+WORDSIZE)->(done-1) == 10) done = done-1;
            a_buffer-->0 = done;
            glk($0086, 8); ! set input style
            glk($0084, a_buffer+WORDSIZE, done); ! put_buffer
            glk($0086, 0); ! set normal style
            print "^";
            jump KPContinue;
        }
    }
    done = false;
    glk($00D0, gg_mainwin, a_buffer+WORDSIZE, INPUT_BUFFER_LEN-WORDSIZE, 0); ! request_line_event
    while (~~done) {
        glk($00C0, gg_event); ! select
        switch (gg_event-->0) {
          5: ! evtype_Arrange
            DrawStatusLine();
          3: ! evtype_LineInput
            if (gg_event-->1 == gg_mainwin) {
                a_buffer-->0 = gg_event-->2;
                done = true;
            }
        }
        ix = HandleGlkEvent(gg_event, 0, a_buffer);
        if (ix == 0) ix = LibraryExtensions.RunWhile(ext_handleglkevent, 0, gg_event, 0, a_buffer);
        if (ix == 2) done = true;
        else if (ix == -1) done = false;
    }
    if (gg_commandstr ~= 0 && gg_command_reading == false) {
        ! put_buffer_stream
        glk($0085, gg_commandstr, a_buffer+WORDSIZE, a_buffer-->0);
        glk($0081, gg_commandstr, 10); ! put_char_stream (newline)
    }
  .KPContinue;
    Tokenise__(a_buffer,a_table);
    ! It's time to close any quote window we've got going.
    if (gg_quotewin) {
        glk($0024, gg_quotewin, 0); ! close_window
        gg_quotewin = 0;
    }
];

#Endif; ! TARGET_

[ Keyboard  a_buffer a_table  nw i w w2 x1 x2;
    DisplayStatus();

  .FreshInput;

    ! Save the start of the buffer, in case "oops" needs to restore it
    ! to the previous time's buffer

    for (i=0 : i<OOPS_WORKSPACE_LEN : i++) oops_workspace->i = a_buffer->i;

    ! In case of an array entry corruption that shouldn't happen, but would be
    ! disastrous if it did:

    #Ifdef TARGET_ZCODE;
    a_buffer->0 = INPUT_BUFFER_LEN - WORDSIZE;
    a_table->0  = MAX_BUFFER_WORDS; ! Allow to split input into this many words
    #Endif; ! TARGET_

    ! Print the prompt, and read in the words and dictionary addresses

    L__M(##Prompt);
    if (AfterPrompt() == 0) LibraryExtensions.RunAll(ext_afterprompt);
    #IfV5;
    DrawStatusLine();
    #Endif; ! V5
    KeyboardPrimitive(a_buffer, a_table);
    nw = NumberWords(a_table);

    ! If the line was blank, get a fresh line
    if (nw == 0) {
        L__M(##Miscellany, 10);
        jump FreshInput;
    }

    ! Unless the opening word was "oops", return
    ! Conveniently, a_table-->1 is the first word in both ZCODE and GLULX.

    w = a_table-->1;
    if (w == OOPS1__WD or OOPS2__WD or OOPS3__WD) jump DoOops;

    if (a_buffer->WORDSIZE == COMMENT_CHARACTER) {
        #Ifdef TARGET_ZCODE;
        if ((HDR_GAMEFLAGS-->0) & $0001 || xcommsdir)
                                           L__M(##Miscellany, 54);
        else                               L__M(##Miscellany, 55);
        #Ifnot; ! TARGET_GLULX
        if (gg_scriptstr || gg_commandstr) L__M(##Miscellany, 54);
        else                               L__M(##Miscellany, 55);
        #Endif; ! TARGET_

        jump FreshInput;
    }

    #IfV5;
    ! Undo handling

    if ((w == UNDO1__WD or UNDO2__WD or UNDO3__WD) && (nw==1)) {
        if (turns == START_MOVE) {
            L__M(##Miscellany, 11);
            jump FreshInput;
        }
        if (undo_flag == 0) {
            L__M(##Miscellany, 6);
            jump FreshInput;
        }
        if (undo_flag == 1) jump UndoFailed;
        #Ifdef TARGET_ZCODE;
        @restore_undo i;
        #Ifnot; ! TARGET_GLULX
        @restoreundo i;
        i = (~~i);
        #Endif; ! TARGET_
        if (i == 0) {
          .UndoFailed;
            L__M(##Miscellany, 7);
        }
        jump FreshInput;
    }
    #Ifdef TARGET_ZCODE;
    @save_undo i;
    #Ifnot; ! TARGET_GLULX
    @saveundo i;
    if (i == -1) {
        GGRecoverObjects();
        i = 2;
    }
    else  i = (~~i);
    #Endif; ! TARGET_
    just_undone = 0;
    undo_flag = 2;
    if (i == -1) undo_flag = 0;
    if (i == 0) undo_flag = 1;
    if (i == 2) {
        RestoreColours();
        #Ifdef TARGET_ZCODE;
        style bold;
        #Ifnot; ! TARGET_GLULX
        glk($0086, 4); ! set subheader style
        #Endif; ! TARGET_
        print (name) location, "^";
        #Ifdef TARGET_ZCODE;
        style roman;
        #Ifnot; ! TARGET_GLULX
        glk($0086, 0); ! set normal style
        #Endif; ! TARGET_
        L__M(##Miscellany, 13);
        just_undone = 1;
        jump FreshInput;
    }
    #Endif; ! V5

    return nw;

  .DoOops;
    if (oops_from == 0) {
        L__M(##Miscellany, 14);
        jump FreshInput;
    }
    if (nw == 1) {
        L__M(##Miscellany, 15);
        jump FreshInput;
    }
    if (nw > 2) {
        L__M(##Miscellany, 16);
        jump FreshInput;
    }

    ! So now we know: there was a previous mistake, and the player has
    ! attempted to correct a single word of it.

    for (i=0 : i<INPUT_BUFFER_LEN : i++) buffer2->i = a_buffer->i;
    #Ifdef TARGET_ZCODE;
    x1 = a_table->9;  ! Start of word following "oops"
    x2 = a_table->8;  ! Length of word following "oops"
    #Ifnot; ! TARGET_GLULX
    x1 = a_table-->6; ! Start of word following "oops"
    x2 = a_table-->5; ! Length of word following "oops"
    #Endif; ! TARGET_

    ! Repair the buffer to the text that was in it before the "oops"
    ! was typed:

    for (i=0 : i < OOPS_WORKSPACE_LEN : i++) a_buffer->i = oops_workspace->i;
    Tokenise__(a_buffer, a_table);

    ! Work out the position in the buffer of the word to be corrected:

    #Ifdef TARGET_ZCODE;
    w = a_table->(4*oops_from + 1); ! Start of word to go
    w2 = a_table->(4*oops_from);    ! Length of word to go

    #Ifnot; ! TARGET_GLULX
    w = a_table-->(3*oops_from);      ! Start of word to go
    w2 = a_table-->(3*oops_from - 1); ! Length of word to go
    #Endif; ! TARGET_

    ! Write spaces over the word to be corrected:

    for (i=0 : i<w2 : i++) a_buffer->(i+w) = ' ';

    if (w2 < x2) {
        ! If the replacement is longer than the original, move up...
        for (i=INPUT_BUFFER_LEN-1 : i>=w+x2 : i--)
            a_buffer->i = a_buffer->(i-x2+w2);

        ! ...increasing buffer size accordingly.
        SetKeyBufLength(GetKeyBufLength(a_buffer) + (x2-w2), a_buffer);
    }

    ! Write the correction in:

    for (i=0 : i<x2 : i++) a_buffer->(i+w) = buffer2->(i+x1);

    Tokenise__(a_buffer, a_table);
    nw=NumberWords(a_table);

    return nw;
]; ! end of Keyboard

#Ifdef TARGET_ZCODE;

! Platform-independent way of printing strings, routines and properties
! to a buffer (defined as length word followed by byte characters).

[ PrintToBuffer buf len a b c d e;
    print_anything_result = 0;
    @output_stream 3 buf;
    switch (metaclass(a)) {
      String:
        print (string) a;
      Routine:
        print_anything_result = a(b, c, d, e);
      Object,Class:
        if (b)
            print_anything_result = PrintOrRun(a, b, true);
        else
            print (name) a;
    }
    @output_stream -3;
    if (buf-->0 > len) RunTimeError(14, len, "in PrintToBuffer()");
    return buf-->0;
];

#Ifnot; ! TARGET_GLULX

[ PrintToBuffer buf len a b c d e;
    if (b) {
        if (metaclass(a) == Object && a.#b == WORDSIZE
            && metaclass(a.b) == String)
            buf-->0 = PrintAnyToArray(buf+WORDSIZE, len, a.b);
        else
            buf-->0 = PrintAnyToArray(buf+WORDSIZE, len, a, b, c, d, e);
    }
    else
        buf-->0 = PrintAnyToArray(buf+WORDSIZE, len, a);
    if (buf-->0 > len) buf-->0 = len;
    return buf-->0;
];

#Endif; ! TARGET_

! Print contents of buffer (defined as length word followed by byte characters).
! no_break == 1: omit trailing newline.
! set_case == 1: capitalise first letter;
!          == 2: capitalise first letter, remainder lower case;
!          == 3: all lower case;
!          == 4: all upper case.
! centred == 1:  add leading spaces.

[ PrintFromBuffer buf no_break set_case centred
    i j k;
    j = (buf-->0) - 1;
    if (buf->(j+WORDSIZE) ~= 10 or 13) j++;     ! trim any trailing newline
    if (centred) {
        k = (ScreenWidth() - j) / 2;
        if (k>0) spaces k;
    }
    for (i=0 : i<j : i++) {
        k = buf->(WORDSIZE+i);
        switch (set_case) {
          0:    break;
          1:    if (i) set_case = 0;
                else   k = UpperCase(k);
          2:    if (i) k = LowerCase(k);
                else   k = UpperCase(k);
          3:           k = LowerCase(k);
          4:           k = UpperCase(k);
        }
        print (char) k;
    }
    if (no_break == false) new_line;
    return j;
];


! ----------------------------------------------------------------------------
! Routines called before and after main 'play' loop

[ GamePrologue i j;
    before_first_turn = true;
    for (i=1 : i<=100 : i++) j = random(i);

    ChangeDefault(cant_go, CANTGO__TX);

    real_location = thedark;
    player = selfobj; actor = player;
    selfobj.capacity = MAX_CARRIED; ! ### change?

    #Ifdef LanguageInitialise;
    LanguageInitialise();
    #Endif; ! LanguageInitialise

    #Ifdef EnglishNaturalLanguage;
    old_itobj = itobj; old_himobj = himobj; old_herobj = herobj;
    #Endif;! EnglishNaturalLanguage

    new_line;
    LibraryExtensions.RunAll(ext_initialise);
    j = Initialise();
    last_score = score;
    initial_lookmode = lookmode;

    objectloop (i in player) give i moved ~concealed;
    move player to location;
    while (parent(location)) location = parent(location);
    real_location = location;
    MoveFloatingObjects();

    actor = player; ! resync, because player may have been changed in Initialise()
    actors_location = location;

    lightflag = OffersLight(parent(player));
    if (lightflag == 0) location = thedark;

    if (j ~= 2) Banner();
#ifndef NOINITIAL_LOOK;
    <Look>;
#endif;
    before_first_turn = false;
];

[ GameEpilogue;
    print "^^    ";
    #Ifdef TARGET_ZCODE;
    #IfV5; style bold; #Endif; ! V5
    #Ifnot; ! TARGET_GLULX
    glk($0086, 5); ! set alert style
    #Endif; ! TARGET_
    print "***";
    switch (deadflag) {
      1:        L__M(##Miscellany, 3);
      2:        L__M(##Miscellany, 4);
      default:  print " ";
                if (DeathMessage() == false)
                    LibraryExtensions.RunAll(ext_deathmessage);
                print " ";
    }
    print "***";
    #Ifdef TARGET_ZCODE;
    #IfV5; style roman; #Endif; ! V5
    #Ifnot; ! TARGET_GLULX
    glk($0086, 0); ! set normal style
    #Endif; ! TARGET_
    print "^^";
    #Ifndef NO_SCORE;
    print "^";
    #Endif; ! NO_SCORE
    Epilogue();
    ScoreSub();
    DisplayStatus();
    AfterGameOver();
];

! ----------------------------------------------------------------------------
! Routines called at end of each turn

[ AdvanceWorldClock;
    turns++;
    if (the_time ~= NULL) {
        if (time_rate >= 0) the_time=the_time+time_rate;
        else {
            time_step--;
            if (time_step == 0) {
                the_time++;
                time_step = -time_rate;
            }
        }
        the_time = the_time % 1440;
    }
];

[ RunTimersAndDaemons i j;
    #Ifdef DEBUG;
    if (debug_flag & DEBUG_TIMERS) {
        for (i=0 : i<active_timers : i++) {
            j = the_timers-->i;
            if (j ~= 0) {
                print (name) (j&~WORD_HIGHBIT), ": ";
                if (j & WORD_HIGHBIT) print "daemon";
                else
                    print "timer with ", j.time_left, " turns to go";
                new_line;
            }
        }
    }
    #Endif; ! DEBUG

    for (i=0 : i<active_timers : i++) {
        if (deadflag) return;
        j = the_timers-->i;
        if (j ~= 0) {
            if (j & WORD_HIGHBIT) RunRoutines(j&~WORD_HIGHBIT, daemon);
            else {
                if (j.time_left == 0) {
                    StopTimer(j);
                    RunRoutines(j, time_out);
                }
                else
                    j.time_left = j.time_left-1;
            }
        }
    }
];

[ RunEachTurnProperties;
    scope_reason = EACH_TURN_REASON; verb_word = 0;
    DoScopeAction(location);
    SearchScope(ScopeCeiling(player), player, 0);
    scope_reason = PARSING_REASON;
];

! ----------------------------------------------------------------------------

#Ifdef TARGET_ZCODE;

[ ActionPrimitive; (#actions_table-->action)(); ];

#Ifnot; ! TARGET_GLULX

[ ActionPrimitive; (#actions_table-->(action+1))(); ];

#Endif; ! TARGET_

[ AfterGameOver i amus_ret;

  .RRQPL;

    L__M(##Miscellany,5);

  .RRQL;

    print "> ";
    #Ifdef TARGET_ZCODE;
    #IfV3; read buffer parse; #Endif; ! V3
    temp_global=0;
    #IfV5; read buffer parse DrawStatusLine; #Endif; ! V5
    #Ifnot; ! TARGET_GLULX
    KeyboardPrimitive(buffer, parse);
    #Endif; ! TARGET_
    i = parse-->1;
    if (i == QUIT1__WD or QUIT2__WD) {
        #Ifdef TARGET_ZCODE;
        quit;
        #Ifnot; ! TARGET_GLULX
        quit;
        #Endif; ! TARGET_
    }
    if (i == RESTART__WD) {
        #Ifdef TARGET_ZCODE;
        @restart;
        #Ifnot; ! TARGET_GLULX
        @restart;
        #Endif; ! TARGET_
    }
    if (i == RESTORE__WD) {
        RestoreSub();
        jump RRQPL;
    }
    if (i == FULLSCORE1__WD or FULLSCORE2__WD && TASKS_PROVIDED==0) {
        new_line; FullScoreSub();
        jump RRQPL;
    }
    if (deadflag == 2 && i == AMUSING__WD) {
        amus_ret = false;
        if (AMUSING_PROVIDED == 0) {
            new_line;
            amus_ret = Amusing();
        }
        if (amus_ret == false) LibraryExtensions.RunAll(ext_amusing);
        jump RRQPL;
    }
    #IfV5;
    if (i == UNDO1__WD or UNDO2__WD or UNDO3__WD) {
        if (undo_flag == 0) {
            L__M(##Miscellany, 6);
            jump RRQPL;
        }
        if (undo_flag == 1) jump UndoFailed2;
        #Ifdef TARGET_ZCODE;
        @restore_undo i;
        #Ifnot; ! TARGET_GLULX
        @restoreundo i;
        i = (~~i);
        #Endif; ! TARGET_
        if (i == 0) {
          .UndoFailed2;
            L__M(##Miscellany, 7);
        }
        jump RRQPL;
    }
    #Endif; ! V5
    L__M(##Miscellany, 8);
    jump RRQL;
];

[ NoteObjectAcquisitions i;
    objectloop (i in player)
        if (i hasnt moved) {
            give i moved;
            if (i has scored) {
                score = score + OBJECT_SCORE;
                things_score = things_score + OBJECT_SCORE;
            }
        }
];

! ----------------------------------------------------------------------------
! R_Process is invoked by the <...> and <<...>> statements, whose syntax is:
!   <action [noun] [second]>                ! traditional
!   <action [noun] [second], actor>         ! introduced at compiler 6.33

[ R_Process a n s p
    s1 s2 s3;
    s1 = inp1; s2 = inp2; s3 = actor;
    inp1 = n; inp2 = s; if (p) actor = p; else actor = player;
    InformLibrary.begin_action(a, n, s, 1);
    inp1 = s1; inp2 = s2; actor = s3;
];

! ----------------------------------------------------------------------------

[ TestScope obj act a al sr x y;
    x = parser_one; y = parser_two;
    parser_one = obj; parser_two = 0; a = actor; al = actors_location;
    sr = scope_reason; scope_reason = TESTSCOPE_REASON;
    if (act == 0) actor = player; else actor = act;
    actors_location = ScopeCeiling(actor);
    SearchScope(actors_location, actor, 0); scope_reason = sr; actor = a;
    actors_location = al; parser_one = x; x = parser_two; parser_two = y;
    return x;
];

[ LoopOverScope routine act x y a al;
    x = parser_one; y = scope_reason; a = actor; al = actors_location;
    parser_one = routine;
    if (act == 0) actor = player; else actor = act;
    actors_location = ScopeCeiling(actor);
    scope_reason = LOOPOVERSCOPE_REASON;
    SearchScope(actors_location, actor, 0);
    parser_one = x; scope_reason = y; actor = a; actors_location = al;
];

[ BeforeRoutines rv;
    if (GamePreRoutine()) rtrue;
    if (rv == false) rv=LibraryExtensions.RunWhile(ext_gamepreroutine, 0);
    if (rv) rtrue;
    if (RunRoutines(player, orders)) rtrue;
    scope_reason = REACT_BEFORE_REASON; parser_one = 0;
    SearchScope(ScopeCeiling(player), player, 0);
    scope_reason = PARSING_REASON;
    if (parser_one) rtrue;
    if (location && RunRoutines(location, before)) rtrue;
    if (inp1 > 1 && RunRoutines(inp1, before)) rtrue;
    rfalse;
];

[ AfterRoutines rv;
    scope_reason = REACT_AFTER_REASON; parser_one = 0;
    SearchScope(ScopeCeiling(player), player, 0); scope_reason = PARSING_REASON;
    if (parser_one) rtrue;
    if (location && RunRoutines(location, after)) rtrue;
    if (inp1 > 1 && RunRoutines(inp1, after)) rtrue;
    rv = GamePostRoutine();
    if (rv == false) rv=LibraryExtensions.RunWhile(ext_gamepostroutine, false);
    return rv;
];

[ RunLife a j;
    #Ifdef DEBUG;
    if (debug_flag & DEBUG_ACTIONS) TraceAction(2, j);
    #Endif; ! DEBUG
    reason_code = j; return RunRoutines(a,life);
];

[ ZRegion addr;
    switch (metaclass(addr)) {      ! Left over from Inform 5
      nothing:          return 0;
      Object, Class:    return 1;
      Routine:          return 2;
      String:           return 3;
    }
];

[ PrintOrRun obj prop flag;
    if (obj.#prop > WORDSIZE) return RunRoutines(obj, prop);
    if (obj.prop == NULL) rfalse;
    switch (metaclass(obj.prop)) {
      Class, Object, nothing:
        return RunTimeError(2,obj,prop);
      String:
        print (string) obj.prop;
        if (flag == 0) new_line;
        rtrue;
      Routine:
        return RunRoutines(obj, prop);
    }
];

[ PrintOrRunVar var flag;
    switch (metaclass(var)) {
      Object:
        print (name) var;
      String:
        print (string) var;
        if (flag == 0) new_line;
      Routine:
        return var();
      default:
        print (char) '(', var, (char) ')';
    }
    rtrue;
];

[ ValueOrRun obj prop;
  !### this is entirely unlikely to work. Does anyone care? (AP)
  ! Well, it's certainly used three times in verblibm.h (RF)
  ! Update: now not used (RF)
    if (obj.prop < 256) return obj.prop;
    return RunRoutines(obj, prop);
];

[ RunRoutines obj prop;
    if (obj == thedark && prop ~= initial or short_name or description) obj = real_location;
    if (obj.&prop == 0 && prop >= INDIV_PROP_START) rfalse;
    return obj.prop();
];

#Ifdef TARGET_ZCODE;

[ ChangeDefault prop val a b;
    ! Use assembly-language here because -S compilation won't allow this:
    @loadw 0 5 -> a;
    b = prop-1;
    @storew a b val;
];

#Ifnot; ! TARGET_GLULX

[ ChangeDefault prop val;
    ! Use assembly-language here because -S compilation won't allow this:
    ! #cpv__start-->prop = val;
    @astore #cpv__start prop val;
];

#Endif; ! TARGET_

! ----------------------------------------------------------------------------

[ StartTimer obj timer i;
    for (i=0 : i<active_timers : i++)
        if (the_timers-->i == obj) rfalse;
    for (i=0 : i<active_timers : i++)
        if (the_timers-->i == 0) jump FoundTSlot;
    i = active_timers++;
    if (i >= MAX_TIMERS) { RunTimeError(4); return; }
  .FoundTSlot;
    if (obj.&time_left == 0) { RunTimeError(5, obj, time_left); return; }
    the_timers-->i = obj; obj.time_left = timer;
];

[ StopTimer obj i;
    for (i=0 : i<active_timers : i++)
        if (the_timers-->i == obj) jump FoundTSlot2;
    rfalse;
  .FoundTSlot2;
    if (obj.&time_left == 0) { RunTimeError(5, obj, time_left); return; }
    the_timers-->i = 0; obj.time_left = 0;
];

[ StartDaemon obj i;
    for (i=0 : i<active_timers : i++)
        if (the_timers-->i == WORD_HIGHBIT + obj) rfalse;
    for (i=0 : i<active_timers : i++)
        if (the_timers-->i == 0) jump FoundTSlot3;
    i = active_timers++;
    if (i >= MAX_TIMERS) RunTimeError(4);
  .FoundTSlot3;
    the_timers-->i = WORD_HIGHBIT + obj;
];

[ StopDaemon obj i;
    for (i=0 : i<active_timers : i++)
        if (the_timers-->i == WORD_HIGHBIT + obj) jump FoundTSlot4;
    rfalse;
  .FoundTSlot4;
    the_timers-->i = 0;
];

! ----------------------------------------------------------------------------

[ DisplayStatus;
    if (sys_statusline_flag == 0) { sline1 = score; sline2 = turns; }
    else { sline1 = the_time/60; sline2 = the_time%60;}
];

[ SetTime t s;
    the_time = t; time_rate = s; time_step = 0;
    if (s < 0) time_step = 0-s;
];

[ NotifyTheScore;
    #Ifdef TARGET_GLULX;
    glk($0086, 6); ! set note style
    #Endif; ! TARGET_GLULX
    print "^[";  L__M(##Miscellany, 50, score-last_score);  print ".]^";
    #Ifdef TARGET_GLULX;
    glk($0086, 0); ! set normal style
    #Endif; ! TARGET_GLULX
];

! ----------------------------------------------------------------------------

[ AdjustLight flag i;
    i = lightflag;
    lightflag = OffersLight(parent(player));

    if (i == 0 && lightflag == 1) {
        location = real_location;
        if (flag == 0) <Look>;
    }

    if (i == 1 && lightflag == 0) {
        real_location = location; location = thedark;
        if (flag == 0) {
            NoteArrival();
            return L__M(##Miscellany, 9);
        }
    }
    if (i == 0 && lightflag == 0) location = thedark;
];

[ OffersLight i j;
    if (i == 0) rfalse;
    if (i has light) rtrue;
    objectloop (j in i)
        if (HasLightSource(j) == 1) rtrue;
    if (i has container) {
        if (i has open || i has transparent)
            return OffersLight(parent(i));
    }
    else {
        if (i has enterable || i has transparent || i has supporter)
            return OffersLight(parent(i));
    }
    rfalse;
];

[ HidesLightSource obj;
    if (obj == player) rfalse;
    if (obj has transparent or supporter) rfalse;
    if (obj has container) return (obj hasnt open);
    return (obj hasnt enterable);
];

[ HasLightSource i j ad;
    if (i == 0) rfalse;
    if (i has light) rtrue;
    if (i has enterable || IsSeeThrough(i) == 1)
        if (~~(HidesLightSource(i)))
            objectloop (j in i)
                if (HasLightSource(j) == 1) rtrue;
    ad = i.&add_to_scope;
    if (parent(i) ~= 0 && ad ~= 0) {
        if (metaclass(ad-->0) == Routine) {
            ats_hls = 0; ats_flag = 1;
            RunRoutines(i, add_to_scope);
            ats_flag = 0; if (ats_hls == 1) rtrue;
        }
        else {
            for (j=0 : (WORDSIZE*j)<i.#add_to_scope : j++)
                if (HasLightSource(ad-->j) == 1) rtrue;
        }
    }
    rfalse;
];

[ ChangePlayer obj flag i;
!   if (obj.&number == 0) return RunTimeError(7, obj);
    if (actor == player) actor=obj;
    give player ~transparent ~concealed;
    i = obj; while (parent(i) ~= 0) {
        if (i has animate) give i transparent;
        i = parent(i);
    }
    if (player == selfobj) player.short_name = FORMER__TX;

    player = obj;

    if (player == selfobj) player.short_name = NULL;
    give player transparent concealed animate proper;
    i = player; while (parent(i) ~= 0) i = parent(i);
    location = i; real_location = location;
    if (parent(player) == 0) return RunTimeError(10);
    MoveFloatingObjects();
    lightflag = OffersLight(parent(player));
    if (lightflag == 0) location = thedark;
    print_player_flag = flag;
];

! ----------------------------------------------------------------------------

#Ifdef DEBUG;

#Ifdef TARGET_ZCODE;

[ DebugParameter w;
    print w;
    if (w >= 1 && w <= top_object) print " (", (name) w, ")";
    if (UnsignedCompare(w, dict_start) >= 0 &&
            UnsignedCompare(w, dict_end) < 0 &&
            (w - dict_start) % dict_entry_size == 0)
        print " ('", (address) w, "')";
];

[ DebugAction a anames;
    #Iftrue (Grammar__Version == 1);
    if (a >= 256) { print "<fake action ", a-256, ">"; return; }
    #Ifnot;
    if (a >= 4096) { print "<fake action ", a-4096, ">"; return; }
    #Endif; ! Grammar__Version
    anames = #identifiers_table;
    anames = anames + 2*(anames-->0) + 2*48;
    print (string) anames-->a;
];

[ DebugAttribute a anames;
    if (a < 0 || a >= 48) print "<invalid attribute ", a, ">";
    else {
        anames = #identifiers_table; anames = anames + 2*(anames-->0);
        print (string) anames-->a;
    }
];

#Ifnot; ! TARGET_GLULX

[ DebugParameter w endmem;
    print w;
    @getmemsize endmem;
    if (w >= 1 && w < endmem) {
        if (w->0 >= $70 && w->0 < $7F) print " (", (name) w, ")";
        if (w->0 >= $60 && w->0 < $6F) print " ('", (address) w, "')";
    }
];

[ DebugAction a str;
    if (a >= 4096) { print "<fake action ", a-4096, ">"; return; }
    if (a < 0 || a >= #identifiers_table-->7) print "<invalid action ", a, ">";
    else {
        str = #identifiers_table-->6;
        str = str-->a;
        if (str) print (string) str; else print "<unnamed action ", a, ">";
    }
];

[ DebugAttribute a str;
    if (a < 0 || a >= NUM_ATTR_BYTES*8) print "<invalid attribute ", a, ">";
    else {
        str = #identifiers_table-->4;
        str = str-->a;
        if (str) print (string) str; else print "<unnamed attribute ", a, ">";
    }
];

#Endif; ! TARGET_

[ TraceAction source ar;
    if (source < 2) print "[ Action ", (DebugAction) action;
    else {
        if (ar == ##Order)
            print "[ Order to ", (name) actor, ": ", (DebugAction) action;
        else
            print "[ Life rule ", (DebugAction) ar;
    }
    if (noun ~= 0)   print " with noun ", (DebugParameter) noun;
    if (second ~= 0) print " and second ", (DebugParameter) second;
    if (source == 0) print " ";
    if (source == 1) print " (from < > statement) ";
    print "]^";
];

[ DebugToken token;
    AnalyseToken(token);
    switch (found_ttype) {
      ILLEGAL_TT:
        print "<illegal token number ", token, ">";
      ELEMENTARY_TT:
        switch (found_tdata) {
          NOUN_TOKEN:           print "noun";
          HELD_TOKEN:           print "held";
          MULTI_TOKEN:          print "multi";
          MULTIHELD_TOKEN:      print "multiheld";
          MULTIEXCEPT_TOKEN:    print "multiexcept";
          MULTIINSIDE_TOKEN:    print "multiinside";
          CREATURE_TOKEN:       print "creature";
          SPECIAL_TOKEN:        print "special";
          NUMBER_TOKEN:         print "number";
          TOPIC_TOKEN:          print "topic";
          ENDIT_TOKEN:          print "END";
        }
      PREPOSITION_TT:
        print "'", (address) found_tdata, "'";
      ROUTINE_FILTER_TT:
        #Ifdef INFIX;
        print "noun=", (InfixPrintPA) found_tdata;
        #Ifnot;
        print "noun=Routine(", found_tdata, ")";
        #Endif; ! INFIX
      ATTR_FILTER_TT:
        print (DebugAttribute) found_tdata;
      SCOPE_TT:
        #Ifdef INFIX;
        print "scope=", (InfixPrintPA) found_tdata;
        #Ifnot;
        print "scope=Routine(", found_tdata, ")";
        #Endif; ! INFIX
      GPR_TT:
        #Ifdef INFIX;
        print (InfixPrintPA) found_tdata;
        #Ifnot;
        print "Routine(", found_tdata, ")";
        #Endif; ! INFIX
    }
];

[ DebugGrammarLine pcount;
    print " * ";
    for (: line_token-->pcount ~= ENDIT_TOKEN : pcount++) {
        if ((line_token-->pcount)->0 & $10) print "/ ";
        print (DebugToken) line_token-->pcount, " ";
    }
    print "-> ", (DebugAction) action_to_be;
    if (action_reversed) print " reverse";
];

#Ifdef TARGET_ZCODE;

[ ShowVerbSub grammar lines j;
    if (noun == 0 || ((noun->#dict_par1) & DICT_VERB) == 0)
        "Try typing ~showverb~ and then the name of a verb.";
    print "Verb";
    if ((noun->#dict_par1) & DICT_META) print " meta";
    for (j=dict_start : j<dict_end : j=j+dict_entry_size)
        if (j->#dict_par2 == noun->#dict_par2)
            print " '", (address) j, "'";
    new_line;
    grammar = (HDR_STATICMEMORY-->0)-->($ff-(noun->#dict_par2));
    lines = grammar->0;
    grammar++;
    if (lines == 0) "has no grammar lines.";
    for (: lines>0 : lines--) {
        grammar = UnpackGrammarLine(grammar);
        print "    "; DebugGrammarLine(); new_line;
    }
];

#Ifnot; ! TARGET_GLULX

[ ShowVerbSub grammar lines j wd dictlen entrylen;
    if (noun == 0 || ((noun->#dict_par1) & DICT_VERB) == 0)
        "Try typing ~showverb~ and then the name of a verb.";
    print "Verb";
    if ((noun->#dict_par1) & DICT_META) print " meta";
    dictlen = #dictionary_table-->0;
    entrylen = DICT_WORD_SIZE + 7;
    for (j=0 : j<dictlen : j++) {
        wd = #dictionary_table + WORDSIZE + entrylen*j;
        if (wd->#dict_par2 == noun->#dict_par2)
            print " '", (address) wd, "'";
    }
    new_line;
    grammar = (#grammar_table)-->($ff-(noun->#dict_par2)+1);
    lines = grammar->0;
    grammar++;
    if (lines == 0) "has no grammar lines.";
    for (: lines>0 : lines--) {
        grammar = UnpackGrammarLine(grammar);
        print "    "; DebugGrammarLine(); new_line;
    }
];

#Endif; ! TARGET_

[ ShowObjSub c f l a n x numattr;
    if (noun == 0) noun = location;
    objectloop (c ofclass Class) if (noun ofclass c) { f++; l=c; }
    if (f == 1) print (name) l, " ~"; else print "Object ~";
    print (name) noun, "~ (", noun, ")";
    if (parent(noun)) print " in ~", (name) parent(noun), "~ (", parent(noun), ")";
    new_line;
    if (f > 1) {
        print "  class ";
        objectloop (c ofclass Class) if (noun ofclass c) print (name) c, " ";
        new_line;
    }
    #Ifdef TARGET_ZCODE;
    numattr = 48;
    #Ifnot; ! TARGET_GLULX
    numattr = NUM_ATTR_BYTES * 8;
    #Endif; ! TARGET_
    for (a=0,f=0 : a<numattr : a++) if (noun has a) f=1;
    if (f) {
        print "  has ";
        for (a=0 : a<numattr : a++) if (noun has a) print (DebugAttribute) a, " ";
        new_line;
    }
    if (noun ofclass Class) return;

    f=0;
    #Ifdef TARGET_ZCODE;
    l = #identifiers_table-->0;
    #Ifnot; ! TARGET_GLULX
    l = INDIV_PROP_START + #identifiers_table-->3;
    #Endif; ! TARGET_
    for (a=1 : a<=l : a++) {
        if ((a ~= 2 or 3) && noun.&a) {
            if (f == 0) { print "  with "; f=1; }
            print (property) a;
            n = noun.#a;
            for (c=0 : WORDSIZE*c<n : c++) {
                print " ";
                x = (noun.&a)-->c;
                if (a == name) print "'", (address) x, "'";
                else {
                    if (a == number or capacity or time_left) print x;
                    else {
                        switch (x) {
                          NULL: print "NULL";
                          0:    print "0";
                          1:    print "1";
                          default:
                            switch (metaclass(x)) {
                              Class, Object:
                                print (name) x;
                              String:
                                print "~", (string) x, "~";
                              Routine:
                                print "[...]";
                           }
                           print " (", x, ")";
                        }
                    }
                }
            }
            print ",^       ";
        }
    }
!   if (f==1) new_line;
];

[ ShowDictSub_helper x; print (address) x; ];
[ ShowDictSub
    dp el ne   f x y z;

    #Ifdef TARGET_ZCODE;
    dp = HDR_DICTIONARY-->0;             ! start of dictionary
    dp = dp + dp->0 + 1;                 ! skip over word-separators table
    el = dp->0;  dp = dp + 1;            ! entry length
    ne = dp-->0; dp = dp + WORDSIZE;     ! number of entries
    #Ifnot; ! TARGET_GLULX;
    dp = #dictionary_table;              ! start of dictionary
    el = DICT_WORD_SIZE + 7;             ! entry length
    ne = dp-->0; dp = dp + WORDSIZE;     ! number of entries
    #Endif; ! TARGET_
                                         ! dp now at first entry
    wn = 2; x = NextWordStopped();
    switch (x) {
      0:
        "That word isn't in the dictionary.";
      -1:
        ;                                ! show all entries
      THEN1__WD:
        dp = './/'; ne = 1;              ! show '.'
      COMMA_WORD:
        dp = ',//'; ne = 1;              ! show ','
      default:
        dp = x; ne = 1; f = true;        ! show specified entry, plus associated objects
    }
    for ( : ne-- : dp=dp+el) {
        print (address) dp, " --> ";
        x = dp->#dict_par1;              ! flag bits
        y = PrintToBuffer(StorageForShortName, SHORTNAMEBUF_LEN, ShowDictSub_helper, dp) + WORDSIZE;
        for (z=WORDSIZE : z<y : z++) {
            !if (x & DICT_NOUN) StorageForShortName->z = UpperCase(StorageForShortName->z);
            if (y > WORDSIZE+1 && StorageForShortName->z == ' ' or '.' or ',') x = x | $8000;
            print (char) StorageForShortName->z;
        }
        print " --> ";
        if (x == 0)
            print " no flags";
        else {
            !if (x & $0040)     print " BIT_6";
            !if (x & $0020)     print " BIT_5";
            !if (x & $0010)     print " BIT_4";
            if (x & $8000)     print " UNTYPEABLE";
            if (x & DICT_NOUN) print " noun";
            if (x & DICT_PLUR) print "+plural";
            if (x & DICT_VERB) print " verb";
            if (x & DICT_META) print "+meta";
            if (x & DICT_PREP) print " preposition";
            if (f && (x & DICT_NOUN)) {
                print " --> refers to these objects:";
                objectloop (x)
                    if (WordInProperty(dp, x, name)) print "^  ", (name) x, " (", x, ")";
            }
        }
        new_line;
    }
];

#Endif; ! DEBUG

! ----------------------------------------------------------------------------
!  Miscellaneous display routines used by DrawStatusLine and available for
!  user.  Most of these vary according to which machine is being compiled to
! ----------------------------------------------------------------------------

#Ifdef TARGET_ZCODE;

[ ClearScreen window;
    switch (window) {
      WIN_ALL:    @erase_window -1;
      WIN_STATUS: @erase_window 1;
      WIN_MAIN:   @erase_window 0;
    }
];

#Iftrue (#version_number == 6);
[ MoveCursorV6 line column  charw;  ! 1-based postion on text grid
    @get_wind_prop 1 13 -> charw; ! font size
    charw = charw & $FF;
    line = 1 + charw*(line-1);
    column = 1 + charw*(column-1);
    @set_cursor line column;
];
#Endif;

#Ifndef MoveCursor;
[ MoveCursor line column;  ! 1-based postion on text grid
    if (~~statuswin_current) {
        @set_window 1;
        #Ifdef COLOUR;
        if (clr_on && clr_bgstatus > 1)
            @set_colour clr_fgstatus clr_bgstatus;
        else
        #Endif; ! COLOUR
            style reverse;
    }
    if (line == 0) { line = 1; column = 1; }
    #Iftrue (#version_number == 6);
    MoveCursorV6(line, column);
    #Ifnot;
    @set_cursor line column;
    #Endif;
    statuswin_current = true;
];
#Endif;

[ MainWindow;
    if (statuswin_current) {
#Ifdef COLOUR;
        if (clr_on && clr_bgstatus > 1) @set_colour clr_fg clr_bg;
        else
#Endif; ! COLOUR
            style roman;
        @set_window 0;
        }
    statuswin_current = false;
];

#Iftrue (#version_number == 6);
[ ScreenWidth  width charw;
    @get_wind_prop 1 3 -> width;
    @get_wind_prop 1 13 -> charw;
    charw = charw & $FF;
    if (charw == 0) return width;
    return (width+charw-1) / charw;
];
#Ifnot;
[ ScreenWidth;
    return (HDR_SCREENWCHARS->0);
];
#Endif;

[ ScreenHeight;
    return (HDR_SCREENHLINES->0);
];

#Iftrue (#version_number == 6);
[ StatusLineHeight height  wx wy x y charh;
    ! Split the window. Standard 1.0 interpreters should keep the window 0
    ! cursor in the same absolute position, but older interpreters,
    ! including Infocom's don't - they keep the window 0 cursor in the
    ! same position relative to its origin. We therefore compensate
    ! manually.
    @get_wind_prop 0 0 -> wy; @get_wind_prop 0 1 -> wx;
    @get_wind_prop 0 13 -> charh; @log_shift charh $FFF8 -> charh;
    @get_wind_prop 0 4 -> y; @get_wind_prop 0 5 -> x;
    height = height * charh;
    @split_window height;
    y = y - height + wy - 1;
    if (y < 1) y = 1;
    x = x + wx - 1;
    @set_cursor y x 0;
    gg_statuswin_cursize = height;
];
#Ifnot;
[ StatusLineHeight height;
    if (gg_statuswin_cursize ~= height)
        @split_window height;
    gg_statuswin_cursize = height;
];
#Endif;

#Ifdef COLOUR;
[ SetColour f b window;
    if (f && b) {
        if (window == 0) {  ! if setting both together, set reverse
            clr_fgstatus = b;
            clr_bgstatus = f;
            }
        if (window == 1) {
            clr_fgstatus = f;
            clr_bgstatus = b;
        }
        if (window == 0 or 2) {
            clr_fg = f;
            clr_bg = b;
        }
        if (clr_on) {
            if (statuswin_current)
                @set_colour clr_fgstatus clr_bgstatus;
            else
                @set_colour clr_fg clr_bg;
        }
    }
];
#Endif; ! COLOUR


#Ifnot; ! TARGET_GLULX

[ ClearScreen window;
    if (window == WIN_ALL or WIN_MAIN) {
        glk($002A, gg_mainwin);
        if (gg_quotewin) {
            glk($0024, gg_quotewin, 0); ! close_window
            gg_quotewin = 0;
        }
    }
    if (gg_statuswin && window == WIN_ALL or WIN_STATUS) glk($002A, gg_statuswin);
];

[ MoveCursor line column;  ! 0-based postion on text grid
    if (gg_statuswin) {
        glk($002F, gg_statuswin); ! set_window
    }
    if (line == 0) { line = 1; column = 1; }
    glk($002B, gg_statuswin, column-1, line-1); ! window_move_cursor
    statuswin_current=1;
];

[ MainWindow;
    glk($002F, gg_mainwin); ! set_window
    statuswin_current=0;
];

[ MakeColourWord c;
    if (c > 9) return c;
    c = c-2;
    return $ff0000*(c&1) + $ff00*(c&2 ~= 0) + $ff*(c&4 ~= 0);
];

[ ScreenWidth  id;
    id=gg_mainwin;
    if (gg_statuswin && statuswin_current) id = gg_statuswin;
    glk($0025, id, gg_arguments, 0); ! window_get_size
    return gg_arguments-->0;
];

[ ScreenHeight;
    glk($0025, gg_mainwin, 0, gg_arguments); ! window_get_size
    return gg_arguments-->0;
];

#Ifdef COLOUR;
[ SetColour f b window doclear  i fwd bwd swin;
    if (f && b) {
        if (window) swin = 5-window; ! 4 for TextGrid, 3 for TextBuffer

        if (clr_on) {
            fwd = MakeColourWord(f);
            bwd = MakeColourWord(b);
            for (i=0 : i<=10: i++) {
                if (f == CLR_DEFAULT || b == CLR_DEFAULT) { ! remove style hints
                    glk($00B1, swin, i, 7);
                    glk($00B1, swin, i, 8);
                }
                else {
                    glk($00B0, swin, i, 7, fwd);
                    glk($00B0, swin, i, 8, bwd);
                }

            ! Now re-open the windows to apply the hints
            if (gg_statuswin) glk($0024, gg_statuswin, 0); ! close_window

            if (doclear || ( window ~= 1 && (clr_fg ~= f || clr_bg ~= b) ) ) {
                glk($0024, gg_mainwin, 0);
                gg_mainwin = glk($0023, 0, 0, 0, 3, GG_MAINWIN_ROCK); ! window_open
                if (gg_scriptstr ~= 0)
                    glk($002D, gg_mainwin, gg_scriptstr); ! window_set_echo_stream
            }

            gg_statuswin = glk($0023, gg_mainwin, $12, gg_statuswin_cursize,
               4, GG_STATUSWIN_ROCK); ! window_open
            if (statuswin_current && gg_statuswin)
                MoveCursor(); else MainWindow();
        }

        if (window ~= 2) {
            clr_fgstatus = f;
            clr_bgstatus = b;
        }
        if (window ~= 1) {
            clr_fg = f;
            clr_bg = b;
        }
    }
];
#Endif; ! COLOUR
#Endif; ! TARGET_

#Ifndef COLOUR;
[ SetColour f b window doclear;
    f = b = window = doclear = 0;
];
#Endif;

[ SetClr f b w;
    SetColour (f, b, w);
];

[ RestoreColours;    ! L61007, L61113
    gg_statuswin_cursize = -1;    ! Force window split in StatusLineHeight()
    #Ifdef COLOUR;
    if (clr_on) { ! check colour has been used
        SetColour(clr_fg, clr_bg, 2); ! make sure both sets of variables are restored
        SetColour(clr_fgstatus, clr_bgstatus, 1, true);
        ClearScreen();
    }
    #Endif;
    #Ifdef TARGET_ZCODE;
    #Iftrue (#version_number == 6); ! request screen update
    (0-->8) = (0-->8) | $$00000100;
    #Endif;
    #Endif; ! TARGET_
];

! ----------------------------------------------------------------------------
!  Except in Version 3, the DrawStatusLine routine does just that: this is
!  provided explicitly so that it can be Replace'd to change the style, and
!  as written it emulates the ordinary Standard game status line, which is
!  drawn in hardware
! ----------------------------------------------------------------------------

#Ifdef TARGET_ZCODE;

#IfV5;

#Iftrue (#version_number == 6);
[ DrawStatusLine width x charw scw mvw;
    HDR_GAMEFLAGS-->0 = (HDR_GAMEFLAGS-->0) & ~$0004;

    StatusLineHeight(gg_statuswin_size);
    ! Now clear the window. This isn't totally trivial. Our approach is to
    ! select the fixed space font, measure its width, and print an appropriate
    ! number of spaces. We round up if the screen isn't a whole number of
    ! characters wide, and rely on window 1 being set to clip by default.
    MoveCursor(1, 1);
    @set_font 4 -> x;
    width = ScreenWidth();
    spaces width;
    ! Back to standard font for the display. We use output_stream 3 to
    ! measure the space required, the aim being to get 50 characters
    ! worth of space for the location name.
    MoveCursor(1, 2);
    @set_font 1 -> x;
    if (location == thedark)
        print (name) location;
    else {
        FindVisibilityLevels();
        if (visibility_ceiling == location) print (name) location;
        else                                print (The) visibility_ceiling;
    }
    @get_wind_prop 1 3 -> width;
    @get_wind_prop 1 13 -> charw;
    charw = charw & $FF;
    @output_stream 3 StorageForShortName;
    print (string) SCORE__TX, "00000";
    @output_stream -3; scw = HDR_PIXELSTO3-->0 + charw;
    @output_stream 3 StorageForShortName;
    print (string) MOVES__TX, "00000";
    @output_stream -3; mvw = HDR_PIXELSTO3-->0 + charw;
    if (width - scw - mvw >= 50*charw) {
        x = 1+width-scw-mvw;
        @set_cursor 1 x; print (string) SCORE__TX, sline1;
        x = x+scw;
        @set_cursor 1 x; print (string) MOVES__TX, sline2;
    }
    else {
        @output_stream 3 StorageForShortName;
        print "00000/00000";
        @output_stream -3; scw = HDR_PIXELSTO3-->0 + charw;
        if (width - scw >= 50*charw) {
            x = 1+width-scw;
            @set_cursor 1 x; print sline1, "/", sline2;
        }
    }
    ! Reselect roman, as Infocom's interpreters interpreters go funny
    ! if reverse is selected twice.
    MainWindow();
];

#Endif; ! #version_number == 6

#Endif; ! V5

#Endif; ! TARGET_ZCODE

#Ifndef DrawStatusLine;
[ DrawStatusLine width posa posb;
    #Ifdef TARGET_GLULX;
    ! If we have no status window, we must not try to redraw it.
    if (gg_statuswin == 0)
        return;
    #Endif;

    ! If there is no player location, we shouldn't try to draw status window
    if (location == nothing || parent(player) == nothing)
        return;

    StatusLineHeight(gg_statuswin_size);
    MoveCursor(1, 1);

    width = ScreenWidth();
    posa = width-26; posb = width-13;
    spaces width;

    MoveCursor(1, 2);
    if (location == thedark) {
        print (name) location;
    }
    else {
        FindVisibilityLevels();
        if (visibility_ceiling == location)
            print (name) location;
        else
            print (The) visibility_ceiling;
    }

    if (sys_statusline_flag && width > 53) {
        MoveCursor(1, posa);
        print (string) TIME__TX;
        LanguageTimeOfDay(sline1, sline2);
    }
    else {
        if (width > 66) {
            #Ifndef NO_SCORE;
            MoveCursor(1, posa);
            print (string) SCORE__TX, sline1;
            #Endif;
            MoveCursor(1, posb);
            print (string) MOVES__TX, sline2;
        }
        #Ifndef NO_SCORE;
        if (width > 53 && width <= 66) {
            MoveCursor(1, posb);
            print sline1, "/", sline2;
        }
        #Endif;
    }

    MainWindow(); ! set_window
];
#Endif;

#Ifdef TARGET_GLULX;

[ StatusLineHeight hgt parwin;
    if (gg_statuswin == 0) return;
    if (hgt == gg_statuswin_cursize) return;
    parwin = glk($0029, gg_statuswin); ! window_get_parent
    glk($0026, parwin, $12, hgt, 0); ! window_set_arrangement
    gg_statuswin_cursize = hgt;
];

[ Box__Routine maxwid arr ix lines lastnl parwin;
    maxwid = 0; ! squash compiler warning
    lines = arr-->0;

    if (gg_quotewin == 0) {
        gg_arguments-->0 = lines;
        ix = InitGlkWindow(GG_QUOTEWIN_ROCK);
        if (ix == false) ix = LibraryExtensions.RunWhile(ext_InitGlkWindow, 0, GG_QUOTEWIN_ROCK);
        if (ix == false)
            gg_quotewin = glk($0023, gg_mainwin, $12, lines, 3,
                GG_QUOTEWIN_ROCK); ! window_open
    }
    else {
        parwin = glk($0029, gg_quotewin); ! window_get_parent
        glk($0026, parwin, $12, lines, 0); ! window_set_arrangement
    }

    lastnl = true;
    if (gg_quotewin) {
        glk($002A, gg_quotewin); ! window_clear
        glk($002F, gg_quotewin); ! set_window
        lastnl = false;
    }

    ! If gg_quotewin is zero here, the quote just appears in the story window.

    glk($0086, 7); ! set blockquote style
    for (ix=0 : ix<lines : ix++) {
        print (string) arr-->(ix+1);
        if (ix < lines-1 || lastnl) new_line;
    }
    glk($0086, 0); ! set normal style

    if (gg_quotewin) {
        glk($002F, gg_mainwin); ! set_window
    }
];

#Endif; ! TARGET_GLULX


#Ifdef TARGET_ZCODE;

[ ZZInitialise;
    standard_interpreter = HDR_TERPSTANDARD-->0;
   transcript_mode = ((HDR_GAMEFLAGS-->0) & $0001);
    sys_statusline_flag = ( (HDR_TERPFLAGS->0) & $0002 ) / 2;
    top_object = #largest_object-255;

    dict_start = HDR_DICTIONARY-->0;
    dict_entry_size = dict_start->(dict_start->0 + 1);
    dict_start = dict_start + dict_start->0 + 4;
    dict_end = dict_start + (dict_start - 2)-->0 * dict_entry_size;
    #Ifdef DEBUG;
    if (dict_start > 0 && dict_end < 0 &&
      ((-dict_start) - dict_end) % dict_entry_size == 0)
        print "** Warning: grammar properties might not work correctly **^";
    #Endif; ! DEBUG

    buffer->0  = INPUT_BUFFER_LEN - WORDSIZE;
    buffer2->0 = INPUT_BUFFER_LEN - WORDSIZE;
    buffer3->0 = INPUT_BUFFER_LEN - WORDSIZE;
    parse->0   = MAX_BUFFER_WORDS;
    parse2->0  = MAX_BUFFER_WORDS;
];

#Ifnot; ! TARGET_GLULX;

[ GGInitialise res;
    @gestalt 4 2 res; ! Test if this interpreter has Glk.
    if (res == 0) {
      ! Without Glk, we're entirely screwed.
      quit;
    }
    ! Set the VM's I/O system to be Glk.
    @setiosys 2 0;

    ! First, we must go through all the Glk objects that exist, and see
    ! if we created any of them. One might think this strange, since the
    ! program has just started running, but remember that the player might
    ! have just typed "restart".

    GGRecoverObjects();
    res = InitGlkWindow(0);
    if (res == false) res = LibraryExtensions.RunWhile(ext_InitGlkWindow, 0, 0);
    if (res) return;

    ! Now, gg_mainwin and gg_storywin might already be set. If not, set them.

    if (gg_mainwin == 0) {
        ! Open the story window.
        res = InitGlkWindow(GG_MAINWIN_ROCK);
        if (res == false) res = LibraryExtensions.RunWhile(ext_InitGlkWindow, 0, GG_MAINWIN_ROCK);
        if (res == false)
            gg_mainwin = glk($0023, 0, 0, 0, 3, GG_MAINWIN_ROCK); ! window_open
        if (gg_mainwin == 0) {
            ! If we can't even open one window, there's no point in going on.
            quit;
        }
    }
    else {
        ! There was already a story window. We should erase it.
        glk($002A, gg_mainwin); ! window_clear
    }

    if (gg_statuswin == 0) {
        res = InitGlkWindow(GG_STATUSWIN_ROCK);
        if (res == false) res = LibraryExtensions.RunWhile(ext_InitGlkWindow, 0, GG_STATUSWIN_ROCK);
        if (res == false) {
            gg_statuswin_cursize = gg_statuswin_size;
            gg_statuswin = glk($0023, gg_mainwin, $12, gg_statuswin_cursize,
                4, GG_STATUSWIN_ROCK); ! window_open
        }
    }
    ! It's possible that the status window couldn't be opened, in which case
    ! gg_statuswin is now zero. We must allow for that later on.

    glk($002F, gg_mainwin); ! set_window

    if (InitGlkWindow(1) == false) LibraryExtensions.RunWhile(ext_InitGlkWindow, 0, 1);
];

[ GGRecoverObjects id;
    ! If GGRecoverObjects() has been called, all these stored IDs are
    ! invalid, so we start by clearing them all out.
    ! (In fact, after a restoreundo, some of them may still be good.
    ! For simplicity, though, we assume the general case.)
    gg_mainwin = 0;
    gg_statuswin = 0;
    gg_quotewin = 0;
    gg_scriptfref = 0;
    gg_scriptstr = 0;
    gg_savestr = 0;
    gg_statuswin_cursize = 0;
    #Ifdef DEBUG;
    gg_commandstr = 0;
    gg_command_reading = false;
    #Endif; ! DEBUG
    ! Also tell the game to clear its object references.
    if (IdentifyGlkObject(0) == false) LibraryExtensions.RunWhile(ext_identifyglkobject, 0, 0);

    id = glk($0040, 0, gg_arguments); ! stream_iterate
    while (id) {
        switch (gg_arguments-->0) {
            GG_SAVESTR_ROCK: gg_savestr = id;
            GG_SCRIPTSTR_ROCK: gg_scriptstr = id;
            #Ifdef DEBUG;
            GG_COMMANDWSTR_ROCK: gg_commandstr = id;
                                 gg_command_reading = false;
            GG_COMMANDRSTR_ROCK: gg_commandstr = id;
                                 gg_command_reading = true;
            #Endif; ! DEBUG
            default: if (IdentifyGlkObject(1, 1, id, gg_arguments-->0) == false)
                         LibraryExtensions.RunWhile(ext_identifyglkobject, false, 1, 1, id, gg_arguments-->0);
        }
        id = glk($0040, id, gg_arguments); ! stream_iterate
    }

    id = glk($0020, 0, gg_arguments); ! window_iterate
    while (id) {
        switch (gg_arguments-->0) {
            GG_MAINWIN_ROCK: gg_mainwin = id;
            GG_STATUSWIN_ROCK: gg_statuswin = id;
            GG_QUOTEWIN_ROCK: gg_quotewin = id;
            default: if (IdentifyGlkObject(1, 0, id, gg_arguments-->0) == false)
                        LibraryExtensions.RunWhile(ext_identifyglkobject, false, 1, 0, id, gg_arguments-->0);
        }
        id = glk($0020, id, gg_arguments); ! window_iterate
    }

    id = glk($0064, 0, gg_arguments); ! fileref_iterate
    while (id) {
        switch (gg_arguments-->0) {
            GG_SCRIPTFREF_ROCK: gg_scriptfref = id;
            default: if (IdentifyGlkObject(1, 2, id, gg_arguments-->0) == false)
                        LibraryExtensions.RunWhile(ext_identifyglkobject, false, 1, 2, id, gg_arguments-->0);
        }
        id = glk($0064, id, gg_arguments); ! fileref_iterate
    }

    ! Tell the game to tie up any loose ends.
    if (IdentifyGlkObject(2) == false)
        LibraryExtensions.RunWhile(ext_identifyglkobject, 0, 2);
];

! This somewhat obfuscated function will print anything.
! It handles strings, functions (with optional arguments), objects,
! object properties (with optional arguments), and dictionary words.
! It does *not* handle plain integers, but you can use
! DecimalNumber or EnglishNumber to handle that case.
!
! Calling:                           Is equivalent to:
! -------                            ----------------
! PrintAnything()                    <nothing printed>
! PrintAnything(0)                   <nothing printed>
! PrintAnything("string");           print (string) "string";
! PrintAnything('word')              print (address) 'word';
! PrintAnything(obj)                 print (name) obj;
! PrintAnything(obj, prop)           obj.prop();
! PrintAnything(obj, prop, args...)  obj.prop(args...);
! PrintAnything(func)                func();
! PrintAnything(func, args...)       func(args...);

[ PrintAnything _vararg_count obj mclass;
    print_anything_result = 0;
    if (_vararg_count == 0) return;
    @copy sp obj;
    _vararg_count--;
    if (obj == 0) return;

    if (obj->0 == $60) {
        ! Dictionary word. Metaclass() can't catch this case, so we do
        ! it manually.
        print (address) obj;
        return;
    }

    mclass = metaclass(obj);
    switch (mclass) {
      nothing:
        return;
      String:
        print (string) obj;
        return;
      Routine:
        ! Call the function with all the arguments which are already
        ! on the stack.
        @call obj _vararg_count print_anything_result;
        return;
      Object:
        if (_vararg_count == 0) {
            print (name) obj;
        }
        else {
            ! Push the object back onto the stack, and call the
            ! veneer routine that handles obj.prop() calls.
            @copy obj sp;
            _vararg_count++;
            @call CA__Pr _vararg_count print_anything_result;
        }
        return;
    }
];

! This does the same as PrintAnything, but the output is sent to a
! byte array in memory. The first two arguments must be the array
! address and length; the following arguments are interpreted as
! for PrintAnything. The return value is the number of characters
! output.
! If the output is longer than the array length given, the extra
! characters are discarded, so the array does not overflow.
! (However, the return value is the total length of the output,
! including discarded characters.)

[ PrintAnyToArray _vararg_count arr arrlen str oldstr len;
    @copy sp arr;
    @copy sp arrlen;
    _vararg_count = _vararg_count - 2;

    oldstr = glk($0048); ! stream_get_current
    str = glk($0043, arr, arrlen, 1, 0); ! stream_open_memory
    if (str == 0) return 0;

    glk($0047, str); ! stream_set_current

    @call PrintAnything _vararg_count 0;

    glk($0047, oldstr); ! stream_set_current
    @copy $ffffffff sp;
    @copy str sp;
    @glk $0044 2 0; ! stream_close
    @copy sp len;
    @copy sp 0;

    return len;
];

! And this calls PrintAnyToArray on a particular array, jiggering
! the result to be a Glulx C-style ($E0) string.

Constant GG_ANYTOSTRING_LEN 66;
Array AnyToStrArr -> GG_ANYTOSTRING_LEN+1;

[ ChangeAnyToCString _vararg_count ix len;
    ix = GG_ANYTOSTRING_LEN-2;
    @copy ix sp;
    ix = AnyToStrArr+1;
    @copy ix sp;
    ix = _vararg_count+2;
    @call PrintAnyToArray ix len;
    AnyToStrArr->0 = $E0;
    if (len >= GG_ANYTOSTRING_LEN)
        len = GG_ANYTOSTRING_LEN-1;
    AnyToStrArr->(len+1) = 0;
    return AnyToStrArr;
];

#Endif; ! TARGET_

! This is a trivial function which just prints a number, in decimal
! digits. It may be useful as a stub to pass to PrintAnything.

[ DecimalNumber num; print num; ];

#Ifndef SHORTNAMEBUF_LEN;               ! Can't use 'Default', unfortunately,
Constant SHORTNAMEBUF_LEN 160;          ! but this is functionally equivalent
#Endif;

#IfV5;
#Ifdef VN_1630;
Array StorageForShortName buffer SHORTNAMEBUF_LEN;
#Ifnot;
Array StorageForShortName -> WORDSIZE + SHORTNAMEBUF_LEN;
#Endif; ! VN_1630

#Endif; ! V5


! None of the following functions should be called for zcode if the
! output exceeds the size of the buffer.

[ StringSize a b c d e;
    PrintToBuffer(StorageForShortName, 160, a, b, c, d, e);
    return StorageForShortName-->0;
];

[ PrintCapitalised a b no_break no_caps centred;
    if (metaclass(a) == Routine or String || b == 0 || metaclass(a.b) == Routine or String)
        PrintToBuffer(StorageForShortName, SHORTNAMEBUF_LEN, a, b);
    else
        if (a.b == NULL) rfalse;
        else             return RunTimeError(2, a, b);
    if (no_caps == 0 or 1) no_caps = ~~no_caps;
    PrintFromBuffer(StorageForShortName, no_break, no_caps, centred);
    return print_anything_result;
];

[ Centre a b;
    PrintCapitalised(a, b, false, true, true);
];

[ CapitRule str no_caps;
    if (no_caps) print (string) str;
    else         PrintCapitalised(str);
];

[ Indefart o saveIndef;
    saveIndef = indef_mode; indef_mode = true; caps_mode = false;
    if (o has proper) {
        indef_mode = NULL;
        print (PSN__) o;
        indef_mode = saveIndef;
        return;
    }

    if (o provides article) {
        PrintOrRun(o, article, 1);
        print " ", (PSN__) o;
        indef_mode = saveIndef;
        return;
    }
    PrefaceByArticle(o, 2);
    indef_mode = saveIndef;
];

[ CInDefArt o saveIndef saveCaps;
    saveIndef = indef_mode; indef_mode = true;
    saveCaps = caps_mode; caps_mode = true;

    if (o has proper) {
        indef_mode = NULL;
        PrintToBuffer(StorageForShortName, SHORTNAMEBUF_LEN, PSN__, o);
        PrintFromBuffer(StorageForShortName, true, caps_mode);
        caps_mode = saveCaps;
        return;
    }

    if (o provides article) {
        PrintCapitalised(o, article, true);
        print " ", (PSN__) o;
        indef_mode = saveIndef;
        caps_mode = saveCaps;
        return;
    }
    PrefaceByArticle(o, 2, 0, 1);
    caps_mode = saveCaps;
    indef_mode = saveIndef;
];

[ Defart o saveIndef;
    saveIndef = indef_mode;
    indef_mode = false;
    caps_mode = false;
    if ((~~o ofclass Object) || o has proper) {
        indef_mode = NULL;
        print (PSN__) o;
        indef_mode = saveIndef;
        return;
    }
    PrefaceByArticle(o, 1);
    indef_mode = saveIndef;
];

[ CDefart o saveIndef saveCaps;
    #Ifdef YOU__TX;
    if (o == player) {
        print (string) YOU__TX;
        return;
    }
    #Endif;
    saveIndef = indef_mode; indef_mode = false;
    saveCaps = caps_mode; caps_mode = true;
    if (~~o ofclass Object) {
        indef_mode = NULL; print (PSN__) o;
    }
    else
        if (o has proper) {
            indef_mode = NULL;
            PrintToBuffer(StorageForShortName, SHORTNAMEBUF_LEN, PSN__, o);
            PrintFromBuffer(StorageForShortName, true, caps_mode);
        }
        else
            PrefaceByArticle(o, 0);
    indef_mode = saveIndef; caps_mode = saveCaps;
];

[ PrintShortName o saveIndef;
    saveIndef = indef_mode; indef_mode = NULL;
    PSN__(o); indef_mode = saveIndef;
];

[ EnglishNumber n; LanguageNumber(n); ];

[ SerialComma n;
    #Ifdef SERIAL_COMMAS;
    if (n>2) print ",";
    #Endif;
    n=0;        ! quell unused n variable warning
];

[ NumberWord o i n;
    n = LanguageNumbers-->0;
    for (i=1 : i<=n : i=i+2)
        if (o == LanguageNumbers-->i) return LanguageNumbers-->(i+1);
    return 0;
];

[ RandomEntry tab;
    if (tab-->0 == 0) return RunTimeError(8);
    return tab-->(random(tab-->0));
];

! ----------------------------------------------------------------------------
!  Useful routine: unsigned comparison (for addresses in Z-machine)
!    Returns 1 if x>y, 0 if x=y, -1 if x<y
! ----------------------------------------------------------------------------

[ UnsignedCompare x y u v;
    if (x == y) return 0;
    if (x < 0 && y >= 0) return 1;
    if (x >= 0 && y < 0) return -1;
    u = x&~WORD_HIGHBIT; v= y&~WORD_HIGHBIT;
    if (u > v) return 1;
    return -1;
];

! ==============================================================================

#Ifdef NITFOL_HOOKS;          ! Code contributed by Evin Robertson
#Ifdef TARGET_GLULX;          ! Might be nice for Z-machine games too,
                              ! but I'm not going to try to make this work
                              ! given #Ifdef funniness.

Array magic_array -->         ! This is so nitfol can do typo correction /
                              ! automapping / debugging on Glulx games
    $6e66726d $4d616763 $ff0010 ! Goes to 'NfrmMagc'  10 refers to length
    Magic_Global_Dispatch__
    DI__check_word            ! DI__check_word(buf, length)
    PrintShortName
    WV__Pr RV__Pr CA__Pr      ! obj.prop = x; x = obj.prop; obj.prop(x)
    RA__Pr RL__Pr RA__Sc      ! obj.&prop; obj.#prop; class::prop
    OP__Pr OC__Cl             ! obj provides prop; obj ofclass class
    OB__Move OB__Remove
    OB__Parent__ OB__Child__ OB__Sibling__  ! No explicit veneer for these
    ;

[ OB__Parent__ obj; return parent(obj); ];

[ OB__Child__ obj; return child(obj); ];

[ OB__Sibling__ obj; return sibling(obj); ];

[ Magic_Global_Dispatch__ glbl;
    switch (glbl) {
      0:
        if (location == TheDark) return real_location; return location;
      1:
        return Player;
      -1:
        return CompassDirection::number; ! Silliness to make exist RA__Sc
                                         ! Should never be called.
    }
    return magic_array;       ! Silences a warning.
];

[ DI__check_word buf wlen  ix val res dictlen entrylen;
    ! Just like in Tokenise__.  In fact, Tokenise__ could call this if
    ! it wanted, instead of doing this itself.
    if (wlen > DICT_WORD_SIZE) wlen = DICT_WORD_SIZE;
    for (ix=0 : ix<wlen : ix++) {
        gg_tokenbuf->ix = glk($00A0, buf->ix);
    }
    for (: ix<DICT_WORD_SIZE : ix++) {
        gg_tokenbuf->ix = 0;
    }
    val = #dictionary_table + WORDSIZE;
    entrylen = DICT_WORD_SIZE + 7;
    @binarysearch gg_tokenbuf DICT_WORD_SIZE val entrylen dictlen 1 1 res;
    return res;
];

#Endif; ! TARGET_
#Endif; ! NITFOL_HOOKS


! ==============================================================================
